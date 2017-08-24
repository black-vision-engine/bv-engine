#include "SimpleNodeBuilder.h"

#include "Engine/Graphics/SceneGraph/TriangleStrip.h"

#include "Common/GeometryBuilder.h"


namespace bv {

// *****************************
//
SceneNode *         SimpleNodeBuilder::CreateRectNodeSolidColor( float w, float h, float z )
{
    RenderableEffectPtr effect      = CreateRenderableEffect( ShaderDataSourceType::SDST_SOLID_COLOR );
    auto vaobuf                     = GeometryBuilder::CreateRectangle( w, h, z, false );
    RenderableEntity *  renderable  = CreateRenderableTriStrip( vaobuf, effect );
    SceneNode *         node        = new SceneNode( static_cast< TransformableEntity * >( renderable ) );

    return node;
}

// *****************************
//
SceneNode *         SimpleNodeBuilder:: CreateRectNodeTexture   ( float w, float h, float z, const std::string & textureFile )
{
    RenderableEffectPtr effect      = CreateRenderableEffect( ShaderDataSourceType::SDST_ONE_TEXTURE, textureFile, "Tex" );
    auto vaobuf                     = GeometryBuilder::CreateRectangle( w, h, z, true );
    RenderableEntity *  renderable  = CreateRenderableTriStrip( vaobuf, effect );
    SceneNode *         node        = new SceneNode( static_cast< TransformableEntity * >( renderable ) );

    return node;
}

// *****************************
//
RenderableEffectPtr  SimpleNodeBuilder::CreateRenderableEffect  ( ShaderDataSourceType sdst, const std::string & textureFile, const std::string & textureName )
{
    static std::vector< IShaderDataSourceConstPtr > dummyFuckerReferenceKeeper; // FIXME:L keeps references to shader params - this suxx as hell - and must be fixed in the model and engine

    // FIXME: these pointers get deleted when function leaves this scope so this is bound to fail, these references must be kept somewhere
    auto vsds = ShaderDataSourceCreator::VertexShader( sdst );
    auto fsds = ShaderDataSourceCreator::FragmentShader( sdst, textureFile, textureName );

    dummyFuckerReferenceKeeper.push_back( vsds );
    dummyFuckerReferenceKeeper.push_back( fsds );

    return std::make_shared<DefaultEffect>( fsds.get(), vsds.get(), nullptr );
}

// *****************************
//
RenderableEntity *   SimpleNodeBuilder::CreateRenderableTriStrip( RenderableArrayDataArraysSingleVertexBuffer * vaobuf, RenderableEffectPtr effect )
{
    RenderableEntity * re = new TriangleStrip( vaobuf, effect );

    re->SetWorldTransform( DefaultTransform() );

    return re;
}

// *****************************
//
Transform           SimpleNodeBuilder::DefaultTransform        ()
{
    return Transform();
}

} // bv
