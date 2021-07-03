#include "Precompiled.h"

#include "InstanceValue.h"
#include "ChaosApplication.h"
#include "ChaosRom.h"
#include "Resource.h"
#include "SegaPattern.h"
#include "Buffer.h"
#include "Buffer_Patterns.h"
#include "Buffer_Blocks.h"
#include "SonicChunk.h"
#include "SonicBlock.h"
#include "SonicMap.h"
#include "WndMap.h"
#include "Level.h"

extern ChaosApplication g_application;

ATOM WndMap::ms_class = 0;
HINSTANCE WndMap::ms_hinst = 0;

InstanceValue_t<int> WndMap::ms_x;
InstanceValue_t<int> WndMap::ms_y;
InstanceValue_t<unsigned int> WndMap::ms_block_width;
InstanceValue_t<unsigned int> WndMap::ms_block_height;

ATOM WndMap::getClass(HINSTANCE hinst)
{
    if (!ms_class)
    {
        WNDCLASS wc;

        wc.style         = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc   = wndProc;
        wc.cbClsExtra    = 0;
        wc.cbWndExtra    = 0;
        wc.hInstance     = hinst;
        wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
        wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = NULL;
        wc.lpszMenuName  = NULL;
        wc.lpszClassName = TEXT("ChaosMap");

        ms_class = RegisterClass(&wc);
        ms_hinst = hinst;

        if (!ms_class)
        {
            MessageBox(NULL, TEXT("Could not register the map window class."), NULL, MB_ICONERROR);
        }
    }

    return ms_class;
}

HWND WndMap::createWindow(HINSTANCE hinst, HWND hwnd_parent)
{
    RECT rect;
    GetClientRect(hwnd_parent, &rect);

    HWND hWnd = CreateWindowEx(
        0,
        (LPCTSTR)MAKELONG(getClass(hinst), 0),
        TEXT("Map"),
        WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL,
        0,
        0,
        rect.right - rect.left,
        rect.bottom - rect.top,
        hwnd_parent,
        NULL,
        hinst,
        NULL);

    return hWnd;
}

void WndMap::resetScrollBars(HWND hwnd)
{
    HWND hwnd_parent = GetParent(hwnd);

    Level* pLevel = g_application.getLevelInstance(hwnd_parent);
    if (!pLevel)
    {
        return;
    }

    const SonicMap& map = pLevel->getMap();

    RECT rect;
    if (GetClientRect(hwnd, &rect) == TRUE)
    {
        SCROLLINFO si;
        si.cbSize = sizeof(SCROLLINFO);
        si.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
        si.nPos = 0;
        si.nPage = rect.right - rect.left;
        si.nMin = 0;
        si.nMax = map.getWidth() * pLevel->getBlockWidth();
        SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);

        si.nPage = rect.bottom - rect.top;
        si.nMax = map.getHeight() * pLevel->getBlockHeight();
        SetScrollInfo(hwnd, SB_VERT, &si, TRUE);

        WndMap::ms_x[hwnd] = 0;
        WndMap::ms_y[hwnd] = 0;
        WndMap::ms_block_width[hwnd] = pLevel->getBlockWidth();
        WndMap::ms_block_height[hwnd] = pLevel->getBlockHeight();
    }
}

void WndMap::scrollHorizontalRelPages(HWND hwnd, int rel_x)
{
    SCROLLINFO si;

    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_POS;

    if (GetScrollInfo(hwnd, SB_HORZ, &si) == TRUE)
    {
        RECT rect;
        if (GetClientRect(hwnd, &rect) == TRUE)
        {
            si.nPos += rel_x * (rect.right - rect.left);

            SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);

            if (GetScrollInfo(hwnd, SB_HORZ, &si) == TRUE)
            {
                WndMap::ms_x[hwnd] = si.nPos;
                InvalidateRect(hwnd, NULL, false);
            }
        }
    }
}

