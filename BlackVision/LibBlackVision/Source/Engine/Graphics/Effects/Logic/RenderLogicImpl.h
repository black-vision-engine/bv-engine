#pragma once

#include "Engine/Graphics/Effects/Logic/State/RenderLogicState.h"

#include "Engine/Graphics/Effects/Logic/OutputRendering/OutputLogic.h"
#include "Engine/Graphics/InputSlots/Logic/InputLogic.h"
#include "Engine/Graphics/Effects/Logic/Components/RenderedChannelsData.h"

#include "Engine/Graphics/Effects/Logic/RenderLogic.h"

#include "Engine/Graphics/Effects/Logic/Components/RenderLogicCore.h"


namespace bv { 

class RenderLogicImpl : public RenderLogic
{
private:
    
    RenderLogicState               m_state;

    RenderedChannelsData *         m_renderedChannelsData;
    OutputLogic *                  m_outputLogic;
    InputLogic *                   m_inputLogic;

    RenderLogicCore                m_renderLogicCore;

private:

                                    RenderLogicImpl        ( unsigned int width, unsigned int height );

public:
                            
                                    ~RenderLogicImpl       ();

    virtual void                    HandleFrame             ( Renderer * renderer, audio::AudioRenderer * audio, const SceneVec & scenes ) override;
    virtual void                    SwitchEditMode          ( bool value ) override;

    virtual OutputLogic *           GetOutputLogic          () override;
    virtual InputLogic *            GetInputLogic           () override;
    virtual RenderedChannelsData *  GetRenderedChannelsData () override;

private:

    void                RenderQueued            ( const SceneVec & scenes );
    void                RenderDepth             ( const SceneVec & scenes );
    void                RenderGizmos            ( const SceneVec & scenes );
    void                BlitGizmoTargets        ();
    void                Render                  ( SceneNode * sceneRoot );

private:

    void                SetRenderedChannelsData ( RenderedChannelsData * rcd );
    void                SetOutputLogic          ( OutputLogic * outputLogic );
    void                SetInputLogic           ( InputLogic * inputLogic );

    RenderLogicState & AccessState             ();

public:

    static RenderLogicImpl *   Create          ( RenderLogicDesc & desc );
    
};

} // bv
