#pragma once


#include "benchmark/benchmark.h"


#include "Framework/TestEnvironment.h"
#include "Framework/BVTestAppLogic.h"

#include "Framework/Benchmark.h"




namespace bv {
namespace impl
{

typedef void ( *BenchmarkLoopInit )( benchmark::State& state );

// ***********************
//
void            BenchmarkLoopImpl       ( benchmark::State& state, BenchmarkLoopInit initFun )
{
    auto env = TestEnvironment::GetEnvironment();

    // Always unload all scenes at the beginning of benchmark to be sure that engine is in clean state. 
    // Not everyone must use MAINLOOP_BENCHMARK macro.
    env->GetAppLogic()->UnloadScenes();

    // Call user defined initialization function.
    initFun( state );

    // Make one main loop step. In this step BV will create engine representation of model scenes
    // and load all needed resource on GPU.
    env->MainLoopStep();

    // Meassured code goes inside this for loop.
    for( auto _ : state )
    {
        env->MainLoopStep();
    }

    // Be good citizen of Framework and unload all scenes.
    env->GetAppLogic()->UnloadScenes();
}

}


}	// bv


#define MAINLOOP_BENCHMARK( FunctionName )                              \
void        FunctionName ## _LoopBench      ( benchmark::State& state ) \
{    ::bv::impl::BenchmarkLoopImpl( state, &FunctionName );     }       \
BV_BENCHMARK( FunctionName ## _LoopBench )


