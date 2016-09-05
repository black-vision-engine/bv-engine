#include "stdafx.h"
#include "PdrTimeQuery.h"



#include "Memory/MemoryLeaks.h"



namespace bv
{

// ***********************
//
PdrTimeQuery::PdrTimeQuery()
    :   m_buffer( 0 )
{
    BVGL::bvglGenQueries( 2, m_timer[ 0 ] );
    BVGL::bvglGenQueries( 2, m_timer[ 1 ] );

    BVGL::bvglQueryCounter( m_timer[ m_buffer ][ 0 ], GL_TIMESTAMP );
    BVGL::bvglQueryCounter( m_timer[ m_buffer ][ 1 ], GL_TIMESTAMP );
}

// ***********************
//
PdrTimeQuery::~PdrTimeQuery()
{
    BVGL::bvglDeleteQueries( 2, m_timer[ 0 ] );
    BVGL::bvglDeleteQueries( 2, m_timer[ 1 ] );
}

// ***********************
//
void        PdrTimeQuery::BeginQuery  ()
{
    BVGL::bvglQueryCounter( m_timer[ m_buffer ][ 0 ], GL_TIMESTAMP );
}

// ***********************
//
void        PdrTimeQuery::EndQuery    ()
{
    BVGL::bvglQueryCounter( m_timer[ m_buffer ][ 1 ], GL_TIMESTAMP );
    SwapBuffers();
}

// ***********************
//
UInt64      PdrTimeQuery::QueryResult ()
{
    UInt64 startTime = 0;
    UInt64 endTime = 0;

    int frontBuff = m_buffer - 1;
    if( frontBuff < 0 )
    {
        frontBuff = NUM_QUERY_TIMER_BACK_BUFFERS - 1;
    }

    BVGL::bvglGetQueryObjectui64v( m_timer[ frontBuff ][ 0 ], GL_QUERY_RESULT, &startTime );
    BVGL::bvglGetQueryObjectui64v( m_timer[ frontBuff ][ 1 ], GL_QUERY_RESULT, &endTime );

    return endTime - startTime;
}

// ***********************
//
void        PdrTimeQuery::SwapBuffers ()
{
    m_buffer = ( m_buffer + 1 ) % NUM_QUERY_TIMER_BACK_BUFFERS;
}

}
