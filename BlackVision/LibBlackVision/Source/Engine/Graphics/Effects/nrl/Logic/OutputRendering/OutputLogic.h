#pragma once

#include <vector>

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/RenderResult.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/OutputInstance.h"


namespace bv { 
    
class Renderer;    
class SharedMemoryVideoBuffer;

namespace nrl {

class OutputPreview;
class OutputVideo;
class OutputStream;

class NRenderContext;

class OutputLogic
{
private:

    std::vector< OutputInstance * >     m_outputs;

    RenderResult *                      m_renderResult;

public:

                    OutputLogic             ();
    virtual         ~OutputLogic            ();

    void            ProcessFrameData        ( NRenderContext * ctx );

    RenderResult *  AccessRenderResult      ();

        // FIXME: nrl - should be accessed via RenderResult, not delegated via OutputLogic
        //bool            IsActive                () const;
        //// void         ClearOutputChannel      ( NRenderContext * ctx, RenderOutputChannelType roct ); //FIXME nrl - implement it here?
    void            ActivateOutputChannel   ( RenderOutputChannelType roct );
    void            DeactivateOutputChannel ( RenderOutputChannelType roct );

    OutputPreview * GetOutputPreview        ();
    OutputVideo *   GetOutputVideo          ();
    OutputStream *  GetOutputStream         ();

};

} //nrl
} //bv
