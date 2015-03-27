#pragma once

namespace bv {

/////////////////////////////
//
class AggregateSizeTracker
{
private:

    size_t  m_totalSize;
    size_t  m_numVisits;

public:

    AggregateSizeTracker    ();

    void    Reset           ();

    size_t  GetTotalSize    () const;
    void    AddToTotalSize  ( size_t chunkSize );

    size_t  GetNumVisits    () const;
    void    IncNumVisits    ();

};

/////////////////////////////
//
class AggregateFrameBufferTracker
{
    size_t totalAttachments0Tex;
    size_t totalAttachments0FrameBuffer;

    size_t totalAttachments1Tex;
    size_t totalAttachments1FrameBuffer;

    size_t totalDepthAttachmentsTex;
    size_t totalDepthAttachmentsFramebuffer;

    size_t  m_numVisits;

public:

    AggregateFrameBufferTracker ();

    void    Reset               ();

    size_t  GetTotalNumAtch0Tex () const;
    size_t  GetTotalNumAtch0FB  () const;

    size_t  GetTotalNumAtch1Tex () const;
    size_t  GetTotalNumAtch1FB  () const;
    
    size_t  GetTotalNumDBAtTex  () const;
    size_t  GetTotalNumDBAtFB   () const;

    void    IncNumAtch0Tex      ();
    void    IncNumAtch0FB       ();

    void    IncNumAtch1Tex      ();
    void    IncNumAtch1FB       ();

    void    IncNumDBAtTex       ();
    void    IncNumDBAtFB        ();

    size_t  GetNumVisits        () const;
    void    IncNumVisits        ();
};

class AggregateVertexArrayTracker
{
private:

    size_t  m_totalRegisteredAttributes;
    size_t  m_totalEnabledAttributes;
    size_t  m_numVisits;

public:

    AggregateVertexArrayTracker         ();

    void    Reset                       ();

    size_t  GetTotalRegisteredAttrs     () const;
    void    AddToTotalRegisteredAttrs   ( size_t numAttrs );

    size_t  GetTotalEnabledAttrs        () const;
    void    AddToTotalEnabledAttrs      ( size_t numAttrs );

    size_t  GetNumVisits                () const;
    void    IncNumVisits                ();

};

} //bv
