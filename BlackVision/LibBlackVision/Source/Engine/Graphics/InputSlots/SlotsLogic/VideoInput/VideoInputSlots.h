#pragma once

#include "Engine/Graphics/InputSlots/InputSlots.h"
#include "Engine/Graphics/InputSlots/SlotsLogic/AVFrame/AVFrameInputSlots.h"

#include "VideoInputSlotEntryDesc.h"



namespace bv
{


/**@brief Wrapper for inputs slots from video cards.

@ingroup EngineInputs
@ingroup VideoCards*/
class VideoInputSlots
{
private:

    typedef SizeType EntryIdx;

private:

    AVFrameInputSlots       m_avInputSlots;

    std::vector< VideoInputSlotEntryDesc >      m_entries;

public:

    explicit        VideoInputSlots ( InputSlotsPtr slots );
    virtual         ~VideoInputSlots();


    bool            RegisterVideoInputChannel       ( const videocards::VideoInputChannelDesc & vidInputDesc );    
    void            UpdateVideoInput                ( videocards::VideoInputID id, AVFramePtr frame );


public:

    bool                        Exists              ( videocards::VideoInputID id ) const;
    bool                        Exists              ( const videocards::VideoInputChannelDesc & vidInputDesc ) const;

    Expected< SlotIndex >       GetSlotIndex        ( videocards::VideoInputID id ) const;

private:

    Expected< EntryIdx >        FindEntry       ( const videocards::VideoInputChannelDesc & vidInputDesc ) const;
    Expected< EntryIdx >        FindEntry       ( videocards::VideoInputID id ) const;

    Expected< SlotIndex >       Register        ( const videocards::VideoInputChannelDesc & vidInputDesc );

    Texture2DPtr                CreateTexture   ( const videocards::VideoInputChannelDesc & vidInputDesc );
    audio::AudioEntity *        CreateAudio     ( const videocards::VideoInputChannelDesc & vidInputDesc );

    std::string                 GenerateName    ( const videocards::VideoInputChannelDesc & vidInputDesc );
};



}	// bv


