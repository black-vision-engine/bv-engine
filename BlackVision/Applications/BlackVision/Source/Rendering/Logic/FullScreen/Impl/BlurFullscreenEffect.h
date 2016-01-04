#pragma once

#include "Rendering/Logic/FullScreen/FullscreenEffect.h"

#include "Engine/Graphics/Resources/Textures/Texture2D.h"


namespace bv {

class BlurFullscreenEffect : public FullscreenEffect
{
private:

    float                       m_blurSize;

    ValueFloatPtr               m_blurSizeVal;

    ValueVec2Ptr                m_textureSize;
    Texture2DPtr                m_texture;

public:

    void                        SetBlurSize ( float s );
    float                       GetBlurSize () const;


                                BlurFullscreenEffect    ( Texture2DPtr tex );
                                ~BlurFullscreenEffect   ();

protected:

    virtual RenderableEntity *  CreateFullscreenQuad        () const override;

    PixelShader *               CreatePS                    () const;

};

DEFINE_PTR_TYPE(BlurFullscreenEffect)
DEFINE_CONST_PTR_TYPE(BlurFullscreenEffect)

} //bv
