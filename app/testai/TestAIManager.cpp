#include "TestAIManager.h"

#include <cassert>

#include "testai/TestAI.h"
#include "testai/AICommands.h"

#include "Engine/Models/Timeline/Dynamic/DefaultTimeline.h"

#include "Engine/Models/Timeline/Dynamic/TimelineEventLoop.h"
#include "Engine/Models/Timeline/Dynamic/TimelineEventNull.h"
#include "Engine/Models/Timeline/Dynamic/TimelineEventStop.h"


namespace bv {

// *********************************
//
TestAIManager::TestAIManager   ()
{
    m_presets.reserve( 2 );
    m_timelines.reserve( 2 );

    m_presets.push_back( nullptr );
    m_presets.push_back( nullptr );

    m_timelines.push_back( nullptr );
    m_timelines.push_back( nullptr );
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

// *********************************
//
TestAI *        TestAIManager::GetAIPreset  ( unsigned int idx )
{
    assert( idx < NumAIPresets() );

    if( m_presets[ idx ] == nullptr )
    {
        m_presets[ idx ] = PreparePreset( idx );
    }

    return m_presets[ idx ];
}

// *********************************
//
TestAI *        TestAIManager::PreparePreset    ( unsigned int idx ) const
{
    if( idx == 0 )
    {
        return PreparePreset0();
    }
    else if( idx == 1 )
    {
        return PreparePreset1();
    }

    return nullptr;
}

// *********************************
//
TestAI *        TestAIManager::PreparePreset0   () const
{
    auto timeline = new model::DefaultTimeline( "timeline preset 0", TimeType( 30.0 ), TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_CLAMP );

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

    TestAI * ai = new TestAI( timeline );

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
    auto timeline = new model::DefaultTimeline( "timeline preset 0", TimeType( 20.0 ), TimelineWrapMethod::TWM_REPEAT, TimelineWrapMethod::TWM_CLAMP );
    timeline->AddKeyFrame( new model::TimelineEventStop( "stop0", TimeType( 5.0 ), timeline ) );
    timeline->AddKeyFrame( new model::TimelineEventStop( "stop1", TimeType( 10.0 ), timeline ) );

    auto c0 = new AICommandPlay( timeline, TimeType( 2.0 ) );
    auto c1 = new AICommandPlay( timeline, TimeType( 9.0 ) );
    auto c2 = new AICommandPlay( timeline, TimeType( 16.0 ) );
    auto c3 = new AICommandSetTimeAndStop( timeline, TimeType( 25.0 ), TimeType( 0.0 ) );
    auto c4 = new AICommandPlay( timeline, TimeType( 27.0 ) );
    auto c5 = new AICommandPlay( timeline, TimeType( 35.0 ) );
    auto c6 = new AICommandPlay( timeline, TimeType( 40.0 ) );

    TestAI * ai = new TestAI( timeline );

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
    return nullptr;    
}

// *********************************
//
TestAI *        TestAIManager::PreparePreset3   () const
{
    return nullptr;
}

// *********************************
//
TestAI *        TestAIManager::PreparePreset4   () const
{
    return nullptr;
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
