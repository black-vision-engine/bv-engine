#include "stdafxBVApp.h"

#include "BVTestAppLogic.h"

#include "Application/ApplicationContext.h"

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
void    BVTestAppLogic::OnUpdate    ( Renderer * , audio::AudioRenderer * )
{
    // Empty. Updating in test framework is called manually.
}
// *********************************
//
void    BVTestAppLogic::LoadScene   ()
{
    // Empty. Test  Framework doesn't load any scene by default.
}

// *********************************
//
void    BVTestAppLogic::Update      ( unsigned int millis )
{
    m_frameStartTime = millis;

    ApplicationContext::Instance().IncrementUpdateCounter();

    GetDefaultEventManager().Update( DefaultConfig.EventLoopUpdateMillis() );

    ApplicationContext::Instance().IncrementUpdateCounter();

    TimeType time = m_renderMode.StartFrame( m_frameStartTime );

    HandleFrame( time, m_renderer, m_audioRenderer );
}

} //bv
