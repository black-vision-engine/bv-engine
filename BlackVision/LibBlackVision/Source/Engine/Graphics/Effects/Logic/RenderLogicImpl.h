#pragma once

#include "Engine/Graphics/Effects/Logic/State/RenderLogicState.h"

#include "Engine/Graphics/Effects/Logic/OutputRendering/OutputLogic.h"
#include "Engine/Graphics/Effects/Logic/Components/RenderedChannelsData.h"

#include "Engine/Graphics/Effects/Logic/RenderLogic.h"

#include "Engine/Graphics/Effects/Logic/Components/RenderLogicCore.h"


namespace bv { 

class RenderLogicImpl : public RenderLogic
{
private:
    
    RenderLogicState               m_state;

    RenderedChannelsData *          m_renderedChannelsData;
    OutputLogic *                   m_outputLogic;

    RenderLogicCore                m_renderLogicCore;

private:

                                    RenderLogicImpl        ( unsigned int width, unsigned int height );

public:
                            
                                    ~RenderLogicImpl       ();

    virtual void                    HandleFrame             ( Renderer * renderer, audio::AudioRenderer * audio, const SceneVec & scenes ) override;

    virtual OutputLogic *           GetOutputLogic          () override;
    virtual RenderedChannelsData *  GetRenderedChannelsData () override;

private:

    void                RenderQueued            ( const SceneVec & scenes );
    void                Render                  ( SceneNode * sceneRoot );

private:

    void                SetRenderedChannelsData ( RenderedChannelsData * rcd );
    void                SetOutputLogic          ( OutputLogic * outputLogic );

    RenderLogicState & AccessState             ();

public:

    static RenderLogicImpl *   Create          ( RenderLogicDesc & desc );

};

} // bv
