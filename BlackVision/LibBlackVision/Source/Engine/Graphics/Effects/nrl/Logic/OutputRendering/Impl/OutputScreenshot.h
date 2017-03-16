#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/Components/RenderedChannelsData.h"
#include "Engine/Graphics/Effects/nrl/Logic/Components/RenderChannel.h"


#include <string>
#include <vector>


namespace bv {
namespace nrl {


class NRenderContext;


struct ScreenshotInfo
{
    RenderChannelType   InputChannel;

    unsigned int        RemainingFrames;
    unsigned int        AllFrames;

    bool                OnRenderedEvent;
    bool                SaveToFileAsync;

    std::string         FilePath;
};


class OutputScreenshot
{
private:

    std::vector< ScreenshotInfo >   m_screenshotDescs;

public:

    explicit        OutputScreenshot    ();

    void            ProcessFrameData    ( NRenderContext * ctx, RenderedChannelsData * rcd );
    void            RequestScreenshot   ( const std::string & filePath, RenderChannelType channelId, unsigned int numFrames = 1, bool onRenderedEvent = false, bool saveImgAsync = true );

private:

    bool            ReadbackNeeded          ( ScreenshotInfo & screenshotInfo );
    void            RemoveFinalizedRequests ();

    void            ProcessFrameData        ( ScreenshotInfo & screenshotInfo, NRenderContext * ctx, const RenderChannel * channel );
};



} //nrl
} //bv
