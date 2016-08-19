//#include "stdafx.h"
#include "win_sock.h"
#include "BLueFish/Inc/BlueVelvet4.h"
#include "Utils.h"


namespace bv { namespace videocards { namespace bluefish {
    
//**************************************
//
std::pair< ChannelName, const char * > ChannelNameMapping[] = 
{   std::make_pair( ChannelName::A, "A" )
    , std::make_pair( ChannelName::B, "B" )
    , std::make_pair( ChannelName::C, "C" )
    , std::make_pair( ChannelName::D, "D" )
};

//**************************************
//
std::pair< IOType, const char * > IOTypeMapping[] = 
{   std::make_pair( IOType::FILL_KEY, "FILL_KEY" )
    , std::make_pair( IOType::FILL, "FILL" )
    , std::make_pair( IOType::KEY, "KEY" )
};

//**************************************
//
std::pair< ReferenceMode, const char * > ReferenceModeMapping[] = 
{   std::make_pair( ReferenceMode::FREERUN, "FREERUN" )
    , std::make_pair( ReferenceMode::IN_A, "IN_A" )
    , std::make_pair( ReferenceMode::IN_B, "IN_B" )
    , std::make_pair( ReferenceMode::ANALOG, "ANALOG" )
    , std::make_pair( ReferenceMode::GENLOCK, "GENLOCK" )
};

//**************************************
//
template<> ChannelName          String2T        ( const std::string & s )   {  return String2Enum( ChannelNameMapping, s ); }

//**************************************
//
template<> std::string          T2String        ( const ChannelName & t )   { return Enum2String( ChannelNameMapping, t ); }

//**************************************
//
template<> IOType               String2T        ( const std::string & s )   { return String2Enum( IOTypeMapping, s ); }

//**************************************
//
template<> std::string          T2String        ( const IOType & t )        { return Enum2String( IOTypeMapping, t ); }

//**************************************
//
template<> ReferenceMode        String2T        ( const std::string & s )   { return String2Enum( ReferenceModeMapping, s ); }

//**************************************
//
template<> std::string          T2String        ( const ReferenceMode & t ) { return Enum2String( ReferenceModeMapping, t ); }


//**************************************
//
template< typename T >
T String2Enum( const std::pair< T, const char* > t2s[], const std::string& s )
{
    int i = 0;
    while( strcmp( t2s[i].second, "" ) )
    {
        if( t2s[i].second == s )
            return t2s[i].first;
        ++i;
    }
    return T(0);
}

//**************************************
//
template< typename T >
std::string Enum2String( const std::pair< T, const char* > t2s[], const T& t )
{
    for( int i = 0; ; i++ )
        if( t2s[i].first == t )
            return t2s[i].second;
        else if( strcmp( t2s[i].second, "" ) == 0 )
            return "";
}

//**************************************
//
std::map< std::size_t, _EVideoMode >    CreateVideoModeMap  ()
{
    std::map< std::size_t, _EVideoMode > videoModeMap;

    videoModeMap[ VideoModeHash( 1080, 5000, true ) ] = VID_FMT_1080I_5000;
    videoModeMap[ VideoModeHash( 1080, 5994, true ) ] = VID_FMT_1080I_5994;
    videoModeMap[ VideoModeHash( 1080, 6000, true ) ] = VID_FMT_1080I_6000;
    videoModeMap[ VideoModeHash( 576, 5000, true ) ] = VID_FMT_576I_5000;
    videoModeMap[ VideoModeHash( 486, 5994, true ) ] = VID_FMT_486I_5994;

    videoModeMap[ VideoModeHash( 1080, 2397, false ) ] = VID_FMT_1080P_2397;
    videoModeMap[ VideoModeHash( 1080, 2397, false ) ] = VID_FMT_1080P_2397;
    videoModeMap[ VideoModeHash( 1080, 2400, false ) ] = VID_FMT_1080P_2400;
    videoModeMap[ VideoModeHash( 1080, 2500, false ) ] = VID_FMT_1080P_2500;
    videoModeMap[ VideoModeHash( 1080, 2997, false ) ] = VID_FMT_1080P_2997;
    videoModeMap[ VideoModeHash( 1080, 3000, false ) ] = VID_FMT_1080P_3000;
    videoModeMap[ VideoModeHash( 1080, 4800, false ) ] = VID_FMT_1080P_4800;
    videoModeMap[ VideoModeHash( 1080, 5000, false ) ] = VID_FMT_1080P_5000;
    videoModeMap[ VideoModeHash( 1080, 5994, false ) ] = VID_FMT_1080P_5994;
    videoModeMap[ VideoModeHash( 1080, 6000, false ) ] = VID_FMT_1080P_6000;
    videoModeMap[ VideoModeHash( 720, 2398, false ) ] = VID_FMT_720P_2398;
    videoModeMap[ VideoModeHash( 720, 2400, false ) ] = VID_FMT_720P_2400;
    videoModeMap[ VideoModeHash( 720, 2500, false ) ] = VID_FMT_720P_2500;
    videoModeMap[ VideoModeHash( 720, 2997, false ) ] = VID_FMT_720P_2997;
    videoModeMap[ VideoModeHash( 720, 3000, false ) ] = VID_FMT_720P_3000;
    videoModeMap[ VideoModeHash( 720, 5000, false ) ] = VID_FMT_720P_5000;
    videoModeMap[ VideoModeHash( 720, 5994, false ) ] = VID_FMT_720P_5994;
    videoModeMap[ VideoModeHash( 720, 6000, false ) ] = VID_FMT_720P_6000;

    return videoModeMap;
}

//**************************************
//
_EVideoMode     ConvertVideoMode    ( UInt32 resolution, UInt32 refresh, bool interlaced )
{
    auto hash = VideoModeHash( resolution, refresh, interlaced );
    if( VideoModeMap.count( hash ) )
    {
        return VideoModeMap.at( hash );
    }

    return VID_FMT_INVALID;
}

//**************************************
//
std::size_t     VideoModeHash( UInt32 resolution, UInt32 refresh, bool interlaced ) {
    std::size_t seed = 0;
    std::hash< UInt32 > hasher;

    seed ^= hasher( resolution ) + 0x9e3779b9 + ( seed << 6 ) + ( seed >> 2 );
    seed ^= hasher( refresh ) + 0x9e3779b9 + ( seed << 6 ) + ( seed >> 2 );
    seed ^= hasher( interlaced ) + 0x9e3779b9 + ( seed << 6 ) + ( seed >> 2 );

    return seed;
}


//**************************************
//
std::map< ReferenceMode, _EBlueGenlockSource >    CreateReferenceModeMap  ()
{
    std::map< ReferenceMode, _EBlueGenlockSource > referenceModeMap;

    referenceModeMap[ ReferenceMode::FREERUN ] = BlueFreeRunning;
    referenceModeMap[ ReferenceMode::IN_A ] = BlueSDI_A_BNC;
    referenceModeMap[ ReferenceMode::IN_B ] = BlueSDI_B_BNC;
    referenceModeMap[ ReferenceMode::ANALOG ] = BlueAnalog_BNC;
    referenceModeMap[ ReferenceMode::GENLOCK ] = BlueGenlockBNC;
    
    return referenceModeMap;
}

//**************************************
//
struct blue_videomode_info gVideoModeInfo[] = {
	{false,4.0,3.0,"PAL",VID_FMT_PAL,576,720,25,false,false,{1920,1920,1920,1920,1920}},
	{false,4.0,3.0,"NTSC",VID_FMT_NTSC,486,720,30,true,false,{1602,1601,1602,1601,1602}},
	{false,16.0,9.0,"720P 59.94",VID_FMT_720P_5994,720,1280,60,true,true,{801,800,801,801,801}},
	{false,16.0,9.0,"720P 60",VID_FMT_720P_6000,720,1280,60,false,true,{800,800,800,800,800}},
	{false,16.0,9.0,"1080PSF 23.97",VID_FMT_1080PSF_2397,1080,1920,24,true,false,{2002,2002,2002,2002,2002}},
	{false,16.0,9.0,"1080PSF 24.00",VID_FMT_1080PSF_2400,1080,1920,24,false,false,{2000,2000,2000,2000,2000}},
	{false,16.0,9.0,"1080P 23.97",VID_FMT_1080P_2397,1080,1920,24,true,true,{2002,2002,2002,2002,2002}},
	{false,16.0,9.0,"1080P 24",VID_FMT_1080P_2400,1080,1920,24,false,true,{2000,2000,2000,2000,2000}},
	{false,16.0,9.0,"1080I 50",VID_FMT_1080I_5000,1080,1920,25,false,false,{1920,1920,1920,1920,1920}},
	{false,16.0,9.0,"1080I 59.94",VID_FMT_1080I_5994,1080,1920,30,true,false,{1602,1601,1602,1601,1602}},
	{false,16.0,9.0,"1080I 60",VID_FMT_1080I_6000,1080,1920,30,false,false,{1600,1600,1600,1600,1600}},
	{false,16.0,9.0,"1080P 25",VID_FMT_1080P_2500,1080,1920,25,false,true,{1920,1920,1920,1920,1920}},
	{false,16.0,9.0,"1080P 29.97",VID_FMT_1080P_2997,1080,1920,30,true,true,{1602,1601,1602,1601,1602}},
	{false,16.0,9.0,"1080P 30",VID_FMT_1080P_3000,1080,1920,30,false,true,{1600,1600,1600,1600,1600}},
	{false,4.0,3.0,"HSDL 14",VID_FMT_HSDL_1498,1556,2048,15,true,false,{1602,1601,1602,1601,1602}},
	{false,4.0,3.0,"HSDL 15",VID_FMT_HSDL_1500,1556,2048,15,false,false,{1602,1601,1602,1601,1602}},
	{false,16.0,9.0,"720P 50",VID_FMT_720P_5000,720,1280,50,false,true,{960,960,960,960,960}},
	{false,16.0,9.0,"720P 23.98",VID_FMT_720P_2398,720,1280,24,true,true,{2002,2002,2002,2002,2002}},
	{false,16.0,9.0,"720P 24",VID_FMT_720P_2400,720,1280,24,false,true,{2000,2000,2000,2000,2000}},
	{false,16.0,9.0,"2Kx1080 PSF 23.97",VID_FMT_2048_1080PSF_2397,1080,2048,24,true,false,{2002,2002,2002,2002,2002}},
	{false,16.0,9.0,"2Kx1080 PSF 24",VID_FMT_2048_1080PSF_2400,1080,2048,24,false,false,{2000,2000,2000,2000,2000}},
	{false,16.0,9.0,"2Kx1080 P 23.97",VID_FMT_2048_1080P_2397,1080,2048,24,true,true,{2002,2002,2002,2002,2002}},
	{false,16.0,9.0,"2Kx1080 P 24",VID_FMT_2048_1080P_2400,1080,2048,24,false,true,{2000,2000,2000,2000,2000}},
	{false,16.0,9.0,"1080PSF 25",VID_FMT_1080PSF_2500,1080,1920,25,false,false,{1920,1920,1920,1920,1920}},
	{false,16.0,9.0,"1080PSF 29.97",VID_FMT_1080PSF_2997,1080,1920,30,true,false,{1602,1601,1602,1601,1602}},
	{false,16.0,9.0,"1080PSF 30",VID_FMT_1080PSF_3000,1080,1920,30,false,false,{1600,1600,1600,1600,1600}},
	{true,16.0,9.0,"1080P 50",VID_FMT_1080P_5000,1080,1920,50,false,true,{960,960,960,960,960}},
	{true,16.0,9.0,"1080P 59.94",VID_FMT_1080P_5994,1080,1920,60,true,true,{801,800,801,801,801}},
	{true,16.0,9.0,"1080P 60",VID_FMT_1080P_6000,1080,1920,60,false,true,{800,800,800,800,800}},
	{false,16.0,9.0,"720P 25",VID_FMT_720P_2500,720,1280,25,false,true,{1920,1920,1920,1920,1920}},
	{false,16.0,9.0,"720P 29.97",VID_FMT_720P_2997,720,1280,30,true,true,{1602,1601,1602,1601,1602}},
	{false,16.0,9.0,"720P 30",VID_FMT_720P_3000,720,1280,30,false,true,{1600,1600,1600,1600,1600}},
	{false,16.0,9.0,"DVB-ASI",VID_FMT_DVB_ASI,720,1280,50,false,true,{960,960,960,960,960}},
	{false,16.0,9.0,"2Kx1080 PSF 25",VID_FMT_2048_1080PSF_2500,1080,2048,25,false,false,{1920,1920,1920,1920,1920}},
	{false,16.0,9.0,"2Kx1080 PSF 29.97",VID_FMT_2048_1080PSF_2997,1080,2048,30,true,false,{1602,1601,1602,1601,1602}},
	{false,16.0,9.0,"2Kx1080 PSF 30",VID_FMT_2048_1080PSF_3000,1080,2048,30,false,false,{1600,1600,1600,1600,1600}},
	{false,16.0,9.0,"2Kx1080 P 25",VID_FMT_2048_1080P_2500,1080,2048,25,false,true,{1920,1920,1920,1920,1920}},
	{false,16.0,9.0,"2Kx1080 P 29.97",VID_FMT_2048_1080P_2997,1080,2048,30,true,true,{1602,1601,1602,1601,1602}},
	{false,16.0,9.0,"2Kx1080 P 30",VID_FMT_2048_1080P_3000,1080,2048,30,false,true,{1600,1600,1600,1600,1600}},
	{true,16.0,9.0,"2Kx1080 P 50",VID_FMT_2048_1080P_5000,1080,2048,50,false,true,{960,960,960,960,960}},
	{true,16.0,9.0,"2Kx1080 P 59.94",VID_FMT_2048_1080P_5994,1080,2048,59,true,true,{801,800,801,801,801}},
	{true,16.0,9.0,"2Kx1080 P 60",VID_FMT_2048_1080P_6000,1080,2048,60,false,true,{800,800,800,800,800}},
	{false,1.0,1.0,"VID_FMT_INVALID",VID_FMT_INVALID,0,0,0,false,true,{0,0,0,0,0}}
};

void BlueMemCpy(void* pDst, void* pSrc, size_t size)
{
	size_t tmpsize = 0;
	if(size >=8)
	{
		tmpsize = size%8;
		size /= 8;

		UINT64* pTmpSrc = (UINT64*)pSrc;
		UINT64* pTmpDst = (UINT64*)pDst;
		for(unsigned int i=0; i<size; i++)
		{
			*pTmpDst = *pTmpSrc;
			pTmpDst++;
			pTmpSrc++;
		}

		if(tmpsize)
			memcpy(pTmpDst, pTmpSrc, tmpsize);
	}
	else
	{
		memcpy(pDst, pSrc, size);
	}
}

void BlueMemZero(void* pData, size_t size)
{
	size_t tmpsize = 0;
	if(size >=8)
	{
		tmpsize = size%8;
		size /= 8;

		UINT64* pTmpData = (UINT64*)pData;
		for(unsigned int i=0; i<size; i++)
		{
			*pTmpData = 0LL;
			pTmpData++;
		}

		if(tmpsize)
			memset(pTmpData, 0, tmpsize);
	}
	else
	{
		memset(pData, 0, size);
	}
}

} //bluefish
} //videocards
} //bv