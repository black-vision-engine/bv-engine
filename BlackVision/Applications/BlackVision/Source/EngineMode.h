#pragma once

#include "CoreDEF.h"

namespace bv
{

enum RenderingMode
{
    RenderRealTime,
    RenderToFile
};


class EngineMode
{
private:
    unsigned int        m_frameNumber;
    unsigned long       m_startTime;        ///< Milliseconds
    unsigned long       m_currentTime;      ///< Milliseconds

    RenderingMode       m_renderMode;
    unsigned int        m_framesToRender;   ///< Only RenderToFile mode
    float               m_nextFrameOffset;  ///< Only RenderToFile mode
public:
    EngineMode();
    ~EngineMode();

    void        SetStartTime                ( unsigned long time );
    void        SetRenderToFileMode         ( float requestedFPS, unsigned int numFrames );
    TimeType    StartFrame                  ( unsigned long millis );

    TimeType        GetFrameTime();
    unsigned int    GetFrameNumber();
};

} //bv
