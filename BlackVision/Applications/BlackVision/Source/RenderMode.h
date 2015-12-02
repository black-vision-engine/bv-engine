#pragma once

#include "CoreDEF.h"

namespace bv
{

class RenderLogic;

enum RenderingMode
{
    RenderRealTime,
    RenderOffscreen
};


class RenderMode
{
private:

    RenderLogic*        m_renderLogic;

    unsigned int        m_frameNumber;
    unsigned long       m_startTime;        ///< Milliseconds
    TimeType            m_currentTime;      ///< Milliseconds
    TimeType            m_realTime;

    RenderingMode       m_renderMode;
    unsigned int        m_framesToRender;   ///< Only RenderToFile mode
    float               m_nextFrameOffset;  ///< Only RenderToFile mode

public:

    RenderMode();
    ~RenderMode();

    void        SetRenderLogic              ( RenderLogic* logic )      { m_renderLogic = logic; }
    void        SetStartTime                ( unsigned long time );
    
    void        SetRenderToFileMode         ( const std::string& filePath, float requestedFPS, unsigned int numFrames );
    void        MakeScreenShot              ( const std::string& filePath );
    TimeType    StartFrame                  ( unsigned long millis );

    unsigned long   GetStartTime()          { return m_startTime; }
    RenderingMode   GetRenderingMode()      { return m_renderMode; }
    TimeType        GetFrameTime();
    unsigned int    GetFrameNumber();
    TimeType        GetRealTime()           { return m_realTime; }

};

} //bv
