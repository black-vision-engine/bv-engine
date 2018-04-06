#pragma once

#include <map>

#include "win_sock.h"
#include "BlueFish/Inc/BlueVelvet4.h"

#include "VideoCardManagerUtils.h"

#include "Memory/MemoryChunk.h"
#include "DataTypes/Reusable.h"
#include "CFrame.h"

#include "CoreDEF.h"


namespace bv { namespace videocards { namespace bluefish {


//**************************************
//
enum class ChannelName : int 
{
    A = 0,
    B,
    C,
    D
};


// ***********************
//
struct ChannelDataBase
{
    IOType                  type;
    EUpdateMethod           updateFormat;
    EMemoryFormat           memoryFormat;
    UInt32                  resolution;
};


//**************************************
//
struct ChannelInputData : public ChannelDataBase
{
    UInt32                  linkedVideoInput;
};

DEFINE_UPTR_TYPE( ChannelInputData )


//**************************************
//
struct ChannelOutputData : public ChannelDataBase
{
    UInt32                  cardNumber;
    UInt32                  refresh;
    bool                    interlaced;
    bool                    flipped;
    _EVideoMode             videoMode;
    UInt64                  linkedVideoOutput;
    bool                    autoGeneratedTimecode;
    bool                    timecodePresented;
};

DEFINE_UPTR_TYPE( ChannelOutputData )


//**************************************
//
_EVideoMode                                             ConvertVideoMode        ( UInt32 resolution, UInt32 refresh, bool interlaced );
std::size_t                                             VideoModeHash           ( UInt32 resolution, UInt32 refresh, bool interlaced );

std::map< std::size_t, _EVideoMode >                    CreateVideoModeMap      ();
std::map< ReferenceMode, _EBlueGenlockSource >          CreateReferenceModeMap  ();

extern  std::map< std::size_t, _EVideoMode >            VideoModeMap;
extern  std::map< ReferenceMode, _EBlueGenlockSource >  ReferenceModeMap;


//**************************************
//
struct ChannelOption 
{
    UInt32          InputChannel;
    UInt32          EpochSDIInput;
    UInt32          EpochInputMemInterface;
    UInt32          OutputChannel;
    UInt32          EpochOutputMemInterface;
    UInt32          EpochSDIOutput;
	UInt32          EpochSDIKeyOutput;
};

void BlueMemCpy(void* pDst, void* pSrc, size_t size);
void BlueMemZero(void* pData, size_t size);


ReturnResult             UpdateReferenceOffset   ( CBlueVelvet4 * pSDK, int refH, int refV );
ReturnResult             UpdateReferenceMode     ( CBlueVelvet4 * pSDK, long referenceMode );


// ***********************
//
inline Reusable< CFramePtr >            CreateReusableVideoChunks   ( UInt32 goldenSize, UInt32 bytesPerLine, UInt32 numChunks )
{
    std::vector< CFramePtr > chunks;

    for( UInt32 i = 0; i < numChunks; ++i )
    {
        chunks.push_back( std::make_shared< CFrame >( goldenSize, bytesPerLine ) );
    }

    return Reusable< CFramePtr >( chunks );
}


// ***********************
//
inline Reusable< CFramePtr >            CreateReusableVideoChunks   ( UInt32 goldenSize, UInt32 bytesPerLine, UInt32 audioSize, UInt32 numChunks )
{
    std::vector< CFramePtr > chunks;

    for( UInt32 i = 0; i < numChunks; ++i )
    {
        chunks.push_back( std::make_shared< CFrame >( goldenSize, bytesPerLine, audioSize ) );
    }

    return Reusable< CFramePtr >( chunks );
}

} //bluefish
} //videocards
} //bv

