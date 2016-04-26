#include "stdafx.h"
#include "RenderQueueStackAllocator.h"



namespace bv
{


// ***********************
//
RenderQueueStackAllocator::RenderQueueStackAllocator()
    :   m_currentIdx( -1 )
{}

// ***********************
//
RenderQueueStackAllocator::~RenderQueueStackAllocator()
{}

// ***********************
//
RenderingQueue *    RenderQueueStackAllocator::Allocate                    ()
{
    ++m_currentIdx;

    if( m_currentIdx >= m_renderQueues.size() )
    {
        m_renderQueues.push_back( RenderingQueue() );
    }

    return &m_renderQueues[ m_currentIdx ];
}

// ***********************
//
RenderingQueue *    RenderQueueStackAllocator::Top                         ()
{
    assert( m_currentIdx < m_renderQueues.size() );
    assert( m_currentIdx >= 0 );
    return &m_renderQueues[ m_currentIdx ];
}

// ***********************
//
bool                RenderQueueStackAllocator::Free                        ()
{
    return Free( 1 );
}

// ***********************
//
bool                RenderQueueStackAllocator::Free                        ( unsigned int num )
{
    assert( m_currentIdx - num >= -1 );
    if( m_currentIdx - num < -1 )
        return false;

    for( unsigned int i = 0; i < num; ++i )
    {
        m_renderQueues[ m_currentIdx - i ].ClearQueue();
    }

    m_currentIdx -= num;
    return true;
}

// ***********************
//
unsigned int        RenderQueueStackAllocator::GetTopIndex                 () const
{
    assert( m_currentIdx >= 0 );
    return m_currentIdx;
}

// ***********************
// Clears (deletes) all RenderingQueues.
void                RenderQueueStackAllocator::Clear                       ()
{
    m_currentIdx = -1;
    m_renderQueues.resize( 0 );
}

}	// bv