#include "stdafxBVApp.h"

#include "TestAIManager.h"

#include <cassert>

#include "testai/TestAI.h"
#include "testai/AICommands.h"

#include "BVAppLogic.h"

#include "Engine/Models/Timeline/Dynamic/DefaultTimeline.h"

#include "Engine/Models/Timeline/Dynamic/TimelineEventLoop.h"
#include "Engine/Models/Timeline/Dynamic/TimelineEventNull.h"
#include "Engine/Models/Timeline/Dynamic/TimelineEventStop.h"


namespace bv {

// *********************************
//
TestAIManager::TestAIManager   ()
{
    m_presets.reserve( 7 );
    m_timelines.reserve( 7 );

    for( unsigned int i = 0; i < 7; ++i )
    {
        m_presets.push_back( nullptr );
        m_timelines.push_back( nullptr );
    }
}

// *********************************
//
TestAIManager::~TestAIManager  ()
{
    for( auto ai : m_presets )
    {
        delete ai;
    }

    for( auto tm : m_timelines )
    {
        delete tm;
    }
}

// *********************************
//
unsigned int    TestAIManager::NumAIPresets () const
{
    return (unsigned int) m_presets.size();
}

TestAI *        TestAIManager::GetAIPreset         ( unsigned int idx, BVAppLogic * logic )
{
    assert( idx == 4 || idx == 5 );

    if( m_presets[ idx ] == nullptr )
    {
        m_presets[ idx ] = PreparePreset( idx, logic );
    }

    return m_presets[ idx ];
}

// *********************************
//
TestAI *        TestAIManager::GetAIPreset  ( unsigned int idx, model::IModelNodePtr node )
{
    assert( idx < NumAIPresets() );

    if( m_presets[ idx ] == nullptr )
    {
        if( idx == 3 || idx == 6 )
            m_presets[ idx ] = PreparePreset( idx, node );
        else
            m_presets[ idx ] = PreparePreset( idx );
    }

    return m_presets[ idx ];
}

// *********************************
//
TestAI *        TestAIManager::PreparePreset    ( unsigned int idx, model::IModelNodePtr node ) const
{
    if( idx == 0 )
    {
        return PreparePreset0();
    }
    else if( idx == 1 )
    {
        return PreparePreset1();
    }
    else if( idx == 2 )
    {
        return PreparePreset2();    
    }
    else if( idx == 3 )
    {
        return PreparePreset3( node );
    }
    else if( idx == 6 )
    {
        return PreparePreset6( node );
    }
    return nullptr;
}

TestAI *        TestAIManager::PreparePreset    ( unsigned int idx, BVAppLogic * logic ) const
{
    if( idx == 4 )
    {
        return PreparePreset4( logic );
    }
    else if( idx == 5 )
    {
        return PreparePreset5( logic );
    }

    return nullptr;
}

// *********************************
//
TestAI *        TestAIManager::PreparePreset0   () const
{
    auto timeline = model::DefaultTimeline::Create( "timeline preset 0", TimeType( 30.0 ), TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_CLAMP );
    timeline->AddKeyFrame( model::TimelineEventLoop::Create( "reverse 2 at 0.0 ", TimeType( 0.0 ), LoopEventAction::LEA_REVERSE, 2, TimeType( 1.0 ), timeline.get() ) );
    timeline->AddKeyFrame( model::TimelineEventStop::Create( "stop at 2.0      ", TimeType( 2.0 ), timeline.get() ) );
    timeline->AddKeyFrame( model::TimelineEventLoop::Create( "reverse 1 at 4.0 ", TimeType( 4.0 ), LoopEventAction::LEA_REVERSE, 1, TimeType( 1.0 ), timeline.get() ) );
    timeline->AddKeyFrame( model::TimelineEventStop::Create( "stop at 6.0      ", TimeType( 6.0 ), timeline.get() ) );
    timeline->AddKeyFrame( model::TimelineEventLoop::Create( "restart 1 at 9.0 ", TimeType( 9.0 ), LoopEventAction::LEA_RESTART, 1, TimeType( 1.0 ), timeline.get() ) );
    timeline->AddKeyFrame( model::TimelineEventLoop::Create( "reverse 1 at 12.0", TimeType( 12.0 ), LoopEventAction::LEA_REVERSE, 1, TimeType( 1.0 ), timeline.get() ) );
    timeline->AddKeyFrame( model::TimelineEventStop::Create( "stop at 15.0     ", TimeType( 15.0 ), timeline.get() ) );

    //auto c0 = new AICommandSetTimeAndPlay( timeline, TimeType( 1.0 ), TimeType( 2.0 ) );
    auto c0 = new AICommandPlay( timeline, TimeType( 1.0 ) );
    auto c1 = new AICommandPlay( timeline, TimeType( 5.0 ) );
    auto c2 = new AICommandPlay( timeline, TimeType( 11.0 ) );
    auto c3 = new AICommandPlay( timeline, TimeType( 16.0 ) );
    auto c4 = new AICommandPlay( timeline, TimeType( 20.5 ) );
    auto c5 = new AICommandPlay( timeline, TimeType( 25.0 ) );
    auto c6 = new AICommandPlay( timeline, TimeType( 30.0 ) );
    auto c7 = new AICommandPlay( timeline, TimeType( 37.0 ) );
    auto c8 = new AICommandPlay( timeline, TimeType( 51.0 ) );
    auto c9 = new AICommandPlay( timeline, TimeType( 56.0 ) );
    auto c10 = new AICommandPlay( timeline, TimeType( 61.0 ) );
    auto c11 = new AICommandPlay( timeline, TimeType( 66.0 ) );
    auto c12 = new AICommandPlay( timeline, TimeType( 76.0 ) );

    TestAI * ai = new TestAI( timeline, nullptr );

    ai->AddCommand( c0 );
    ai->AddCommand( c1 );
    ai->AddCommand( c2 );
    ai->AddCommand( c3 );
    ai->AddCommand( c4 );
    ai->AddCommand( c5 );
    ai->AddCommand( c6 );
    ai->AddCommand( c7 );
    ai->AddCommand( c8 );
    ai->AddCommand( c9 );
    ai->AddCommand( c10 );
    ai->AddCommand( c11 );
    ai->AddCommand( c12 );

    return ai;
}

// *********************************
//FIXME: test and fix in runtime
TestAI *        TestAIManager::PreparePreset1   () const
{
    auto timeline = model::DefaultTimeline::Create( "timeline preset 0", TimeType( 30.0 ), TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_CLAMP );
    timeline->AddKeyFrame( model::TimelineEventLoop::Create( "reverse 2 at 0.0 ", TimeType( 0.0 ), LoopEventAction::LEA_REVERSE, 2, TimeType( 1.0 ), timeline.get() ) );
    timeline->AddKeyFrame( model::TimelineEventStop::Create( "stop at 2.0      ", TimeType( 2.0 ), timeline.get() ) );
    timeline->AddKeyFrame( model::TimelineEventLoop::Create( "reverse 1 at 4.0 ", TimeType( 4.0 ), LoopEventAction::LEA_REVERSE, 1, TimeType( 1.0 ), timeline.get() ) );
    timeline->AddKeyFrame( model::TimelineEventStop::Create( "stop at 6.0      ", TimeType( 6.0 ), timeline.get() ) );
    timeline->AddKeyFrame( model::TimelineEventLoop::Create( "restart 1 at 9.0 ", TimeType( 9.0 ), LoopEventAction::LEA_RESTART, 1, TimeType( 1.0 ), timeline.get() ) );
    timeline->AddKeyFrame( model::TimelineEventLoop::Create( "reverse 1 at 12.0", TimeType( 12.0 ), LoopEventAction::LEA_REVERSE, 1, TimeType( 1.0 ), timeline.get() ) );
    timeline->AddKeyFrame( model::TimelineEventStop::Create( "stop at 15.0     ", TimeType( 15.0 ), timeline.get() ) );

    //auto c0 = new AICommandSetTimeAndPlay( timeline, TimeType( 1.0 ), TimeType( 2.0 ) );
    auto c0 = new AICommandPlay( timeline, TimeType( 1.0 ) );
    auto c1 = new AICommandSetTimeAndStop( timeline, TimeType( 4.0 ), TimeType( 6.0 ) ); //start at stop, target stop STOP
    auto c2 = new AICommandPlay( timeline, TimeType( 6.0 ) );
    auto c3 = new AICommandSetTimeAndPlay( timeline, TimeType( 13.0 ), TimeType( 2.0 ) ); //start at stop, target self stop PLAY
    auto c4 = new AICommandSetTimeAndPlay( timeline, TimeType( 18.0 ), TimeType( 15.0 ) ); //start at stop, target stop PLAY
    auto c5 = new AICommandSetTimeAndStop( timeline, TimeType( 26.0 ), TimeType( 5.0 ) ); //start at stop, target casual STOP
    auto c6 = new AICommandSetTimeAndPlay( timeline, TimeType( 28.0 ), TimeType( 8.0 ) ); //start at casual, target casual PLAY
    auto c7 = new AICommandSetTimeAndPlay( timeline, TimeType( 37.0 ), TimeType( 2.0 ) ); //start at casual, target stop PLAY
    auto c8 = new AICommandReverse( timeline, TimeType( 42.0 ) ); //reverse at STOP
    auto c9 = new AICommandPlay( timeline, TimeType( 43.0 ) ); //reverse at STOP
    auto c10 = new AICommandReverse( timeline, TimeType( 45.0 ) ); //reverse at PLAY
    auto c11 = new AICommandStop( timeline, TimeType( 46.0 ) ); //STOP at PLAY
    auto c12 = new AICommandPlay( timeline, TimeType( 48.0 ) ); //PLAY after stop
    auto c13 = new AICommandStop( timeline, TimeType( 50.0 ) ); //STOP at STOP
    auto c14 = new AICommandPlay( timeline, TimeType( 52.0 ) ); //PLAY at STOP
    auto c15 = new AICommandStop( timeline, TimeType( 62.0 ) ); //STOP at STOP
    auto c16 = new AICommandSetTimeAndStop( timeline, TimeType( 64.0 ), TimeType( 22.0 ) );
    auto c17 = new AICommandSetTimeAndPlay( timeline, TimeType( 66.0 ), TimeType( 13.0 ) );
    auto c18 = new AICommandPlay( timeline, TimeType( 71.0 ) );

    TestAI * ai = new TestAI( timeline, nullptr );

    ai->AddCommand( c0 );
    ai->AddCommand( c1 );
    ai->AddCommand( c2 );
    ai->AddCommand( c3 );
    ai->AddCommand( c4 );
    ai->AddCommand( c5 );
    ai->AddCommand( c6 );
    ai->AddCommand( c7 );
    ai->AddCommand( c8 );
    ai->AddCommand( c9 );
    ai->AddCommand( c10 );
    ai->AddCommand( c11 );
    ai->AddCommand( c12 );
    ai->AddCommand( c13 );
    ai->AddCommand( c14 );
    ai->AddCommand( c15 );
    ai->AddCommand( c16 );
    ai->AddCommand( c17 );
    ai->AddCommand( c18 );

    return ai;
}

// *********************************
//
TestAI *        TestAIManager::PreparePreset2   () const
{
    auto timeline = model::DefaultTimeline::Create( "timeline preset 0", TimeType( 20.0 ), TimelineWrapMethod::TWM_REPEAT, TimelineWrapMethod::TWM_CLAMP );
    timeline->AddKeyFrame( model::TimelineEventStop::Create( "stop0", TimeType( 2.0 ), timeline.get() ) );
    auto c1 = new AICommandPlay( timeline, TimeType( 0.2 ) );
    auto c3 = new AICommandSetTimeAndStop( timeline, TimeType( 0.5 ), TimeType( 7.5 ) );
    //auto c4 = new AICommandSetTimeAndStop( timeline, TimeType( 3.0 ), TimeType( 3.5 ) );

    TestAI * ai = new TestAI( timeline, nullptr );

    ai->AddCommand( c1 );
    ai->AddCommand( c3 );
    //ai->AddCommand( c4 );

    return ai;
#if OLD_FUCK_OFFFFFF
    auto timeline = model::DefaultTimelinePtr( new model::DefaultTimeline( "timeline preset 0", TimeType( 20.0 ), TimelineWrapMethod::TWM_REPEAT, TimelineWrapMethod::TWM_CLAMP ) );
    timeline->AddKeyFrame( new model::TimelineEventStop( "stop0", TimeType( 5.0 ), timeline.get() ) );
    timeline->AddKeyFrame( new model::TimelineEventStop( "stop1", TimeType( 10.0 ), timeline.get() ) );

    auto c0 = new AICommandPlay( timeline, TimeType( 1.0 ) );
    auto c1 = new AICommandPlay( timeline, TimeType( 7.0 ) );
    auto c2 = new AICommandPlay( timeline, TimeType( 16.5 ) );
    auto c3 = new AICommandSetTimeAndStop( timeline, TimeType( 25.0 ), TimeType( 3.5 ) );
    auto c4 = new AICommandPlay( timeline, TimeType( 27.0 ) );
    auto c5 = new AICommandPlay( timeline, TimeType( 35.0 ) );
    auto c6 = new AICommandPlay( timeline, TimeType( 40.0 ) );

    TestAI * ai = new TestAI( timeline, nullptr );

    ai->AddCommand( c0 );
    ai->AddCommand( c1 );
    ai->AddCommand( c2 );
    ai->AddCommand( c3 );
    ai->AddCommand( c4 );
    ai->AddCommand( c5 );
    ai->AddCommand( c6 );

    return ai;
#endif
}

#define PRESET1

// *********************************
//
TestAI *        TestAIManager::PreparePreset3   ( model::IModelNodePtr node ) const
{
    auto timeline = model::DefaultTimeline::Create( "timeline preset 0", TimeType( 30.0 ), TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_CLAMP );

    TestAI * ai = new TestAI( timeline, node );
    
#ifdef PRESET0
    auto c0 = new AICommandEnableOverridenAlpha( node, ".", 2.f );
    auto c1 = new AICommandEnableOverridenAlpha( node, "./node0/node02", 5.f );
    auto c2 = new AICommandDisableAlpha( node, ".", 20.f );
    auto c3 = new AICommandDisableAlpha( node, "./node0/node02", 22.f );

    ai->AddCommand( c0 );
    ai->AddCommand( c1 );
    ai->AddCommand( c2 );
    ai->AddCommand( c3 );

    return ai;
#endif

#ifdef PRESET1

    auto c0 = new AICommandEnableOverridenAlpha( node, "./node0/node02", 0.f );
    auto c1 = new AICommandEnableOverridenAlpha( node, "./node0", 1.f );
    auto c2 = new AICommandEnableOverridenAlpha( node, ".", 2.f );
    auto c3 = new AICommandDisableAlpha( node, "./node0/node02", 10.f );
    auto c4 = new AICommandDisableAlpha( node, "./node0", 11.f );
    auto c5 = new AICommandDisableAlpha( node, ".", 12.f );
    auto c6 = new AICommandEnableOverridenAlpha( node, "./node0/node02", 13.f );
    auto c7 = new AICommandDisableAlpha( node, "./node0/node02", 23.f );
    auto c8 = new AICommandEnableOverridenAlpha( node, ".", 26.f );
    auto c9 = new AICommandDisableAlpha( node, ".", 36.f );

    ai->AddCommand( c0 );
    ai->AddCommand( c1 );
    ai->AddCommand( c2 );
    ai->AddCommand( c3 );
    ai->AddCommand( c4 );
    ai->AddCommand( c5 );
    ai->AddCommand( c6 );
    ai->AddCommand( c7 );
    ai->AddCommand( c8 );
    ai->AddCommand( c9 );

    return ai;
#endif
}

// *********************************
//
TestAI *        TestAIManager::PreparePreset4   ( BVAppLogic * logic ) const
{    
    auto timeline = model::DefaultTimeline::Create( "timeline preset 1", TimeType( 30.0 ), TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_CLAMP );

    TestAI * ai = new TestAI( timeline, nullptr );

    auto c0 = new AICommandReloadScene( logic, 2.f );
    auto c1 = new AICommandReloadScene( logic, 16.65f );
    auto c2 = new AICommandReloadScene( logic, 24.f );

    ai->AddCommand( c0 );
    ai->AddCommand( c1 );
    ai->AddCommand( c2 );

    return ai;
}

// *********************************
//
TestAI *        TestAIManager::PreparePreset5   ( BVAppLogic * logic ) const
{
    auto timeline = model::DefaultTimeline::Create( "timeline preset 5", TimeType( 130.0 ), TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_CLAMP );

    auto c0 = new AICommandReloadTexture( logic, 3.f, "Root", "texture", "alfai00.tga" );
    auto c1 = new AICommandReloadTexture( logic, 4.f, "Root", "texture", "simless_01.jpg" );
    auto c2 = new AICommandReloadTexture( logic, 5.5f, "Root", "alpha_mask", "simless_01.jpg" );
    auto c3 = new AICommandReloadTexture( logic, 7.f, "Root", "texture", "chuj_nie_kwiatek.jpg" );
    auto c4 = new AICommandReloadTexture( logic, 7.5f, "Root", "alpha_mask", "bar_mask_red.png" );
    auto c5 = new AICommandReloadTexture( logic, 8.f, "Root", "texture", "simless_00.jpg" );
    auto c6 = new AICommandReloadTexture( logic, 11.f, "Root", "texture", "full.jpg" );
    auto c7 = new AICommandReloadTexture( logic, 12.f, "Root", "alpha_mask", "test.bmp" );

    TestAI * ai = new TestAI( timeline, nullptr );

    ai->AddCommand( c0 );
    ai->AddCommand( c1 );
    ai->AddCommand( c2 );
    ai->AddCommand( c3 );
    ai->AddCommand( c4 );
    ai->AddCommand( c5 );
    ai->AddCommand( c6 );
    ai->AddCommand( c7 );

    return ai;
}

#define NM_PRESET0

// *********************************
//
TestAI *        TestAIManager::PreparePreset6   ( model::IModelNodePtr node ) const
{
    auto timeline = model::DefaultTimeline::Create( "timeline preset 0", TimeType( 30.0 ), TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_CLAMP );

    TestAI * ai = new TestAI( timeline, node );
    
#ifdef NM_PRESET0
    auto c0 = new AICommandEnableOverridenAlphaNM( node, "nodem1", 2.f );
    auto c1 = new AICommandDisableAlphaNM( node, "nodem1", 12.f );

    ai->AddCommand( c0 );
    ai->AddCommand( c1 );

    return ai;
#endif

#ifdef NM_PRESET1

    auto c0 = new AICommandEnableOverridenAlpha( node, "./node0/node02", 0.f );
    auto c1 = new AICommandEnableOverridenAlpha( node, "./node0", 1.f );
    auto c2 = new AICommandEnableOverridenAlpha( node, ".", 2.f );
    auto c3 = new AICommandDisableAlpha( node, "./node0/node02", 10.f );
    auto c4 = new AICommandDisableAlpha( node, "./node0", 11.f );
    auto c5 = new AICommandDisableAlpha( node, ".", 12.f );
    auto c6 = new AICommandEnableOverridenAlpha( node, "./node0/node02", 13.f );
    auto c7 = new AICommandDisableAlpha( node, "./node0/node02", 23.f );
    auto c8 = new AICommandEnableOverridenAlpha( node, ".", 26.f );
    auto c9 = new AICommandDisableAlpha( node, ".", 36.f );

    ai->AddCommand( c0 );
    ai->AddCommand( c1 );
    ai->AddCommand( c2 );
    ai->AddCommand( c3 );
    ai->AddCommand( c4 );
    ai->AddCommand( c5 );
    ai->AddCommand( c6 );
    ai->AddCommand( c7 );
    ai->AddCommand( c8 );
    ai->AddCommand( c9 );

    return ai;
#endif
}

// *********************************
//
TestAIManager & TestAIManager::Instance     ()
{
    static TestAIManager instance;

    return instance;
}


} //bv
