#pragma once

#include <vector>

#include "Engine/Graphics/Effects/nrl/Logic/RenderLogicComponents/RenderedChannelsData.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Output.h"


namespace bv { 
    
namespace nrl {

class NRenderContext;

class OutputLogic
{
private:

    std::vector< Output * >     m_outputs;

public:

                        OutputLogic             ();
    virtual             ~OutputLogic            ();

    // API directly related to frame rendering
    void                ProcessFrameData        ( NRenderContext * ctx );

    // API relarted to global output state manipulation
    Output *            GetOutput               ( CustomOutputType outputType );

};

} //nrl
} //bv
