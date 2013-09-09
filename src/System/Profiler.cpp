#include "Profiler.h"

#include <string>
#include <Windows.h>

#include <sys/timeb.h>

static bool gsInitializedTime = false;
static __int64 gsInitialSec = 0;
static __int64 gsInitialUSec = 0;

__int64 GetTimeInMicroseconds()
{
    struct timeb currentTime;

    if (!gsInitializedTime)
    {
        gsInitializedTime = true;
        ftime(&currentTime);
        gsInitialSec = currentTime.time;
        gsInitialUSec = 1000*currentTime.millitm;
    }

    ftime(&currentTime);
    __int64 currentSec = currentTime.time;
    __int64 currentUSec = 1000*currentTime.millitm;
    __int64 deltaSec = currentSec - gsInitialSec;
    __int64 deltaUSec = currentUSec - gsInitialUSec;
    if (deltaUSec < 0)
    {
        deltaUSec += 1000000;
        --deltaSec;
    }

    return 1000000*deltaSec + deltaUSec;
}

namespace bv
{
Profiler::Profiler(const std::string& name, std::ostream* output)
    : m_pOutput(output)
    , m_name(name)
{
    m_startTimeMicro = GetTimeInMicroseconds();
}

Profiler::~Profiler()
{
    double secs = 1e-06*(GetTimeInMicroseconds() - m_startTimeMicro);

    if(m_pOutput != nullptr)
    {
        //*m_pOutput << "Profiler " << m_name << " : Elapsed Time  = " << secs << " seconds." << std::endl;
        *m_pOutput << "Profiler " << m_name << " : Elapsed Time  = " << 1.0 / secs << " seconds." << std::endl;
    }
    else
    {
        std::cout << "Profiler " << m_name << " : Elapsed Time  = " << secs << " seconds." << std::endl;
    }
}

}