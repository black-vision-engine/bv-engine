#pragma once

#include <vector>
#include <hash_map>

#include "Engine/Graphics/Effects/nrl/Logic/Output.h"

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

    OVC_SD_1,
    OVC_SD_2,
    OVC_SD_3,
    OVC_SD_4,
    OVC_SD_5,
    OVC_SD_6,
    OVC_SD_7,
    OVC_SD_8,

    OVC_TOTAL
};

class RenderResult;
class NRenderContext;
class OutputVideo;

// FIXME: nrl - this class is in a way responsible for handling intercard synchronization issues
class OutputVideoContainer : public Output
{
private:

    // FIXME: nrl - add intermediate state here (for caching intermediate output results and minimize readback times)
    // ReadbackCache    m_readbackCache;

    std::vector< OutputVideo * >                        m_outputs;
    std::hash_map< OutputVideoChannel, OutputVideo * >  m_outputsMapping;

public:

                    OutputVideoContainer    ();
                    ~OutputVideoContainer   ();

    virtual void    ProcessFrameData        ( NRenderContext * ctx, RenderResult * input ) override;

    OutputVideo *   GetVideoOutput          ( OutputVideoChannel ovc );

    // API for outputs manipulation
    void            RegisterOutput          ( OutputVideo * output, OutputVideoChannel ovc );
    bool            RemoveOutput            ( OutputVideoChannel ovc );

};

} //nrl
} //bv
