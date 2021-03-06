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
    FRIEND_TEST_ACCESSOR( VideoInputHandler )
private:

    videocards::VideoCardManager *      m_videoCardManager;
    VideoInputSlotsPtr                  m_inputSlots;
    
    // We need this to release resources. Resources need better handling :(
    Renderer *              m_renderer;
    audio::AudioRenderer *  m_audioRenderer;

public:

    explicit                VideoInputHandler       ( videocards::VideoCardManager * videoCardMan, InputSlotsPtr slots );
    virtual                 ~VideoInputHandler      ();

    virtual void            UnregisterInputs        () override;
    virtual void            ProcessInputs           ( RenderContext * ctx ) override;

    virtual void            RegisterInputs          ( RenderContext * ctx, InputSlotsPtr inputSlots ) override;

    VideoInputSlotsPtr      GetVideoInputSlots      () const { return m_inputSlots; }

private:

    void                    RegisterInputs          ( const videocards::InputChannelsDescsVec & channelsDesc );
    void                    ProcessFrameData        ( videocards::VideoInputFrameData & frameData );

private:

    void                    OnAllSlotReferencesRemoved      ( IEventPtr evt );
    void                    OnFirstSlotReference            ( IEventPtr evt );

    void                    EnableChannel           ( videocards::VideoCardID cardID, videocards::VideoInputID inputID );
    void                    DisableChannel          ( videocards::VideoCardID cardID, videocards::VideoInputID inputID );

    videocards::IVideoCardPtr   FindVideoCard       ( videocards::VideoCardID cardID ) const;
};

DEFINE_PTR_TYPE( VideoInputHandler )


}	// bv

