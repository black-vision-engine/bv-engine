#pragma once

#include "Engine/Graphics/SceneGraph/Scene.h"

#include "Engine/Graphics/Effects/Logic/Components/RenderChannel.h"

      
namespace bv {  

class RenderedChannelsData;
class RenderContext;
class FullscreenEffect;

class RenderLogicCore
{
private:

    std::vector< RenderChannelType > m_allChannels;
    FullscreenEffect *     m_blitWithAlphaEffect;

public:

                            RenderLogicCore    ();

    void                    Render              ( const SceneVec & scenes, RenderedChannelsData * result, RenderContext * ctx );
    void                    RenderDepth         ( const SceneVec & scenes, RenderedChannelsData * result, RenderContext * ctx );

private:

    void                    RenderScenes        ( const SceneVec & scenes, RenderedChannelsData * result, RenderContext * ctx );
    void                    RenderScene         ( Scene * scene, const RenderTarget * outputRT, RenderContext * ctx );

    void                    ClearActiveChannels ( RenderedChannelsData * result, RenderContext * ctx );

    void                    PreRender           ( RenderedChannelsData * result );
    void                    PostRender          ( RenderedChannelsData * result, RenderContext * ctx );

};


} //bv
