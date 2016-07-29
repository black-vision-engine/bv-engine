#include "stdafx.h"

#include "ShadowFSE.h"

#include "Engine/Graphics/Effects/Fullscreen/Impl/SimpleFullscreenEffect.h"
#include "Engine/Graphics/Effects/Fullscreen/Impl/CompositeFullscreenEffect.h"
#include "Engine/Graphics/Effects/Fullscreen/Impl/Graph/FullscreenEffectGraph.h"

#include "Engine/Types/Values/ValuesFactory.h"

#include "Engine/Graphics/Effects/Utils/ShaderSourceProvider.h"

#include "BlurFSE.h"



#include "Memory/MemoryLeaks.h"



namespace bv
{

// **************************
//
FullscreenEffect *       ShadowFSECreator::CreateShadowFSE         ( const std::vector< IValuePtr > & values )
{
    auto blurSize = ValuesFactory::CreateValueFloat( "blurSize" );

    std::vector< IValuePtr > blurValues;
    blurValues.insert( blurValues.begin(), values.begin(), values.end() );
    blurValues.push_back( blurSize );

    auto blurEffect = BlurFSECreator::CreateCompositeFSE( blurValues );

    auto colorVal = ValuesFactory::CreateValueVec4( "color" );
	auto shiftVal = ValuesFactory::CreateValueVec2( "shift" );
    auto innerVal = ValuesFactory::CreateValueInt( "inner" );

    FullscreenEffectData fseData;

    fseData.AppendInputTexture( nullptr, "BluredTex0" );
    fseData.AppendInputTexture( nullptr, "Tex0" );
    

    fseData.AppendValues( blurValues );
    fseData.AppendValue( colorVal );
    fseData.AppendValue( shiftVal );
    fseData.AppendValue( innerVal );

    auto src = FSEShaderSourceProvider->ReadShader( "shadow.frag" );
    fseData.SetPixelShaderSource( src );

    fseData.SetBlendEnabled( false );
    fseData.SetCullEnabled( false );
    fseData.SetDepthTestEnabled( false );

    auto shadowEffect = new SimpleFullscreenEffect( fseData );

    // building graph
    auto graph = new FullscreenEffectGraph();

    auto blurEffectNode = std::shared_ptr< FullscreenEffectGraphNode >( new FullscreenEffectGraphNode( blurEffect ) );

    auto shadowEffectNode = std::shared_ptr< FullscreenEffectGraphNode >( new FullscreenEffectGraphNode( shadowEffect ) );

    shadowEffectNode->AddInput( blurEffectNode );

    auto sourceNode = std::shared_ptr< FullscreenEffectGraphNode >( new InputFullscreenEffectGraphNode() );

    shadowEffectNode->AddInput( sourceNode );
    blurEffectNode->AddInput( sourceNode );

    graph->AddNode( shadowEffectNode );

    graph->MarkSourceNode( sourceNode );
    graph->SetSinkNode( shadowEffectNode );

    graph->FlattenGraph();

    return new CompositeFullscreenEffect( graph );
}

} // bv