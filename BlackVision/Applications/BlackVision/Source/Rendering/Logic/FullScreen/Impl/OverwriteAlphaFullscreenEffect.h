#pragma once

#include "Rendering/Logic/FullScreen/FullscreenEffect.h"

#include "Engine/Graphics/Shaders/RenderableEffect.h"


namespace bv {

class OverwriteAlphaFullscreenEffect : public FullscreenEffect
{
private:

    float m_alpha;

public:

                                OverwriteAlphaFullscreenEffect  ( float alpha = 1.0f );
                                ~OverwriteAlphaFullscreenEffect ();

    void                        SetAlpha                        ( float alpha );
    float                       GetAlpha                        () const;

protected:

    virtual RenderableEntity *  CreateFullscreenQuad            () const override;

private:

    RenderableEffectPtr         CreateRenderableEffect          () const;

    PixelShader *               CreatePS                        () const;

};

DEFINE_PTR_TYPE(OverwriteAlphaFullscreenEffect)
DEFINE_CONST_PTR_TYPE(OverwriteAlphaFullscreenEffect)

} //bv
