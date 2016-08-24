#pragma once

#include <map>

#include "win_sock.h"
#include "BlueFish/Inc/BlueVelvet4.h"

#include "CoreDEF.h"


namespace bv { namespace videocards { namespace bluefish {

//**************************************
//
enum class IOType: int
{
    FILL_KEY,
    FILL,
    KEY,
    INVALID
};

enum class ChannelName : int 
{
    A = 0,
    B,
    C,
    D
};

enum class ReferenceMode : int 
{
    FREERUN = 0,
    IN_A,
    IN_B,
    ANALOG,
    GENLOCK
};


//**************************************
//
template< typename T >
std::string T2String    ( const T & t );

template< typename T >
T           String2T    ( const std::string & s );

template< typename T >
T           String2Enum ( const std::pair< T, const char* > t2s[], const std::string& s );

template< typename T >
std::string Enum2String ( const std::pair< T, const char* > t2s[], const T& t );


//**************************************
//
_EVideoMode                             ConvertVideoMode    ( UInt32 resolution, UInt32 refresh, bool interlaced );
std::map< std::size_t, _EVideoMode >    CreateVideoModeMap  ();
std::size_t                             VideoModeHash       ( UInt32 resolution, UInt32 refresh, bool interlaced );
extern  std::map< std::size_t, _EVideoMode >    VideoModeMap;

std::map< ReferenceMode, _EBlueGenlockSource >  CreateReferenceModeMap  ();
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

