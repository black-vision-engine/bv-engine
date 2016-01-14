#pragma once


namespace bv {

class SceneNode;
class RenderTarget;
class RenderLogicContext;

class PostFullscreenEffectLogic
{
private:

public:

    virtual         ~PostFullscreenEffectLogic  ();

    virtual void    Render                      ( SceneNode * node, RenderLogicContext * ctx, RenderTarget * inputRenderTarget ) = 0;

};

} //bv
