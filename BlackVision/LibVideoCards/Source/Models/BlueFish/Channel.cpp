#include "Channel.h"

#include "System/Time.h"


#include <process.h>


namespace bv { namespace videocards { namespace bluefish {




// ***********************
/// @todo Add SD resolution.
UInt32          Resolution2Width    ( UInt32 resolutionWidth )
{
    switch( resolutionWidth )
    {
    case 1080:
        return 1920;
    case 720:
        return 1280;
    case 576:
        return 720;
    }

    return 1920;
}






//**************************************
//
Channel::ChannelOptionMap     Channel::ChannelOptions = CreateChannelOptionMap();

// ***********************
//
Channel::Channel( ChannelName name )
    : m_channelName( name )
{}

//**************************************
//
Channel::~Channel()
{}

//**************************************
//
ChannelName Channel::GetName() const
{
    return m_channelName;
}

// ***********************
//
AVFrameDescriptor       Channel::CreateFrameDesc        () const
{
    AVFrameDescriptor frameDesc;

    frameDesc.height = GetResolution();
    frameDesc.width = Resolution2Width( frameDesc.height );
    frameDesc.depth = 4;

    frameDesc.channels = 2;                 // Stereo audio.
    frameDesc.numSamples = 48000 / 50;      // FIXME: Hardcoded frequency and framerate.
    frameDesc.channelDepth = 2;             // 16 bits per sample.

    return frameDesc;
}

// ***********************
//
Reusable< MemoryChunkPtr >      Channel::CreateReusableVideoChunks  ( UInt32 numChunks ) const
{
    auto desc = CreateFrameDesc();
    auto size = desc.width * desc.height * desc.depth;
    
    return CreateReusableChunks( numChunks, size );
}

// ***********************
//
Reusable< MemoryChunkPtr >      Channel::CreateReusableAudioChunks  ( UInt32 numChunks ) const
{
    auto desc = CreateFrameDesc();
    auto size = desc.numSamples * desc.channels * 2;

    return CreateReusableChunks( numChunks, size );
}

// ***********************
//
Reusable< MemoryChunkPtr >      Channel::CreateReusableChunks       ( UInt32 numChunks, UInt32 size ) const
{
    std::vector< MemoryChunkPtr > chunks;

    for( UInt32 i = 0; i < numChunks; ++i )
    {
        // FIXME: Why we need to substract 4096. For some reason frame is to long.
        MemoryChunkPtr chunk = MemoryChunk::Create( size );
        chunks.push_back( chunk );
    }

    return Reusable< MemoryChunkPtr >( chunks );
}

//**************************************
//
Channel::ChannelOptionMap       Channel::CreateChannelOptionMap     ()
{
    ChannelOptionMap channelOptionMap;

    ChannelOption A;
    A.InputChannel = BLUE_VIDEO_INPUT_CHANNEL_A;
    A.OutputChannel = BLUE_VIDEO_OUTPUT_CHANNEL_A;
    A.EpochSDIInput = EPOCH_SRC_SDI_INPUT_A;
    A.EpochSDIOutput = EPOCH_DEST_SDI_OUTPUT_A;
	A.EpochSDIKeyOutput = EPOCH_DEST_SDI_OUTPUT_B;
	A.EpochInputMemInterface = EPOCH_DEST_INPUT_MEM_INTERFACE_CHA;
    A.EpochOutputMemInterface = EPOCH_SRC_OUTPUT_MEM_INTERFACE_CHA;
	

    ChannelOption B;
    B.InputChannel = BLUE_VIDEO_INPUT_CHANNEL_B;
    B.OutputChannel = BLUE_VIDEO_OUTPUT_CHANNEL_B;
    B.EpochSDIInput = EPOCH_SRC_SDI_INPUT_B;
    B.EpochSDIOutput = EPOCH_DEST_SDI_OUTPUT_B;
	B.EpochSDIKeyOutput = EPOCH_DEST_SDI_OUTPUT_C;
    B.EpochInputMemInterface = EPOCH_DEST_INPUT_MEM_INTERFACE_CHB;
    B.EpochOutputMemInterface = EPOCH_SRC_OUTPUT_MEM_INTERFACE_CHB;

    ChannelOption C;
    C.InputChannel = BLUE_VIDEO_INPUT_CHANNEL_C;
    C.OutputChannel = BLUE_VIDEO_OUTPUT_CHANNEL_C;
    C.EpochSDIInput = EPOCH_SRC_SDI_INPUT_C;
    C.EpochSDIOutput = EPOCH_DEST_SDI_OUTPUT_C;
	C.EpochSDIKeyOutput = EPOCH_DEST_SDI_OUTPUT_D;
    C.EpochInputMemInterface = EPOCH_DEST_INPUT_MEM_INTERFACE_CHC;
    C.EpochOutputMemInterface = EPOCH_SRC_OUTPUT_MEM_INTERFACE_CHC;

    ChannelOption D;
    D.InputChannel = BLUE_VIDEO_INPUT_CHANNEL_D;
    D.OutputChannel = BLUE_VIDEO_OUTPUT_CHANNEL_D;
    D.EpochSDIInput = EPOCH_SRC_SDI_INPUT_D;
    D.EpochSDIOutput = EPOCH_DEST_SDI_OUTPUT_D;
	D.EpochSDIKeyOutput = EPOCH_DEST_SDI_OUTPUT_D;
    D.EpochInputMemInterface = EPOCH_DEST_INPUT_MEM_INTERFACE_CHD;
    D.EpochOutputMemInterface = EPOCH_SRC_OUTPUT_MEM_INTERFACE_CHD;

    channelOptionMap[ ChannelName::A ] = A;
    channelOptionMap[ ChannelName::B ] = B;
    channelOptionMap[ ChannelName::C ] = C;
    channelOptionMap[ ChannelName::D ] = D;

    return channelOptionMap;
}

} //bluefish
} //videocards
} //bv
