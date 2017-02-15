#include "stdafx.h"

#include "VideoOutputsPreprocessor.h"


namespace bv { namespace nrl {

// *********************************
//
const AVOutputsData &   VideoOutputsPreprocessor::GetAVOutputsData    () const
{
    return m_avFrames;
}

} //bv
} //nrl
