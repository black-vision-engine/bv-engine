#include "StatsFormatters.h"

#include "System/HerarchicalProfiler.h"
#include "FrameStatsService.h"


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

} //bv
