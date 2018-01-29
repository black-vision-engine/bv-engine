#include "stdafx.h"
#include "VideoInputSlots.h"

#include "Engine/Graphics/Resources/Textures/Texture2D.h"
#include "Engine/Graphics/Effects/Logic/Components/RenderContext.h"

#include "Assets/Input/VideoInput/VideoInputAsset.h"
#include "Assets/Input/VideoInput/VideoInputAssetDesc.h"

#include "UseLoggerLibBlackVision.h"


namespace bv
{


// ***********************
//
VideoInputSlots::VideoInputSlots    ( InputSlotsPtr slots )
    :   m_avInputSlots( slots )
{}

// ***********************
/// @todo Destructor of VideoInputSlots should release all textures and eudio entities.
/// We can't do this without RenderContext so resposibility goes to outside class to call Unregister all.
VideoInputSlots::~VideoInputSlots()
{}

// ***********************
//
bool            VideoInputSlots::RegisterVideoInputChannel      ( const videocards::VideoInputChannelDesc & vidInputDesc )
{
    std::lock_guard< std::recursive_mutex > guard( m_lock );

    if( Exists( vidInputDesc ) || Exists( vidInputDesc.GetInputID() ) )
    {
        LOG_MESSAGE( SeverityLevel::error ) << "Can't add video input slot [" << vidInputDesc.GetCardName() << "], channel [" << vidInputDesc.GetChannelName() << "], input id [" << Convert::T2String( vidInputDesc.GetInputID() ) << "]. Similar input exists.";
        return false;
    }

    Expected< SlotIndex > slotIdx = Register( vidInputDesc );
    if( slotIdx.IsValid() )
    {
        VideoInputSlotEntryDesc desc( vidInputDesc, slotIdx.GetVal() );
        m_entries.push_back( desc );
        
        return true;
    }

    // Fail message was already logged by InputSlots class.
    return false;
}

// ***********************
//
bool                        VideoInputSlots::UnregisterVideoInputChannel    ( RenderContext * ctx, videocards::VideoInputID id )
{
    std::lock_guard< std::recursive_mutex > guard( m_lock );

    auto entryIdx = FindEntry( id );
    if( entryIdx.IsValid() )
    {
        auto slotIdx = GetSlotIndex( id );
        auto slot = m_avInputSlots.GetInputSlots()->AccessSource( slotIdx );

        if( slot.IsValid() )
        {
            // Free texture and audio entity after removing slot.
            auto texture = slot.GetVal().Texture;
            auto audio = slot.GetVal().Audio;

            if( m_avInputSlots.GetInputSlots()->UnregisterSource( slotIdx ) )
            {
                FreeTexture( ctx, texture );
                FreeAudio( ctx, audio );

                m_entries.erase( m_entries.begin() + entryIdx.GetVal() );
                return true;
            }

            LOG_MESSAGE( SeverityLevel::error ) << "Input Slot disapeared. This looks like race conditions!";
        }
    }

    return false;
}

// ***********************
//
bool                        VideoInputSlots::UnregisterAllChannels          ( RenderContext * ctx )
{
    std::lock_guard< std::recursive_mutex > guard( m_lock );

    bool result = true;
    auto entriesCopy = m_entries;   // Can't iterate over vector and remove elements in the same time.

    for( auto & entry : entriesCopy )
    {
        bool entryRemoveResult = UnregisterVideoInputChannel( ctx, entry.GetVideoInputID() );
        result = result && entryRemoveResult;
    }

    return result;
}

// ***********************
//
void                        VideoInputSlots::UpdateVideoInput   ( videocards::VideoInputID id, AVFramePtr frame )
{
    std::lock_guard< std::recursive_mutex > guard( m_lock );

    auto slotIdx = GetSlotIndex( id );
    m_avInputSlots.UpdateSlot( slotIdx, frame );
}

// ***********************
//
Expected< SlotIndex >       VideoInputSlots::Register       ( const videocards::VideoInputChannelDesc & vidInputDesc )
{
    auto texture = CreateTexture( vidInputDesc );
    auto audio = CreateAudio( vidInputDesc );

    InputSlot input( texture, audio );
    
    return m_avInputSlots.GetInputSlots()->RegisterSource( input, GenerateName( vidInputDesc ) );
}

// ***********************
//
Texture2DPtr                VideoInputSlots::CreateTexture  ( const videocards::VideoInputChannelDesc & vidInputDesc )
{
    auto width = vidInputDesc.GetDataDesc().width;
    auto height = vidInputDesc.GetDataDesc().height;
    TextureFormat format = TextureFormat::F_A8R8G8B8;
    DataBuffer::Semantic semantic = DataBuffer::Semantic::S_TEXTURE_STREAMING_WRITE;
    UInt32 levels = 1;

    auto pixelDepth = vidInputDesc.GetDataDesc().depth;
    if( pixelDepth == 4 )
    {
        format = TextureFormat::F_A8R8G8B8;
    }
    else
    {
        LOG_MESSAGE( SeverityLevel::warning ) << "Unknown video input format.";
        assert( false );
        return nullptr;
    }

    return std::make_shared< Texture2D >( format, width, height, semantic, levels );
}

// ***********************
//
audio::AudioEntity *        VideoInputSlots::CreateAudio    ( const videocards::VideoInputChannelDesc & vidInputDesc )
{
    vidInputDesc;
    return nullptr;
}

// ***********************
//
void                        VideoInputSlots::FreeTexture    ( RenderContext * ctx, Texture2DPtr texture )
{
    if( texture )
        ctx->GetRenderer()->DeletePDR( texture.get() );
}

// ***********************
//
void                        VideoInputSlots::FreeAudio      ( RenderContext * ctx, audio::AudioEntity * audio )
{
    if( audio )
    {
        ctx->GetAudio()->DeletePDR( audio );
        delete audio;
    }
}

// ***********************
//
std::string                 VideoInputSlots::GenerateName   ( const videocards::VideoInputChannelDesc & vidInputDesc )
{
    return vidInputDesc.GetCardName() + " channel " + vidInputDesc.GetChannelName() + " input " + Convert::T2String( vidInputDesc.GetInputID() );
}

// ***********************
//
Expected< SlotIndex >       VideoInputSlots::GetSlotIndex   ( videocards::VideoInputID id ) const
{
    std::lock_guard< std::recursive_mutex > guard( m_lock );

    Expected< EntryIdx > entryIdx = FindEntry( id );
    
    if( entryIdx.IsValid() )
        return m_entries[ entryIdx.GetVal() ].GetSlotIdx();

    return Expected< EntryIdx >();
}

// ***********************
//
bool            VideoInputSlots::Exists         ( videocards::VideoInputID id ) const
{
    std::lock_guard< std::recursive_mutex > guard( m_lock );

    return FindEntry( id ).IsValid();
}

// ***********************
//
bool            VideoInputSlots::Exists         ( const videocards::VideoInputChannelDesc & vidInputDesc ) const
{
    std::lock_guard< std::recursive_mutex > guard( m_lock );

    return FindEntry( vidInputDesc ).IsValid();
}

// ***********************
//
Expected< VideoInputSlots::EntryIdx >       VideoInputSlots::FindEntry  ( const videocards::VideoInputChannelDesc & vidInputDesc ) const
{
    for( EntryIdx idx = 0; idx < m_entries.size(); idx++ )
    {
        if( vidInputDesc.IsEquivalent( m_entries[ idx ].GetVideoChannelDesc() ) )
            return idx;
    }

    return Expected< VideoInputSlots::EntryIdx >();
}

// ***********************
//
Expected< VideoInputSlots::EntryIdx >       VideoInputSlots::FindEntry  ( videocards::VideoInputID id ) const
{
    for( EntryIdx idx = 0; idx < m_entries.size(); idx++ )
    {
        if( m_entries[ idx ].GetVideoInputID() == id )
            return idx;
    }

    return Expected< VideoInputSlots::EntryIdx >();
}


// ***********************
//
VideoInputAssetConstPtr         VideoInputSlots::CreateAsset        ( VideoInputSlotsPtr thisPtr, VideoInputAssetDescConstPtr desc )
{
    std::lock_guard< std::recursive_mutex > guard( m_lock );

    auto asset = VideoInputAsset::Create( thisPtr, desc->GetVideoInputID() );
    asset->EvaluateSlot();

    return asset;
}



}	// bv


