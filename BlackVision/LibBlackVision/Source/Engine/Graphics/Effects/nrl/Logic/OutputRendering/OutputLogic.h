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

                        OutputLogic             ( unsigned int width, unsigned int height, unsigned int shmScaleFactor, RenderTargetStackAllocator * allocator, unsigned int numTrackedRenderTargetsPerOutput );
    virtual             ~OutputLogic            ();

    // API directly related to frame rendering
    void                ProcessFrameData        ( NRenderContext * ctx );

    // API relarted to global output state manipulation
    bool                IsEnabled               ( CustomOutputType outputType );
    void                EnableOutput            ( CustomOutputType outputType );
    void                DisableOutput           ( CustomOutputType outputType );

    OutputInstance *    GetOutput               ( CustomOutputType outputType );

    // API related to render buffers state manipulation
    RenderResult *      AccessRenderResult      ();

    void                ActivateRenderChannel   ( RenderChannelType rct );
    void                DeactivateRenderChannel ( RenderChannelType rct );

    void                UpdateRenderChannels    ();

};

} //nrl
} //bv
