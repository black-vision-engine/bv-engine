#pragma once

#include "Engine/Graphics/Effects/NodeEffect/PreFullScreenEffectLogic.h"


namespace bv {

class DefaultPreFullscreenEffectLogic : public PreFullscreenEffectLogic
{
public:

    virtual void                        Render                              ( SceneNode * node, RenderLogicContext * ctx, const std::vector< RenderTarget * > & outputs ) override;

    virtual std::vector< IValuePtr >    GetValues                           () const override;

    virtual unsigned int                GetPreferredNumOutputs              () const override;

    virtual bool                        IsFSERequired                       () const override;

};

} //bv