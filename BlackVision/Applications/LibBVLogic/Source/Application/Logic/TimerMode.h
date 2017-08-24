#pragma once

#include "CoreDEF.h"



namespace bv
{


// ***********************
//
enum RenderingMode
{
    RM_RenderRealTime,
    RM_RenderOffscreen,

    RM_Total
};


// ***********************
//
class TimerMode
{
private:

    unsigned int        m_frameNumber;
    unsigned long       m_startTime;        ///< Milliseconds
    TimeType            m_currentTime;      ///< Milliseconds
    TimeType            m_realTime;

    RenderingMode       m_renderMode;
    UInt64              m_framesToRender;   ///< Only RenderToFile mode
    TimeType            m_nextFrameOffset;  ///< Only RenderToFile mode

    unsigned long		m_fps;

public:

    explicit        TimerMode();

    void            SetStartTime                ( unsigned long time );
    void            SetOffscreenRenderMode      ( float requestedFPS, UInt64 numFrames );

    TimeType        StartFrame                  ( unsigned long millis );
    TimeType	    GetFramesDelta				() const;

    unsigned long   GetStartTime                () const    { return m_startTime; }
    RenderingMode   GetRenderingMode            () const    { return m_renderMode; }
    TimeType        GetFrameTime                () const    { return m_currentTime; }
    unsigned int    GetFrameNumber              () const    { return m_frameNumber; }
    TimeType        GetRealTime                 () const    { return m_realTime; }

};


}	// bv