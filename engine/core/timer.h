#pragma once
#include "types.h"
#include "chrono.h"

namespace deadrop
{
    // a high-resolution timer class implemented using the Chrono class
    class Timer
    {
    public:
        // the constructor initializes the start timepoint
        Timer();

        // returns the amount of time that has passed since the timer started in microseconds
        // NOTE: do not call this function directly after calling Reset(),
        // it will most likely return zero or very small values
        f64 GetElapsedTime();

        // resets the timer causing to start again
        void Reset();

    private:
        // keep track of the starting time
        Chrono::time_point m_start_time;
    };

    Timer::Timer()
    {
        // initialize the start time
        m_start_time = Chrono::GetHighResolutionNowTimepoint();
    }

    f64 Timer::GetElapsedTime()
    {
        // return the time difference in microseconds
        return Chrono::GetTimepointDifference(m_start_time, Chrono::GetHighResolutionNowTimepoint());
    }

    void Timer::Reset()
    {
        // reset the timer so that the starting time is now
        m_start_time = Chrono::GetHighResolutionNowTimepoint();
    }
}