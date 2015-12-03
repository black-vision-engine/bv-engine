#pragma once

#include <string>

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
public:
    ScreenShotLogic();
    ~ScreenShotLogic();

    void        MakeScreenShot  ( const std::string& filePath, unsigned int numFrames = 1 );

    void        FrameRendered   ( Renderer* renderer, OffscreenRenderLogic* offscreenRenderLogic );
    bool        ReadbackNeeded  ();
};

} //bv
