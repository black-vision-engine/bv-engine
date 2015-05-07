#pragma once

#include "Engine/Graphics/Shaders/RenderableEffect.h"

namespace bv { namespace effect
{

class BlurEffect : public RenderableEffect
{

public:

						BlurEffect		( Texture2DPtr texture, TextureFilteringMode filteringMode, TextureWrappingMode wrapModeX, TextureWrappingMode wrapModeY, const glm::vec4 & borderColor );
						~BlurEffect		();

private:

    PixelShader *       CreatePS        ( Texture2DPtr texture, TextureFilteringMode filteringMode, TextureWrappingMode wrapModeX, TextureWrappingMode wrapModeY, const glm::vec4 & borderColor );
    VertexShader *      CreateVS        ();

    TextureSampler *    CreateSampler   ( TextureFilteringMode filteringMode, TextureWrappingMode wrapModeX, TextureWrappingMode wrapModeY, const glm::vec4 & borderColor );
};

DEFINE_PTR_TYPE(BlurEffect)
DEFINE_CONST_PTR_TYPE(BlurEffect)

} // effect
} // bv