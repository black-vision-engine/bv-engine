#include "stdafx.h"

#include "NBlurFSEStep.h"

#include "Engine/Graphics/Effects/nrl/Logic/NRenderContext.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NNodeRenderLogic.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Types/Values/TypedValues.h"

#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"

#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/NFullscreenEffectFactory.h"

#include <cmath>

namespace bv { namespace nrl {

class RenderTargetsAllocators
{
	typedef std::map< 
		std::pair< UInt32, UInt32 >, 
		RenderTargetStackAllocator *
		>
		RTsMapType;

	RTsMapType							m_rtAllocatorsMap;

	//void					Clear( const RenderTarget * rt )
	//{
	//	auto prevRT = disableBoundRT( m_ctx );
	//	enable( m_ctx, rt );
	//	clearBoundRT( m_ctx, glm::vec4() );
	//	disableBoundRT( m_ctx );

	//	if( prevRT )
	//		enable( m_ctx, prevRT );
	//}

public:

	explicit				RenderTargetsAllocators()
	{};

	RenderTarget *			Allocate( UInt32 w, UInt32 h, RenderTarget::RTSemantic semantic )
	{
		auto k = std::make_pair( w, h );

		auto it = m_rtAllocatorsMap.find( k );

		if( it != m_rtAllocatorsMap.end() )
		{
			return it->second->Allocate( semantic );
		}
		else
		{
			 auto rt = new RenderTargetStackAllocator( w, h );
			 m_rtAllocatorsMap[ k ] = rt;
			 return rt->Allocate( semantic );
		}
	}

	bool					Free( UInt32 w, UInt32 h )
	{
		auto it = m_rtAllocatorsMap.find( std::make_pair( w, h ) );

		if( it != m_rtAllocatorsMap.end() )
		{
			return it->second->Free();
		}
		else
		{
			assert( false );
			return false;
		}

	}

	~RenderTargetsAllocators()
	{
		for( auto & allocator : m_rtAllocatorsMap )
			delete allocator.second;
	}
};


// **************************
//
NBlurFSEStep::NBlurFSEStep          ()
    : Parent( nullptr )
	, m_rtAllocators( new RenderTargetsAllocators(  ) )
{
    // FIXME: maybe one class is enough as we only use NFullscreenEffectType type here and always set FSE state as current state
    m_blurEffect = CreateFullscreenEffect( NFullscreenEffectType::NFET_BLUR );
	m_simpleBlitEffect = CreateFullscreenEffect( NFullscreenEffectType::NFET_SIMPLE_BLIT );
    SetState( m_blurEffect->GetState() );
}

// **************************
//
NBlurFSEStep::~NBlurFSEStep           ()
{
    delete m_blurEffect;
	delete m_simpleBlitEffect;
}

// **************************
//
unsigned int            NBlurFSEStep::GetNumRequiredInputs         () const
{
    return 1;
}

// **************************
//
void                    NBlurFSEStep::ReadInputState               ()
{}

// **************************
//
void                    NBlurFSEStep::ApplyImpl                    ( NRenderContext * ctx, const NRenderedData * input )
{
    assert( input );

	// FIXME: Some general mechanism should be implemented to set values in effect.
	auto textureSize = GetState()->GetValueAt( 0 );

	auto wrt = input->GetEntry( 0 )->Width();
	auto hrt = input->GetEntry( 0 )->Height();

	auto blurQualityLevelVal = GetState()->GetValueAt( 4 );
	auto blurQualityLevel = QueryTypedValue< ValueIntPtr >( blurQualityLevelVal );

	auto blurSizeVal = GetState()->GetValueAt( 1 );
	auto blurSize = QueryTypedValue< ValueFloatPtr >( blurSizeVal )->GetValue();

	// Set textureSize param value needed by blur shader.
	QueryTypedValue< ValueVec2Ptr >( textureSize )->SetValue( glm::vec2( wrt, hrt ) );

	auto mainRT = disableBoundRT( ctx );

	auto stepsNum = blurQualityLevel->GetValue() + 1;

	if( stepsNum > 0 )
	{
		NRenderedData rd( 1 );
		rd.SetEntry( 0, input->GetEntry( 0 ) );

		const RenderTarget * output = nullptr;

		blurSize /= stepsNum;

		for( auto i = 0; i < stepsNum - 1; ++i )
		{
			output = FastBlur( ctx, rd, blurSize, output );
			auto oldInputRT = rd.GetEntry( 0 );
			rd.SetEntry( 0, output );
			output = oldInputRT;

			// Clear blur output render target.
			enable( ctx, output );
			clearBoundRT( ctx, glm::vec4() );
			disableBoundRT( ctx );
		}

		FastBlur( ctx, rd, blurSize, mainRT );

		if( stepsNum > 1 )
		{
			allocator( ctx )->Free();
		}
	}
}

// **************************
//
const RenderTarget *	NBlurFSEStep::FastBlur						( NRenderContext * ctx, const NRenderedData & input, Float32 blurSize, const RenderTarget * output )
{
	if( blurSize > 16 )
	{
		NRenderedData inputResized( 1 );

		auto stepsNum = int( ceil( std::log( blurSize / 16 ) / std::log( 2 ) ) );

		auto inputW = input.GetEntry( 0 )->Width();
		auto inputH = input.GetEntry( 0 )->Height();

		auto outputW = UInt32( std::ceil( inputW / 2.f ) );
		auto outputH = UInt32( std::ceil( inputH / 2.f ) );

		RenderTarget * resizedRT = nullptr;
		NRenderedData inputTmp( 1 );
		inputTmp.SetEntry( 0, input.GetEntry( 0 ) );

		for( int i = 0; i < stepsNum; ++i )
		{
			resizedRT = m_rtAllocators->Allocate( outputW, outputH, RenderTarget::RTSemantic::S_DRAW_ONLY );

			ResizeInput( ctx, inputTmp, resizedRT );
			inputResized.SetEntry( 0, resizedRT );
			blurSize /= 2.f;

			if( i > 0 )
			{
				m_rtAllocators->Free( inputW, inputH );
			}

			inputW = outputW;
			inputH = outputH;

			if( i < stepsNum - 1 )
			{
				outputW = UInt32( std::ceil( inputW / 2.f ) );
				outputH = UInt32( std::ceil( inputH / 2.f ) );

				inputTmp.SetEntry( 0, resizedRT );
			}
		}


		inputResized.SetEntry( 0, resizedRT );

		auto ret =  BlurInput( ctx, inputResized, blurSize, output );
		m_rtAllocators->Free( outputW, outputH );
		return ret;
	}
	else
	{
		return BlurInput( ctx, input, blurSize, output );
	}
}

// **************************
//
const RenderTarget *	NBlurFSEStep::BlurInput						( NRenderContext * ctx, const NRenderedData & input, Float32 blurSize, const RenderTarget * output )
{
	if( !output )
	{
		// Allocate output render target if not passed
		output = allocator( ctx )->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );
		NRenderedData rd( 1 );
		rd.SetEntry( 0, output );
		enable( ctx, output );
		clearBoundRT( ctx, glm::vec4() );
		disableBoundRT( ctx );
	}

