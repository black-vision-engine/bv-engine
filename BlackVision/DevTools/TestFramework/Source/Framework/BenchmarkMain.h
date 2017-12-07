#pragma once

// ***********************
//
// Include this file in your application entry point main.cpp file.
// Remember to include it only once in whole project.

#include "benchmark/benchmark.h"

#include "TestEnvironment.h"



// ***********************
//
int main( int argc, char** argv )
{
    auto env = new bv::TestEnvironment( &argc, argv );
    bv::TestEnvironment::SetEnvironment( env );

    env->SetUp();

    ::benchmark::Initialize( &argc, argv );
    if( ::benchmark::ReportUnrecognizedArguments( argc, argv ) )
    {
        env->TearDown();
        return 1;
    }
    
    ::benchmark::RunSpecifiedBenchmarks();

    env->TearDown();
    return 0;
}

