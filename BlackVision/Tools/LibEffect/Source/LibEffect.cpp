#include "LibEffect.h"

#include <cassert>

#include "Application/WindowedApplication.h"

#include "Effects/BlurEffect.h"

#include "Engine/Graphics/Resources/Textures/Texture2D.h"

#include "Engine/Graphics/SceneGraph/Camera.h"

#include "Engine/Graphics/Renderers/Renderer.h"

namespace bv { namespace effect
{

bv::Renderer * g_renderer = nullptr;

// *********************************
// Initialize library
void InitializeLibEffect( bv::Renderer * renderer )
{
	g_renderer = renderer; 
}

// *********************************
// Returns current renderer
bv::Renderer * GetRenderer()
{
	return g_renderer;
} 

// *********************************
// Returns RenderebleEffect implementing blur.
RenderableEffectPtr GetBlurEffect( UInt32 blurLength, Float32 pixelWidth, Float32 pixelHeight, Texture2DPtr texture, TextureFilteringMode filteringMode, TextureWrappingMode wrapModeX, TextureWrappingMode wrapModeY, const glm::vec4 & borderColor )
{
	return std::make_shared< BlurEffect >( blurLength, pixelWidth, pixelHeight, texture, filteringMode, wrapModeX, wrapModeY, borderColor );
}

// *********************************
// Bluring image with GPU
MemoryChunkConstPtr		GLBlurImage( const MemoryChunkConstPtr & in, UInt32 width, UInt32 height, UInt32 bpp, UInt32 blurLength )
{

    { bpp; }

    assert( false );

    assert( in->Size() == width * height * bpp / 8 );
	auto tex = std::make_shared< Texture2D >( TextureFormat::F_A8R8G8B8, width, height, DataBuffer::Semantic::S_TEXTURE_STATIC, 1 );

	tex->SetData( in );

	auto effect = GetBlurEffect( blurLength, 1.f / width, 1.f / height, tex, TextureFilteringMode::TFM_POINT, TextureWrappingMode::TWM_CLAMP_EDGE, TextureWrappingMode::TWM_CLAMP_EDGE, glm::vec4( 0.f, 0.f, 0.f, 0.f ) );

    // FIXME: REIMPLEMENT
	//auto renderLogic = new EffectRenderLogic( width, height, effect, new Camera() );

	//renderLogic->Draw( GetRenderer() );

	//auto texOut = renderLogic->ReadTarget( GetRenderer() );

	//return texOut->GetData();
    return nullptr;
}


} // effect
} // bv