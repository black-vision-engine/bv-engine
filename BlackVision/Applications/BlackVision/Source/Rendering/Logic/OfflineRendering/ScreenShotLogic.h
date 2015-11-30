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
    bool                m_makeScreenShot;

    unsigned int        m_curReadbackFrame;
public:
    ScreenShotLogic();
    ~ScreenShotLogic();

    void        MakeScreenShot  ( const std::string& filePath );

    void        FrameRendered   ( Renderer* renderer, OffscreenRenderLogic* offscreenRenderLogic );
    bool        ReadbackNeeded  ();
};

} //bv
