#include "stdafx.h"

#include "OutputFactory.h"


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
        case CustomOutputType::COT_VIDEO:
        case CustomOutputType::COT_STREAM:
            return nullptr;
            // return new OutputStreamSharedMem( widthSD, heightSD );
        default:
            assert( false );
    };

    return nullptr;
}

} // nrl
} // bv
