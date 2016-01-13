#pragma once


namespace bv {

class SceneNode;
class RenderTarget;
class RenderLogicContext;

class PostFullScreenEffectLogic
{
private:

public:

    virtual         ~PostFullScreenEffectLogic  ();

    virtual void    Render                      ( SceneNode * node, RenderLogicContext * ctx, RenderTarget * inputRenderTarget ) = 0;

};

} //bv
