#include "stdafx.h"

#include "ShadowFSE.h"

#include "Engine/Graphics/Effects/Fullscreen/Impl/SimpleFullscreenEffect.h"
#include "Engine/Graphics/Effects/Fullscreen/Impl/CompositeFullscreenEffect.h"
#include "Engine/Graphics/Effects/Fullscreen/Impl/Graph/FullscreenEffectGraph.h"

#include "Engine/Types/Values/ValuesFactory.h"

#include "Engine/Graphics/Effects/Utils/ShaderSourceProvider.h"

#include "BlurFSE.h"

namespace bv
{

// **************************
//
FullscreenEffect *       ShadowFSECreator::CreateShadowFSE         ( const std::vector< IValuePtr > & values )
{
    auto blurSize = ValuesFactory::CreateValueInt( "blurSize", 0 );

    std::vector< IValuePtr > blurValues;
    blurValues.insert( blurValues.begin(), values.begin(), values.end() );
    blurValues.push_back( blurSize );
    
    assert( blurValues.size() == 0 );

    auto blurEffect = BlurFSECreator::CreateCompositeFSE( blurValues );

    auto colorVal = ValuesFactory::CreateValueVec4( "color" );
	auto shiftVal = ValuesFactory::CreateValueVec2( "shift" );
    auto innerVal = ValuesFactory::CreateValueInt( "inner" );

    FullscreenEffectData fseData;

    fseData.AppendInputTexture( nullptr, "Tex0" );
    fseData.AppendInputTexture( nullptr, "BluredTex0" );

    fseData.AppendValues( values );
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

    std::vector< FullscreenEffectGraphNodePtr > predecessors;

    auto blurEffectNode = std::shared_ptr< FullscreenEffectGraphNode >( new FullscreenEffectGraphNode( blurEffect ) );

    graph->InsertNode( blurEffectNode, predecessors );

    predecessors.push_back( blurEffectNode );

    auto shadowEffectNode = std::shared_ptr< FullscreenEffectGraphNode >( new FullscreenEffectGraphNode( shadowEffect ) );

    graph->InsertNode( shadowEffectNode, predecessors );

    graph->MarkSourceNode( blurEffectNode );
    graph->MarkSourceNode( shadowEffectNode );
    graph->SetSinkNode( shadowEffectNode );

    return new CompositeFullscreenEffect( graph );
}

} // bv