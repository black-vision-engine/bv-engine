#pragma once

#include <string>

#include "CoreDEF.h"
#include "TimerMode.h"



namespace bv
{

class RenderLogic;
class OutputScreenshot;

class Renderer;



class RenderMode
{
private:

    RenderLogic *       m_renderLogic;
    Renderer *          m_renderer;

    TimerMode           m_timer;

    RenderingMode       m_renderMode;
    bool                m_editMode;

public:

    RenderMode	();
    ~RenderMode	();

    void            Init                        ( RenderLogic * logic, Renderer * renderer )      { m_renderLogic = logic; m_renderer = renderer; }

    // Timer
    void            SetStartTime                ( unsigned long time );
    TimeType        StartFrame                  ( unsigned long millis );

    TimeType	    GetFramesDelta				() const    { return m_timer.GetFramesDelta(); }
    unsigned long   GetStartTime                () const    { return m_timer.GetStartTime(); }
    RenderingMode   GetRenderingMode            () const    { return m_renderMode; }
    TimeType        GetFrameTime                () const    { return m_timer.GetFrameTime(); }
    unsigned int    GetFrameNumber              () const    { return m_timer.GetFrameNumber(); }
    TimeType        GetRealTime                 () const    { return m_timer.GetRealTime(); }

    // Rendering modes
    void            SetRenderToFileMode         ( const std::string & filePath, float requestedFPS, UInt64 numFrames );
    void            SetOffscreenRenderMode      ( float requestedFPS, UInt64 numFrames );
    void            MakeScreenShot              ( const std::string & filePath, bool onRenderedEvent = false, bool asyncWrite = true );
	
    
    // Editor mode
    void            SwitchToEditMode            ( bool mode );
    bool            IsEditMode                  () const;

    // AVFileOutput
    void            StartToAVFileRendering      ( const std::string & outputFilePath );
    void            StopToAVFileRendering       ();
};

} //bv
