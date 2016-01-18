#pragma once

#include "Rendering/Logic/FullScreen/FullscreenEffect.h"

#include "Engine/Types/Values/TypedValues.h"


namespace bv {

class OverwriteAlphaFullscreenEffect : public FullscreenEffect
{
private:

    float           m_alpha;

    ValueFloatPtr   m_alphaVal;

public:

                                OverwriteAlphaFullscreenEffect  ( float alpha = 1.0f );
                                ~OverwriteAlphaFullscreenEffect ();

    void                        SetAlpha                        ( float alpha );
    float                       GetAlpha                        () const;

protected:

    virtual RenderableEntity *  CreateFullscreenQuad            () const override;

    PixelShader *               CreatePS                        () const;

};

DEFINE_PTR_TYPE(OverwriteAlphaFullscreenEffect)
DEFINE_CONST_PTR_TYPE(OverwriteAlphaFullscreenEffect)

} //bv
