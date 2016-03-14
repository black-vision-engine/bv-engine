#pragma once

#include "Engine/Graphics/Effects/NodeEffect/Logic/PreFullScreenEffectLogic.h"

#include "Engine/Types/Values/TypedValues.h"


namespace bv { 

class LightScatteringPreFullscreenEffectLogic : public PreFullscreenEffectLogic
{
private:

public:
                LightScatteringPreFullscreenEffectLogic        ();
    virtual     ~LightScatteringPreFullscreenEffectLogic       ();

protected:

    virtual void                        RenderImpl                          ( SceneNode * node, RenderLogicContext * ctx, std::vector< RenderTarget * > & outputs ) override;

public:

    virtual std::vector< IValuePtr >    GetValues                           () const override;

    virtual unsigned int                GetPreferredNumOutputs              () const override;

    virtual bool                        IsFSERequired                       () const override;

};

} // bv