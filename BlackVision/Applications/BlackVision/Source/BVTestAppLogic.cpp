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
    // Empty. Update in test framework is called manually.
}

// *********************************
//
void    BVTestAppLogic::Update      ()
{
    m_frameStartTime = m_timer.ElapsedMillis();

    ApplicationContext::Instance().IncrementUpdateCounter();

    GetDefaultEventManager().Update( DefaultConfig.EventLoopUpdateMillis() );

    ApplicationContext::Instance().IncrementUpdateCounter();

    TimeType time = m_renderMode.StartFrame( m_frameStartTime );

    HandleFrame( time, m_renderer, m_audioRenderer );
}

} //bv
