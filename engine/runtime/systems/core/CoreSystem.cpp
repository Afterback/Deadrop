#include "CoreSystem.h"
using namespace deadrop::systems;

CoreSystem& detail::instance()
{
    // make sure there is only one instace, created on the first call
    static CoreSystem s_intance;
    return s_intance;
}