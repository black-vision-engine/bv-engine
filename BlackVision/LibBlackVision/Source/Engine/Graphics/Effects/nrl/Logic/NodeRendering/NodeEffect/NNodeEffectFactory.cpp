#include "stdafx.h"

#include "NNodeEffectFactory.h"

#include <cassert>

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/NNodeEffectImpl.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/NNodeEffectRenderLogic.h"

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/NPreFullscreenEffectStep.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/NFullscreenEffectPass.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/NFinalizePass.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/NEmptyPass.h"

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/NPreFullscreenEffectStep.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/NFullscreenEffectStep.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/Impl/NDefaultFinalizeStep.h"

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/Impl/NAlphaMaskPreFSEStep.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/Impl/NAlphaMaskFSEStep.h"

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/Impl/NNodeMaskPreFSEStep.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/Impl/NNodeMaskFSEStep.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/Impl/NNodeMaskFinalizeStep.h"

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/Impl/NBlurPreFSEStep.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/Impl/NBlurFSEStep.h"

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/Impl/NLightScatteringPreFSEStep.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/Impl/NLightScatteringFSEStep.h"

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/Impl/NShadowPreFSEStep.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/Impl/NShadowFSEStep.h"

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/Impl/NZSortFinalizeStep.h"

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/Impl/NGlowPreFSEStep.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/Impl/NGlowFSEStep.h"

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/Impl/NSoftMaskPreFSEStep.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/Impl/NSoftMaskFSEStep.h"

#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/NFullscreenEffectFactory.h"
#include "Engine/Graphics/Effects/nrl/Logic/State/NFullscreenEffectComponentState.h"

#include "Engine/Types/Values/ValuesFactory.h"


