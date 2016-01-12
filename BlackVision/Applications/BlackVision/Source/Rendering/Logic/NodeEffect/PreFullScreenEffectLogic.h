#pragma once

#include <vector>


namespace bv {

class SceneNode;
class Renderer;
class RenderTarget;
class RenderTargetStackAllocator;

class PreFullScreenEffectLogic
{
private:

public:

    virtual         ~PreFullScreenEffectLogic   ();

    virtual void    Render                      ( SceneNode * node, Renderer * renderer, RenderTargetStackAllocator * allocator, const std::vector< RenderTarget * > & outputs ) = 0;

};

} //bv
