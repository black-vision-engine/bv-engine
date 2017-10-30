#include "FrameworkTest.h"
#include "BVTestAppLogic.h"



namespace bv
{

extern HighResolutionTimer GTimer;



// ***********************
//
void            FrameworkTest::RunImpl      () const
{
    const_cast< FrameworkTest * >( this )->PreRunImpl();
    const_cast< FrameworkTest * >( this )->RunImplNotConst();
    const_cast< FrameworkTest * >( this )->PostRunImpl();
}

// ***********************
//
void            FrameworkTest::RunImplNotConst  ()
{
    m_frameTime = ComputeFrameTimeImpl();

    // Events
    PreEvents();
    m_appLogic->EventsPhase();

    // Model
    PreModelUpdate();
    m_appLogic->ModelUpdatePhase( m_frameTime );

    // Engine
    PreRender();
    m_appLogic->RenderPhase( m_frameTime, m_appLogic->m_renderer, m_appLogic->m_audioRenderer );
    PostRender();

    GTimer.StartTimer();
}

// ***********************
//
BVProjectEditor *   FrameworkTest::GetProjectEditor     () const
{
    return m_appLogic->GetBVProject()->GetProjectEditor();
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
{    m_isLastFrame = value;     }

// ***********************
//
void FrameworkTest::UseOverridenTime( bool value )
{    m_overrideTime = value;    }

// ========================================================================= //
// Internal
// ========================================================================= //

// ***********************
//
TimeType        FrameworkTest::ComputeFrameTimeImpl()
{
    if( m_overrideTime )
        return ComputeFrameTime();
    else
        return m_appLogic->ComputeFrameTime();
}

// ***********************
//
void            FrameworkTest::PreRunImpl()
{
    // Empty for now...
}

// ***********************
//
void            FrameworkTest::PostRunImpl()
{
    m_frameNum++;
}


}	// bv

