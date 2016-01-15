#pragma once

#include "Rendering/Logic/NodeEffect/PreFullScreenEffectLogic.h"


namespace bv {

class DefaultPreFullscreenEffectLogic : public PreFullscreenEffectLogic
{
public:

    virtual void                        Render                              ( SceneNode * node, RenderLogicContext * ctx, const std::vector< RenderTarget * > & outputs ) override;

    virtual std::vector< IValuePtr >    GetValues                           () const override;

};

} //bv
