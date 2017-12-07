#include "Benchmark.h"

#include "BVTestAppLogic.h"


namespace bv
{

extern HighResolutionTimer  GTimer;



// ***********************
//
Benchmark::Benchmark()
{
    EndTestAfterThisFrame( false );
}



// ***********************
//
void            Benchmark::Run      ()
{
    TimeType time = ComputeFrameTime();

    GetAppLogic()->EventsPhase();
    GetAppLogic()->ModelUpdatePhase( time );
    GetAppLogic()->RenderPhase( time, GetAppLogic()->m_renderer, GetAppLogic()->m_audioRenderer );

    GTimer.StartTimer();
}


}	// bv


