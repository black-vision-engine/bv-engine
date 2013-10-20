#include "MockScenes.h"

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Plugins/GeometryPluginRect.h"
#include "Engine/Models/Plugins/Channels/Transform/SimpleTransformChannel.h"
#include "Engine/Models/Plugins/SolidColorPlugin.h"
#include "Engine/Models/Plugins/Channels/Geometry/Simple/RectComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/Simple/AnimatedStripComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/GeometryChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Interfaces/IVertexAttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/GeometryChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/SolidColorShaderChannel.h"
#include "Engine/Models/Plugins/SimpleTexturePlugin.h"

namespace bv
{

namespace
{
model::BasicNode *          GreenRect()
{
    model::BasicNode * root = new model::BasicNode();

    ///////////////////////////// Geometry plugin //////////////////////////
    FloatInterpolator w; w.setWrapPostMethod( bv::WrapMethod::pingPong );
    FloatInterpolator h; h.setWrapPostMethod( bv::WrapMethod::pingPong );
    
    w.addKey(0.f, 1.f);
    h.addKey(0.f, 1.f);

    model::GeometryRectPlugin*          rectPlugin = new model::GeometryRectPlugin(w, h);

    /// Set Geometry Channel

    model::AnimatedStripComponent *     rect        = model::AnimatedStripComponent::Create( 2.f, 1.f, 10, 0.f );

    model::GeometryChannelDescriptor desc;

    for( auto compDesc : rect->GetVertexAttributeChannels() )
    {
        desc.AddVertexAttrChannelDesc( static_cast< const model::VertexAttributeChannelDescriptor * >( compDesc->GetDescriptor() ) );
    }

    model::GeometryChannel *    geomCh      = new model::GeometryChannel( PrimitiveType::PT_TRIANGLE_STRIP, desc );
    geomCh->AddConnectedComponent( rect );
    rectPlugin->SetGeometryChannel( geomCh );


    /// Set Transform Channel
    TransformF *    trans  = new TransformF                ();

    FloatInterpolator xs; xs.setWrapPostMethod( bv::WrapMethod::pingPong );
    FloatInterpolator ys; ys.setWrapPostMethod( bv::WrapMethod::pingPong );
    FloatInterpolator zs;

    xs.addKey(0.f, 1.f);
    ys.addKey(0.f, 1.f);
    zs.addKey(0.f, 1.f);

    trans->addScale( xs, ys, zs );

    FloatInterpolator xt; xt.setWrapPostMethod( bv::WrapMethod::pingPong );
    FloatInterpolator yt; yt.setWrapPostMethod( bv::WrapMethod::repeat );
    FloatInterpolator zt;

    xt.addKey(0.f, 0.f);
    yt.addKey(0.f, 0.f);
    zt.addKey(0.f, 0.f);

    trans->addTranslation( xt, yt, zt );

    model::SimpleTransformChannel*      trasformChannel  = new model::SimpleTransformChannel();
    trasformChannel->AddTransform( trans );


    Vec4Interpolator color; color.setWrapPostMethod( bv::WrapMethod::pingPong );
    color.addKey(0.f, glm::vec4( 0.f, 1.f, 0.f, 1.f ) );

    rectPlugin->SetTransformChannel      ( trasformChannel );
    root->AddPlugin(rectPlugin);

    ///////////////////////////// Solid plugin //////////////////////////// 

    auto solidPlugin = new model::SolidColorPlugin( rectPlugin );

    // Set Pixel Shader Channel
    solidPlugin->SetPixelShaderChannel    ( new model::SolidColorShaderChannel( "../dep/media/shaders/solid.frag", color ) );

    root->AddPlugin(solidPlugin);

    return root;
}

model::BasicNode *          TexturedRect()
{
    model::BasicNode * root = new model::BasicNode();

    ///////////////////////////// Geometry plugin //////////////////////////
    FloatInterpolator w; w.setWrapPostMethod( bv::WrapMethod::pingPong );
    FloatInterpolator h; h.setWrapPostMethod( bv::WrapMethod::pingPong );
    
    w.addKey(0.f, 1.f);
    h.addKey(0.f, 1.f);

    model::GeometryRectPlugin*          rectPlugin = new model::GeometryRectPlugin(w, h);

    /// Set Geometry Channel

    model::RectComponent *     rect        = model::RectComponent::Create();

    model::GeometryChannelDescriptor desc;

    for( auto compDesc : rect->GetVertexAttributeChannels() )
    {
        desc.AddVertexAttrChannelDesc( static_cast< const model::VertexAttributeChannelDescriptor * >( compDesc->GetDescriptor() ) );
    }

    model::GeometryChannel *    geomCh      = new model::GeometryChannel( PrimitiveType::PT_TRIANGLE_STRIP, desc );
    geomCh->AddConnectedComponent( rect );
    rectPlugin->SetGeometryChannel( geomCh );


    /// Set Transform Channel
    TransformF *    trans  = new TransformF                ();

    FloatInterpolator xs; xs.setWrapPostMethod( bv::WrapMethod::pingPong );
    FloatInterpolator ys; ys.setWrapPostMethod( bv::WrapMethod::pingPong );
    FloatInterpolator zs;

    xs.addKey(0.f, 1.f);
    ys.addKey(0.f, 1.f);
    zs.addKey(0.f, 1.f);

    trans->addScale( xs, ys, zs );

    FloatInterpolator xt; xt.setWrapPostMethod( bv::WrapMethod::pingPong );
    FloatInterpolator yt; yt.setWrapPostMethod( bv::WrapMethod::repeat );
    FloatInterpolator zt;

    xt.addKey(0.f, 1.f);
    yt.addKey(0.f, 1.f);
    zt.addKey(0.f, 0.f);

    trans->addTranslation( xt, yt, zt );

    model::SimpleTransformChannel*      trasformChannel  = new model::SimpleTransformChannel();
    trasformChannel->AddTransform( trans );


    Vec4Interpolator color; color.setWrapPostMethod( bv::WrapMethod::pingPong );
    color.addKey(0.f, glm::vec4( 0.f, 1.f, 0.f, 1.f ) );

    rectPlugin->SetTransformChannel      ( trasformChannel );
    root->AddPlugin(rectPlugin);

    ///////////////////////////// Texture plugin //////////////////////////// 


    std::vector< std::string > textures;

    textures.push_back( "simless_00.jpg" );

    auto texturePlugin = new model::SimpleTexturePlugin( rectPlugin, textures );

    // Set Pixel Shader Channel
    texturePlugin->SetPixelShaderChannel    ( new model::SolidColorShaderChannel( "../dep/media/shaders/simpletexture.frag", color ) );
    // Set Vertex Shader Channel
    texturePlugin->SetVertexShaderChannel    ( new model::SolidColorShaderChannel( "../dep/media/shaders/simpletexture.vert", color ) );

    root->AddPlugin(texturePlugin);

    return root;

}

} // anonymous



model::BasicNode *          TestScenesFactory::AnotherTestScene()
{
    auto root = GreenRect();
    root->AddChild( TexturedRect() );

    return root;
}

} // bv