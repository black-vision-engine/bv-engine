#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/OutputInstance.h"


namespace bv { namespace nrl {

class OutputScreenshot : public OutputInstance
{
public:

                    OutputScreenshot( unsigned int width, unsigned int height );
 
    virtual void    ProcessFrameData( NRenderContext * ctx, RenderResult * input ) override;

};

} //nrl
} //bv