void WndMap::scrollHorizontalRel(HWND hwnd, int rel_x)
{
    SCROLLINFO si;

    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_POS;

    if (GetScrollInfo(hwnd, SB_HORZ, &si) == TRUE)
    {
        si.nPos += rel_x;

        SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);

        if (GetScrollInfo(hwnd, SB_HORZ, &si) == TRUE)
        {
            WndMap::ms_x[hwnd] = si.nPos;
            InvalidateRect(hwnd, NULL, false);
        }
    }
}

void WndMap::scrollHorizontalRefresh(HWND hwnd, short w)
{
    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_PAGE;
    si.nPage = w;

    WndMap::ms_x[hwnd] = SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
}

void WndMap::scrollVerticalRelPages(HWND hwnd, int rel_y)
{
    SCROLLINFO si;

    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_POS;

    if (GetScrollInfo(hwnd, SB_VERT, &si) == TRUE)
    {
        RECT rect;
        if (GetClientRect(hwnd, &rect) == TRUE)
        {
            si.nPos += rel_y * (rect.bottom - rect.top);

            SetScrollInfo(hwnd, SB_VERT, &si, TRUE);

            if (GetScrollInfo(hwnd, SB_VERT, &si) == TRUE)
            {
                WndMap::ms_y[hwnd] = si.nPos;
                InvalidateRect(hwnd, NULL, false);
            }
        }
    }
}

void WndMap::scrollVerticalRel(HWND hwnd, int rel_y)
{
    SCROLLINFO si;

    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_POS;

    if (GetScrollInfo(hwnd, SB_VERT, &si) == TRUE)
    {
        si.nPos += rel_y;

        SetScrollInfo(hwnd, SB_VERT, &si, TRUE);

        if (GetScrollInfo(hwnd, SB_VERT, &si) == TRUE)
        {
            WndMap::ms_y[hwnd] = si.nPos;
            InvalidateRect(hwnd, NULL, false);
        }
    }
}

void WndMap::scrollVerticalRefresh(HWND hwnd, short h)
{
    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_PAGE;
    si.nPage = h;

    WndMap::ms_y[hwnd] = SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
}

int WndMap::messageScrollHorizontal(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (LOWORD(wparam))
    {
    case SB_THUMBTRACK:
        WndMap::ms_x[hwnd] = HIWORD(wparam);
        SetScrollPos(hwnd, SB_HORZ, WndMap::ms_x[hwnd], TRUE);
        InvalidateRect(hwnd, NULL, false);
        break;

    case SB_LINEUP:
        scrollHorizontalRel(hwnd, 0 - WndMap::ms_block_height[hwnd]);
        break;

    case SB_LINEDOWN:
        scrollHorizontalRel(hwnd, WndMap::ms_block_height[hwnd]);
        break;

    case SB_PAGEUP:
        scrollHorizontalRelPages(hwnd, -1);
        break;

    case SB_PAGEDOWN:
        scrollHorizontalRelPages(hwnd, 1);
        break;
    }

    return 0;
}

int WndMap::messageScrollVertical(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (LOWORD(wparam))
    {
    case SB_THUMBTRACK:
        WndMap::ms_y[hwnd] = HIWORD(wparam);
        SetScrollPos(hwnd, SB_VERT, WndMap::ms_y[hwnd], TRUE);
        InvalidateRect(hwnd, NULL, false);
        break;

    case SB_LINEUP:
        scrollVerticalRel(hwnd, 0 - WndMap::ms_block_width[hwnd]);
        break;

    case SB_LINEDOWN:
        scrollVerticalRel(hwnd, WndMap::ms_block_width[hwnd]);
        break;

    case SB_PAGEUP:
        scrollVerticalRelPages(hwnd, -1);
        break;

    case SB_PAGEDOWN:
        scrollVerticalRelPages(hwnd, 1);
        break;
    }

    return TRUE;
}

