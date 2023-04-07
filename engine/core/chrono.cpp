#include "chrono.h"
using namespace deadrop;


f64 Chrono::GetTimepointDifference(Chrono::time_point start, Chrono::time_point end)
{
    auto time_difference_microseconds = std::chrono::duration_cast<std::chrono::microseconds>
        (end - start).count();
    return static_cast<f64>(time_difference_microseconds);
}

u64 Chrono::GetHighResolutionTimeSinceEpoch()
{
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::steady_clock::now().time_since_epoch()).count();
    return milliseconds;
}
