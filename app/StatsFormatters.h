#pragma once

namespace bv
{

class ProfilerDataFormatter
{
public:

    void    PrintToConsole  ();

};

class FrameStatsFormatter
{
public:

    void    PrintToConsole  ( const class FrameStatsCalculator & fsc, const char * name );
    void    PrintToConsole  ( const class FrameStatsCalculator & fsc );

};


} //bv
