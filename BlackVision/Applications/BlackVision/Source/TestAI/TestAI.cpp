#include "stdafxBVApp.h"

#include "TestAI.h"

#include <algorithm>
#include <cassert>
#include <sstream>

#include "testai/AICommands.h"

#include "Engine/Models/Timeline/Dynamic/DefaultTimeline.h"


namespace bv {

namespace {

// *********************************
//
bool aiCommandComparator( AICommandBase * c0, AICommandBase * c1 )
{
    return c0->GetTriggerTime() <  c1->GetTriggerTime();
}

}

// *********************************
//
TestAI::TestAI      ( model::DefaultTimelinePtr timeline, model::IModelNodePtr root )
    : m_timeline( timeline )
    , m_root( root )
{
}

// *********************************
//
TestAI::~TestAI     ()
{
    for( auto cmd : m_commands )
    {
        delete cmd;
    }
}

// *********************************
//
void    TestAI::EvalAt  ( TimeType t )
{
    m_timeline->SetGlobalTime( t );

    printf( "\rGT: %0.3f LC: %0.3f ", t, m_timeline->GetLocalTime() );

    for( auto cmd : m_commands )
    {
        auto tt = cmd->GetTriggerTime();

        if ( std::abs( tt - t ) < TimeType( 0.05 ) )
        {
            if( cmd->TriggerCommand( t ) )
            {
                std::stringstream ss;
                ss << std::endl << "TRIGGERED: " << cmd->Repr() << " at " << cmd->GetTriggerTime() << std::endl;
                printf( "%s", ss.str().c_str() );

                return;
            }
        }
    }
}

// *********************************
//
std::string TestAI::Repr    () const
{
    std::stringstream ss;

    ss << "TestAI with " << m_commands.size() << " commands" << std::endl;
    
    for( unsigned int i = 0; i < m_commands.size(); ++i )
    {
        ss << "Command " << i << ": " << m_commands[ i ]->Repr() << " at " << m_commands[ i ]->GetTriggerTime() << std::endl;
    }

    return ss.str();
}

// *********************************
//
void    TestAI::SetTimeline ( model::DefaultTimelinePtr timeline )
{
    m_timeline = timeline;

    for( auto c : m_commands )
    {
        c->SetTimeline( timeline );
    }
}

// *********************************
//
void    TestAI::AddCommand  ( AICommandBase * cmd )
{
    assert( CanBeAdded( cmd ) );

    m_commands.push_back( cmd );

    std::sort( m_commands.begin(), m_commands.end(), aiCommandComparator );
}

// *********************************
//
bool    TestAI::CanBeAdded  ( AICommandBase * cmd ) const
{
    for( auto c : m_commands )
    {
        if( std::abs( c->GetTriggerTime() - cmd->GetTriggerTime() ) < TimeType( 0.1 ) )
        {
            return false;
        }
    }

    return true;
}


} //bv
