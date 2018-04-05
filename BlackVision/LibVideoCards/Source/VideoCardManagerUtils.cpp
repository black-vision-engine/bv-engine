#include "VideoCardManagerUtils.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"


namespace bv {
namespace Convert
{


// ***********************
//
std::pair< videocards::ReferenceMode, const char* > VideoCardVideoReferenceModeMapping[] =
{
    std::make_pair( videocards::ReferenceMode::FreeRun, "FreeRun" )
    , std::make_pair( videocards::ReferenceMode::AnalogBlackBurst, "AnalogBlackBurst" )
    , std::make_pair( videocards::ReferenceMode::AnalogTriLevel, "AnalogTriLevel" )
    , std::make_pair( videocards::ReferenceMode::DigitalInput1, "DigitalInput1" )
    , std::make_pair( videocards::ReferenceMode::DigitalInput2, "DigitalInput2" )
    , std::make_pair( videocards::ReferenceMode::Interlock, "Interlock" )
    , std::make_pair( videocards::ReferenceMode::AnalogBlackBurstAux, "AnalogBlackBurstAux" )
    , std::make_pair( videocards::ReferenceMode::AnalogTriLevelAux, "AnalogTriLevelAux" )
    , std::make_pair( videocards::ReferenceMode::FailMode, "" )      // default
};

IMPLEMENT_ENUM_SERIALIZATION( videocards::ReferenceMode, VideoCardVideoReferenceModeMapping );

}   // Convert


namespace videocards {
  
//**************************************
//
std::pair< IOType, const char * > IOTypeMapping[] = 
{   std::make_pair( IOType::FILL_KEY, "FILL_KEY" )
    , std::make_pair( IOType::FILL, "FILL" )
    , std::make_pair( IOType::KEY, "KEY" )
};


namespace
{
//**************************************
//
struct VideoModeInfo
{
    bool			is3G;
    float			frameAspectRatioX;
    float			frameAspectRatioY;
    std::string		videoModeFriendlyName;
    unsigned int	videoMode;
    unsigned int	height;
    unsigned int	width;
    unsigned int	frameRate;
    bool		    is1001FrameRate;
    bool			isProgressive;
    unsigned int	audioFrameSequence[ 5 ];
};

//**************************************
//
typedef enum _EVideoMode
{
    VID_FMT_PAL = 0,        /**< 720  x 576  50       Interlaced */
    VID_FMT_NTSC = 1,       /**< 720  x 486  60/1.001 Interlaced */
    VID_FMT_720P_5994,		/**< 1280 x 720  60/1.001 Progressive */
    VID_FMT_720P_6000,		/**< 1280 x 720  60       Progressive */
    VID_FMT_1080PSF_2397,	/**< 1920 x 1080 24/1.001 Segment Frame */
    VID_FMT_1080PSF_2400,	/**< 1920 x 1080 24       Segment Frame */
    VID_FMT_1080P_2397,		/**< 1920 x 1080 24/1.001 Progressive */
    VID_FMT_1080P_2400,		/**< 1920 x 1080 24       Progressive */
    VID_FMT_1080I_5000,		/**< 1920 x 1080 50       Interlaced */
    VID_FMT_1080I_5994,		/**< 1920 x 1080 60/1.001 Interlaced */
    VID_FMT_1080I_6000,		/**< 1920 x 1080 60       Interlaced */
    VID_FMT_1080P_2500,		/**< 1920 x 1080 25       Progressive */
    VID_FMT_1080P_2997,		/**< 1920 x 1080 30/1.001 Progressive */
    VID_FMT_1080P_3000,		/**< 1920 x 1080 30       Progressive */
    VID_FMT_HSDL_1498,		/**< 2048 x 1556 15/1.0   Segment Frame */
    VID_FMT_HSDL_1500,		/**< 2048 x 1556 15			Segment Frame */
    VID_FMT_720P_5000,		/**< 1280 x 720  50			Progressive */
    VID_FMT_720P_2398,		/**< 1280 x 720  24/1.001	Progressive */
    VID_FMT_720P_2400,		/**< 1280 x 720  24	Progressive */
    VID_FMT_2048_1080PSF_2397 = 19,	/**< 2048 x 1080 24/1.001	Segment Frame */
    VID_FMT_2048_1080PSF_2400 = 20,	/**< 2048 x 1080 24	Segment Frame */
    VID_FMT_2048_1080P_2397 = 21,	/**< 2048 x 1080 24/1.001	progressive */
    VID_FMT_2048_1080P_2400 = 22,	/**< 2048 x 1080 24	progressive  */
    VID_FMT_1080PSF_2500 = 23,
    VID_FMT_1080PSF_2997 = 24,
    VID_FMT_1080PSF_3000 = 25,
    VID_FMT_1080P_5000 = 26,
    VID_FMT_1080P_5994 = 27,
    VID_FMT_1080P_6000 = 28,
    VID_FMT_720P_2500 = 29,
    VID_FMT_720P_2997 = 30,
    VID_FMT_720P_3000 = 31,
    VID_FMT_DVB_ASI = 32,
    VID_FMT_2048_1080PSF_2500 = 33,
    VID_FMT_2048_1080PSF_2997 = 34,
    VID_FMT_2048_1080PSF_3000 = 35,
    VID_FMT_2048_1080P_2500 = 36,
    VID_FMT_2048_1080P_2997 = 37,
    VID_FMT_2048_1080P_3000 = 38,
    VID_FMT_2048_1080P_5000 = 39,
    VID_FMT_2048_1080P_5994 = 40,
    VID_FMT_2048_1080P_6000 = 41,
    VID_FMT_1080P_4800 = 42,
    VID_FMT_2048_1080P_4800 = 43,

    VID_FMT_INVALID = 44
} EVideoMode;

//**************************************
//
struct VideoModeInfo gVideoModeInfo [] = {
    { false,4.0,3.0,"PAL",VID_FMT_PAL,576,720,25,false,false, { 1920,1920,1920,1920,1920 } },
    { false,4.0,3.0,"NTSC",VID_FMT_NTSC,486,720,30,true,false, { 1602,1601,1602,1601,1602 } },
    { false,16.0,9.0,"720P 59.94",VID_FMT_720P_5994,720,1280,60,true,true, { 801,800,801,801,801 } },
    { false,16.0,9.0,"720P 60",VID_FMT_720P_6000,720,1280,60,false,true, { 800,800,800,800,800 } },
    { false,16.0,9.0,"1080PSF 23.97",VID_FMT_1080PSF_2397,1080,1920,24,true,false, { 2002,2002,2002,2002,2002 } },
    { false,16.0,9.0,"1080PSF 24.00",VID_FMT_1080PSF_2400,1080,1920,24,false,false, { 2000,2000,2000,2000,2000 } },
    { false,16.0,9.0,"1080P 23.97",VID_FMT_1080P_2397,1080,1920,24,true,true, { 2002,2002,2002,2002,2002 } },
    { false,16.0,9.0,"1080P 24",VID_FMT_1080P_2400,1080,1920,24,false,true, { 2000,2000,2000,2000,2000 } },
    { false,16.0,9.0,"1080I 50",VID_FMT_1080I_5000,1080,1920,25,false,false, { 1920,1920,1920,1920,1920 } },
    { false,16.0,9.0,"1080I 59.94",VID_FMT_1080I_5994,1080,1920,30,true,false, { 1602,1601,1602,1601,1602 } },
    { false,16.0,9.0,"1080I 60",VID_FMT_1080I_6000,1080,1920,30,false,false, { 1600,1600,1600,1600,1600 } },
    { false,16.0,9.0,"1080P 25",VID_FMT_1080P_2500,1080,1920,25,false,true, { 1920,1920,1920,1920,1920 } },
    { false,16.0,9.0,"1080P 29.97",VID_FMT_1080P_2997,1080,1920,30,true,true, { 1602,1601,1602,1601,1602 } },
    { false,16.0,9.0,"1080P 30",VID_FMT_1080P_3000,1080,1920,30,false,true, { 1600,1600,1600,1600,1600 } },
    { false,4.0,3.0,"HSDL 14",VID_FMT_HSDL_1498,1556,2048,15,true,false, { 1602,1601,1602,1601,1602 } },
    { false,4.0,3.0,"HSDL 15",VID_FMT_HSDL_1500,1556,2048,15,false,false, { 1602,1601,1602,1601,1602 } },
    { false,16.0,9.0,"720P 50",VID_FMT_720P_5000,720,1280,50,false,true, { 960,960,960,960,960 } },
    { false,16.0,9.0,"720P 23.98",VID_FMT_720P_2398,720,1280,24,true,true, { 2002,2002,2002,2002,2002 } },
    { false,16.0,9.0,"720P 24",VID_FMT_720P_2400,720,1280,24,false,true, { 2000,2000,2000,2000,2000 } },
    { false,16.0,9.0,"2Kx1080 PSF 23.97",VID_FMT_2048_1080PSF_2397,1080,2048,24,true,false, { 2002,2002,2002,2002,2002 } },
    { false,16.0,9.0,"2Kx1080 PSF 24",VID_FMT_2048_1080PSF_2400,1080,2048,24,false,false, { 2000,2000,2000,2000,2000 } },
    { false,16.0,9.0,"2Kx1080 P 23.97",VID_FMT_2048_1080P_2397,1080,2048,24,true,true, { 2002,2002,2002,2002,2002 } },
    { false,16.0,9.0,"2Kx1080 P 24",VID_FMT_2048_1080P_2400,1080,2048,24,false,true, { 2000,2000,2000,2000,2000 } },
    { false,16.0,9.0,"1080PSF 25",VID_FMT_1080PSF_2500,1080,1920,25,false,false, { 1920,1920,1920,1920,1920 } },
    { false,16.0,9.0,"1080PSF 29.97",VID_FMT_1080PSF_2997,1080,1920,30,true,false, { 1602,1601,1602,1601,1602 } },
    { false,16.0,9.0,"1080PSF 30",VID_FMT_1080PSF_3000,1080,1920,30,false,false, { 1600,1600,1600,1600,1600 } },
    { true,16.0,9.0,"1080P 50",VID_FMT_1080P_5000,1080,1920,50,false,true, { 960,960,960,960,960 } },
    { true,16.0,9.0,"1080P 59.94",VID_FMT_1080P_5994,1080,1920,60,true,true, { 801,800,801,801,801 } },
    { true,16.0,9.0,"1080P 60",VID_FMT_1080P_6000,1080,1920,60,false,true, { 800,800,800,800,800 } },
    { false,16.0,9.0,"720P 25",VID_FMT_720P_2500,720,1280,25,false,true, { 1920,1920,1920,1920,1920 } },
    { false,16.0,9.0,"720P 29.97",VID_FMT_720P_2997,720,1280,30,true,true, { 1602,1601,1602,1601,1602 } },
    { false,16.0,9.0,"720P 30",VID_FMT_720P_3000,720,1280,30,false,true, { 1600,1600,1600,1600,1600 } },
    { false,16.0,9.0,"DVB-ASI",VID_FMT_DVB_ASI,720,1280,50,false,true, { 960,960,960,960,960 } },
    { false,16.0,9.0,"2Kx1080 PSF 25",VID_FMT_2048_1080PSF_2500,1080,2048,25,false,false, { 1920,1920,1920,1920,1920 } },
    { false,16.0,9.0,"2Kx1080 PSF 29.97",VID_FMT_2048_1080PSF_2997,1080,2048,30,true,false, { 1602,1601,1602,1601,1602 } },
    { false,16.0,9.0,"2Kx1080 PSF 30",VID_FMT_2048_1080PSF_3000,1080,2048,30,false,false, { 1600,1600,1600,1600,1600 } },
    { false,16.0,9.0,"2Kx1080 P 25",VID_FMT_2048_1080P_2500,1080,2048,25,false,true, { 1920,1920,1920,1920,1920 } },
    { false,16.0,9.0,"2Kx1080 P 29.97",VID_FMT_2048_1080P_2997,1080,2048,30,true,true, { 1602,1601,1602,1601,1602 } },
    { false,16.0,9.0,"2Kx1080 P 30",VID_FMT_2048_1080P_3000,1080,2048,30,false,true, { 1600,1600,1600,1600,1600 } },
    { true,16.0,9.0,"2Kx1080 P 50",VID_FMT_2048_1080P_5000,1080,2048,50,false,true, { 960,960,960,960,960 } },
    { true,16.0,9.0,"2Kx1080 P 59.94",VID_FMT_2048_1080P_5994,1080,2048,59,true,true, { 801,800,801,801,801 } },
    { true,16.0,9.0,"2Kx1080 P 60",VID_FMT_2048_1080P_6000,1080,2048,60,false,true, { 800,800,800,800,800 } },
    { false,1.0,1.0,"VID_FMT_INVALID",VID_FMT_INVALID,0,0,0,false,true, { 0,0,0,0,0 } }
};

}
} //videocards


namespace Convert
{

//**************************************
//
template<> Expected< videocards::IOType >           String2T        ( const std::string & s )
{ 
    return String2Enum( videocards::IOTypeMapping, s ); 
}

//**************************************
//
template<> std::string                              T2String        ( const videocards::IOType & t )
{
    return Enum2String( videocards::IOTypeMapping, t );
}

} //SerializationHelper

} //bv