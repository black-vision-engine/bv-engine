#pragma once

#include "Engine/Graphics/Effects/Logic/Components/RenderChannel.h"


namespace bv { 

struct OutputStaticData
{
    unsigned int        width;
    unsigned int        height;

    RenderChannelType   selectedRenderedChannel;

    OutputStaticData( unsigned int width_, unsigned int height_, RenderChannelType rct_ );
    OutputStaticData();

};


} // bv