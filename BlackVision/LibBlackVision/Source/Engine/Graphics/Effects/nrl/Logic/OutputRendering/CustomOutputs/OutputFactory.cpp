#include "stdafx.h"

#include "OutputFactory.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/CustomOutputs/OutputPreview.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/CustomOutputs/OutputVideoContainer.h"
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
        case CustomOutputType::COT_PREVIEW:
        {
            auto res = new OutputPreview( widthHD, heightHD );
            
            assert( false );
            // res->Enable();

            return res;
        }
        case CustomOutputType::COT_VIDEO:
            return new OutputVideoContainer();
        case CustomOutputType::COT_STREAM_SHM:
            return new OutputStreamSharedMem( widthSD, heightSD );
        case CustomOutputType::COT_SCREENSHOT:
            return new OutputScreenshot( widthHD, heightHD );
        default:
            assert( false );
    };

    return nullptr;
}

} // nrl
} // bv
