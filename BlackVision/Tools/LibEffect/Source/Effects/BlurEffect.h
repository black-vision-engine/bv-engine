#pragma once

#include "Engine/Graphics/Shaders/RenderableEffect.h"

namespace bv { namespace effect
{

class BlurEffect : public RenderableEffect
{
private:
	UInt32				m_blurLength;
	Float32				m_pixelWidth;
	Float32				m_pixelHeight;
	ValueIntPtr			m_blurLenghtVal;
	ValueFloatPtr		m_pixelWidthVal;
	ValueFloatPtr		m_pixelHeightVal;

public:

						BlurEffect		( UInt32 blurLenght, Float32 pixelWidth, Float32 pixelHeight, Texture2DPtr texture, TextureFilteringMode filteringMode, TextureWrappingMode wrapModeX, TextureWrappingMode wrapModeY, const glm::vec4 & borderColor );
						~BlurEffect		();

private:

    PixelShader *       CreatePS        ( Texture2DPtr texture, TextureFilteringMode filteringMode, TextureWrappingMode wrapModeX, TextureWrappingMode wrapModeY, const glm::vec4 & borderColor );
    VertexShader *      CreateVS        ();

    TextureSampler *    CreateSampler   ( TextureFilteringMode filteringMode, TextureWrappingMode wrapModeX, TextureWrappingMode wrapModeY, const glm::vec4 & borderColor );

	std::string			GetPSSource		() const;
	std::string			GetVSSource		() const;

	static std::string	ReadFile		( const std::string & filename );
};

DEFINE_PTR_TYPE(BlurEffect)
DEFINE_CONST_PTR_TYPE(BlurEffect)

} // effect
} // bv