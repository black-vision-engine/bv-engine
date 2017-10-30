#include "FrameworkTest.h"
#include "BVTestAppLogic.h"

#include "TestEnvironment.h"


namespace bv
{

extern HighResolutionTimer  GTimer;


// ***********************
//
void            FrameworkTest::Run              ()
{
    const_cast< FrameworkTest * >( this )->PreRunImpl();
    const_cast< FrameworkTest * >( this )->RunImpl();
    const_cast< FrameworkTest * >( this )->PostRunImpl();
}

// ***********************
//
void            FrameworkTest::RunImpl          ()
{
    m_frameTime = ComputeFrameTimeImpl();

    // Events
    PreEvents();
    if( HandleFailure() ) return;
    
    m_appLogic->EventsPhase();

    // Model
    PreModelUpdate();
    if( HandleFailure() ) return;

    m_appLogic->ModelUpdatePhase( m_frameTime );

    // Engine
    PreRender();
    if( HandleFailure() ) return;

    m_appLogic->RenderPhase( m_frameTime, m_appLogic->m_renderer, m_appLogic->m_audioRenderer );

    PostRender();
    if( HandleFailure() ) return;

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

// ***********************
//
bool            FrameworkTest::HandleFailure()
{
    if( HasFatalFailure() )
    {
        EndTestAfterThisFrame( true );
        return true;
    }
    
    return false;
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

// ***********************
//
void            FrameworkTest::TestBody     ()
{
    do
    {
        bool end = TestEnvironment::GetEnvironment()->MainLoopStep();
        { end; }    // Maybe we can use this in future.

    } while( !IsLastFrame() );
}

// ***********************
//
void            FrameworkTest::SetUp        ()
{
    SetAppLogic( TestEnvironment::GetEnvironment()->GetAppLogic() );

    m_appLogic->InjectTest( this );
}

// ***********************
//
void            FrameworkTest::TearDown     ()
{
    // Clean engine before next test.
    m_appLogic->UnloadScenes();
    m_appLogic->RestartTimer();

    // To be sure.
    m_appLogic->InjectTest( nullptr );
}

}	// bv

