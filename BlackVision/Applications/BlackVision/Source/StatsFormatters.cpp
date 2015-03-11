#include "StatsFormatters.h"

#include <sstream>
#include <algorithm>

#include "Tools/HerarchicalProfiler.h"
#include "FrameStatsService.h"

#include "BVConfig.h"


namespace bv
{

// *********************************
//
void    ProfilerDataFormatter::PrintToConsole  ( const char * msg )
{
    unsigned int frame = HPROFILER_GET_ACTIVE_FRAME();
    const ProfilerSample * samples = HPROFILER_GET_ONE_FRAME_SAMPLES( frame );
    unsigned int numSamples = HPROFILER_GET_NUM_SAMPLES();

    printf( "%s\n", msg );

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
void    ProfilerDataFormatter::PrintToDevNull   ( const char * msg )
{
    { msg; } // FIXME: suppress unused warning
}

// *********************************
//
void    FrameStatsFormatter::PrintToConsole     ( const FrameStatsCalculator & fsc, const char * name, unsigned int nameLen )
{
    unsigned int minFrame = 0;
    unsigned int maxFrame = 0;

    double minDuration = fsc.MinVal( name, &minFrame ) * 1000.0;
    double maxDuration = fsc.MaxVal( name, &maxFrame ) * 1000.0;
    double avg = fsc.ExpectedValue( name ) * 1000.0;
    double dev = sqrt( fsc.Variance( name ) ) * 1000.0;

    printf( "%*s: Min[%6d] %3.4f ms Max[%6d] %3.4f ms (%3.4f ms / %3.4f ms)\n", nameLen, name, minFrame, minDuration, maxFrame, maxDuration, avg, dev );
}

// *********************************
//
void    FrameStatsFormatter::PrintToConsole     ( const FrameStatsCalculator & fsc )
{
    unsigned int maxLen = 0;
    for( auto name : fsc.RegisteredSections() )
    {
        maxLen = std::max( maxLen, (unsigned int)strlen( name ) );
    }

    PrintToConsole( fsc, DefaultConfig.FrameStatsSection(), maxLen );
    for( auto name : fsc.RegisteredSections() )
    {
        if( name != DefaultConfig.FrameStatsSection() )
            PrintToConsole( fsc, name, maxLen );
    }

    unsigned int minFrame = 0;
    unsigned int maxFrame = 0;

    fsc.MinVal( DefaultConfig.FrameStatsSection(), &minFrame );
    fsc.MaxVal( DefaultConfig.FrameStatsSection(), &maxFrame );

    PrintFrameStatsToConsole( minFrame, fsc, "MinFrame", maxLen );
    PrintFrameStatsToConsole( maxFrame, fsc, "MaxFrame", maxLen );
}

// *********************************
//
std::wstring FrameStatsFormatter::FPSStatsLine  ( const FrameStatsCalculator & fsc )
{
    const char * fnm = DefaultConfig.FrameStatsSection();

    auto sample = fsc.RecentSample( fnm );

    double lastDuration = sample.duration * 1000.f;
    double avgDuration = fsc.ExpectedValue( fnm ) * 1000.0;
    double dev = sqrt( fsc.Variance( fnm ) * 1000.0 * 1000.0 );

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

// *********************************
//
void  FrameStatsFormatter::PrintFrameStatsToConsole( unsigned int frame, const FrameStatsCalculator & fsc, const char * sectionName, unsigned int frameLen )
{
    auto stats = fsc.FrameStats( frame );

    printf( "Frame stats for frame %d - %s: ", frame, sectionName );

    for( auto stat : stats )
    {
        if( stat.first == DefaultConfig.FrameStatsSection() )
            printf( "%3.4f ms \n", stat.second.duration * 1000.0 );
    }

    for( auto stat : stats )
    {
        if( stat.first != DefaultConfig.FrameStatsSection() )
            printf( "    %*s: %3.4f ms\n", frameLen, stat.first, stat.second.duration * 1000.0 );
    }
}

} //bv
