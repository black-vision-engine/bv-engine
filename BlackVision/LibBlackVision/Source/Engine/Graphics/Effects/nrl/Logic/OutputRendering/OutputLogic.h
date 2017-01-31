#pragma once

#include <vector>

#include "Engine/Graphics/Effects/nrl/Logic/RenderedChannelsData.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Output.h"


namespace bv { 
    
namespace nrl {

class NRenderContext;

class OutputLogic
{
private:

    std::vector< Output * >     m_outputs;

    RenderResult                m_renderResult;

public:

                        OutputLogic             ( RenderTargetStackAllocator * allocator, unsigned int numTrackedRenderTargetsPerOutput );
    virtual             ~OutputLogic            ();

    // API directly related to frame rendering
    void                ProcessFrameData        ( NRenderContext * ctx );

    // API relarted to global output state manipulation
    Output *            GetOutput               ( CustomOutputType outputType );

    // API related to render buffers state manipulation
    RenderResult *      AccessRenderResult      ();

    void                UpdateRenderChannels    ();

};

} //nrl
} //bv
