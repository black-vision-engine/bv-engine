#include "LibEffect.h"

#include "EffectRenderLogic.h"

#include "Application/WindowedApplication.h"

#include "Effects/BlurEffect.h"

#include "Engine/Graphics/Resources/Texture2DImpl.h"

#include "Engine/Graphics/SceneGraph/Camera.h"

#include "Engine/Graphics/Renderers/Renderer.h"

#include <cassert>

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
RenderableEffectPtr GetBlurEffect( Texture2DPtr texture, TextureFilteringMode filteringMode, TextureWrappingMode wrapModeX, TextureWrappingMode wrapModeY, const glm::vec4 & borderColor )
{
	return std::make_shared< BlurEffect >( texture, filteringMode, wrapModeX, wrapModeY, borderColor );
}

// *********************************
// Bluring image using GPU
MemoryChunkConstPtr		GLBlurImage( const MemoryChunkConstPtr & in, UInt32 width, UInt32 height, UInt32 bbp )
{
	assert( in->Size() == width * height * bbp / 8 );

	auto tex = std::make_shared< Texture2DImpl >( TextureFormat::F_A8R8G8B8, width, height );

	std::vector< MemoryChunkConstPtr > d;
	d.push_back( in );

	tex->SetRawData( d, TextureFormat::F_A8R8G8B8, width, height );

	auto effect = GetBlurEffect( tex, TextureFilteringMode::TFM_POINT, TextureWrappingMode::TWM_CLAMP_BORDER, TextureWrappingMode::TWM_CLAMP_BORDER, glm::vec4( 0.f, 0.f, 0.f, 0.f ) );

	auto renderLogic = new EffectRenderLogic( width, height, 4, effect );

	renderLogic->SetRendererCamera( new Camera() );

	renderLogic->DrawDisplayRenderTarget( GetRenderer() );

	auto texOut = renderLogic->ReadDisplayTarget( GetRenderer(), 0 );

	return texOut->GetData();
}


} // effect
} // bv