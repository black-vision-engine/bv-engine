#pragma once

#include <map>

#include "win_sock.h"
#include "BlueFish/Inc/BlueVelvet4.h"

#include "VideoCardManagerUtils.h"

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


//**************************************
//
struct ChannelInputData
{
    IOType                  type;
    bool                    playthrough;
};

DEFINE_UPTR_TYPE( ChannelInputData )


//**************************************
//
struct ChannelOutputData
{
    UInt32                  cardNumber;
    IOType                  type;
    UInt32                  resolution;
    UInt32                  refresh;
    bool                    interlaced;
    bool                    flipped;
    _EVideoMode             videoMode;
    _EBlueGenlockSource     referenceMode;
    Int32                   referenceH;
    Int32                   referenceV;
    EUpdateMethod           updateFormat;
    EMemoryFormat           memoryFormat;
    UInt64                  id;
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


//**************************************
//
struct blue_videomode_info
{
	bool			bIs3G;
	float			fFrameAspectRatioX;
	float			fFrameAspectRatioY;	
	std::string		strVideoModeFriendlyName;
	unsigned int	nVideoMode;
	unsigned int	nHeight;
	unsigned int	nWidth;
	unsigned int	nFrameRate;
	bool			bIs1001FrameRate;
	bool			bIsProgressive;
	unsigned int	nAudioFrameSequence[5];
};

void BlueMemCpy(void* pDst, void* pSrc, size_t size);
void BlueMemZero(void* pData, size_t size);


} //bluefish
} //videocards
} //bv

