#pragma once

#include "Engine/Graphics/SceneGraph/Scene.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/RenderChannel.h"

      
namespace bv {  namespace nrl {

class RenderResult;
class NRenderContext;

class NRenderLogicCore
{
private:

    std::vector< RenderChannelType > m_allChannels;

public:

                            NRenderLogicCore    ();

    void                    Render              ( const SceneVec & scenes, RenderResult * result, NRenderContext * ctx );

private:

    void                    RenderScenes        ( const SceneVec & scenes, RenderResult * result, NRenderContext * ctx );
    void                    RenderScene         ( Scene * scene, const RenderTarget * outputRT, NRenderContext * ctx );

    void                    PreRender           ( RenderResult * result );
    void                    PostRender          ( RenderResult * result, NRenderContext * ctx );

};

} //nrl
} //bv
