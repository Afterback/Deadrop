#pragma once
#include "types.h"
#include <chrono>
// NOTE: this file takes parameters as chrono literals 
// to avoid using the wrong time representation
using namespace std::chrono_literals;

// for implementing std::cout support
#include <ostream>

namespace deadrop
{
    // NOTE: HMSTime can actually represent negative values
    struct HMSTime
    {
        // signed 16-bit hours can store up to (65535/2) hours,
        // or approx (2730/2 days), or (7.47/2) years
        i16 hours;
        // minutes stores a max of 60
        i8 minutes;
        // seconds stores a max of 60
        i8 seconds;
    };

    // A helper function to make it easier to correctly std::cout an HMSTime
    std::ostream& operator<<(std::ostream& os, const HMSTime& hmst)
    {
        // NOTE: we have to cast everything to an int else it would print characters
        // instead of actual integer values
        os <<
            static_cast<int>(hmst.hours) << ':' <<
            static_cast<int>(hmst.minutes) << ':' <<
            static_cast<int>(hmst.seconds);
        return os;
    }

    class Time
    {
    public:
        // TimeLeftUntil compares the current time (unix_time) and unix_time param and returns an HMSTime
        // with the time left (HMSTime is positive), or time passed since (HMSTime is negative)
        // NOTE: it will return negative values in HMSTime if the time has passed.
        // NOTE: use HasPassed() to check if the returned value means time left or time passed.
        [[nodiscard]]
        static HMSTime TimeLeftUntil(std::chrono::seconds unix_time)
        {
            auto now = std::chrono::system_clock::now();
            auto now_duration = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch());
            auto until_duration = std::chrono::duration(std::chrono::seconds(unix_time));
            auto left_duration = until_duration - now_duration;
            return HMSTime
            {
                static_cast<i16>(std::chrono::duration_cast<std::chrono::hours>(left_duration).count()),
                static_cast<i8>(std::chrono::duration_cast<std::chrono::minutes>(left_duration % std::chrono::hours(1)).count()),
                static_cast<i8>(std::chrono::duration_cast<std::chrono::seconds>(left_duration % std::chrono::minutes(1)).count())
            };
        }

        // NowUnixTime returns the number of seconds since epoch time (00:00:00 UTC on 1 January 1970)
        // NOTE: can be used to track the number of seconds that passed, by subtracting the return values of two calls.
        [[nodiscard]]
        static u64 NowUnixTime()
        {
            auto now_unix = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch());
            return now_unix.count();
        }

        // HasPassed returns true if the provided HMSTime contains time that has elapsed, else returns false
        // NOTE: this function can be used to check if the HMSTime returned by other functions represent time left, or time elapsed.
        [[nodiscard]]
        constexpr static bool HasPassed(const HMSTime& hmst)
        {
            // if one member is negative, it means the time has passed
            // NOTE: one member can be negative while the others aren't,
            // that why we use && operator
            if (hmst.hours > 0 && hmst.minutes > 0 && hmst.seconds > 0)
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    };
}