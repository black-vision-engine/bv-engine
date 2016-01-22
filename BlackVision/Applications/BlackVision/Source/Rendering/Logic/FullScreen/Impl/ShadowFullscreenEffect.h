#pragma once

#include "Rendering/Logic/FullScreen/FullscreenEffect.h"

#include "Engine/Graphics/Resources/Textures/Texture2D.h"


namespace bv {

class ShadowFullscreenEffect : public FullscreenEffect
{
private:

    glm::vec4                   m_color;
    glm::vec2                   m_shift;

    ValueVec4Ptr                m_colorVal;
    ValueVec2Ptr                m_shiftVal;
    ValueIntPtr                 m_innerVal;

    ValueVec2Ptr                m_textureSize;
    Texture2DPtr                m_texture;
    Texture2DPtr                m_bluredTexture;

    mutable PixelShader *       m_pixelShader;

public:
    void                        SetColor    ( const glm::vec4 & color );
    glm::vec4                   GetColor    () const;

    void                        SetShift    ( const glm::vec2 & xy );
    glm::vec2                   GetShift    () const;

    void                        SetBlurSize ( float s );
    float                       GetBlurSize () const;

    void                        SetInner    ( Int32 flag );


                                ShadowFullscreenEffect    ( Texture2DPtr tex, Texture2DPtr bluredTexture );
                                ~ShadowFullscreenEffect   ();

protected:

    virtual RenderableEntity *  CreateFullscreenQuad        () const override;

    PixelShader *               CreatePS                    () const;

};

DEFINE_PTR_TYPE(ShadowFullscreenEffect)
DEFINE_CONST_PTR_TYPE(ShadowFullscreenEffect)

} //bv
