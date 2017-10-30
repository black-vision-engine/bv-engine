#include "stdafxBVApp.h"

#include "BVTestAppLogic.h"

#include "Application/ApplicationContext.h"

#include "FrameworkTest.h"


#include <limits>


namespace bv
{

// *********************************
//
BVTestAppLogic::BVTestAppLogic      ( Renderer * renderer, audio::AudioRenderer * audioRenderer )
    : BVAppLogic( renderer, audioRenderer )
{}

// *********************************
//
BVTestAppLogic::~BVTestAppLogic     ()
{}

// *********************************
//
void            BVTestAppLogic::OnUpdate    ( Renderer * , audio::AudioRenderer * )
{
    // Note: we can't restart timer earlier, because AppLogic isn't initialized.
    static bool firstExecution = true;
    if( firstExecution )
    {
        RestartTimer();
        firstExecution = false;
    }

    m_test->Run();
}

// ***********************
//
void            BVTestAppLogic::EndExecution()
{
    // This function should close application and release everything.
    ChangeState( BVAppState::BVS_CLOSING );
}


// *********************************
//
void            BVTestAppLogic::LoadScene   ()
{
    // Empty. Test  Framework doesn't load any scene by default.
}

// ***********************
//
void            BVTestAppLogic::PostFrameLogic()
{
    // Empty. Test Framework doesn't compute frame statistics.
}

// ***********************
//
void            BVTestAppLogic::RestartTimer()
{
    StartTime();
    m_renderMode.SetOffscreenRenderMode( 50.0f, std::numeric_limits< UInt64 >::max() );
}

// ***********************
//
void            BVTestAppLogic::InjectTest  ( FrameworkTest * test )
{
    m_test = test;
}


} //bv
