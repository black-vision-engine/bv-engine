#pragma once


#include "Memory/AVFrame.h"

#include <vector>



namespace bv
{


// ***********************
//
struct FakeInputFrames
{
    UInt32          m_nextFramePtr;

    std::vector< AVFrameConstPtr >      m_frames;


// ***********************
//
    FakeInputFrames()
        : m_nextFramePtr( 0 )
    {}
};


}	// bv


