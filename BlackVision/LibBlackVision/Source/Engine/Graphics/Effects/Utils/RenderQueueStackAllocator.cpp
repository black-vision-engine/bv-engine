#include "stdafx.h"
#include "RenderQueueStackAllocator.h"





#include "Memory/MemoryLeaks.h"



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

    if( m_currentIdx >= ( Int32 )m_renderQueues.size() )
    {
        m_renderQueues.push_back( RenderingQueueUPtr( new RenderingQueue() ) );
    }

    return m_renderQueues[ m_currentIdx ].get();
}

// ***********************
//
RenderingQueue *    RenderQueueStackAllocator::Top                         ()
{
    assert( m_currentIdx < ( Int32 )m_renderQueues.size() );
    assert( m_currentIdx >= 0 );
    return m_renderQueues[ m_currentIdx ].get();
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
    assert( m_currentIdx + 1 >= (int)num );
    if( m_currentIdx + 1 < (int)num )
        return false;

    for( unsigned int i = 0; i < num; ++i )
    {
        m_renderQueues[ m_currentIdx - i ]->ClearQueue();
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