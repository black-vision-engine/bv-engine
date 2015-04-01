#include "ResourceTrackingAggregates.h"

#include <sstream>
#include <iomanip>

#include "BVGLTools/BVGLStringFormatters.h"


namespace bv {

// ***************************
//
AggregateSizeTracker::AggregateSizeTracker      ()
{
    Reset();
}

// ***************************
//
void    AggregateSizeTracker::Reset             ()
{
    m_totalSize = 0;
    m_numVisits = 0;
}

// ***************************
//
size_t  AggregateSizeTracker::GetTotalSize      () const
{
    return m_totalSize;
}

// ***************************
//
void    AggregateSizeTracker::AddToTotalSize    ( size_t chunkSize )
{
    m_totalSize += chunkSize;
}

// ***************************
//
size_t  AggregateSizeTracker::GetNumVisits    () const
{
    return m_numVisits;
}

// ***************************
//
void    AggregateSizeTracker::IncNumVisits    ()
{
    m_numVisits++;
}

// ***************************
//
string  AggregateSizeTracker::Summary         () const
{
    std::stringstream ss;

    ss << "Num elts: " << std::setfill( ' ' ) << std::setw( 2 ) << m_numVisits << " Total Size: " << FormatSizeString( m_totalSize );

    return ss.str();
}

// *************************************************************************************************************************************************

// ***************************
//
AggregateFrameBufferTracker::AggregateFrameBufferTracker ()
{
    Reset();
}

// ***************************
//
void    AggregateFrameBufferTracker::Reset               ()
{
    totalAttachments0Tex                = 0;
    totalAttachments0FrameBuffer        = 0;
    
    totalAttachments1Tex                = 0;
    totalAttachments1FrameBuffer        = 0;
    
    totalDepthAttachmentsTex            = 0;
    totalDepthAttachmentsFramebuffer    = 0;

    m_numVisits                         = 0;
}  

// ***************************
//
size_t  AggregateFrameBufferTracker::GetTotalNumAtch0Tex () const
{
    return totalAttachments0Tex;
}

// ***************************
//
size_t  AggregateFrameBufferTracker::GetTotalNumAtch0FB  () const
{
    return totalAttachments0FrameBuffer;
}

// ***************************
//
size_t  AggregateFrameBufferTracker::GetTotalNumAtch1Tex () const
{
    return totalAttachments1Tex;
}

// ***************************
//
size_t  AggregateFrameBufferTracker::GetTotalNumAtch1FB  () const
{
    return totalAttachments1FrameBuffer;
}

// ***************************
//
size_t  AggregateFrameBufferTracker::GetTotalNumDBAtTex  () const
{
    return totalDepthAttachmentsTex;
}

// ***************************
//
size_t  AggregateFrameBufferTracker::GetTotalNumDBAtFB   () const
{
    return totalDepthAttachmentsFramebuffer;
}

// ***************************
//
void    AggregateFrameBufferTracker::IncNumAtch0Tex      ()
{
    totalAttachments0Tex++;
}

// ***************************
//
void    AggregateFrameBufferTracker::IncNumAtch0FB       ()
{
    totalAttachments0FrameBuffer++;
}

// ***************************
//
void    AggregateFrameBufferTracker::IncNumAtch1Tex      ()
{
    totalAttachments1Tex++;
}

// ***************************
//
void    AggregateFrameBufferTracker::IncNumAtch1FB       ()
{
    totalAttachments1FrameBuffer++;
}

// ***************************
//
void    AggregateFrameBufferTracker::IncNumDBAtTex       ()
{
    totalDepthAttachmentsTex++;
}

// ***************************
//
void    AggregateFrameBufferTracker::IncNumDBAtFB        ()
{
    totalDepthAttachmentsFramebuffer++;
}

// ***************************
//
size_t  AggregateFrameBufferTracker::GetNumVisits        () const
{
    return m_numVisits;
}

// ***************************
//
void    AggregateFrameBufferTracker::IncNumVisits        ()
{
    m_numVisits++;
}

// ***************************
//
string  AggregateFrameBufferTracker::Summary             () const
{
    std::stringstream ss;
    bool comma = false;

    ss << "Num elts: " << std::setfill( ' ' ) << std::setw( 2 ) << m_numVisits << " [";

    if( totalAttachments0Tex > 0 )
    {
        ss << "CA0TX: " << std::setfill( ' ' ) << std::setw( 2 ) << totalAttachments0Tex;
        comma = true;
    }

    if( totalAttachments0FrameBuffer )
    {
        if ( comma ) 
            ss << ", ";

        ss << "CA0FB: " << std::setfill( ' ' ) << std::setw( 2 ) << totalAttachments0FrameBuffer;

        comma = true;
    }

    if( totalAttachments1Tex )
    {
        if ( comma ) 
            ss << ", ";

        ss << "CA1TX: " << std::setfill( ' ' ) << std::setw( 2 ) << totalAttachments1Tex;

        comma = true;
    }

    if( totalAttachments1FrameBuffer )
    {
        if ( comma ) 
            ss << ", ";

        ss << "CA1FB: " << std::setfill( ' ' ) << std::setw( 2 ) << totalAttachments1FrameBuffer;

        comma = true;
    }

    if( totalDepthAttachmentsTex )
    {
        if ( comma ) 
            ss << ", ";

        ss << "DATX: " << std::setfill( ' ' ) << std::setw( 2 ) << totalDepthAttachmentsTex;

        comma = true;
    }

    if( totalDepthAttachmentsFramebuffer )
    {
        if ( comma ) 
            ss << ", ";

        ss << "DAFB: " << std::setfill( ' ' ) << std::setw( 2 ) << totalDepthAttachmentsFramebuffer;
    }

    ss << "]";

    return ss.str();
}

// *************************************************************************************************************************************************

// ***************************
//
AggregateVertexArrayTracker::AggregateVertexArrayTracker        ()
{
    Reset();
}

// ***************************
//
void    AggregateVertexArrayTracker::Reset                      ()
{
    m_numVisits                 = 0;
    m_totalEnabledAttributes    = 0;
    m_totalRegisteredAttributes = 0;
}

// ***************************
//
size_t  AggregateVertexArrayTracker::GetTotalRegisteredAttrs    () const
{
    return m_totalRegisteredAttributes;
}

// ***************************
//
void    AggregateVertexArrayTracker::AddToTotalRegisteredAttrs  ( size_t numAttrs )
{
    m_totalRegisteredAttributes += numAttrs;
}

// ***************************
//
size_t  AggregateVertexArrayTracker::GetTotalEnabledAttrs       () const
{
    return m_totalEnabledAttributes;
}

// ***************************
//
void    AggregateVertexArrayTracker::AddToTotalEnabledAttrs     ( size_t numAttrs )
{
    m_totalEnabledAttributes += numAttrs;
}

// ***************************
//
size_t  AggregateVertexArrayTracker::GetNumVisits               () const
{
    return m_numVisits;
}

// ***************************
//
void    AggregateVertexArrayTracker::IncNumVisits               ()
{
    m_numVisits++;
}

// ***************************
//
string  AggregateVertexArrayTracker::Summary                    () const
{
    std::stringstream ss;

    ss << "Num elts: " << std::setfill( ' ' ) << std::setw( 2 ) << m_numVisits << " ";
    ss << "TotalRegAttrs: " << std::setfill( ' ' ) << std::setw( 2 ) << m_totalRegisteredAttributes << " ";
    ss << "TotalEnabledAttrs: " << std::setfill( ' ' ) << std::setw( 2 ) << m_totalEnabledAttributes << " ";

    return ss.str();
}

} //bv
