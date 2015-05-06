#include "LibEffect.h"

#include "EffectRenderLogic.h"

#include "Application/WindowedApplication.h"

//#include "Engine/Graphics/Renderers/Renderer.h"

#include <cassert>

namespace bv { namespace effect
{

// *********************************
// Returns current renderer
bv::Renderer * Renderer()
{
	return const_cast< bv::Renderer * >( reinterpret_cast< bv::WindowedApplication * >(ApplicationBase::ApplicationInstance)->Renderer() ); // FIXME: Very ugly. How about ConsoleApplication ?
} 

// *********************************
// Returns RenderebleEffect implementing blur.
RenderableEffectPtr BlurEffect(  )
{
	return nullptr;
}

// *********************************
// Bluring image using GPU
MemoryChunkConstPtr		GLBlurImage( const MemoryChunkConstPtr & in, UInt32 width, UInt32 height, UInt32 bbp )
{
	assert( in->Size() == width * height * bbp / 8 );

	auto effect = BlurEffect();

	auto renderLogic = new EffectRenderLogic( width, height, 1, effect );

	renderLogic->DrawDisplayRenderTarget( Renderer() );

	return nullptr;
}


} // effect
} // bv