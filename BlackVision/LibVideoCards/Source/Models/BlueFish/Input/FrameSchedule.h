#pragma once


#include "FifoCapture.h"


#include <queue>


namespace bv {
namespace videocards {
namespace bluefish
{


// ***********************
//
struct ScheduledID
{
    UInt16      ID;
    bool        Odd;

public:

    // ***********************
    //
    ScheduledID( UInt16 id )
        : ID( id )
        , Odd( false )
    {}

    // ***********************
    //
    void        operator=( ScheduledID & other )
    {
        ID = other.ID;
        Odd = other.Odd;
    }

    // ***********************
    //
    operator unsigned long()
    {
        return ID;
    }
};


// ***********************
//
struct FrameSchedule
{
    const int           NumBuffers;

    unsigned long       CurrentFieldCount;
    unsigned long       LastFieldCount;

    ScheduledID         ScheduleID;
    ScheduledID         CapturingID;
    ScheduledID         DoneField1ID;
    ScheduledID         DoneField2ID;
    ScheduledID         DoneID;


public:

    FrameSchedule       ( int numBuffers );

    void                SyncToNextFrameInterrupt        ( CFifoCapture * capture );
    void                SyncToOddFrame                  ( CFifoCapture * capture );
    void                ScheduleNextFrame               ( CFifoCapture * capture );

    bool                IsAudioFrame                    ();

    void                NextFrame                       ();
};


}
}
}


