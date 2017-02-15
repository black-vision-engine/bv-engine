#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/CompositeOutputs/Components/AVOutputsData.h"


namespace bv { namespace nrl {

class VideoOutputsPreprocessor
{
private:

    AVOutputsData   m_avFrames;

public:

    const AVOutputsData &   GetAVOutputsData    () const;

};

} //bv
} //nrl
