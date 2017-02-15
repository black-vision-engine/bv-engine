#include "stdafx.h"

#include <cassert>

#include "AVOutputsData.h"


namespace bv { namespace nrl {
   
// *********************************
//
AVFramePtr  AVOutputsData::GetAVFrame  ( unsigned int videoOutputId )
{
    assert( m_frames.find( videoOutputId ) != m_frames.end() );

    return m_frames[ videoOutputId ];
}

// *********************************
//
void        AVOutputsData::SetAVFrame  ( unsigned int videoOutputId, AVFramePtr frame )
{
    m_frames[ videoOutputId ] = frame;
}

} //bv
} //nrl
