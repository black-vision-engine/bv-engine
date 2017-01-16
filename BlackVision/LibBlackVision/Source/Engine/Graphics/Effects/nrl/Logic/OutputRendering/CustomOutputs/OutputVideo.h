#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/OutputInstance.h"


namespace bv { namespace nrl {

class RenderResult;
class NRenderContext;

class OutputVideo : public OutputInstance
{
public:

    virtual void    ProcessFrameData( NRenderContext * ctx, RenderResult * result ) override;

};

} //nrl
} //bv
