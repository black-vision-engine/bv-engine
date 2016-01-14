#pragma once

#include <vector>


namespace bv {

class SceneNode;
class RenderTarget;
class RenderLogicContext;

class PreFullscreenEffectLogic
{
private:

public:

    virtual         ~PreFullscreenEffectLogic   ();

    virtual void    Render                      ( SceneNode * node, RenderLogicContext * ctx, const std::vector< RenderTarget * > & outputs ) = 0;

};

} //bv
