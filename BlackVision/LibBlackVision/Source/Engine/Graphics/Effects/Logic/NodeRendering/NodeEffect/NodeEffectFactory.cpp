#include "stdafx.h"

#include "NodeEffectFactory.h"

#include <cassert>

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/NodeEffectImpl.h"
#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/NodeEffectRenderLogic.h"

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/PreFullscreenEffectStep.h"
#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/FullscreenEffectPass.h"
#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/FinalizePass.h"
#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/EmptyPass.h"

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/PreFullscreenEffectStep.h"
#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/FullscreenEffectStep.h"
#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/Impl/DefaultFinalizeStep.h"

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/Impl/AlphaMaskPreFSEStep.h"
#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/Impl/AlphaMaskFSEStep.h"

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/Impl/NodeMaskPreFSEStep.h"
#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/Impl/NodeMaskFSEStep.h"
#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/Impl/NodeMaskFinalizeStep.h"

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/Impl/BlurPreFSEStep.h"
#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/Impl/BlurFSEStep.h"

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/Impl/LightScatteringPreFSEStep.h"
#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/Impl/LightScatteringFSEStep.h"

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/Impl/ShadowPreFSEStep.h"
#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/Impl/ShadowFSEStep.h"

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/Impl/ZSortFinalizeStep.h"

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/Impl/GlowPreFSEStep.h"
#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/Impl/GlowFSEStep.h"

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/Impl/SoftMaskPreFSEStep.h"
#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/Impl/SoftMaskFSEStep.h"

#include "Engine/Graphics/Effects/Logic/FullscreenRendering/FullscreenEffectFactory.h"
#include "Engine/Graphics/Effects/Logic/State/FullscreenEffectComponentState.h"

#include "Engine/Types/Values/ValuesFactory.h"


namespace bv { namespace nrl {

// **************************
//
NodeEffectPtr       CreateDefaultNodeEffect    ()
{
	//RenderLogic - default
	//Passes
	// - fse - empty
	// - fin - default rendering
	//    - finalize step with default rendering
    // Create STEPS
	auto emptyPass      = new EmptyPass();	
    
    auto finalizeStep   = new DefaultFinalizeStep();
    auto finPass        = new FinalizePass( finalizeStep );

    std::vector< NodeEffectRenderPass * > passes( 2 );

    passes[ 0 ] = emptyPass;
    passes[ 1 ] = finPass;

	auto nnerl = new NodeEffectRenderLogic( passes );

    return std::make_shared< NodeEffectImpl >( nnerl, NNodeEffectType::NNET_DEFAULT );
}

// **************************
//
NodeEffectPtr       CreateAlphaMaskNodeEffect()
{
	//RenderLogic - default
	//Passes
	// - fse - default pre
    //    - pre step - alpha
    //    - fse step - alpha
	// - fin - default rendering
	//    - finalize step with default rendering
    // Create STEPS
    
    float minAlphaThreshold = 0.01f;
    float maxAlphaThreshold = 0.99f;

    auto fseStep        = new AlphaMaskFSEStep     ( minAlphaThreshold, maxAlphaThreshold );
    auto fseState       = fseStep->GetState();

    auto preFSEStep     = new AlphaMaskPreFSEStep  ( fseState, minAlphaThreshold, maxAlphaThreshold );
    auto finalizeStep   = new DefaultFinalizeStep();

    auto fsePass        = new FullscreenEffectPass ( preFSEStep, fseStep );
    auto finPass        = new FinalizePass( finalizeStep );

    std::vector< NodeEffectRenderPass * > passes( 2 );

    passes[ 0 ] = fsePass;
    passes[ 1 ] = finPass;

	auto nnerl = new NodeEffectRenderLogic( passes );

    return std::make_shared< NodeEffectImpl >( nnerl, NNodeEffectType::NNET_ALPHA_MASK );
}
 
// **************************
//
NodeEffectPtr       CreateNodeMaskNodeEffect   ()
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

    auto fseStep        = new NodeMaskFSEStep      ( minAlphaThreshold );

    auto alphaVal       = get_value( fseStep->GetState(), "alpha" );
    
    auto preFSEStep     = new NodeMaskPreFSEStep   ( alphaVal, minAlphaThreshold );
	auto finalizeStep	= new NodeMaskFinalizeStep ();

    auto fsePass        = new FullscreenEffectPass ( preFSEStep, fseStep );
	auto finPass		= new FinalizePass( finalizeStep );

    std::vector< NodeEffectRenderPass * > passes( 2 );

    passes[ 0 ] = fsePass;
	passes[ 1 ] = finPass;

	auto nnerl = new NodeEffectRenderLogic( passes );

