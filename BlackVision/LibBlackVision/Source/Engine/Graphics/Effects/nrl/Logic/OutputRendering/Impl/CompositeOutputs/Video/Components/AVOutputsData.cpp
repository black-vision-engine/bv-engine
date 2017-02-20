#include "stdafx.h"

#include <cassert>

#include "AVOutputsData.h"


namespace bv { namespace nrl {

// *********************************
//
AVOutputsData::AVOutputsData                    ()
{
}

// *********************************
//
AVFramePtr      AVOutputsData::GetAVFrame       ( unsigned int videoOutputId ) const
{
    assert( m_frames.find( videoOutputId ) != m_frames.end() );

    return m_frames[ videoOutputId ];
}

// *********************************
//
void            AVOutputsData::SetAVFrame       ( unsigned int videoOutputId, AVFramePtr frame )
{
    m_frames[ videoOutputId ] = frame;
}

// *********************************
//
unsigned int    AVOutputsData::GetNumEntries   () const
{
    return (unsigned int) m_frames.size();
}

} //bv
} //nrl
