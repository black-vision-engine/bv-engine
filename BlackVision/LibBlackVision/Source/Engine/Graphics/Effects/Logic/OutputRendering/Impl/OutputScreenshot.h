#pragma once

#include "Engine/Graphics/Effects/Logic/Components/RenderedChannelsData.h"
#include "Engine/Graphics/Effects/Logic/Components/RenderChannel.h"


#include <string>
#include <vector>


namespace bv {



class RenderContext;


struct ScreenshotInfo
{
    RenderChannelType   InputChannel;

    unsigned int        RemainingFrames;
    unsigned int        AllFrames;

    bool                OnRenderedEvent;
    bool                SaveToFileAsync;

    bool                IsSequence;

    std::string         FilePath;
};


class OutputScreenshot
{
private:

    std::vector< ScreenshotInfo >   m_screenshotDescs;

public:

    explicit        OutputScreenshot    ();

    void            ProcessFrameData    ( RenderContext * ctx, RenderedChannelsData * rcd );
    void            RequestScreenshot   ( const std::string & filePath, RenderChannelType channelId, unsigned int numFrames = 1, bool onRenderedEvent = false, bool saveImgAsync = true );

private:

    bool            ReadbackNeeded          ( ScreenshotInfo & screenshotInfo );
    void            RemoveFinalizedRequests ();

    std::string     GenerateFileName        ( ScreenshotInfo & screenshotInfo );

    void            ProcessFrameData        ( ScreenshotInfo & screenshotInfo, RenderContext * ctx, const RenderChannel * channel );
};




} //bv
