#pragma once

#include <vector>

#include "Engine/Graphics/Effects/nrl/Logic/Components/RenderedChannelsData.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Output.h"
#include "Engine/Graphics/Effects/nrl/Logic/Components/Initialization/OutputLogicDesc.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/OutputScreenshot.h"

namespace bv { 
    
namespace nrl {

class NRenderContext;

class OutputLogic
{
private:

    std::vector< Output * >     m_outputs;
    OutputScreenshot            m_screenshotLogic; // FIXME: Implement it using the outputs handlers design model.

private:

                            OutputLogic             ();

public:

    virtual                 ~OutputLogic            ();

    // API directly related to frame rendering
    void                    ProcessFrameData        ( NRenderContext * ctx, RenderedChannelsData * rcd );

    // API relarted to global output state manipulation
    Output *                GetOutput               ( CustomOutputType outputType );

    void                    RequestScreenshot       ( const std::string & filePath, RenderChannelType channelId, unsigned int numFrames = 1, bool onRenderedEvent = false, bool saveImgAsync = true );

private:

    void                    AppendOutput            ( Output * output );

public:

    static OutputLogic *    Create                  ( OutputLogicDesc & desc );

};

} //nrl
} //bv
