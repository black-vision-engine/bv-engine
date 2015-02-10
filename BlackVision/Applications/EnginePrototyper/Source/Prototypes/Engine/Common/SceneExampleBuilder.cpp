#include "SceneExampleBuilder.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/Effects/DefaultEffect.h"

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
    auto effect = CreateRenderableEffect( ShaderDataSourceType::SDST_SOLID_COLOR );

    // CreateStrip
    // CreateRenderable
    // CreateTransform

    //IPluginConstPtr finalizer = GetFinalizePlugin();

    //SceneNode * node = CreateSceneNode( finalizer );
    //    RenderableEntity * renderable = CreateRenderable( finalizer );
    //    SceneNode * node        = new SceneNode( renderable );


    //node->SetOverrideAlphaVal( GetOverrideState()->GetAlphaValue().get() );

    //for( auto ch : m_children )
    //{
    //    node->AddChildNode( ch->BuildScene() );
    //}

    //return node;}

    return nullptr;
}

// *****************************
//
RenderableEffectPtr  SceneExampleBuilder::CreateRenderableEffect  ( ShaderDataSourceType sdst )
{
    auto vsds = ShaderDataSourceCreator::VertexShader( sdst );
    auto fsds = ShaderDataSourceCreator::FragmentShader( sdst );

    return std::make_shared<DefaultEffect>( fsds.get(), vsds.get(), nullptr );
}

#if 0
RenderableEntity *                  BasicNode::CreateRenderable         ( IPluginConstPtr finalizer ) const
{
    RenderableEntity * renderable = nullptr;

    if( finalizer->GetVertexAttributesChannel() )
    {
        auto renderableType = finalizer->GetVertexAttributesChannel()->GetPrimitiveType();

        RenderableEffectPtr effect = CreateDefaultEffect( finalizer );

        //RenderableArrayDataSingleVertexBuffer * rad = CreateRenderableArrayData( renderableType );
        //CreateRenderableData( &vao ); // TODO: Powinno zwracac indeksy albo vao w zaleznosci od rodzaju geometrii
        //effect = ;

        //FIXME: to powinna ogarniac jakas faktoria-manufaktura
        switch( renderableType )
        {
            case PrimitiveType::PT_TRIANGLE_STRIP:
            {
                //FIXME: it should be constructed as a proper type RenderableArrayDataArraysSingleVertexBuffer * in the first place
                //FIXME: this long type name suggests that something wrong is happening here (easier to name design required)
                RenderableArrayDataArraysSingleVertexBuffer * radasvb = CreateRenderableArrayDataTriStrip();

                if( radasvb )
                {
                    renderable = new TriangleStrip( radasvb, effect );
                }
                break;
            }
            case PrimitiveType::PT_TRIANGLES:
            case PrimitiveType::PT_TRIANGLE_MESH:
                assert( false );
            default:
                return nullptr;
        }
    }
    else
    {
        renderable = new TriangleStrip( nullptr, nullptr );
    }

    auto worldTransformVec = CreateTransformVec( finalizer );

    renderable->SetWorldTransforms( worldTransformVec );

    return renderable;
}
#endif

} // bv
