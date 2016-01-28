#pragma once

#include "Engine/Graphics/Effects/NodeEffect/Logic/PreFullScreenEffectLogic.h"


namespace bv {

class RenderOffscreenPreFullscreenEffectLogic : public PreFullscreenEffectLogic
{
protected:

    virtual void                        RenderImpl              ( SceneNode * node, RenderLogicContext * ctx, std::vector< RenderTarget * > & outputs ) override;

public:

    virtual std::vector< IValuePtr >    GetValues               () const override;

    virtual unsigned int                GetPreferredNumOutputs  () const override;

    virtual bool                        IsFSERequired           () const override;

};

} //bv
