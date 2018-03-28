#pragma once

#include "Engine/Graphics/InputSlots/InputSlots.h"
#include "Engine/Graphics/InputSlots/SlotsLogic/AVFrame/AVFrameInputSlots.h"

#include "VideoInputSlotEntryDesc.h"

#include <mutex>


namespace bv
{

class RenderContext;
class VideoInputSlots;
class VideoInputAssetDesc;
class VideoInputAsset;
class VideoInputTextureAssetDesc;
class VideoInputTextureAsset;
class VideoInputAudioAsset;
class VideoInputAudioAssetDesc;
DEFINE_PTR_TYPE( VideoInputSlots )
DEFINE_CONST_PTR_TYPE( VideoInputAsset )
DEFINE_CONST_PTR_TYPE( VideoInputAssetDesc )
DEFINE_PTR_TYPE( VideoInputTextureAsset )
DEFINE_CONST_PTR_TYPE( VideoInputTextureAsset )
DEFINE_CONST_PTR_TYPE( VideoInputTextureAssetDesc )
DEFINE_PTR_TYPE( VideoInputAudioAsset )
DEFINE_CONST_PTR_TYPE( VideoInputAudioAsset )
DEFINE_CONST_PTR_TYPE( VideoInputAudioAssetDesc )




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
    mutable std::recursive_mutex                m_lock;

public:

    explicit        VideoInputSlots ( InputSlotsPtr slots );
    virtual         ~VideoInputSlots();


    bool            RegisterVideoInputChannel       ( const videocards::VideoInputChannelDesc & vidInputDesc );    
    bool            UnregisterVideoInputChannel     ( RenderContext * ctx, videocards::VideoInputID id );
    bool            UnregisterAllChannels           ( RenderContext * ctx );
    void            UpdateVideoInput                ( videocards::VideoInputID id, AVFrameConstPtr frame );


public:

    bool                        Exists              ( videocards::VideoInputID id ) const;
    bool                        Exists              ( const videocards::VideoInputChannelDesc & vidInputDesc ) const;

    Expected< SlotIndex >       GetSlotIndex        ( videocards::VideoInputID id ) const;

    InputSlotsPtr               GetInputSlots       () const { return m_avInputSlots.GetInputSlots(); }


    Expected< videocards::VideoInputChannelDesc >   GetVideoCardFromSlot    ( SlotIndex idx );

private:

    Expected< EntryIdx >        FindEntry       ( const videocards::VideoInputChannelDesc & vidInputDesc ) const;
    Expected< EntryIdx >        FindEntry       ( videocards::VideoInputID id ) const;

    Expected< SlotIndex >       Register        ( const videocards::VideoInputChannelDesc & vidInputDesc );

    Texture2DPtr                CreateTexture   ( const videocards::VideoInputChannelDesc & vidInputDesc );
    audio::AudioBufferPtr       CreateAudio     ( const videocards::VideoInputChannelDesc & vidInputDesc );

    void                        FreeTexture     ( RenderContext * ctx, Texture2DPtr texture );
    void                        FreeAudio       ( RenderContext * ctx, AudioSlotPtr audio );

    std::string                 GenerateName    ( const videocards::VideoInputChannelDesc & vidInputDesc );

public:

    ///@name VideoInputAsset creation
    ///@{
    VideoInputTextureAssetConstPtr  CreateAsset     ( VideoInputSlotsPtr thisPtr, VideoInputTextureAssetDescConstPtr desc );
    VideoInputAudioAssetConstPtr    CreateAsset     ( VideoInputSlotsPtr thisPtr, VideoInputAudioAssetDescConstPtr desc );
    ///@}

};



}	// bv


