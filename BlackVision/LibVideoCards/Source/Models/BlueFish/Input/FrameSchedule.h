#pragma once


#include "FifoCapture.h"



namespace bv {
namespace videocards {
namespace bluefish
{


// ***********************
//
struct FrameSchedule
{
    const int           NumBuffers;

    unsigned long       CurrentFieldCount;
    unsigned long       LastFieldCount;

    unsigned long       ScheduleID;
    unsigned long       CapturingID;
    unsigned long       DoneID;
    unsigned long       DoneHANC;
    unsigned long       ScheduleHANC;


public:

    FrameSchedule       ( int numBuffers );

    void                SyncToNextFrameInterrupt        ( CFifoCapture * capture );
    void                SyncToOddFrame                  ( CFifoCapture * capture );
    void                ScheduleNextFrame               ( CFifoCapture * capture );

    void                NextFrame                       ();
};


}
}
}


