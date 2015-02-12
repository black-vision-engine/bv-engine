#include "SimpleNodeBuilder.h"

#include "Engine/Graphics/SceneGraph/TriangleStrip.h"

#include "Prototypes/Engine/Common/GeometryBuilder.h"


namespace bv {

// *****************************
//
SceneNode *         SimpleNodeBuilder::CreateRectNodeSolidColor( float w, float h )
{
    RenderableEffectPtr effect      = CreateRenderableEffect( ShaderDataSourceType::SDST_SOLID_COLOR );
    auto vaobuf                     = GeometryBuilder::CreatreRectangle( w, h, false );
    RenderableEntity *  renderable  = CreateRenderableTriStrip( vaobuf, effect );
    SceneNode *         node        = new SceneNode( static_cast< TransformableEntity * >( renderable ) );

    return node;
}

// *****************************
//
SceneNode *         SimpleNodeBuilder:: CreateRectNodeTexture   ( float w, float h )
{
    RenderableEffectPtr effect      = CreateRenderableEffect( ShaderDataSourceType::SDST_ONE_TEXTURE );
    auto vaobuf                     = GeometryBuilder::CreatreRectangle( w, h, true );
    RenderableEntity *  renderable  = CreateRenderableTriStrip( vaobuf, effect );
    SceneNode *         node        = new SceneNode( static_cast< TransformableEntity * >( renderable ) );

    return node;
}

// *****************************
//
RenderableEffectPtr  SimpleNodeBuilder::CreateRenderableEffect  ( ShaderDataSourceType sdst )
{
    auto vsds = ShaderDataSourceCreator::VertexShader( sdst );
    auto fsds = ShaderDataSourceCreator::FragmentShader( sdst );

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