int WndMap::messagePaint(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    HWND hwnd_parent = GetParent(hwnd);

    Level* pLevel = g_application.getLevelInstance(hwnd_parent);
    if (!pLevel)
    {
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }

    const SonicMap& map = pLevel->getMap();

    const Buffer_Patterns& patternBuffer = pLevel->getPatternBuffer();
    const Buffer_Blocks& blockBuffer = pLevel->getBlockBuffer();
    const unsigned int ms_block_width = WndMap::ms_block_width[hwnd];
    const unsigned int ms_block_height = WndMap::ms_block_height[hwnd];

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    int first_block_x = WndMap::ms_x[hwnd] / ms_block_width;
    int first_block_y = WndMap::ms_y[hwnd] / ms_block_height;

    int scroll_offset_x = WndMap::ms_x[hwnd] % ms_block_width;
    int scroll_offset_y = WndMap::ms_y[hwnd] % ms_block_height;

    if (hdc)
    {
        try
        {
            RECT rect;
            GetClientRect(hwnd, &rect);

            for (unsigned int y = first_block_y; y < map.getHeight(); y++)
            {
                int py = (y - first_block_y) * ms_block_height - scroll_offset_y;
                if (py > rect.bottom - rect.top)
                {
                    break;
                }

                for (unsigned int x = first_block_x; x < map.getWidth(); x++)
                {
                    int px = (x - first_block_x) * ms_block_width - scroll_offset_x;
                    if (px > rect.right - rect.left)
                    {
                        break;
                    }

                    blockBuffer.drawBlock(map.getValue(0, x, y), hdc, px, py);
                }
            }
        }
        catch (std::exception& e)
        {
            REPORT_ERROR(e.what(), "Drawing exception");
            PostQuitMessage(0);
        }

        EndPaint(hwnd, &ps);
    }

    return 0;
}

int WndMap::messageKeyDown(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (wparam)
    {
    case VK_LEFT:
        scrollHorizontalRel(hwnd, 0 - WndMap::ms_block_width[hwnd]);
        break;

    case VK_RIGHT:
        scrollHorizontalRel(hwnd, WndMap::ms_block_width[hwnd]);
        break;

    case VK_UP:
        scrollVerticalRel(hwnd, 0 - WndMap::ms_block_height[hwnd]);
        break;

    case VK_DOWN:
        scrollVerticalRel(hwnd, WndMap::ms_block_height[hwnd]);
        break;
    }

    return 0;
}

int WndMap::messageCreate(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    resetScrollBars(hwnd);

    return 0;
}

int WndMap::messageDestroy(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    ms_x.unset(hwnd);
    ms_y.unset(hwnd);

    return DefWindowProc(hwnd, msg, wparam, lparam);
}

int WndMap::messageMouseWheel(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    scrollVerticalRel(hwnd, 0 - GET_WHEEL_DELTA_WPARAM(wparam));
    InvalidateRect(hwnd, NULL, false);

    return 0;
}

int WndMap::messageSize(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    scrollVerticalRefresh(hwnd, HIWORD(lparam));
    scrollHorizontalRefresh(hwnd, LOWORD(lparam));
    InvalidateRect(hwnd, NULL, false);

    return 0;
}

LRESULT CALLBACK WndMap::wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_CREATE:
        return messageCreate(hwnd, msg, wparam, lparam);

    case WM_DESTROY:
        return messageDestroy(hwnd, msg, wparam, lparam);

    case WM_PAINT:
        return messagePaint(hwnd, msg, wparam, lparam);

    case WM_HSCROLL:
        return messageScrollHorizontal(hwnd, msg, wparam, lparam);

    case WM_VSCROLL:
        return messageScrollVertical(hwnd, msg, wparam, lparam);

    case WM_KEYDOWN:
        return messageKeyDown(hwnd, msg, wparam, lparam);

    case WM_MOUSEWHEEL:
        return messageMouseWheel(hwnd, msg, wparam, lparam);

    case WM_SIZE:
        return messageSize(hwnd, msg, wparam, lparam);
    }

    return DefWindowProc(hwnd, msg, wparam, lparam);
}

void WndMap::destroyClass()
{
    if (ms_class)
    {
        UnregisterClass((LPCTSTR)MAKELONG(ms_class, 0), ms_hinst);

        ms_hinst = 0;
        ms_class = 0;
    }
}
