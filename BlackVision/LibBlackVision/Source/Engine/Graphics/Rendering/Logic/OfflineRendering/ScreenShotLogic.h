#pragma once

#include <string>
#include <vector>
#include <future>

namespace bv
{
class RenderLogicContext;
class Renderer;
class RenderTarget;

class ScreenShotLogic
{
private:
    std::string         m_filePath;

    unsigned int        m_remainingFrames;
    unsigned int        m_allFrames;

    std::vector<std::future<void> >     m_asyncWrites;
    unsigned int                        m_curReadbackFrame;

    bool                                m_onRenderedEvent;

public:
    ScreenShotLogic( unsigned int numReadBuffers );
    ~ScreenShotLogic();

    void        MakeScreenShot  ( const std::string& filePath, unsigned int numFrames = 1, bool onRenderedEvent = false );

    void        FrameRendered   ( RenderTarget * videoRenderTarget, RenderLogicContext * ctx );
    bool        ReadbackNeeded  ();
};

} //bv
