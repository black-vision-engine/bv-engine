#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/OutputInstance.h"


namespace bv { namespace nrl {

class RenderResult;
class NRenderContext;

class OutputStream : public OutputInstance
{
public:

    virtual void    ProcessFrameData( NRenderContext * ctx, const RenderResult * result ) override;

};

} //nrl
} //bv
