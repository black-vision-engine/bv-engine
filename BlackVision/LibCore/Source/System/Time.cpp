#include "stdafxCore.h"

#include "Time.h"

#include <Windows.h>

namespace bv
{

// *********************************
// 
UInt64       Time::Now     ()
{
    return timeGetTime();
}

} // bv