namespace bv { namespace nrl {

// **************************
//
NNodeEffectPtr       CreateDefaultNodeEffect    ()
{
	//RenderLogic - default
	//Passes
	// - fse - empty
	// - fin - default rendering
	//    - finalize step with default rendering
    // Create STEPS
	auto emptyPass      = new NEmptyPass();	
    
    auto finalizeStep   = new NDefaultFinalizeStep();
    auto finPass        = new NFinalizePass( finalizeStep );

    std::vector< NNodeEffectRenderPass * > passes( 2 );

    passes[ 0 ] = emptyPass;
    passes[ 1 ] = finPass;

	auto nnerl = new NNodeEffectRenderLogic( passes );

    return std::make_shared< NNodeEffectImpl >( nnerl, NNodeEffectType::NNET_DEFAULT );
}

// **************************
//
NNodeEffectPtr       CreateAlphaMaskNodeEffect()
{
	//RenderLogic - default
	//Passes
	// - fse - default pre
    //    - pre step - alpha
    //    - fse step - alpha
	// - fin - default rendering
	//    - finalize step with default rendering
    // Create STEPS
    
    float minAlphaThreshold = -1.0f;
    float maxAlphaThreshold = 1.1f;

    auto fseStep        = new NAlphaMaskFSEStep     ( minAlphaThreshold, maxAlphaThreshold );
    auto fseState       = fseStep->GetState();

    auto preFSEStep     = new NAlphaMaskPreFSEStep  ( fseState, minAlphaThreshold, maxAlphaThreshold );
    auto finalizeStep   = new NDefaultFinalizeStep();

    auto fsePass        = new NFullscreenEffectPass ( preFSEStep, fseStep );
    auto finPass        = new NFinalizePass( finalizeStep );

    std::vector< NNodeEffectRenderPass * > passes( 2 );

    passes[ 0 ] = fsePass;
    passes[ 1 ] = finPass;

	auto nnerl = new NNodeEffectRenderLogic( passes );

    return std::make_shared< NNodeEffectImpl >( nnerl, NNodeEffectType::NNET_ALPHA_MASK );
}
 
// **************************
//
NNodeEffectPtr       CreateNodeMaskNodeEffect   ()
{
	//RenderLogic - default
	//Passes
	// - fse - default pre
    //    - pre step - node mask
    //    - fse step - node mask
	// - fin - default rendering
	//    - finalize step with default rendering
    // Create STEPS
    
    float minAlphaThreshold = 0.01f;

    auto fseStep        = new NNodeMaskFSEStep      ( minAlphaThreshold );

    auto alphaVal       = get_value( fseStep->GetState(), "alpha" );
    
    auto preFSEStep     = new NNodeMaskPreFSEStep   ( alphaVal, minAlphaThreshold );
	auto finalizeStep	= new NNodeMaskFinalizeStep ();

    auto fsePass        = new NFullscreenEffectPass ( preFSEStep, fseStep );
	auto finPass		= new NFinalizePass( finalizeStep );

    std::vector< NNodeEffectRenderPass * > passes( 2 );

    passes[ 0 ] = fsePass;
	passes[ 1 ] = finPass;

	auto nnerl = new NNodeEffectRenderLogic( passes );

    return std::make_shared< NNodeEffectImpl >( nnerl, NNodeEffectType::NNET_NODE_MASK );
}

// **************************
//
NNodeEffectPtr       CreateBlurNodeEffect   ()
{
	//RenderLogic - default
	//Passes
	// - fse - default pre
	//    - pre step - node mask
	//    - fse step - node mask
	// - fin - default rendering
	//    - finalize step with default rendering
	// Create STEPS

	
	auto fseStep = new NBlurFSEStep();
	auto blurSizeVal = get_value( fseStep->GetState(), "blurSize" );

	auto preFSEStep = new NBlurPreFSEStep( blurSizeVal );

	auto fsePass = new NFullscreenEffectPass ( preFSEStep, fseStep );

	auto finalizeStep = new NDefaultFinalizeStep();
	auto finPass = new NFinalizePass( finalizeStep );

	std::vector< NNodeEffectRenderPass * > passes( 2 );

	passes[ 0 ] = fsePass;
	passes[ 1 ] = finPass;

	auto nnerl = new NNodeEffectRenderLogic( passes );

	return std::make_shared< NNodeEffectImpl >( nnerl, NNodeEffectType::NNET_BLUR );
}

// **************************
//
NNodeEffectPtr       CreateLightScatteringNodeEffect   ()
{
	//RenderLogic - default
	//Passes
	// - fse - default pre
	//    - pre step - node mask
	//    - fse step - node mask
	// - fin - default rendering
	//    - finalize step with default rendering
	// Create STEPS


	auto fseStep = new NLightScatteringFSEStep();

	auto preFSEStep = new NLightScatteringPreFSEStep();

	auto fsePass = new NFullscreenEffectPass ( preFSEStep, fseStep );

	auto finalizeStep = new NDefaultFinalizeStep();
	auto finPass = new NFinalizePass( finalizeStep );

	std::vector< NNodeEffectRenderPass * > passes( 2 );

	passes[ 0 ] = fsePass;
	passes[ 1 ] = finPass;

	auto nnerl = new NNodeEffectRenderLogic( passes );

	return std::make_shared< NNodeEffectImpl >( nnerl, NNodeEffectType::NNET_LIGHT_SCATTERING );
}

// ***********************
//
NNodeEffectPtr       CreateZSortNodeEffect      ()
{
    auto sortStep = new NZSortFinalizeStep();
    auto sortPass = new NFinalizePass( sortStep );

    std::vector< NNodeEffectRenderPass * > passes( 1 );

    passes[ 0 ] = sortPass;

    auto nnerl = new NNodeEffectRenderLogic( passes, false );

    return std::make_shared< NNodeEffectImpl >( nnerl, NNodeEffectType::NNET_Z_SORT );
}

// **************************
//
NNodeEffectPtr       CreateShadowNodeEffect   ()
{
	//RenderLogic - default
	//Passes
	// - fse - default pre
	//    - pre step - node mask
	//    - fse step - node mask
	// - fin - default rendering
	//    - finalize step with default rendering
	// Create STEPS


	auto fseStep = new NShadowFSEStep(  );

	auto blurSizeVal = get_value( fseStep->GetState(), "blurSize" );
	auto shiftVal = get_value( fseStep->GetState(), "shift" );
	auto innerVal = get_value( fseStep->GetState(), "inner" );
	auto outerVal = get_value( fseStep->GetState(), "outer" );

	auto preFSEStep = new NShadowPreFSEStep( blurSizeVal, shiftVal, innerVal, outerVal );

	auto fsePass = new NFullscreenEffectPass ( preFSEStep, fseStep );

	auto finalizeStep = new NDefaultFinalizeStep();
	auto finPass = new NFinalizePass( finalizeStep );

	std::vector< NNodeEffectRenderPass * > passes( 2 );

	passes[ 0 ] = fsePass;
	passes[ 1 ] = finPass;

	auto nnerl = new NNodeEffectRenderLogic( passes );

	return std::make_shared< NNodeEffectImpl >( nnerl, NNodeEffectType::NNET_SHADOW );
}

// **************************
//
NNodeEffectPtr       CreateGlowNodeEffect   ()
{
	//RenderLogic - default
	//Passes
	// - fse - default pre
	//    - pre step - node mask
	//    - fse step - node mask
	// - fin - default rendering
	//    - finalize step with default rendering
	// Create STEPS


	auto fseStep = new NGlowFSEStep();

	auto blurSizeVal = get_value( fseStep->GetState(), "blurSize" );
	auto innerVal = get_value( fseStep->GetState(), "inner" );
	auto outerVal = get_value( fseStep->GetState(), "outer" );

	auto preFSEStep = new NGlowPreFSEStep( blurSizeVal, innerVal, outerVal );

	auto fsePass = new NFullscreenEffectPass ( preFSEStep, fseStep );

	auto finalizeStep = new NDefaultFinalizeStep();
	auto finPass = new NFinalizePass( finalizeStep );

	std::vector< NNodeEffectRenderPass * > passes( 2 );

	passes[ 0 ] = fsePass;
	passes[ 1 ] = finPass;

	auto nnerl = new NNodeEffectRenderLogic( passes );

	return std::make_shared< NNodeEffectImpl >( nnerl, NNodeEffectType::NNET_SHADOW );
}

// **************************
//
NNodeEffectPtr       CreateSoftMaskNodeEffect   ()
{
	//RenderLogic - default
	//Passes
	// - fse - default pre
	//    - pre step - node mask
	//    - fse step - node mask
	// - fin - default rendering
	//    - finalize step with default rendering
	// Create STEPS


	auto fseStep = new NSoftMaskFSEStep();

	auto preFSEStep = new NSoftMaskPreFSEStep();

	auto fsePass = new NFullscreenEffectPass ( preFSEStep, fseStep );

	auto finalizeStep = new NDefaultFinalizeStep();
	auto finPass = new NFinalizePass( finalizeStep );

	std::vector< NNodeEffectRenderPass * > passes( 2 );

	passes[ 0 ] = fsePass;
	passes[ 1 ] = finPass;

	auto nnerl = new NNodeEffectRenderLogic( passes );

	return std::make_shared< NNodeEffectImpl >( nnerl, NNodeEffectType::NNET_SOFT_MASK );
}

// **************************
//
NNodeEffectPtr       CreateNodeEffect( NNodeEffectType nnodeEffectType )
{
    switch( nnodeEffectType )
    {
        case  NNodeEffectType::NNET_DEFAULT:
            return CreateDefaultNodeEffect();
        case NNodeEffectType::NNET_ALPHA_MASK:
            return CreateAlphaMaskNodeEffect();
        case NNodeEffectType::NNET_NODE_MASK:
            return CreateNodeMaskNodeEffect();
		case NNodeEffectType::NNET_BLUR:
			return CreateBlurNodeEffect();
		case NNodeEffectType::NNET_LIGHT_SCATTERING:
			return CreateLightScatteringNodeEffect();
		case NNodeEffectType::NNET_SHADOW:
			return CreateShadowNodeEffect();
		case NNodeEffectType::NNET_Z_SORT:
			return CreateZSortNodeEffect();
		case NNodeEffectType::NNET_GLOW:
			return CreateGlowNodeEffect();
		case NNodeEffectType::NNET_SOFT_MASK:
			return CreateSoftMaskNodeEffect();
		case NNodeEffectType::NNET_WIREFRAME:
            //return CreateWireframeNodeEffect();
        case NNodeEffectType::NNET_MIX_CHANNELS:
            //return CreateMixchannelsNodeEffect();
			//Interlace and so on
        default:
            assert( false );
    }

    return nullptr;
}

} //nrl
} //bv


/*
namespace {
// **************************
//
NNodeEffectPtr  CreateNodeEffect( NodeEffectLogic * logic, NodeEffectType neType )
{
    return std::make_shared< NodeEffect >( logic, neType );
}

// **************************
//
NodeEffectLogic *  CreateNodeEffectLogic()
{
    return new NodeEffectLogic();
}

// **************************
//
void    SetLogicComponents( NodeEffectLogic * logic, PreFullscreenEffectLogic * pre, FullscreenEffectInstance * fse, PostFullscreenEffectLogic * post )
{
    assert( logic && ( pre || fse || post ) );

    logic->SetComponent( pre );
    logic->SetComponent( fse );
    logic->SetComponent( post );
}
} // anonoymnous

*/
