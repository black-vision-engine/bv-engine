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

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/Impl/ColorCorrectionFSEStep.h"

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


namespace bv { 

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

	auto nerl = new NodeEffectRenderLogic( passes );

    return std::make_shared< NodeEffectImpl >( nerl, NodeEffectType::NET_DEFAULT );
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

    return std::make_shared< NodeEffectImpl >( nnerl, NodeEffectType::NET_ALPHA_MASK );
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
    auto maskPreview    = get_value( fseStep->GetState(), "maskPreview" );
    
    auto preFSEStep     = new NodeMaskPreFSEStep   ( alphaVal, maskPreview, minAlphaThreshold );
	auto finalizeStep	= new NodeMaskFinalizeStep ();

    auto fsePass        = new FullscreenEffectPass ( preFSEStep, fseStep );
	auto finPass		= new FinalizePass( finalizeStep );

    std::vector< NodeEffectRenderPass * > passes( 2 );

    passes[ 0 ] = fsePass;
	passes[ 1 ] = finPass;

	auto nnerl = new NodeEffectRenderLogic( passes );

    return std::make_shared< NodeEffectImpl >( nnerl, NodeEffectType::NET_NODE_MASK );
}

// **************************
//
NodeEffectPtr   CreateColorCorrectionEffect( FullscreenEffectType nnodeEffectType ) // FIXME this code is intentionally left as shit to be double-checked
{
    //RenderLogic - default
    //Passes
    // - fse - default pre
    //    - pre step - color grade
    //    - fse step - color grade
    // - fin - default rendering
    //    - finalize step with default rendering
    // Create STEPS

    //float minAlphaThreshold = 0.01f;
    //float maxAlphaThreshold = 1.f;

    //auto fseStep        = new AlphaMaskFSEStep( minAlphaThreshold, maxAlphaThreshold );
    auto fseStep        = new ColorCorrectionFSEStep( nnodeEffectType );

    auto alphaVal       = ValuesFactory::CreateValueFloat( "alpha" ); //get_value( fseStep->GetState(), "alpha" );
//    auto maskPreview    = get_value( fseStep->GetState(), "maskPreview" );

//    auto preFSEStep     = new AlphaMaskPreFSEStep( fseStep->GetState(), minAlphaThreshold, maxAlphaThreshold );
    auto preFSEStep     = new GlowPreFSEStep( alphaVal, alphaVal, alphaVal ); // FIXME this is evident shit

    auto fsePass        = new FullscreenEffectPass( preFSEStep, fseStep );

    //auto finalizeStep	= new DefaultFinalizeStep(); //new NodeMaskFinalizeStep();
    //auto finPass		= new FinalizePass( finalizeStep );

    std::vector< NodeEffectRenderPass * > passes( 1 );

    passes[ 0 ] = fsePass;
    //passes[ 1 ] = finPass;

    auto nnerl = new NodeEffectRenderLogic( passes );

    return std::make_shared< NodeEffectImpl >( nnerl, NodeEffectType::NET_COLOR_CORRECTION );
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

	return std::make_shared< NodeEffectImpl >( nnerl, NodeEffectType::NET_BLUR );
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

	return std::make_shared< NodeEffectImpl >( nnerl, NodeEffectType::NET_LIGHT_SCATTERING );
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

    return std::make_shared< NodeEffectImpl >( nnerl, NodeEffectType::NET_Z_SORT );
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

	return std::make_shared< NodeEffectImpl >( nnerl, NodeEffectType::NET_SHADOW );
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

	return std::make_shared< NodeEffectImpl >( nnerl, NodeEffectType::NET_SHADOW );
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

	return std::make_shared< NodeEffectImpl >( nnerl, NodeEffectType::NET_SOFT_MASK );
}

// **************************
//
NodeEffectPtr   CreateColorGradeEffect()
{
    return nullptr;
}

// **************************
//
NodeEffectPtr       CreateNodeEffect( NodeEffectType nnodeEffectType )
{
    switch( nnodeEffectType )
    {
        case  NodeEffectType::NET_DEFAULT:
            return CreateDefaultNodeEffect();
        case NodeEffectType::NET_ALPHA_MASK:
            return CreateAlphaMaskNodeEffect();
        case NodeEffectType::NET_NODE_MASK:
            return CreateNodeMaskNodeEffect();
		case NodeEffectType::NET_BLUR:
			return CreateBlurNodeEffect();
		case NodeEffectType::NET_LIGHT_SCATTERING:
			return CreateLightScatteringNodeEffect();
		case NodeEffectType::NET_SHADOW:
			return CreateShadowNodeEffect();
		case NodeEffectType::NET_Z_SORT:
			return CreateZSortNodeEffect();
		case NodeEffectType::NET_GLOW:
			return CreateGlowNodeEffect();
		case NodeEffectType::NET_SOFT_MASK:
			return CreateSoftMaskNodeEffect();
		case NodeEffectType::NET_WIREFRAME:
            //return CreateWireframeNodeEffect();
            assert( false );
            break;
        case NodeEffectType::NET_MIX_CHANNELS:
            //return CreateMixchannelsNodeEffect();
			//Interlace and so on
            assert( false );
            break;
        case NodeEffectType::NET_COLOR_CORRECTION:
            return CreateColorCorrectionEffect( FullscreenEffectType::NFET_COLOR_CORRECTION );
        case NodeEffectType::NET_COLOR_BALANCE:
            return CreateColorCorrectionEffect( FullscreenEffectType::NFET_COLOR_BALANCE );
        default:
            assert( false );
    }

    return nullptr;
}


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
