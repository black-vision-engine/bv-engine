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
    m_presets.reserve( 6 );
    m_timelines.reserve( 6 );

    for( unsigned int i = 0; i < 6; ++i )
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
    return m_presets.size();
}

TestAI *        TestAIManager::GetAIPreset         ( unsigned int idx, BVAppLogic * logic )
{
    assert( idx == 4 );

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
        if( idx == 3 )
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

    return nullptr;
}

TestAI *        TestAIManager::PreparePreset    ( unsigned int idx, BVAppLogic * logic ) const
{
    if( idx == 4 )
    {
        return PreparePreset4( logic );
    }

    return nullptr;
}

// *********************************
//
TestAI *        TestAIManager::PreparePreset0   () const
{
    auto timeline = model::DefaultTimelinePtr( new model::DefaultTimeline( "timeline preset 0", TimeType( 30.0 ), TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_CLAMP ) );

    auto c0 = new AICommandPlay( timeline, TimeType( 2.0 ) );
    auto c1 = new AICommandStop( timeline, TimeType( 4.0 ) );
    auto c2 = new AICommandPlay( timeline, TimeType( 7.0 ) );
    auto c3 = new AICommandStop( timeline, TimeType( 10.0 ) );
    auto c4 = new AICommandSetTimeAndPlay( timeline, TimeType( 12.0 ), TimeType( 2.0 ) );
    auto c5 = new AICommandSetTimeAndStop( timeline, TimeType( 15.0 ), TimeType( 1.0 ) );
    auto c6 = new AICommandPlay( timeline, TimeType( 18.0 ) );
    auto c7 = new AICommandReverse( timeline, TimeType( 20.0 ) );
    auto c8 = new AICommandReverse( timeline, TimeType( 24.0 ) );
    auto c9 = new AICommandStop( timeline, TimeType( 29.0 ) );

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

    return ai;
}

// *********************************
//FIXME: test and fix in runtime
TestAI *        TestAIManager::PreparePreset1   () const
{
    auto timeline = model::DefaultTimelinePtr( new model::DefaultTimeline( "timeline preset 0", TimeType( 20.0 ), TimelineWrapMethod::TWM_REPEAT, TimelineWrapMethod::TWM_CLAMP ) );
    timeline->AddKeyFrame( new model::TimelineEventStop( "stop0", TimeType( 5.0 ), timeline.get() ) );
    timeline->AddKeyFrame( new model::TimelineEventStop( "stop1", TimeType( 10.0 ), timeline.get() ) );

    auto c0 = new AICommandPlay( timeline, TimeType( 2.0 ) );
    auto c1 = new AICommandPlay( timeline, TimeType( 9.0 ) );
    auto c2 = new AICommandPlay( timeline, TimeType( 16.0 ) );
    auto c3 = new AICommandSetTimeAndStop( timeline, TimeType( 25.0 ), TimeType( 0.0 ) );
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
}

// *********************************
//
TestAI *        TestAIManager::PreparePreset2   () const
{
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
}

// *********************************
//
TestAI *        TestAIManager::PreparePreset3   ( model::IModelNodePtr node ) const
{
    auto timeline = model::DefaultTimelinePtr( new model::DefaultTimeline( "timeline preset 0", TimeType( 30.0 ), TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_CLAMP ) );

    TestAI * ai = new TestAI( timeline, node );

    auto c0 = new AICommandEnableOverridenAlpha( node, "./node0", 2.f );
    auto c1 = new AICommandDisableAlpha( node, "./node0", 10.f );
    auto c2 = new AICommandEnableOverridenAlpha( node, "./node0/node02", 12.f );
    auto c3 = new AICommandDisableAlpha( node, "./node0/node02", 22.f );

    ai->AddCommand( c0 );
    ai->AddCommand( c1 );
    ai->AddCommand( c2 );
    ai->AddCommand( c3 );

    return ai;
}

// *********************************
//
TestAI *        TestAIManager::PreparePreset4   ( BVAppLogic * logic ) const
{    
    auto timeline = model::DefaultTimelinePtr( new model::DefaultTimeline( "timeline preset 1", TimeType( 30.0 ), TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_CLAMP ) );

    TestAI * ai = new TestAI( timeline, nullptr );

    auto c0 = new AICommandReloadScene( logic, 7.f );
    auto c1 = new AICommandReloadScene( logic, 16.65f );
    auto c2 = new AICommandReloadScene( logic, 24.f );

    ai->AddCommand( c0 );
    ai->AddCommand( c1 );
    ai->AddCommand( c2 );

    return ai;
}

// *********************************
//
TestAI *        TestAIManager::PreparePreset5   () const
{
    return nullptr;
}

// *********************************
//
TestAIManager & TestAIManager::Instance     ()
{
    static TestAIManager instance;

    return instance;
}


} //bv
