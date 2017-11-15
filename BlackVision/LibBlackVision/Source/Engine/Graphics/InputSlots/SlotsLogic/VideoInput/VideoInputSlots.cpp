#include "stdafx.h"
#include "VideoInputSlots.h"

#include "Engine/Graphics/Resources/Textures/Texture2D.h"

#include "UseLoggerLibBlackVision.h"


namespace bv
{


// ***********************
//
VideoInputSlots::VideoInputSlots    ( InputSlotsPtr slots )
    :   m_avInputSlots( slots )
{}

// ***********************
//
VideoInputSlots::~VideoInputSlots()
{}

// ***********************
//
bool            VideoInputSlots::RegisterVideoInputChannel      ( const videocards::VideoInputChannelDesc & vidInputDesc )
{
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
std::string                 VideoInputSlots::GenerateName   ( const videocards::VideoInputChannelDesc & vidInputDesc )
{
    return vidInputDesc.GetCardName() + " channel " + vidInputDesc.GetChannelName() + " input " + Convert::T2String( vidInputDesc.GetInputID() );
}

// ***********************
//
Expected< SlotIndex >       VideoInputSlots::GetSlotIndex   ( videocards::VideoInputID id ) const
{
    Expected< EntryIdx > entryIdx = FindEntry( id );
    
    if( entryIdx.IsValid() )
        return m_entries[ entryIdx.GetVal() ].GetSlotIdx();

    return Expected< EntryIdx >();
}

bool            VideoInputSlots::Exists         ( videocards::VideoInputID id ) const
{
    return FindEntry( id ).IsValid();
}

// ***********************
//
bool            VideoInputSlots::Exists         ( const videocards::VideoInputChannelDesc & vidInputDesc ) const
{
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



}	// bv


