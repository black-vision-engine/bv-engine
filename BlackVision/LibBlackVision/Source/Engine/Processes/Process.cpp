#include "stdafx.h"

#include "Process.h"

#include <cassert>




#include "Memory/MemoryLeaks.h"



namespace bv
{

// *******************************
//
Process::Process     ()
    : m_state( ProcessState::PS_UNINITIALIZED )
    , m_childProcess( nullptr )
{
}

// *******************************
//
Process::~Process    ()
{
	if ( m_childProcess )
	{
        m_childProcess->OnAbort();
	}
}

// *******************************
//
void Process::OnInit     ()
{
    m_state = ProcessState::PS_RUNNING;
}

// *******************************
//
void Process::OnSuccess  ()
{
    //Default implementation is empty
}

// *******************************
//
void Process::OnFail     ()
{
    //Default implementation is empty
}

// *******************************
//
void Process::OnAbort    ()
{
    //Default implementation is empty
}

// *******************************
//
ProcessState    Process::GetState    () const
{ 
    return m_state; 
}

// *******************************
//
bool            Process::IsAlive     () const
{
    return ( m_state == ProcessState::PS_RUNNING || m_state == ProcessState::PS_PAUSED );
}

// *******************************
//
bool            Process::IsDead      () const
{ 
    return ( m_state == ProcessState::PS_SUCCEEDED || m_state == ProcessState::PS_FAILED || m_state == ProcessState::PS_ABORTED );
}

// *******************************
//
bool            Process::IsRemoved   () const 
{
    return ( m_state == ProcessState::PS_REMOVED );
}

// *******************************
//
bool            Process::IsPaused    () const
{ 
    return m_state == ProcessState::PS_PAUSED;
}

// *******************************
//
void Process::Succeed     ()
{
	assert( m_state == ProcessState::PS_RUNNING || m_state == ProcessState::PS_PAUSED );
	
    m_state = ProcessState::PS_SUCCEEDED;
}

// *******************************
//
void Process::Fail        ()
{
	assert( m_state == PS_RUNNING || m_state == PS_PAUSED );

    m_state = PS_FAILED;
}

// *******************************
//
void Process::Pause       ()
{
	if ( m_state == PS_RUNNING )
    {
		m_state = PS_PAUSED;
    }
    else
    {
	    assert( !"Only running pocess can be paused" );
    }
}

// *******************************
//
void Process::UnPause     ()
{
	if ( m_state == PS_PAUSED )
    {
		m_state = PS_RUNNING;
    }
    else
    {
	    assert( !"Only running pocess can be paused" );
    }
}

// *******************************
//
void Process::AttachChild       ( ProcessPtr child )
{
    if( m_childProcess )
    {
        m_childProcess->AttachChild( child );
    }
    else
    {
        m_childProcess = child;
    }
}

// *******************************
//
ProcessPtr Process::RemoveChild ()
{
    if ( m_childProcess )
    {
        ProcessPtr child = m_childProcess;
        m_childProcess.reset();

        return child;
    }

    return nullptr;
}

// *******************************
//
ProcessPtr Process::PeekChild   () 
{ 
    return m_childProcess; 
}

// *******************************
//
void        Process::SetState    ( ProcessState state )
{
    m_state = state;
}

} //bv
