#include "timer.h"
using namespace deadrop;

Timer::Timer()
{
    // initialize the start time
    m_start_time = Chrono::Now();
}

f64 Timer::GetElapsedTime()
{
    // return the time difference in microseconds
    return Chrono::GetTimeDiffInMicro(m_start_time, Chrono::Now());
}

void Timer::Reset()
{
    // reset the timer so that the starting time is now
    m_start_time = Chrono::Now();
}
