#include "stdafx.h"

#include "OutputStaticData.h"


namespace bv { namespace nrl {

// **************************
//
OutputStaticData::OutputStaticData( unsigned int width_, unsigned int height_, RenderChannelType rct_ )
    : width( width_ )
    , height( height_ )
    , selectedRenderedChannel( rct_ )
{    
}

} //nrl
} //bv
