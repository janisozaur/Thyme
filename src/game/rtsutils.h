////////////////////////////////////////////////////////////////////////////////
//                               --  THYME  --                                //
////////////////////////////////////////////////////////////////////////////////
//
//  Project Name:: Thyme
//
//          File:: RTSUTILS.H
//
//        Author:: CCHyper
//
//  Contributors:: OmniBlade
//
//   Description:: 
//
//       License:: Thyme is free software: you can redistribute it and/or 
//                 modify it under the terms of the GNU General Public License 
//                 as published by the Free Software Foundation, either version 
//                 2 of the License, or (at your option) any later version.
//
//                 A full copy of the GNU General Public License can be found in
//                 LICENSE
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

#ifndef RTSUTILS_H
#define RTSUTILS_H

#include "endiantype.h"

namespace rts {

// Less than comparator for STL containers.
// Use only for string classes.
template<typename T>
struct less_than_nocase
{
    typedef T first_argument_type;
    typedef T second_argument_type;
    typedef bool result_type;

    constexpr bool operator()(const T &left, const T &right) const
    {
        return (left.Compare_No_Case(right) < 0);
    }
};

template <int a, int b, int c, int d>
struct FourCC
{
#ifdef SYSTEM_LITTLE_ENDIAN
    static const uint32_t value = (((((d << 8) | c) << 8) | b) << 8) | a;
#else
    static const uint32_t value = (((((a << 8) | b) << 8) | c) << 8) | d;
#endif
};

} // namespace rts

#endif // _RTSUTILS_H
