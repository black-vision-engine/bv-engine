#pragma once

#include "Engine/Graphics/InputSlots/Logic/IInputHandler.h"
#include "Engine/Graphics/Effects/Logic/Components/RenderContext.h"

#include "Engine/Graphics/InputSlots/SlotsLogic/VideoInput/VideoInputSlots.h"

#include "VideoCardManager.h"


namespace bv
{

// ***********************
/// @ingroup EngineInputs
class VideoInputHandler : public IInputHandler
{
private:

    videocards::VideoCardManager *      m_videoCardManager;
    
    // We need this to release resources.
    Renderer *              m_renderer;
    audio::AudioRenderer *  m_audioRenderer;

public:

    explicit                VideoInputHandler       ( videocards::VideoCardManager * videoCardMan );

    virtual void            UnregisterInputs        () override;
    virtual void            ProcessInputs           ( RenderContext * ctx ) override;

    virtual void            RegisterInputs          ( RenderContext * ctx, InputSlotsPtr inputSlots ) override;

};

DEFINE_PTR_TYPE( VideoInputHandler )


}	// bv

