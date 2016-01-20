#pragma once

#include "Rendering/Logic/FullScreen/FullscreenEffect.h"

#include "Engine/Graphics/Resources/Textures/Texture2D.h"


namespace bv {

class BlurFullscreenEffect : public FullscreenEffect
{
private:

    float                       m_blurSize;
    bool                        m_vertical;
    bool                        m_normalize;
    Int32                       m_blurKernelType;

    ValueIntPtr                 m_verticalVal;
    ValueIntPtr                 m_normalizeVal;
    ValueIntPtr                 m_blurKernelTypeVal;
    ValueFloatPtr               m_blurSizeVal;

    ValueVec2Ptr                m_textureSize;
    Texture2DPtr                m_texture;

    mutable PixelShader *       m_pixelShader;

public:

    void                        SetBlurSize             ( float s );
    float                       GetBlurSize             () const;

    void                        SetVertical             ( bool val );
    bool                        GetVertical             () const;

    void                        SetNormalize            ( bool val );

    void                        SetBlurKernelTypeSize   ( Int32 val );

    void                        SetTexture              ( Texture2DPtr tex );

                                BlurFullscreenEffect    ( Texture2DPtr tex );
                                ~BlurFullscreenEffect   ();

protected:

    virtual RenderableEntity *  CreateFullscreenQuad    () const override;

    PixelShader *               CreatePS                () const;

};

DEFINE_PTR_TYPE(BlurFullscreenEffect)
DEFINE_CONST_PTR_TYPE(BlurFullscreenEffect)

} //bv
