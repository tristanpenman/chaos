#pragma once

struct NativeColor
{
    NativeColor()
      : r(0)
      , g(0)
      , b(0)
    {

    }

    NativeColor(unsigned char r, unsigned char g, unsigned char b)
      : r(r)
      , g(g)
      , b(b)
    {

    }

    unsigned char r;
    unsigned char g;
    unsigned char b;
};
