#include "stdafx.h"

#include "BlurFSE.h"

#include "Engine/Graphics/Effects/Fullscreen/Impl/SimpleFullscreenEffect.h"
#include "Engine/Graphics/Effects/Fullscreen/Impl/CompositeFullscreenEffect.h"
#include "Engine/Graphics/Effects/Fullscreen/Impl/Graph/FullscreenEffectGraph.h"

#include "Engine/Types/Values/ValuesFactory.h"

#include "Engine/Graphics/Effects/Utils/ShaderSourceProvider.h"

namespace bv
{

// **************************
//
SimpleFullscreenEffect *       BlurFSECreator::CreateSimpleFSE         ( const std::vector< IValuePtr > & values )
{
    FullscreenEffectData fseData;
    auto src = FSEShaderSourceProvider->ReadShader( "blur.frag" );

    assert( values.size() == 2 );

    auto verticalVal        = ValuesFactory::CreateValueInt( "vertical", 0 );
    auto normalizeVal       = ValuesFactory::CreateValueInt( "normalize" );
    auto blurKernelTypeVal  = ValuesFactory::CreateValueInt( "blurKernelType" );

    fseData.AppendInputTexture( nullptr, "Tex0" );

    fseData.AppendValues( values );
    fseData.AppendValue( verticalVal );
    fseData.AppendValue( normalizeVal );
    fseData.AppendValue( blurKernelTypeVal );

    fseData.SetPixelShaderSource( src );

    fseData.SetBlendEnabled( true );
    fseData.SetCullEnabled( false );
    fseData.SetDepthTestEnabled( false );

    return new SimpleFullscreenEffect( fseData );
}

// **************************
//
CompositeFullscreenEffect *       BlurFSECreator::CreateCompositeFSE      ( const std::vector< IValuePtr > & values )
{
    FullscreenEffectData fseData;
    auto src = FSEShaderSourceProvider->ReadShader( "blur.frag" );

    assert( values.size() == 2 );

    auto verticalVal        = ValuesFactory::CreateValueInt( "vertical", 0 );
    auto normalizeVal       = ValuesFactory::CreateValueInt( "normalize" );
    auto blurKernelTypeVal  = ValuesFactory::CreateValueInt( "blurKernelType" );

    fseData.AppendInputTexture( nullptr, "Tex0" );

    fseData.AppendValues( values );
    fseData.AppendValue( verticalVal );
    fseData.AppendValue( normalizeVal );
    fseData.AppendValue( blurKernelTypeVal );

    fseData.SetPixelShaderSource( src );

    fseData.SetBlendEnabled( false );
    fseData.SetCullEnabled( false );
    fseData.SetDepthTestEnabled( false );

	FullscreenEffectData fseData1;

    fseData1.AppendInputTexture( nullptr, "Tex0" );

    fseData.AppendValues( values );
    fseData1.AppendValue( ValuesFactory::CreateValueInt( "vertical", 1 ) );
    fseData1.AppendValue( normalizeVal );
    fseData1.AppendValue( blurKernelTypeVal );

    fseData1.SetPixelShaderSource( src );

    fseData1.SetBlendEnabled( false );
    fseData1.SetCullEnabled( false );
    fseData1.SetDepthTestEnabled( false );


    auto firstPass = new SimpleFullscreenEffect( fseData );

    auto firstPassNode = std::shared_ptr< FullscreenEffectGraphNode >( new FullscreenEffectGraphNode( firstPass ) );

    auto sourceNode = std::shared_ptr< FullscreenEffectGraphNode >( new InputFullscreenEffectGraphNode() );

    auto secondPass = new SimpleFullscreenEffect( fseData1 );

    auto secondPassNode = std::shared_ptr< FullscreenEffectGraphNode >( new FullscreenEffectGraphNode( secondPass ) );

    auto graph = new FullscreenEffectGraph();

    auto compositeEffect = new CompositeFullscreenEffect( graph );

    secondPassNode->AddInput( firstPassNode );
    firstPassNode->AddInput( sourceNode );

    graph->AddNode( secondPassNode );

    graph->SetSinkNode( secondPassNode );
    graph->MarkSourceNode( sourceNode );

    return compositeEffect;
}

// **************************
//
CompositeFullscreenEffect *       BlurFSECreator::CreateCompositeOneFSE   ( const std::vector< IValuePtr > & values )
{
    FullscreenEffectData fseData;
    auto src = FSEShaderSourceProvider->ReadShader( "blur.frag" );

    assert( values.size() == 2 );

    auto verticalVal        = ValuesFactory::CreateValueInt( "vertical", 0 );
    auto normalizeVal       = ValuesFactory::CreateValueInt( "normalize", 1 );
    auto blurKernelTypeVal  = ValuesFactory::CreateValueInt( "blurKernelType", 0 );

    fseData.AppendInputTexture( nullptr, "Tex0" );

    fseData.AppendValues( values );
    fseData.AppendValue( verticalVal );
    fseData.AppendValue( normalizeVal );
    fseData.AppendValue( blurKernelTypeVal );

    fseData.SetPixelShaderSource( src );

    fseData.SetBlendEnabled( false );
    fseData.SetCullEnabled( false );
    fseData.SetDepthTestEnabled( false );

    auto firstPass = new SimpleFullscreenEffect( fseData );

    auto firstPassNode = std::shared_ptr< FullscreenEffectGraphNode >( new FullscreenEffectGraphNode( firstPass ) );

    auto graph = new FullscreenEffectGraph();

    std::vector< FullscreenEffectGraphNodePtr > predecessors;

    graph->InsertNode( firstPassNode, predecessors );

    auto compositeEffect = new CompositeFullscreenEffect( graph );

    graph->SetSinkNode( firstPassNode );
    graph->MarkSourceNode( firstPassNode );

    return compositeEffect;
}

} // bv