#pragma once

#include "Rendering/Logic/FullScreen/FullscreenEffectData.h"


namespace bv {

class Renderer;
class RenderTargetStackAllocator;

class FullscreenEffectContext
{
private:

    FullscreenEffectData            m_data;

    Renderer *                      m_renderer;

    RenderTargetStackAllocator *    m_rtAllocator;

public:
                                    FullscreenEffectContext     ( Renderer * renderer, RenderTargetStackAllocator * allocator );
                                    ~FullscreenEffectContext    ();

    FullscreenEffectData &          AccessData                  ();
    Renderer *                      GetRenderer                 ();
    RenderTargetStackAllocator *    GetRenderTargetAllocator    ();

};

} //bv
