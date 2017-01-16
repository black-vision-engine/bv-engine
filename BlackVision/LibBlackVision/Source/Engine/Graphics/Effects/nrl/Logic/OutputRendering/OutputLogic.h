#pragma once

#include <vector>

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/RenderResult.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/OutputInstance.h"


namespace bv { 
    
namespace nrl {

class NRenderContext;

class OutputLogic
{
private:

    std::vector< OutputInstance * >     m_outputs;

    RenderResult                        m_renderResult;

public:

                        OutputLogic             ( RenderTargetStackAllocator * allocator, unsigned int numTrackedRenderTargetsPerOutput );
    virtual             ~OutputLogic            ();

    void                ProcessFrameData        ( NRenderContext * ctx );

    void                UpdateOutputChannels    ();

    RenderResult *      AccessRenderResult      ();

    void                ActivateOutputChannel   ( RenderOutputChannelType roct );
    void                DeactivateOutputChannel ( RenderOutputChannelType roct );

    OutputInstance *    GetOutput               ( CustomOutputType outputType );

};

} //nrl
} //bv
