#pragma once

#include <string>


namespace bv
{

class ProfilerDataFormatter
{
public:

    static void    PrintToConsole  ();

};

class FrameStatsFormatter
{
public:

    static void         PrintToConsole  ( const class FrameStatsCalculator & fsc, const char * name );
    static void         PrintToConsole  ( const class FrameStatsCalculator & fsc );

    static std::wstring FPSStatsLine    ( const class FrameStatsCalculator & fsc );

};


} //bv
