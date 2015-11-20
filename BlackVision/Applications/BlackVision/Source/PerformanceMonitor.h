#include "win_sock.h"

#include "psapi.h"
#include "pdh.h"
#include <pdhmsg.h>
#include "FrameStatsService.h"

#include <string>

#pragma once

namespace bv{
using std::string;
using std::to_string;


class PerformanceStats
{
public:
    string cpu;
    string ram;
    string vram;
    string fps;
    string fps_avg;
};
class PerformanceMonitor
{
private:
    static PDH_HQUERY cpuQuery;
    static PDH_HCOUNTER cpuTotal;
    
public:
    PerformanceMonitor(void);
    ~PerformanceMonitor(void);
    static void Init();
    static void Calculate(FrameStatsCalculator* fsc);
    static PerformanceStats Stats;
};

}