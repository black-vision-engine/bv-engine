#pragma once

#include <glm/glm.hpp>

#include "Engine/Graphics/Shaders/RenderableEffect.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamFloat.h"


namespace bv {

class Texture2D;
class IValue;
class TextureSampler;

class Texture2DEffectWithMask : public RenderableEffect
{
private:

    ShaderParamFloat *  m_alphaParam;

public:

                Texture2DEffectWithMask ( Texture2DPtr texture, Texture2DPtr mask,
                                        bool hasAlpha = false,
                                        TextureFilteringMode filteringMode = TextureFilteringMode::TFM_POINT,
                                        TextureWrappingMode wrapModeX = TextureWrappingMode::TWM_CLAMP_BORDER,
                                        TextureWrappingMode wrapModeY = TextureWrappingMode::TWM_CLAMP_BORDER,
                                        const glm::vec4 & borderColor = glm::vec4( 0.f, 0.f, 0.f, 0.f ) );
                ~Texture2DEffectWithMask();

    void        SetAlphaValModel        ( const IValue * val );

private:

    PixelShader *       CreatePS        ( Texture2DPtr texture, Texture2DPtr mask, TextureFilteringMode filteringMode, TextureWrappingMode wrapModeX, TextureWrappingMode wrapModeY, const glm::vec4 & borderColor, bool hasAlpha );
    VertexShader *      CreateVS        ();

    TextureSampler *    CreateSampler   ( TextureFilteringMode filteringMode, TextureWrappingMode wrapModeX, TextureWrappingMode wrapModeY, const glm::vec4 & borderColor );

};

DEFINE_PTR_TYPE(Texture2DEffectWithMask)
DEFINE_CONST_PTR_TYPE(Texture2DEffectWithMask)

} //bv
