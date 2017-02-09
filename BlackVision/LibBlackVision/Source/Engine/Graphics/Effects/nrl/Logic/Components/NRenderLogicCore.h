#pragma once

#include "Engine/Graphics/SceneGraph/Scene.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/RenderChannel.h"

      
namespace bv {  namespace nrl {

class RenderedChannelsData;
class NRenderContext;

class NRenderLogicCore
{
private:

    std::vector< RenderChannelType > m_allChannels;

public:

                            NRenderLogicCore    ();

    void                    Render              ( const SceneVec & scenes, RenderedChannelsData * result, NRenderContext * ctx );

private:

    void                    RenderScenes        ( const SceneVec & scenes, RenderedChannelsData * result, NRenderContext * ctx );
    void                    RenderScene         ( Scene * scene, const RenderTarget * outputRT, NRenderContext * ctx );

    void                    ClearActiveChannels ( RenderedChannelsData * result, NRenderContext * ctx );

    void                    PreRender           ( RenderedChannelsData * result );
    void                    PostRender          ( RenderedChannelsData * result, NRenderContext * ctx );

};

} //nrl
} //bv
