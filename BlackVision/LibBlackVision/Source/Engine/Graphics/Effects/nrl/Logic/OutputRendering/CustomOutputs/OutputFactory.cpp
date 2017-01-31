#include "stdafx.h"

#include "OutputFactory.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/CustomOutputs/OutputPreview.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/CustomOutputs/OutputVideo.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/CustomOutputs/OutputStreamSharedMem.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/CustomOutputs/OutputScreenshot.h"


namespace bv { namespace nrl {

namespace {

const unsigned int widthHD  = 1920;
const unsigned int heightHD = 1080;

const unsigned int widthSD  = widthHD / 2;
const unsigned int heightSD = heightHD / 2;

} // anonymous

// *********************************
//
Output *    CreateOutput( CustomOutputType cot )
{
    switch( cot )
    {
        case COT_PREVIEW:
            return 
        case COT_VIDEO:
        case COT_STREAM_SHM:
        case COT_SCREENSHOT:
        default:
            assert( false );
        return nullptr;
    };
}

} // nrl
} // bv
