#include "stdafx.h"

#include "OutputStaticData.h"


namespace bv { 

// **************************
//
OutputStaticData::OutputStaticData( unsigned int width_, unsigned int height_, RenderChannelType rct_ )
    : width( width_ )
    , height( height_ )
    , selectedRenderedChannel( rct_ )
{    
}

// **************************
//
OutputStaticData::OutputStaticData()
    : width( 0 )
    , height( 0 )
    , selectedRenderedChannel( RenderChannelType::RCT_TOTAL )
{
}


} //bv
