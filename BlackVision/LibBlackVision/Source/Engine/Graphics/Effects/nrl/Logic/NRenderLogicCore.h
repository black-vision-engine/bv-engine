#pragma once

#include "Engine/Graphics/SceneGraph/Scene.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/RenderOutputChannel.h"

      
namespace bv {  namespace nrl {

class RenderResult;
class NRenderContext;

class NRenderLogicCore
{
private:

    std::vector< RenderOutputChannelType > m_allChannels;

public:

                            NRenderLogicCore    ();

    void                    Render              ( const SceneVec & scenes, RenderResult * result, NRenderContext * ctx );

private:

    void                    RenderScenes        ( const SceneVec & scenes, RenderResult * result, NRenderContext * ctx );

    void                    PreRender           ( RenderResult * result );
    void                    PostRender          ( RenderResult * result, NRenderContext * ctx );

    void                    ClearChannelState   ( RenderResult * result, RenderOutputChannelType roct );

};

} //nrl
} //bv
