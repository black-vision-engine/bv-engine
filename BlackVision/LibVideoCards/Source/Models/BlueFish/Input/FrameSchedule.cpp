#include "FrameSchedule.h"

#include "Serialization/ConversionHelper.h"
#include "Models/BlueFish/Channel.h"

#include "BlueFish/inc/BlueHancUtils.h"
#include "Bluefish/inc/BlueVelvet4.h"

#include "UseLoggerVideoModule.h"


namespace bv {
namespace videocards {
namespace bluefish
{




// ***********************
//
FrameSchedule::FrameSchedule( int numBuffers )
    : CurrentFieldCount( 0 )
    , LastFieldCount( 0 )
    , ScheduleID( 0 )
    , CapturingID( 0 )
    , DoneID( 0 )
    , DoneHANC( 0 )
    , ScheduleHANC( 0 )
    , NumBuffers( numBuffers )
{}

// ***********************
//
void            FrameSchedule::SyncToNextFrameInterrupt     ( CFifoCapture * capture )
{
    capture->m_pSDK->wait_input_video_synch( capture->m_nUpdateFormat, CurrentFieldCount );

    if( LastFieldCount + 1 < CurrentFieldCount )
    {
        UInt32 dropped = CurrentFieldCount - ( LastFieldCount + 1 );
        LOG_MESSAGE( SeverityLevel::info ) << "BlueFish input channel " + Convert::T2String( ( ChannelName )capture->m_videoChannel ) << ". Dropped [" << Convert::T2String( dropped ) << "] frames.";
    }
}

// ***********************
//
void            FrameSchedule::SyncToOddFrame               ( CFifoCapture * capture )
{
    if( ( CurrentFieldCount & 0x1 ) == 0 )
        capture->m_pSDK->wait_input_video_synch( capture->m_nUpdateFormat, CurrentFieldCount );
}

// ***********************
//
void            FrameSchedule::ScheduleNextFrame            ( CFifoCapture * capture )
{
    bool odd = ScheduleID & 0x1;

    if( !odd )
    {
        capture->m_pSDK->render_buffer_capture( BlueBuffer_Image_HANC( ScheduleID ), 0 );

        DoneHANC = ScheduleHANC;
        ScheduleHANC = ScheduleID;
    }
    else
    {
        capture->m_pSDK->render_buffer_capture( BlueBuffer_Image( ScheduleID ), 0 );
    }
}

// ***********************
//
void            FrameSchedule::NextFrame                    ()
{
    LastFieldCount = CurrentFieldCount;

    DoneID = CapturingID;
    CapturingID = ScheduleID;
    ScheduleID = ( ++ScheduleID % NumBuffers );
}




}
}
}


