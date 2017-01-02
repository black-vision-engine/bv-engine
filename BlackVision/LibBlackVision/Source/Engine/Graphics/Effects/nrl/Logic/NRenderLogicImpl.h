#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/State/NRenderLogicState.h"

#include "Engine/Graphics/Effects/nrl/Logic/NRenderLogic.h"
#include "Engine/Graphics/Effects/nrl/Logic/NRenderLogicCore.h"
#include "Engine/Graphics/Effects/nrl/Logic/NAudioLogicCore.h"


namespace bv { 

class Renderer;
   
namespace nrl {

class NRenderLogicImpl : public NRenderLogic
{
private:
    
    NRenderLogicState               m_state;

    NRenderLogicCore                m_renderLogicCore;
    NAudioLogicCore                 m_audioLogicCore;

public:

                            NRenderLogicImpl    ( unsigned int width, unsigned int height, unsigned int numTrackedRenderTargetsPerOutputType = 2 );

    virtual void            RenderFrame         ( Renderer * renderer, audio::AudioRenderer * audio, const SceneVec & scenes ) override;

    virtual Preview *       GetPreview          () override;
    virtual VideoOutput *   GetVideoOutput      () override;

private:
            void            RenderQueued        ( const SceneVec & scenes, RenderResult * result );
            void            Render              ( SceneNode * sceneRoot );

};

} // nrl
} // bv
