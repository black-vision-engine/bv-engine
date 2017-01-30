#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/OutputInstance.h"

#include "Engine/Graphics/Effects/nrl/Logic/State/NRenderedData.h"

#include "AVFrame.h"


namespace bv { namespace nrl {

enum class OutputVideoChannel : unsigned int
{
    OVC_HD_1 = 0,
    OVC_HD_2,
    OVC_HD_3,
    OVC_HD_4,
    OVC_HD_5,
    OVC_HD_6,
    OVC_HD_7,
    OVC_HD_8,
    OVC_HD_9,
    OVC_HD_10,
    OVC_HD_11,
    OVC_HD_12,
    OVC_HD_14,
    OVC_HD_15,
    OVC_HD_16,

    OVC_SD_1,
    OVC_SD_2,
    OVC_SD_3,
    OVC_SD_4,
    OVC_SD_5,
    OVC_SD_6,
    OVC_SD_7,
    OVC_SD_8,
    OVC_SD_9,
    OVC_SD_10,
    OVC_SD_11,
    OVC_SD_12,
    OVC_SD_14,
    OVC_SD_15,
    OVC_SD_16,

    OVC_TOTAL
};

class RenderResult;
class NRenderContext;
class OutputVideo;

// FIXME: nrl - this class is in a way responsible for handling intercard synchronization issues
class OutputVideoContainer : public OutputInstance
{
private:

    // FIXME: nrl - add intermediate state here (for caching intermediate output results and minimize readback times)

    std::vector< OutputVideo * >    m_outputs;

public:

                    OutputVideoContainer    ( unsigned int widthHD, unsigned int heightHD );
                    ~OutputVideoContainer   ();

    virtual void    ProcessFrameData        ( NRenderContext * ctx, RenderResult * input ) override;

    OutputVideo *   GetVideoOutput          ( OutputVideoChannel ovc );

};

} //nrl
} //bv
