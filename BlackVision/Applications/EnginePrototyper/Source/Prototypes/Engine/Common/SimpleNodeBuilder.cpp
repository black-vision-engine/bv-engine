#include "SimpleNodeBuilder.h"

#include "Engine/Graphics/SceneGraph/TriangleStrip.h"

#include "Prototypes/Engine/Common/GeometryBuilder.h"


namespace bv {

// *****************************
//
SceneNode *         SimpleNodeBuilder::CreateRectNodeSolidColor( float w, float h, float z )
{
    RenderableEffectPtr effect      = CreateRenderableEffect( ShaderDataSourceType::SDST_SOLID_COLOR );
    auto vaobuf                     = GeometryBuilder::CreatreRectangle( w, h, z, false );
    RenderableEntity *  renderable  = CreateRenderableTriStrip( vaobuf, effect );
    SceneNode *         node        = new SceneNode( static_cast< TransformableEntity * >( renderable ) );

    return node;
}

// *****************************
//
SceneNode *         SimpleNodeBuilder:: CreateRectNodeTexture   ( float w, float h, float z )
{
    RenderableEffectPtr effect      = CreateRenderableEffect( ShaderDataSourceType::SDST_ONE_TEXTURE );
    auto vaobuf                     = GeometryBuilder::CreatreRectangle( w, h, z, true );
    RenderableEntity *  renderable  = CreateRenderableTriStrip( vaobuf, effect );
    SceneNode *         node        = new SceneNode( static_cast< TransformableEntity * >( renderable ) );

    return node;
}

// *****************************
//
RenderableEffectPtr  SimpleNodeBuilder::CreateRenderableEffect  ( ShaderDataSourceType sdst )
{
    static std::vector< IShaderDataSourceConstPtr > dummyFucker;

    // FIXME: these pointers get deleted when function leaves this scope so this is bound to fail, these references must be kept somewhere
    auto vsds = ShaderDataSourceCreator::VertexShader( sdst );
    auto fsds = ShaderDataSourceCreator::FragmentShader( sdst );

    dummyFucker.push_back( vsds );
    dummyFucker.push_back( fsds );

    return std::make_shared<DefaultEffect>( fsds.get(), vsds.get(), nullptr );
}

// *****************************
//
RenderableEntity *   SimpleNodeBuilder::CreateRenderableTriStrip( RenderableArrayDataArraysSingleVertexBuffer * vaobuf, RenderableEffectPtr effect )
{
    RenderableEntity * re = new TriangleStrip( vaobuf, effect );

    re->SetWorldTransforms( DefaultTransform() );

    return re;
}

// *****************************
//
TTransformVec        SimpleNodeBuilder::DefaultTransform        ()
{
    TTransformVec   transforms;

    transforms.push_back( Transform() );

    return transforms;
}

} // bv
