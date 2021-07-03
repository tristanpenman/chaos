#pragma once

#include <map>
#include <stdexcept>

#include "windows.h"

template<typename T>
class InstanceValue : public std::map<HWND, typename T>
{
public:
    const T get(HWND) const;
    T get(HWND);
    void set(HWND, T val);
    T unset(HWND);
};

template<typename T>
inline T InstanceValue<T>::get(HWND hwnd)
{
    iterator itr = find(hwnd);
    if (itr == end())
    {
        throw std::runtime_error("Instance not found");
    }

    return itr->second;
}

template<typename T>
inline const T InstanceValue<T>::get(HWND hwnd) const
{
    const_iterator itr = find(hwnd);
    if (itr == end())
    {
        throw std::runtime_error("Instance not found");
    }

    return itr->second;
}

template<typename T>
inline void InstanceValue<T>::set(HWND hwnd, T val)
{
    iterator itr = find(hwnd);
    if (itr != end())
    {
        throw std::runtime_error("Instance already exists");
    }

    insert(value_type(hwnd, val));
}

template<typename T>
inline T InstanceValue<T>::unset(HWND hwnd)
{
    iterator itr = find(hwnd);
    if (itr == end())
    {
        throw std::runtime_error("Instance not found");
    }

    T t = itr->second;
    erase(itr);
    return t;
}
