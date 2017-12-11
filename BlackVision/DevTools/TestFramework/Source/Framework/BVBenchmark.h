#pragma once

#include "benchmark/benchmark.h"


#include <algorithm>




// ***********************
//
inline double       MaxValStatistic     ( const std::vector< double >& v )
{
    return *( std::max_element( std::begin( v ), std::end( v ) ) );
}

// ***********************
//
inline double       MinValStatistic     ( const std::vector< double >& v )
{
    return *( std::min_element( std::begin( v ), std::end( v ) ) );
}


#define BV_BENCHMARK( FunctionName ) BENCHMARK( FunctionName )  \
->ReportAggregatesOnly( true )                                  \
->ComputeStatistics( "Max", &MaxValStatistic )                  \
->ComputeStatistics( "Min", &MinValStatistic )                  \
->Unit( benchmark::TimeUnit::kMillisecond )


