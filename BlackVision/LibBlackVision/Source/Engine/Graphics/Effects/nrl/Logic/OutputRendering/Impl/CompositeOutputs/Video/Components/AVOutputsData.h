#pragma once

#include <hash_map>

#include "AVFrame.h"



namespace bv { 

using videocards::AVFramePtr;

namespace nrl {
   
class AVOutputsData
{
public:

    typedef unsigned int VideoCardID;
    
    typedef std::hash_map< VideoCardID, AVFramePtr >    AVFramePtrMap;

private:

    mutable AVFramePtrMap   m_frames;

private:

    // Disable copying (when delete keyword is not present)
    AVOutputsData               ( const AVOutputsData & src );
    AVOutputsData               ( const AVOutputsData && src );

    AVOutputsData & operator =  ( const AVOutputsData & src );

public:

                    AVOutputsData   ();

    AVFramePtr      GetAVFrame      ( unsigned int videoOutputId ) const;
    void            SetAVFrame      ( unsigned int videoOutputId, AVFramePtr frame );

    unsigned int    GetNumEntries   () const;

};


} //bv
} //nrl
