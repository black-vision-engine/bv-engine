#pragma once

#include <string>
#include <vector>
#include <future>

namespace bv
{
class OffscreenRenderLogic;
class Renderer;

class ScreenShotLogic
{
private:
    std::string         m_filePath;

    unsigned int        m_remainingFrames;
    unsigned int        m_allFrames;

    std::vector<std::future<bool> >     m_asyncWrites;
    unsigned int                        m_curReadbackFrame;
public:
    ScreenShotLogic( unsigned int numReadBuffers );
    ~ScreenShotLogic();

    void        MakeScreenShot  ( const std::string& filePath, unsigned int numFrames = 1 );

    void        FrameRendered   ( Renderer* renderer, OffscreenRenderLogic* offscreenRenderLogic );
    bool        ReadbackNeeded  ();
};

} //bv