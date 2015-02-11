#include "SceneExampleBuilder.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/Effects/DefaultEffect.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"
#include "Engine/Graphics/SceneGraph/TriangleStrip.cpp"

#include "Prototypes/Engine/Common/GeometryBuilder.h"


namespace bv {

// *****************************
//
SceneNode *  SceneExampleBuilder::BuildScene( unsigned int sceneNum )
{
    switch ( sceneNum )
    {
        case 0:
            return Scene0();
            break;
        default:
            return Scene0();
    }
}

// *****************************
//
SceneNode *  SceneExampleBuilder::Scene0    ()
{
    RenderableEffectPtr effect      = CreateRenderableEffect( ShaderDataSourceType::SDST_SOLID_COLOR );
    RenderableEntity *  renderable  = CreateRenderable( effect );
    SceneNode *         node        = new SceneNode( static_cast< TransformableEntity * >( renderable ) );

    // TODO: Add child nodes here if required

    return node;
}

// *****************************
//
RenderableEffectPtr  SceneExampleBuilder::CreateRenderableEffect  ( ShaderDataSourceType sdst )
{
    auto vsds = ShaderDataSourceCreator::VertexShader( sdst );
    auto fsds = ShaderDataSourceCreator::FragmentShader( sdst );

    return std::make_shared<DefaultEffect>( fsds.get(), vsds.get(), nullptr );
}

// *****************************
//
RenderableEntity *   SceneExampleBuilder::CreateRenderable        ( RenderableEffectPtr effect )
{
    RenderableArrayDataArraysSingleVertexBuffer * vaobuf = GeometryBuilder::CreatreRectangle( 1.f, 1.f );

    RenderableEntity * re = new TriangleStrip( vaobuf, effect );

    re->SetWorldTransforms( CreateDefaultTransform() );

    return re;
}

// *****************************
//
TTransformVec        SceneExampleBuilder::CreateDefaultTransform  ()
{
    TTransformVec   transforms;

    transforms.push_back( Transform() );

    return transforms;
}

} // bv
