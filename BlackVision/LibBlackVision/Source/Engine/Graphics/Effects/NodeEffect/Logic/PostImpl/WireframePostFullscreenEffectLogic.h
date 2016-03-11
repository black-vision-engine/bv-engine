#pragma once

#include "Engine/Graphics/Effects/NodeEffect/Logic/PostFullScreenEffectLogic.h"

#include "Engine/Types/Values/TypedValues.h"


namespace bv {

class WireframeEffect;
class Renderer;


class WireframePostFullscreenEffectLogic : public PostFullscreenEffectLogic
{
private:

    WireframeEffect *   m_effect;

public:

                                        WireframePostFullscreenEffectLogic  ();
                                        ~WireframePostFullscreenEffectLogic ();

    virtual void                        Render                              ( SceneNode * node, RenderLogicContext * ctx );

    virtual std::vector< IValuePtr >    GetValues                           () const override;

    virtual void                        GetRenderPasses                     ( std::set< const RenderablePass * > * ) const override;

private:

    void                                EnableWireframeEffect               ( Renderer * renderer, SceneNode * node );
    void                                DrawWirefreameNodeOnly              ( Renderer * renderer, SceneNode * node );

};

} //bv
