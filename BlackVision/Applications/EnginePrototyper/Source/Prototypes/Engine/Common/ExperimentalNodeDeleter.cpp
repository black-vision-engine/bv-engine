#include "ExperimentalNodeDeleter.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"
#include "Engine/Graphics/Resources/RenderableArrayData.h"
#include "Engine/Graphics/SceneGraph/TriangleStrip.cpp"
#include "Engine/Graphics/Resources/RenderableArrayDataArrays.h"
#include "Engine/Graphics/Resources/VertexArray.h"
#include "Engine/Graphics/Shaders/Shader.h"


namespace bv
{

// **************************
//
void    ExperimentalDeleteAndCleanup( SceneNode * node, Renderer * renderer )
{
    std::vector< SceneNode * > nodes;
    for( unsigned int i = 0; i < node->NumChildNodes(); ++i )
    {
        nodes.push_back( node->GetChild( i ) );
    }

    while( node->NumChildNodes() > 0 )
    {
        node->DetachChildNode( 0 );
    }

    for( auto node : nodes )
    {
        ExperimentalDeleteAndCleanup( node, renderer );
    }

    ExperimentalDeleteSingleNode( node, renderer );
}

// **************************
//
void    ExperimentalDeleteSingleNode( SceneNode * node, Renderer * renderer )
{
    assert( node->NumChildNodes() == 0 );

    ExperimentalFreePdrResources( static_cast< RenderableEntity * >( node->GetTransformable() ), renderer );

    delete node;
}

// **************************
//
void    ExperimentalFreePdrResources( RenderableEntity * renderable, Renderer * renderer )
{
    assert( renderable->GetType() == RenderableEntity::RenderableType::RT_TRIANGLE_STRIP );

    // FIXME: this suxx as we implictly assume that RenderableArrayDataSingleVertexBuffer is in fact of type RenderableArrayDataArraysSingleVertexBuffer
    auto radasvb = static_cast< RenderableArrayDataArraysSingleVertexBuffer * >( renderable->GetRenderableArrayData() );
    auto vao = radasvb->VAO();

    auto vb = vao->GetVertexBuffer();
    auto vd = vao->GetVertexDescriptor();

    renderer->DeletePDR( vb );
    renderer->DeletePDR( vd );
    renderer->DeletePDR( vao );

    ExperimentalDelOursPdrEffect( renderable->GetRenderableEffect(), renderer );
}

// **************************
//
void    ExperimentalDelOursPdrEffect( RenderableEffectPtr effect, Renderer * renderer )
{
    for( unsigned int i = 0; i < effect->NumPasses(); ++i )
    {
        auto pass = effect->GetPass( i );

        auto ps = pass->GetPixelShader();
        auto vs = pass->GetVertexShader();
        auto gs = pass->GetGeometryShader();

        ExperimentalDeleteShaderPdr( ps, renderer );
        ExperimentalDeleteShaderPdr( vs, renderer );
        ExperimentalDeleteShaderPdr( gs, renderer );

        renderer->DeletePDR( pass );
    }
}

// **************************
//
void    ExperimentalDeleteShaderPdr ( Shader * shader, Renderer * renderer )
{
    if( shader )
    {
        auto params = shader->GetParameters();
        { renderer; }
        for( unsigned int i = 0; i < params->NumTextures(); ++i )
        {
            auto tx = params->GetTexture( i );

            // TODO: implement some smart machinery to track texture resources
            // FIXME: right now it any other node uses the same texture it would have to be reloaded
            renderer->DeletePDR( tx.get() );
        }
    }
}

} // bv
