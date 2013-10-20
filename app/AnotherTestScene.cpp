#include "MockScenes.h"

#include "Engine/Models/BasicNode.h"

#include "Engine/Models/Plugins/GeometryPlugin.h"
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
#include "Engine/Models/Plugins/Channels/PixelShader/TexturePixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/VertexShader/TextureVertexShaderChannel.h"

#include "Engine\Models\Plugins\Channels\Geometry\Simple\AnimatedStripComponent.h"
#include "Engine\Models\Plugins\Channels\Geometry\Simple\GeometryChannelAnimatedVertices.h"

namespace bv
{

namespace
{
// ******************************
//
model::BasicNode *          AnimatedSolid ( float w, float h, float z, unsigned int numSegments, float speedX, float speedY, float cyclesX, float cyclesY, float sizeY, float sizeZ )
{
    model::BasicNode * root = new model::BasicNode();
    
    ///////////////////////////// Geometry plugin //////////////////////////
    model::GeometryPlugin *     geomPlugin  = new model::GeometryPlugin();

    ///////////////////////////// Channels //////////////////////////
    model::GeometryChannel *        geomChannel     = model::GeometryChannelAnimatedVertices::Create( w, h, z, numSegments, speedX, speedY, cyclesX, cyclesY, sizeY, sizeZ );

    TransformF *                    trans           = new TransformF();
    model::SimpleTransformChannel * trasformChannel = new model::SimpleTransformChannel();
    trasformChannel->AddTransform( trans );

    geomPlugin->SetGeometryChannel  ( geomChannel );
    geomPlugin->SetTransformChannel ( trasformChannel );
    
    
    ///////////////////////////// Solid plugin //////////////////////////// 
    auto solidPlugin = new model::SolidColorPlugin( geomPlugin );

    Vec4Interpolator color; color.setWrapPostMethod( bv::WrapMethod::pingPong );
    color.addKey(0.f, glm::vec4( 1.f, 1.f, 0.f, 1.f ) );

    // Set Pixel Shader Channel
    solidPlugin->SetPixelShaderChannel( new model::SolidColorShaderChannel( "../dep/media/shaders/solid.frag", color ) );


    // Add plugins to node
    root->AddPlugin( geomPlugin );
    root->AddPlugin( solidPlugin );

    return root;
}

// ******************************
//
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

    model::AnimatedStripComponent *     rect        = model::AnimatedStripComponent::Create( 2.f, 1.f, 10, 0.f, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 );

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

// ******************************
//
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
    std::vector<TransformF> txMat;
    std::vector<FloatInterpolator> alphas;
    texturePlugin->SetPixelShaderChannel( new model::TexturePixelShaderChannel( "../dep/media/shaders/simpletexture.frag"
                                        , alphas
                                        , txMat )
                                        );

    texturePlugin->SetVertexShaderChannel( new model::TextureVertexShaderChannel( "../dep/media/shaders/simpletexture.vert" )
                                        );

    root->AddPlugin(texturePlugin);

    return root;

}

} // anonymous



// ******************************
//
model::BasicNode *          TestScenesFactory::AnotherTestScene()
{
    auto root = GreenRect();
    root->AddChild( TexturedRect() );

    return root;
}

} // bv