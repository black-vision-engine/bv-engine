#pragma once

#include <vector>

#include "Engine/Interfaces/IValue.h"


namespace bv {

class SceneNode;
class RenderTarget;
class RenderLogicContext;

class PreFullscreenEffectLogic
{
private:


public:

    virtual                             ~PreFullscreenEffectLogic   ();

    virtual void                        Render                      ( SceneNode * node, RenderLogicContext * ctx, const std::vector< RenderTarget * > & outputs ) = 0;

    virtual std::vector< IValuePtr >    GetValues                   () const = 0;

protected:

    void                                RenderToRenderTarget        ( RenderLogicContext * ctx, RenderTarget * rt, SceneNode * node );

};

} //bv
