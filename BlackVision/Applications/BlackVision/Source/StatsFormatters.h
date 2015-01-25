#pragma once

#include <string>


namespace bv
{

class ProfilerDataFormatter
{
public:

    static void     PrintToConsole  ( const char * msg );
    static void     PrintToDevNull  ( const char * msg );

};

class FrameStatsFormatter
{
public:

    static void         PrintToConsole          ( const class FrameStatsCalculator & fsc, const char * name, unsigned int nameLen );
    static void         PrintToConsole          ( const class FrameStatsCalculator & fsc );

    static std::wstring FPSStatsLine            ( const class FrameStatsCalculator & fsc );

    static void         PrintFrameStatsToConsole( unsigned int frame, const class FrameStatsCalculator & fsc, const char * sectionName, unsigned int frameLen );
};


} //bv
