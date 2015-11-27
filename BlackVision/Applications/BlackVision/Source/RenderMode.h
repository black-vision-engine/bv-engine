#pragma once

#include "CoreDEF.h"

namespace bv
{

enum RenderingMode
{
    RenderRealTime,
    RenderOffscreen
};


class RenderMode
{
private:
    unsigned int        m_frameNumber;
    unsigned long       m_startTime;        ///< Milliseconds
    unsigned long       m_currentTime;      ///< Milliseconds

    RenderingMode       m_renderMode;
    unsigned int        m_framesToRender;   ///< Only RenderToFile mode
    float               m_nextFrameOffset;  ///< Only RenderToFile mode
public:
    RenderMode();
    ~RenderMode();

    void        SetStartTime                ( unsigned long time );
    void        SetRenderToFileMode         ( float requestedFPS, unsigned int numFrames );
    TimeType    StartFrame                  ( unsigned long millis );

    unsigned long   GetStartTime()          { return m_startTime; }
    RenderingMode   GetRenderingMode()      { return m_renderMode; }
    TimeType        GetFrameTime();
    unsigned int    GetFrameNumber();

};

} //bv
