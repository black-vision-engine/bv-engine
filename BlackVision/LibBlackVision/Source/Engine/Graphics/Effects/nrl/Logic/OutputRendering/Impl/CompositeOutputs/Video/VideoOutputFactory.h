#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/Components/Initialization/OutputDesc.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/CompositeOutputs/Video/OutputCompositeVideo.h"


namespace bv { namespace nrl {

class VideoOutputFactory
{
public:

    struct VideoOutputDesc
    {
        OutputStaticData    staticOutputData;

        unsigned int        outputID;
    
    };

public:

    static OutputCompositeVideo *           CreateCompositeVideoOutput  ( const OutputDesc & desc );

private:

    static VideoOutputDesc                  ReadEntry                   ( const OutputDesc::TOutputPropertyMap & props );

    static std::vector< VideoOutputDesc >   ReadInputEntryVec           ( const OutputDesc::TOutputPropertiesVector & props );

    static std::hash_map< unsigned int, unsigned int >  ReadMapping     ( const std::vector< VideoOutputDesc > & videoOutputs );
 
    static std::vector< OutputStaticData >  ReadUniqueStaticDataVec     ( const std::vector< VideoOutputDesc > & inputVec );

};

} // nrl
} // bv
