#pragma once

#include <vector>
#include <set>

#include "Engine/Interfaces/IValue.h"


namespace bv {

class SceneNode;
class RenderTarget;
class RenderLogicContext;
class RenderablePass;

class PostFullscreenEffectLogic
{
private:

public:

    virtual                             ~PostFullscreenEffectLogic  ();

    virtual void                        Render                      ( SceneNode * node, RenderLogicContext * ctx )  = 0;

    virtual std::vector< IValuePtr >    GetValues                   () const                                        = 0;

    virtual void                        GetRenderPasses             ( std::set< const RenderablePass * > * ) const {};
};

} //bv
