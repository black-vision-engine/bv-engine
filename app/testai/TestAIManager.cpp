#include "TestAIManager.h"

#include <cassert>

#include "testai/TestAI.h"
#include "testai/AICommands.h"

#include "Engine/Models/Timeline/Dynamic/DefaultTimeline.h"


namespace bv {

// *********************************
//
TestAIManager::TestAIManager   ()
{
    m_presets.reserve( 1 );

    m_presets[ 0 ] = nullptr;
}

// *********************************
//
TestAIManager::~TestAIManager  ()
{
    for( auto ai : m_presets )
    {
        delete ai;
    }
}

// *********************************
//
unsigned int    TestAIManager::NumAIPresets ()
{
    return m_presets.size();
}

// *********************************
//
TestAI *        TestAIManager::GetAIPreset  ( unsigned int idx, model::DefaultTimeline * timeline )
{
    assert( idx < NumAIPresets() );

    if( m_presets[ idx ] == nullptr )
    {
        m_presets[ idx ] = PreparePreset( idx, timeline );
    }

    return m_presets[ idx ];
}

// *********************************
//
TestAI *        TestAIManager::PreparePreset    ( unsigned int idx, model::DefaultTimeline * timeline )
{
    if( idx == 0 )
    {
        return PreparePreset0( timeline );
    }

    return nullptr;
}

// *********************************
//
TestAI *        TestAIManager::PreparePreset0   ( model::DefaultTimeline * timeline )
{
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

    TestAI * ai = new TestAI();

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
//
TestAI *        TestAIManager::PreparePreset1   ( model::DefaultTimeline * timeline )
{
    return nullptr;
}

// *********************************
//
TestAI *        TestAIManager::PreparePreset2   ( model::DefaultTimeline * timeline )
{
    return nullptr;    
}

// *********************************
//
TestAI *        TestAIManager::PreparePreset3   ( model::DefaultTimeline * timeline )
{
    return nullptr;
}

// *********************************
//
TestAI *        TestAIManager::PreparePreset4   ( model::DefaultTimeline * timeline )
{
    return nullptr;
}

// *********************************
//
TestAI *        TestAIManager::PreparePreset5   ( model::DefaultTimeline * timeline )
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
