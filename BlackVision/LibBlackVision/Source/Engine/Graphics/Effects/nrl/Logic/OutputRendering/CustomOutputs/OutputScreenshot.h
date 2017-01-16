#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/OutputInstance.h"


namespace bv { namespace nrl {

class RenderResult;
class NRenderContext;

class OutputScreenshot : public OutputInstance
{
private:

public:

    void    HandleFrame( NRenderContext * ctx, const RenderResult * result );

};

} //nrl
} //bv
