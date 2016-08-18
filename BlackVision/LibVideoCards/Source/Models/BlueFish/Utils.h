#pragma once

#include <string>

namespace bv { namespace videocards { namespace bluefish {


enum class IOType: int
{
    FILL_KEY,
    FILL,
    KEY
};

enum class ChannelName : int 
{
    A = 0,
    B,
    C,
    D
};


template< typename T >
std::string T2String( const T & t );

template< typename T >
T           String2T( const std::string & s );

template< typename T >
T String2Enum( const std::pair< T, const char* > t2s[], const std::string& s );

template< typename T >
std::string Enum2String( const std::pair< T, const char* > t2s[], const T& t );


struct ChannelOption 
{
    UInt32          InputChannel;
    UInt32          EpochSDIInput;
    UInt32          EpochInputMemInterface;
    UInt32          OutputChannel;
    UInt32          EpochOutputMemInterface;
    UInt32          EpochSDIOutput;
};


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