    return std::make_shared< NodeEffectImpl >( nnerl, NNodeEffectType::NNET_NODE_MASK );
}

// **************************
//
NodeEffectPtr       CreateBlurNodeEffect   ()
{
	//RenderLogic - default
	//Passes
	// - fse - default pre
	//    - pre step - node mask
	//    - fse step - node mask
	// - fin - default rendering
	//    - finalize step with default rendering
	// Create STEPS

	
	auto fseStep = new BlurFSEStep();
	auto blurSizeVal = get_value( fseStep->GetState(), "blurSize" );

	auto preFSEStep = new BlurPreFSEStep( blurSizeVal );

	auto fsePass = new FullscreenEffectPass ( preFSEStep, fseStep );

	auto finalizeStep = new DefaultFinalizeStep();
	auto finPass = new FinalizePass( finalizeStep );

	std::vector< NodeEffectRenderPass * > passes( 2 );

	passes[ 0 ] = fsePass;
	passes[ 1 ] = finPass;

	auto nnerl = new NodeEffectRenderLogic( passes );

	return std::make_shared< NodeEffectImpl >( nnerl, NNodeEffectType::NNET_BLUR );
}

// **************************
//
NodeEffectPtr       CreateLightScatteringNodeEffect   ()
{
	//RenderLogic - default
	//Passes
	// - fse - default pre
	//    - pre step - node mask
	//    - fse step - node mask
	// - fin - default rendering
	//    - finalize step with default rendering
	// Create STEPS


	auto fseStep = new LightScatteringFSEStep();

	auto preFSEStep = new LightScatteringPreFSEStep();

	auto fsePass = new FullscreenEffectPass ( preFSEStep, fseStep );

	auto finalizeStep = new DefaultFinalizeStep();
	auto finPass = new FinalizePass( finalizeStep );

	std::vector< NodeEffectRenderPass * > passes( 2 );

	passes[ 0 ] = fsePass;
	passes[ 1 ] = finPass;

	auto nnerl = new NodeEffectRenderLogic( passes );

	return std::make_shared< NodeEffectImpl >( nnerl, NNodeEffectType::NNET_LIGHT_SCATTERING );
}

// ***********************
//
NodeEffectPtr       CreateZSortNodeEffect      ()
{
    auto sortStep = new ZSortFinalizeStep();
    auto sortPass = new FinalizePass( sortStep );

    std::vector< NodeEffectRenderPass * > passes( 1 );

    passes[ 0 ] = sortPass;

    auto nnerl = new NodeEffectRenderLogic( passes, false );

    return std::make_shared< NodeEffectImpl >( nnerl, NNodeEffectType::NNET_Z_SORT );
}

// **************************
//
NodeEffectPtr       CreateShadowNodeEffect   ()
{
	//RenderLogic - default
	//Passes
	// - fse - default pre
	//    - pre step - node mask
	//    - fse step - node mask
	// - fin - default rendering
	//    - finalize step with default rendering
	// Create STEPS


	auto fseStep = new ShadowFSEStep(  );

	auto blurSizeVal = get_value( fseStep->GetState(), "blurSize" );
	auto shiftVal = get_value( fseStep->GetState(), "shift" );
	auto innerVal = get_value( fseStep->GetState(), "inner" );
	auto outerVal = get_value( fseStep->GetState(), "outer" );

	auto preFSEStep = new ShadowPreFSEStep( blurSizeVal, shiftVal, innerVal, outerVal );

	auto fsePass = new FullscreenEffectPass ( preFSEStep, fseStep );

	auto finalizeStep = new DefaultFinalizeStep();
	auto finPass = new FinalizePass( finalizeStep );

	std::vector< NodeEffectRenderPass * > passes( 2 );

	passes[ 0 ] = fsePass;
	passes[ 1 ] = finPass;

	auto nnerl = new NodeEffectRenderLogic( passes );

	return std::make_shared< NodeEffectImpl >( nnerl, NNodeEffectType::NNET_SHADOW );
}

// **************************
//
NodeEffectPtr       CreateGlowNodeEffect   ()
{
	//RenderLogic - default
	//Passes
	// - fse - default pre
	//    - pre step - node mask
	//    - fse step - node mask
	// - fin - default rendering
	//    - finalize step with default rendering
	// Create STEPS


	auto fseStep = new GlowFSEStep();

	auto blurSizeVal = get_value( fseStep->GetState(), "blurSize" );
	auto innerVal = get_value( fseStep->GetState(), "inner" );
	auto outerVal = get_value( fseStep->GetState(), "outer" );

	auto preFSEStep = new GlowPreFSEStep( blurSizeVal, innerVal, outerVal );

	auto fsePass = new FullscreenEffectPass ( preFSEStep, fseStep );

	auto finalizeStep = new DefaultFinalizeStep();
	auto finPass = new FinalizePass( finalizeStep );

	std::vector< NodeEffectRenderPass * > passes( 2 );

	passes[ 0 ] = fsePass;
	passes[ 1 ] = finPass;

	auto nnerl = new NodeEffectRenderLogic( passes );

	return std::make_shared< NodeEffectImpl >( nnerl, NNodeEffectType::NNET_SHADOW );
}

// **************************
//
NodeEffectPtr       CreateSoftMaskNodeEffect   ()
{
	//RenderLogic - default
	//Passes
	// - fse - default pre
	//    - pre step - node mask
	//    - fse step - node mask
	// - fin - default rendering
	//    - finalize step with default rendering
	// Create STEPS


	auto fseStep = new SoftMaskFSEStep();

	auto preFSEStep = new SoftMaskPreFSEStep();

	auto fsePass = new FullscreenEffectPass ( preFSEStep, fseStep );

	auto finalizeStep = new DefaultFinalizeStep();
	auto finPass = new FinalizePass( finalizeStep );

	std::vector< NodeEffectRenderPass * > passes( 2 );

	passes[ 0 ] = fsePass;
	passes[ 1 ] = finPass;

	auto nnerl = new NodeEffectRenderLogic( passes );

	return std::make_shared< NodeEffectImpl >( nnerl, NNodeEffectType::NNET_SOFT_MASK );
}

// **************************
//
NodeEffectPtr       CreateNodeEffect( NNodeEffectType nnodeEffectType )
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
NodeEffectPtr  CreateNodeEffect( NodeEffectLogic * logic, NodeEffectType neType )
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
