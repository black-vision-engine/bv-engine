#include "FrameworkTest.h"
#include "BVTestAppLogic.h"



namespace bv
{

extern HighResolutionTimer GTimer;



// ***********************
//
void            FrameworkTest::RunImpl      () const
{
    const_cast< FrameworkTest * >( this )->RunImplNotConst();
}

// ***********************
//
void            FrameworkTest::RunImplNotConst  ()
{
    TimeType time = m_appLogic->ComputeFrameTime();

    // Events
    PreEvents();
    m_appLogic->EventsPhase();

    // Model
    PreModelUpdate();
    m_appLogic->ModelUpdatePhase( time );

    // Engine
    PreRender();
    m_appLogic->RenderPhase( time, m_appLogic->m_renderer, m_appLogic->m_audioRenderer );
    PostRender();

    GTimer.StartTimer();
}

// ***********************
//
void            FrameworkTest::SetAppLogic      ( BVTestAppLogic * logic )
{
    m_appLogic = logic;
}


// ========================================================================= //
// User interaction functions
// ========================================================================= //

// ***********************
//
void            FrameworkTest::EndTestAfterThisFrame        ( bool value )
{
    m_isLastFrame = value;
}


}	// bv

