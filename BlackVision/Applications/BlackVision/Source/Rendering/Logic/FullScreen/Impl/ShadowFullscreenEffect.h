#pragma once

#include "Rendering/Logic/FullScreen/FullscreenEffect.h"

#include "Engine/Graphics/Resources/Textures/Texture2D.h"


namespace bv {

class ShadowFullscreenEffect : public FullscreenEffect
{
private:

    glm::vec4                   m_color;
    glm::vec2                   m_shift;
    float                       m_blurSize;

    ValueVec4Ptr                m_colorVal;
    ValueVec2Ptr                m_shiftVal;
    ValueFloatPtr               m_blurSizeVal;

    Texture2DPtr                m_texture;

public:
    void                        SetColor    ( const glm::vec4 & color );
    glm::vec4                   GetColor    () const;

    void                        SetShift    ( const glm::vec2 & xy );
    glm::vec2                   GetShift    () const;

    void                        SetBlurSize ( float s );
    float                       GetBlurSize () const;


                                ShadowFullscreenEffect    ( Texture2DPtr tex );
                                ~ShadowFullscreenEffect   ();

protected:

    virtual RenderableEntity *  CreateFullscreenQuad        () const override;

    PixelShader *               CreatePS                    () const;

};

DEFINE_PTR_TYPE(ShadowFullscreenEffect)
DEFINE_CONST_PTR_TYPE(ShadowFullscreenEffect)

} //bv
