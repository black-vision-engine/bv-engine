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

    virtual RenderableEntity *  CreateFullscreenQuad            ( Renderer * renderer ) const override;

private:

    RenderableEffectPtr         CreateRenderableEffect          ( Renderer * renderer ) const;

};

} //bv
