#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/State/NRenderLogicState.h"

#include "Engine/Graphics/Effects/nrl/Logic/NRenderLogic.h"
#include "Engine/Graphics/Effects/nrl/Logic/NRenderLogicCore.h"


namespace bv { namespace nrl {

class NRenderLogicImpl : public NRenderLogic
{
private:
    
    NRenderLogicState               m_state;

    NRenderLogicCore                m_renderLogicCore;

public:

                            NRenderLogicImpl    ( unsigned int width, unsigned int height, unsigned int numTrackedRenderTargetsPerOutputType, unsigned int sharedMemScaleFactor );

    virtual void            HandleFrame         ( Renderer * renderer, audio::AudioRenderer * audio, const SceneVec & scenes ) override;

    virtual OutputLogic *   GetOutputLogic      () override;

private:
            void            RenderQueued        ( const SceneVec & scenes, RenderResult * result );
            void            Render              ( SceneNode * sceneRoot );

};

} // nrl
} // bv
