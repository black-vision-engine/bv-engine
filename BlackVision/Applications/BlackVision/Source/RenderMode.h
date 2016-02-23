#pragma once

#include "CoreDEF.h"
#include <string>

namespace bv
{

class RenderLogic;
class Renderer;

enum RenderingMode
{
    RenderRealTime,
    RenderOffscreen
};


class RenderMode
{
private:

    RenderLogic*        m_renderLogic;
    Renderer*           m_renderer;

    unsigned int        m_frameNumber;
    unsigned long       m_startTime;        ///< Milliseconds
    TimeType            m_currentTime;      ///< Milliseconds
    TimeType            m_realTime;

    RenderingMode       m_renderMode;
    unsigned int        m_framesToRender;   ///< Only RenderToFile mode
    TimeType            m_nextFrameOffset;  ///< Only RenderToFile mode

	unsigned long		m_fps;

public:

    RenderMode	();
    ~RenderMode	();

    void        Init                        ( RenderLogic* logic, Renderer* renderer )      { m_renderLogic = logic; m_renderer = renderer; }
    void        SetStartTime                ( unsigned long time );
    
    void        SetRenderToFileMode         ( const std::string& filePath, float requestedFPS, unsigned int numFrames );
    void        MakeScreenShot              ( const std::string& filePath, bool onRenderedEvent = false );
    TimeType    StartFrame                  ( unsigned long millis );
	TimeType	GetFramesDelta				() const;

    unsigned long   GetStartTime        () const    { return m_startTime; }
    RenderingMode   GetRenderingMode    () const    { return m_renderMode; }
    TimeType        GetFrameTime        () const    { return m_currentTime; }
    unsigned int    GetFrameNumber      () const    { return m_frameNumber; }
    TimeType        GetRealTime         () const    { return m_realTime; }

};

} //bv