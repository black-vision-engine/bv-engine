#pragma once


namespace bv {

class SceneNode;
class Renderer;
class RenderTarget;
class RenderTargetStackAllocator;

class PostFullScreenEffectLogic
{
private:

public:

    virtual         ~PostFullScreenEffectLogic  ();

    virtual void    Render                      ( SceneNode * node, Renderer * renderer, RenderTargetStackAllocator * allocator, RenderTarget * inputRenderTarget ) = 0;

};

} //bv