	auto blurSizeVal = GetState()->GetValueAt( 1 );
	QueryTypedValue< ValueFloatPtr >( blurSizeVal )->SetValue( blurSize );

	auto vertical = GetState()->GetValueAt( 3 );

	auto wrt = input.GetEntry( 0 )->Width();
	auto hrt = input.GetEntry( 0 )->Height();

	// Allocate new render target for vertical blur pass
	auto resizedHorizontallyBluredRT = m_rtAllocators->Allocate( wrt, hrt, RenderTarget::RTSemantic::S_DRAW_ONLY );

	NRenderedData rd( 1 );
	rd.SetEntry( 0, resizedHorizontallyBluredRT );

	// Clear blur output render target.
	enable( ctx, resizedHorizontallyBluredRT );
	clearBoundRT( ctx, glm::vec4() );

	auto textureSize = GetState()->GetValueAt( 0 );
	QueryTypedValue< ValueVec2Ptr >( textureSize )->SetValue( glm::vec2( wrt, hrt ) );

	// Run vertical blur pass
	QueryTypedValue< ValueBoolPtr >( vertical )->SetValue( true );
	m_blurEffect->Render( ctx, resizedHorizontallyBluredRT, input );
	disableBoundRT( ctx );


	// Run horizontal blur pass
	enable( ctx, output );
	QueryTypedValue< ValueBoolPtr >( vertical )->SetValue( false );
	m_blurEffect->Render( ctx, rd );

	m_rtAllocators->Free( wrt, hrt );

	//allocator( ctx )->Free();  // free allocated locally render target.

	return output;
}

// **************************
//
void					NBlurFSEStep::ResizeInput					( NRenderContext * ctx, const NRenderedData & input, const RenderTarget * output )
{
	// Clear blur output render target.
	enable( ctx, output );
	clearBoundRT( ctx, glm::vec4() );

	m_simpleBlitEffect->Render( ctx, input );

	disableBoundRT( ctx );
}

// **************************
//
void                    NBlurFSEStep::FreeRenderTargets				( NRenderContext * ctx, const NRenderedData * input )
{
	input;
    assert( input->GetNumEntries() == GetNumRequiredInputs() );
    assert( allocator( ctx )->GetTopIndex() == input->GetLastRenderTargetIdx() );

    allocator( ctx )->Free( GetNumRequiredInputs() );
}

// **************************
//
Float32					NBlurFSEStep::GetBlurSize				() const
{
	auto blurSize = GetState()->GetValueAt( 1 );

	return QueryTypedValue< ValueFloatPtr >( blurSize )->GetValue();
}

// **************************
//
bool                    NBlurFSEStep::IsIdle                       ( SceneNodeRepr * ) const
{
	// Nothing to do if blur size is 0.
	return GetBlurSize() == 0;
}

// **************************
// If 
bool                    NBlurFSEStep::IsFinal                      ( SceneNodeRepr * ) const
{
	// if we've just rendered blured image fo not run next default finalize pass.
	// else run it.
	return GetBlurSize() != 0;
}

} // nrl
} // bv
