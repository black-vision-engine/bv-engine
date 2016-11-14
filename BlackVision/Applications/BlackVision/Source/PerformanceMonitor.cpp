#include "stdafxBVApp.h"

#include "PerformanceMonitor.h"
#include "BVConfig.h"
#pragma comment(lib,"pdh.lib")
#include <cstdio>


#include "psapi.h"
#include "pdh.h"
#include <pdhmsg.h>

namespace bv{

PerformanceStats PerformanceMonitor::Stats;
PDH_HQUERY PerformanceMonitor::cpuQuery;
PDH_HCOUNTER PerformanceMonitor::cpuTotal;

PerformanceMonitor::PerformanceMonitor(void)
{
}


PerformanceMonitor::~PerformanceMonitor(void)
{
}

void PerformanceMonitor::Init()
{
    // get cpu usage
   

    static PDH_STATUS            status;
    status = PdhOpenQuery(NULL, NULL, &cpuQuery);
    if(status != ERROR_SUCCESS)
    {
        printf("error PhdOpenQuery\n");
    }
    PdhAddCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
    PdhCollectQueryData(cpuQuery);
}


void PerformanceMonitor::Calculate(bv::FrameStatsCalculator* fsc)
{
    static PDH_STATUS               status;
    MEMORYSTATUSEX                  memInfo;
    memInfo.dwLength                = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    PROCESS_MEMORY_COUNTERS_EX      pmc;

    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    //SIZE_T virtualMemUsedByMe       = pmc.PrivateUsage;

    //DWORDLONG totalVirtualMem       = memInfo.ullTotalPageFile;
    //DWORDLONG totalPhysMem          = memInfo.ullTotalPhys;
    SIZE_T physMemUsedByMe          = pmc.WorkingSetSize;
           

    PDH_FMT_COUNTERVALUE            counterVal;

    status                          = PdhCollectQueryData(cpuQuery);

    if(status != ERROR_SUCCESS)
    {
       /* printf("PhdCollectQueryData() ***Error: 0x%X\n",status);
        if(status==PDH_INVALID_HANDLE) 
            printf("PDH_INVALID_HANDLE\n");
        else if(status==PDH_NO_DATA)
            printf("PDH_NO_DATA\n");
        else
            printf("Unknown error\n");*/
    }

    PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);

    std::string S;
    
    int fps = (int)(1000.0 / (fsc->RecentSample(DefaultConfig.FrameStatsSection()).duration*1000));
    int fps_avg = (int)(1000.0 / (fsc->ExpectedValue(DefaultConfig.FrameStatsSection())*1000));
    long cpu_percent=(long)counterVal.doubleValue;
    string ram=to_string(physMemUsedByMe/(1024*1024));
    string vram="";

    Stats.cpu = to_string(cpu_percent);
    Stats.fps = to_string(fps);
    Stats.fps_avg   =   to_string(fps_avg);
    Stats.ram = ram;
    Stats.vram = vram;

}
}