#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/State/NRenderLogicState.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/OutputLogic.h"
#include "Engine/Graphics/Effects/nrl/Logic/Components/RenderedChannelsData.h"

#include "Engine/Graphics/Effects/nrl/Logic/NRenderLogic.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/NRenderLogicCore.h"


namespace bv { namespace nrl {

class NRenderLogicImpl : public NRenderLogic
{
private:
    
    NRenderLogicState               m_state;

    RenderedChannelsData *          m_renderedChannelsData;
    OutputLogic *                   m_outputLogic;

    NRenderLogicCore                m_renderLogicCore;

private:

                                    NRenderLogicImpl        ( unsigned int width, unsigned int height );

public:
                            
                                    ~NRenderLogicImpl       ();

    virtual void                    HandleFrame             ( Renderer * renderer, audio::AudioRenderer * audio, const SceneVec & scenes ) override;

    virtual OutputLogic *           GetOutputLogic          () override;
    virtual RenderedChannelsData *  GetRenderedChannelsData () override;

private:

    void                RenderQueued            ( const SceneVec & scenes );
    void                Render                  ( SceneNode * sceneRoot );

private:

    void                SetRenderedChannelsData ( RenderedChannelsData * rcd );
    void                SetOutputLogic          ( OutputLogic * outputLogic );

    NRenderLogicState & AccessState             ();

public:

    static NRenderLogicImpl *   Create          ( NRenderLogicDesc & desc );

};

} // nrl
} // bv
