#include "BlueFishUtils.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"


namespace bv { namespace videocards { namespace bluefish {
  
//**************************************
//
std::map< std::size_t, _EVideoMode >    VideoModeMap = CreateVideoModeMap();

//**************************************
//
std::map< ReferenceMode, _EBlueGenlockSource >  ReferenceModeMap = CreateReferenceModeMap();

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
std::map< std::size_t, _EVideoMode >    CreateVideoModeMap  ()
{
    std::map< std::size_t, _EVideoMode > videoModeMap;

    videoModeMap[ VideoModeHash( 1080, 5000, true ) ] = VID_FMT_1080I_5000;
    videoModeMap[ VideoModeHash( 1080, 5994, true ) ] = VID_FMT_1080I_5994;
    videoModeMap[ VideoModeHash( 1080, 6000, true ) ] = VID_FMT_1080I_6000;
    videoModeMap[ VideoModeHash( 576, 5000, true ) ] = VID_FMT_576I_5000;
    videoModeMap[ VideoModeHash( 486, 5994, true ) ] = VID_FMT_486I_5994;

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

    referenceModeMap[ ReferenceMode::FreeRun ] = BlueFreeRunning;
    referenceModeMap[ ReferenceMode::DigitalInput1 ] = BlueSDI_A_BNC;
    referenceModeMap[ ReferenceMode::DigitalInput2 ] = BlueSDI_B_BNC;
    referenceModeMap[ ReferenceMode::AnalogBlackBurst ] = BlueGenlockBNC;
    referenceModeMap[ ReferenceMode::AnalogTriLevel ] = BlueGenlockBNC;
    referenceModeMap[ ReferenceMode::AnalogTriLevel ] = BlueGenlockBNC;
    referenceModeMap[ ReferenceMode::Interlock ] = BlueInterlock;
    referenceModeMap[ ReferenceMode::AnalogBlackBurstAux ] = BlueGenlockAux;
    referenceModeMap[ ReferenceMode::AnalogTriLevelAux ] = BlueGenlockAux;
    
    return referenceModeMap;
}

// ***********************
//
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

// ***********************
//
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


//**************************************
//
ReturnResult        UpdateReferenceOffset            ( CBlueVelvet4 * pSDK, int refH, int refV )
{
    if( pSDK )
    {
        unsigned int HPhase = 0, VPhase = 0, MaxHPhase = 0, MaxVPhase = 0;
        pSDK->get_timing_adjust( HPhase, VPhase, MaxHPhase, MaxVPhase );

        if( refH > ( int )MaxHPhase )
            return "Reference offset V [" + Convert::T2String( refH ) + "] exceeds max value [" + Convert::T2String( MaxHPhase ) + "].";

        if( refV > ( int )MaxVPhase )
            return "Reference offset H [" + Convert::T2String( refV ) + "] exceeds max value [" + Convert::T2String( MaxVPhase ) + "].";

        VARIANT varVal;
        varVal.vt = VT_UI4;
        varVal.ulVal = refH;
        varVal.ulVal |= ( ( refV & 0xFFFF ) << 16 );
        pSDK->SetCardProperty( GENLOCK_TIMING, varVal );

        return Result::Success();
    }
    else
    {
        return "BlueFish SDK not INITIALISED (UpdateReferenceOffset in CFifoPlayback).";
    }
}

//**************************************
//
ReturnResult        UpdateReferenceMode              ( CBlueVelvet4 * pSDK, long referenceMode )
{
    if( pSDK )
    {
        VARIANT varVal;
        varVal.vt = VT_UI4;
        varVal.ulVal = referenceMode;
        pSDK->SetCardProperty( VIDEO_GENLOCK_SIGNAL, varVal );

        return Result::Success();
    }
    else
    {
        return "BlueFish SDK not INITIALISED (UpdateReferenceMode in CFifoPlayback).";
    }
}

// ***********************
//
bool                IsReferenceSignalLocked         ( CBlueVelvet4 * pSDK )
{
    if( pSDK )
    {
        VARIANT varVal;
        varVal.vt = VT_UI4;
        pSDK->QueryCardProperty( EPOCH_GENLOCK_IS_LOCKED, varVal );

        if( varVal.ulVal ) return true;
        return false;
    }
    else
    {
        return false;
    }
}

// ***********************
//
ReferenceInfo       GetReferenceInfoImpl            ( CBlueVelvet4 * pSDK )
{
    if( pSDK )
    {
        ReferenceInfo info;

        unsigned int HPhase = 0, VPhase = 0, MaxHPhase = 0, MaxVPhase = 0;
        pSDK->get_timing_adjust( HPhase, VPhase, MaxHPhase, MaxVPhase );

        info.ReferenceH = HPhase;
        info.ReferenceV = VPhase;
        info.Mode = GetReferenceModeImpl( pSDK );

        return info;
    }

    return ReferenceInfo();
}

// ***********************
//
ReferenceMode       GetReferenceModeImpl            ( CBlueVelvet4 * pSDK )
{
    VARIANT varVal;
    varVal.vt = VT_UI4;
    pSDK->QueryCardProperty( EPOCH_GENLOCK_IS_LOCKED, varVal );

    //auto nGenlockSignal = varVal.ulVal & 0xFFFF; //reference source signal (lower 16 bits)
    _EBlueGenlockSource nGenlockSource = _EBlueGenlockSource( varVal.ulVal & 0xFFFF0000 ); //reference source (higher 16 bits); type EBlueGenlockSource

    for( auto & element : ReferenceModeMap )
    {
        if( element.second == nGenlockSource )
            return element.first;
    }

    return ReferenceMode::FailMode;
}

} //bluefish
} //videocards


namespace Convert
{
    
//**************************************
//
template<> Expected< videocards::bluefish::ChannelName >    String2T        ( const std::string & s )
{
    return String2Enum( videocards::bluefish::ChannelNameMapping, s );
}

//**************************************
//
template<> std::string                                      T2String        ( const videocards::bluefish::ChannelName & t )
{
    return Enum2String( videocards::bluefish::ChannelNameMapping, t );
}

} //Convert

} //bv