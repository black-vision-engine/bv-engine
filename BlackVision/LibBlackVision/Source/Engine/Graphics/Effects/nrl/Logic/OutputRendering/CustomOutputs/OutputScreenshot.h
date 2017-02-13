#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/OutputInstance.h"


#include <string>
#include <vector>
#include <future>


namespace bv { namespace nrl {

class OutputScreenshot : public OutputInstance
{
private:

    std::string         m_filePath;

    unsigned int        m_remainingFrames;
    unsigned int        m_allFrames;

    bool                m_onRenderedEvent;
    bool                m_saveToFileAsync;

public:

                    OutputScreenshot( unsigned int width, unsigned int height );
 
    virtual void    ProcessFrameData( NRenderContext * ctx, RenderResult * input ) override;

    void            MakeScreenShot  ( const std::string& filePath, unsigned int numFrames = 1, bool onRenderedEvent = false, bool saveImgAsync = true );
    bool            ReadbackNeeded  ();
};

} //nrl
} //bv
