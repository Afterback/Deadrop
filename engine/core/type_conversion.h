#pragma once
#include "types.h"
#include <cstring> // for memcpy
#include <array>

namespace deadrop
{
    // compose a u32 integer from 4 u8 integers
    inline constexpr u32 u32FromChars(u8 a, u8 b, u8 c, u8 d)
    {
        // make sure that the size of u32 is 4 bytes and u8 is 1 byte
        static_assert(sizeof(u32) == 4 && sizeof(u8) == 1, "u32 is not 4 bytes long, or u8 is not 1 byte long!");
        // NOTE: this sort of conversion only works with unsigned types!
        u32 value =
            (d << 24) |
            (c << 16) |
            (b << 8)  |
            (a);
        return value;
    }

    // compose a u32 integer from an array of four u8 integers
    inline constexpr u32 u32FromChars(const std::array<u8, 4>& bytes)
    {
        return u32FromChars(bytes[0], bytes[1], bytes[2], bytes[3]);
    }

    // decompose a u32 integer into four u8 integers array
    inline std::array<u8, 4> u32ToChars(u32 v)
    {
        // make sure that the size of u32 is 4 bytes and u8 is 1 byte
        static_assert(sizeof(u32) == 4 && sizeof(u8) == 1, "u32 is not 4 bytes long, or u8 is not 1 byte long!");

        std::array<u8, 4> value_array;
        memcpy(&value_array[0], &v, sizeof(u32));
        return value_array;
    }

    // compose a u64 integer from two u32 integers
    inline constexpr u64 u64FromU32(u32 a, u32 b)
    {
        static_assert(sizeof(u64) == 8 && sizeof(u32) == 4, "u64 is not 8 bytes long, or u32 is not 4 bytes long");

        u64 value = (u64)b << 32 | (u64)a;
        return value;
    }

    // compose a u64 integer from an array of two u32 integers
    inline constexpr u64 u64FromU32(const std::array<u32, 2>& bytes)
    {
        return u64FromU32(
            bytes[0], bytes[1]
        );
    }

    // decompose a u64 integer into an array of two u32 integers
    inline std::array<u32, 2> u64ToU32(u64 v)
    {
        static_assert(sizeof(u64) == 8 && sizeof(u32) == 4, "u64 is not 8 bytes long, or u32 is not 4 bytes long");

        std::array<u32, 2> value_array;
        memcpy(&value_array[0], &v, sizeof(u64));
        return value_array;
    }
}
