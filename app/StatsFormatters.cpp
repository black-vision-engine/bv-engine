#include "StatsFormatters.h"

#include <sstream>

#include "System/HerarchicalProfiler.h"
#include "FrameStatsService.h"

#include "BVConfig.h"


namespace bv
{

// *********************************
//
void    ProfilerDataFormatter::PrintToConsole  ()
{
    unsigned int frame = HPROFILER_GET_ACTIVE_FRAME();
    const ProfilerSample * samples = HPROFILER_GET_ONE_FRAME_SAMPLES( frame );
    unsigned int numSamples = HPROFILER_GET_NUM_SAMPLES();

    for( unsigned int i = 0; i < numSamples; ++i )
    {
        const ProfilerSample & sample = samples[ i ];
        const char * section = sample.type == AutoProfileType::APT_FUNCTION ? "F" : "S";

        for( unsigned int k = 0; k < sample.depth * 2; ++k )
            printf( " " );

        printf( "%s %s    %2.4f ms\n", section, sample.name, sample.durationSecs * 1000.0 );
        //printf( "%*s %s duration: %2.4f ms\n", sample.depth * 6, section, sample.name, sample.durationSecs * 1000.0 );
    }
}

// *********************************
//
void    FrameStatsFormatter::PrintToConsole     ( const FrameStatsCalculator & fsc, const char * name )
{
}

// *********************************
//
void    FrameStatsFormatter::PrintToConsole     ( const FrameStatsCalculator & fsc )
{
}

// *********************************
//
std::wstring FrameStatsFormatter::FPSStatsLine  ( const FrameStatsCalculator & fsc )
{
    const char * fnm = DefaultConfig.FrameStatsSection();

    auto sample = fsc.RecentSample( fnm );

    double lastDuration = sample.duration * 1000.f;
    double avgDuration = fsc.ExpectedValue( fnm ) * 1000.0;
    double dev = sqrt( fsc.Variance( fnm ) * 1000.0 );

    unsigned int minFrame = 0;
    unsigned int maxFrame = 0;
    double minDuration = fsc.MinVal( fnm, &minFrame ) * 1000.0;
    double maxDuration = fsc.MaxVal( fnm, &maxFrame ) * 1000.0;

    unsigned long numSamples = fsc.WindowSize();

    std::ostringstream  s;

    s.precision( 4 );
    s << "FPS: " << 1000.0 / lastDuration << " frame time: " << lastDuration << " ms | Stats[" << numSamples << "]->";
    s << " FPS: " << 1000.0 / avgDuration << " dura - avg: " << avgDuration << " ms dev: " << dev;
    s << " ms min: " << minDuration << " ms at " << minFrame << " max " << maxDuration <<  " ms at " << maxFrame << std::endl;

    std::string ss = s.str();
    std::wstring stemp = std::wstring( ss.begin(), ss.end() );

    return stemp;
}

} //bv
