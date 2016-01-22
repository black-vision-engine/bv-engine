#pragma once

#include "Engine/Graphics/Effects/NodeEffectTr/PreFullScreenEffectLogic.h"

#include "Engine/Types/Values/TypedValues.h"


namespace bv {

class NodeMaskPreFullscreenEffectLogic : public PreFullscreenEffectLogic
{
private:

    ValueFloatPtr   m_alphaValue;
    ValueIntPtr     m_foregroundIndex;
    ValueIntPtr     m_maskIndex;

    float           m_minAlphaThreshold;

public:

                                        NodeMaskPreFullscreenEffectLogic    ( float minAlphaThreshold );

    virtual void                        Render                              ( SceneNode * node, RenderLogicContext * ctx, const std::vector< RenderTarget * > & outputs ) override;

    virtual std::vector< IValuePtr >    GetValues                           () const override;

    virtual unsigned int                GetPreferredNumOutputs              () const override;

};

} //bv
