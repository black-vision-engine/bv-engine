#pragma once

#include "Engine/Graphics/Shaders/RenderableEffect.h"


namespace bv {

class DefaultEffect : public RenderableEffect
{
private:

    typedef std::vector< RenderablePass * > TRenderablePassVector;

private:

    TRenderablePassVector   m_passes;

public:

            DefaultEffect   ();
            ~DefaultEffect  ();

};

}