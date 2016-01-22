#pragma once

#include "Engine/Graphics/Effects/NodeEffectTr/PreFullScreenEffectLogic.h"
#include "Engine/Types/Values/TypedValues.h"


namespace bv {

class AlphaMaskPreFullscreenEffectLogic : public PreFullscreenEffectLogic
{
private:

    ValueFloatPtr   m_alphaValue;
    
    float           m_minAlphaThreshold;
    float           m_maxAlphaThreshold;

public:

                                        AlphaMaskPreFullscreenEffectLogic   ( float minAlphaThreshold, float maxAlphaThreshold );

    virtual void                        Render                              ( SceneNode * node, RenderLogicContext * ctx, const std::vector< RenderTarget * > & outputs ) override;

    virtual std::vector< IValuePtr >    GetValues                           () const override;

    virtual unsigned int                GetPreferredNumOutputs              () const override;

};

} //bv
