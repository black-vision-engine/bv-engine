#include "MockScenes.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <wchar.h>

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Plugins/GeometryPluginRing.h"

#include "Engine/Models/Plugins/GeometryPlugin.h"
#include "Engine/Models/Plugins/GeometryPluginRect.h"
#include "Engine/Models/Plugins/Channels/Transform/SimpleTransformChannel.h"
#include "Engine/Models/Plugins/SolidColorPlugin.h"
#include "Engine/Models/Plugins/Channels/Geometry/Simple/RectComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/Simple/AnimatedStripComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/Simple/RingComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/GeometryChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Interfaces/IVertexAttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/GeometryChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/SolidColorShaderChannel.h"
#include "Engine/Models/Plugins/SimpleTexturePlugin.h"
#include "Engine/Models/Plugins/SimpleAnimationPlugin.h"
#include "Engine/Models/Plugins/Channels/PixelShader/TexturePixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/VertexShader/TextureVertexShaderChannel.h"
#include "Engine/Models/Plugins/SimpleTextPlugin.h"
#include "Engine/Models/Plugins/Channels/GeometryShader/ExtrudeGeometryShaderChannel.h"
#include "Engine/Models/Plugins/SimpleTransformPlugin.h"
#include "Engine/Models/Plugins/Channels/Geometry/Simple/AnimatedStripComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/Simple/GeometryChannelAnimatedVertices.h"
#include "Engine/Models/Plugins/Channels/Geometry/Simple/VariableTopologyStripComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/Simple/GeometryChannelVariableTopology.h"
#include "Engine/Models/Resources/TextHelpers.h"

#include "Engine/Models/Plugins/PluginsFactory.h"

#define _USE_MATH_DEFINES
#include <math.h>


namespace bv
{

namespace
{

struct AnimationSequenceDesc
{
    unsigned int fps;
    unsigned int numFrames;

    std::string path;
    std::string baseName;
    std::string ext;

};

using namespace model;

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

    auto solidPlugin = PluginsFactory::CreateSolidColorPlugin( geomPlugin, glm::vec4( 1.f, 1.f, 0.f, 1.f ) );

    ////// Add plugins to node
    root->AddPlugin( geomPlugin );
    root->AddPlugin( solidPlugin );

    return root;


    ///////////////////////////// Texture plugin //////////////////////////// 
    std::vector< std::string > textures;
    textures.push_back( "simless_00.jpg" );

    auto texturePlugin = PluginsFactory::CreateTexturePlugin( geomPlugin, textures );

    root->AddPlugin( texturePlugin );

    return root;
    
}

// ******************************
//
model::BasicNode * VariableTopologySolids( float size, float speed, float oscilationSpeed, int numSegments, int numComponents )
{
    model::BasicNode * root = new model::BasicNode();
    
    ///////////////////////////// Geometry plugin //////////////////////////
    model::GeometryPlugin *     geomPlugin  = new model::GeometryPlugin();

    ///////////////////////////// Channels //////////////////////////
    model::GeometryChannel *        geomChannel     = model::GeometryChannelVariableTopology::Create( size, speed, oscilationSpeed, numSegments, numComponents );

    TransformF *                    trans           = new TransformF();
    model::SimpleTransformChannel * trasformChannel = new model::SimpleTransformChannel();
    trasformChannel->AddTransform( trans );

    geomPlugin->SetGeometryChannel  ( geomChannel );
    geomPlugin->SetTransformChannel ( trasformChannel );


    ///////////////////////////// Solid plugin //////////////////////////// 

    auto solidPlugin = PluginsFactory::CreateSolidColorPlugin( geomPlugin, glm::vec4( 1.f, 1.f, 0.f, 1.f ) );

    //// Add plugins to node
    root->AddPlugin( geomPlugin );
    root->AddPlugin( solidPlugin );

    return root;
}

// ******************************
//
model::BasicNode * AnimatedSequenceRect( const std::vector< AnimationSequenceDesc > & animations )
{
    unsigned int numAnimations = animations.size();

    assert( numAnimations >= 1 );

    model::BasicNode * root = new model::BasicNode();

    float hds = 1.7778f;

    ///////////////////////////// Geometry plugin //////////////////////////
    model::GeometryRectPlugin * rectPlugin   = PluginsFactory::CreateGeometryRectPlugin( hds, 1.f );

    root->AddPlugin( rectPlugin );

    ///////////////////////////// Transform plugin //////////////////////////// 
    TransformF *    trans  = new TransformF                ();

    float scl = 1.95f / float(numAnimations);
    FloatInterpolator sx; sx.setWrapPostMethod(bv::WrapMethod::pingPong);
    FloatInterpolator sy; sy.setWrapPostMethod(bv::WrapMethod::pingPong);

    //sx.addKey( 0.f, scl * 1.777777778f );
    //sy.addKey( 0.f, scl * 1.777777778f );
    sx.addKey( 0.f, 1.2f );
    sy.addKey( 0.f, 1.2f );

    trans->addScale( sx, sy, PluginsFactory::CreateConstValueFloat( 1.f ) );
    trans->addTranslation( PluginsFactory::CreateConstValueFloat( -float(numAnimations - 1) * hds / 2.f ), PluginsFactory::CreateConstValueFloat( 0.f ), PluginsFactory::CreateConstValueFloat( 0.f ) );

    auto transformPlugin = PluginsFactory::CreateTransformPlugin( rectPlugin, trans );

    root->AddPlugin( transformPlugin ); // Plugin with transformation
    
    ///////////////////////////// Material plugin //////////////////////////// 

    unsigned int numFrames = animations[ 0 ].numFrames;
    unsigned int animationFPS = animations[ 0 ].fps;
    const std::string & path = animations[ 0 ].path;
    const std::string & baseName = animations[ 0 ].baseName;
    const std::string & ext = animations[ 0 ].ext;

    std::vector< std::string > textures;

    for( unsigned int i = 0; i < numFrames; ++i )
    {
        char buffer[10];
        sprintf_s( buffer, 10, "%02d", i );
        
        std::string txName( path + baseName + std::string( buffer ) + "." + ext );        
        textures.push_back( txName );
    }

    auto animationPlugin = PluginsFactory::CreateAnimationPlugin( transformPlugin, textures, animationFPS );

    root->AddPlugin( animationPlugin );

    for( unsigned int i = 1; i < numAnimations; ++i )
    {
        model::BasicNode * root1 = new model::BasicNode();

        ///////////////////////////// Geometry plugin //////////////////////////
        model::GeometryRectPlugin * rectPlugin   = PluginsFactory::CreateGeometryRectPlugin( 1.77778f * 1.f, 1.f );

        root1->AddPlugin( rectPlugin );

        ///////////////////////////// Transform plugin //////////////////////////// 
        TransformF *    trans  = new TransformF                ();

        FloatInterpolator sx; sx.setWrapPostMethod(bv::WrapMethod::pingPong);
        FloatInterpolator sy; sy.setWrapPostMethod(bv::WrapMethod::pingPong);

        sx.addKey( 0.f, 1.f );
        sy.addKey( 0.f, 1.f );

        trans->addScale( sx, sy, PluginsFactory::CreateConstValueFloat( 1.f ) );
        trans->addTranslation( PluginsFactory::CreateConstValueFloat( float(i) * hds * 1.02f ), PluginsFactory::CreateConstValueFloat( 0.f ), PluginsFactory::CreateConstValueFloat( 0.1f ) );

        auto transformPlugin = PluginsFactory::CreateTransformPlugin( rectPlugin, trans );

        root1->AddPlugin( transformPlugin ); // Plugin with transformation
    
        ///////////////////////////// Material plugin //////////////////////////// 

        unsigned int numFrames = animations[ i ].numFrames;
        unsigned int animationFPS = animations[ i ].fps;
        const std::string & path = animations[ i ].path;
        const std::string & baseName = animations[ i ].baseName;
        const std::string & ext = animations[ i ].ext;

        std::vector< std::string > textures;

        for( unsigned int i = 0; i < numFrames; ++i )
        {
            char buffer[40];
            sprintf_s( buffer, 40, "%02d", i );
        
            std::string txName( path + baseName + std::string( buffer ) + "." + ext );        
            textures.push_back( txName );
        }

        auto animationPlugin = PluginsFactory::CreateAnimationPlugin( transformPlugin, textures, animationFPS );

        root1->AddPlugin( animationPlugin );

        root->AddChild( root1 );
    }

    return root;
}

// ******************************
//
model::BasicNode *          GreenRect()
{
    model::BasicNode * root = new model::BasicNode();

    ///////////////////////////// Geometry plugin //////////////////////////
    model::GeometryRectPlugin*          rectPlugin  = PluginsFactory::CreateGeometryRectPlugin(1.f, 1.f);

    root->AddPlugin(rectPlugin); 

    ///////////////////////////// Transform plugin //////////////////////////// 
    TransformF *    trans  = new TransformF                ();

    trans->addScale( PluginsFactory::CreateConstValueFloat( 1.f ), PluginsFactory::CreateConstValueFloat( 1.f ), PluginsFactory::CreateConstValueFloat( 1.f ) );
    trans->addTranslation( PluginsFactory::CreateConstValueFloat( 0.f ), PluginsFactory::CreateConstValueFloat( 0.f ), PluginsFactory::CreateConstValueFloat( 0.f ) );
    
    auto transformPlugin = PluginsFactory::CreateTransformPlugin( rectPlugin, trans );

    root->AddPlugin( transformPlugin ); // Plugin with transformation

    ///////////////////////////// Material plugin //////////////////////////// 

    auto colorPlugin = PluginsFactory::CreateSimpleColorPlugin( transformPlugin, PluginsFactory::CreateConstValueVec4( glm::vec4(0.f, 1.f, 0.f, 1.f ) ) );

    root->AddPlugin( colorPlugin );

    auto pixelShaderPlugin = PluginsFactory::CreateSimplePixelShaderPlugin( colorPlugin,  "../dep/media/shaders/solid.frag", PluginsFactory::CreateDefaultRenderableContext() );

    root->AddPlugin( pixelShaderPlugin );

    return root;
}

// ******************************
//
model::BasicNode *          TexturedRect()
{
    model::BasicNode * root = new model::BasicNode();

    ///////////////////////////// Geometry plugin //////////////////////////
    model::GeometryRectPlugin* rectPlugin   = PluginsFactory::CreateGeometryRectPlugin(1.f, 1.f);

    root->AddPlugin(rectPlugin);

    ///////////////////////////// Transform plugin //////////////////////////// 
    TransformF *    trans  = new TransformF                ();

    trans->addScale( PluginsFactory::CreateConstValueFloat( 1.f ), PluginsFactory::CreateConstValueFloat( 1.f ), PluginsFactory::CreateConstValueFloat( 1.f ) );
    trans->addTranslation( PluginsFactory::CreateConstValueFloat( 1.f ), PluginsFactory::CreateConstValueFloat( 1.f ), PluginsFactory::CreateConstValueFloat( 0.f ) );

    auto transformPlugin = PluginsFactory::CreateTransformPlugin( rectPlugin, trans );

    root->AddPlugin( transformPlugin ); // Plugin with transformation
    
    ///////////////////////////// Material plugin //////////////////////////// 

    std::vector< std::string > textures;

    textures.push_back( "simless_00.jpg" );

    auto texturePlugin = PluginsFactory::CreateTexturePlugin( transformPlugin, textures );

    root->AddPlugin(texturePlugin);

    return root;

}


// ******************************
//
model::BasicNode *          TexturedRing()
{
    model::BasicNode * root = new model::BasicNode();

    ///////////////////////////// Geometry plugin //////////////////////////

    model::GeometryRingPlugin*          ringPlugin = PluginsFactory::CreateGeometryRingPlugin( 0.f, 1.5f * 3.14f, 1.f, 2.f, 200 );

    root->AddPlugin(ringPlugin);

    ///////////////////////////// Transform plugin //////////////////////////// 

    TransformF *    trans  = new TransformF                ();

    trans->addScale( PluginsFactory::CreateConstValueFloat( 0.25f ), PluginsFactory::CreateConstValueFloat( 0.25f ), PluginsFactory::CreateConstValueFloat( 1.f ) );
    trans->addTranslation( PluginsFactory::CreateConstValueFloat( -1.f ), PluginsFactory::CreateConstValueFloat( -1.f ), PluginsFactory::CreateConstValueFloat( 0.001f ) );

    auto transformPlugin = PluginsFactory::CreateTransformPlugin( ringPlugin, trans );
    
    root->AddPlugin( transformPlugin );

    ///////////////////////////// Material plugin //////////////////////////// 

    std::vector< std::string > textures;

    textures.push_back( "simless_01.jpg" );

    auto texturePlugin = PluginsFactory::CreateTexturePlugin( transformPlugin, textures );

    root->AddPlugin(texturePlugin);

    return root;
}

// ******************************
//
model::BasicNode *     Text1()
{
    model::BasicNode * root = new model::BasicNode();

    std::wstring str    =   TextHelper::LoadUtf8FileToString( L"text_example.txt");

    Vec4Interpolator color; color.setWrapPostMethod( bv::WrapMethod::pingPong );

    color.addKey(0.f, glm::vec4( 1.f, 0.f, 0.f, 1.f ) );
    color.addKey(3.f, glm::vec4( 0.f, 1.f, 0.f, 1.f ) );
    color.addKey(5.f, glm::vec4( 0.f, 0.f, 1.f, 1.f ) );
    color.addKey(7.f, glm::vec4( 1.f, 1.f, 1.f, 1.f ) );

    auto texPlugin      =   PluginsFactory::CreateTextPlugin( str, "../dep/Media/fonts/ARIALUNI.TTF", 64 );

    root->AddPlugin( texPlugin );

    TransformF *    trns  = new TransformF                ();

    FloatInterpolator xt; xt.setWrapPostMethod( bv::WrapMethod::pingPong );
    FloatInterpolator yt; yt.setWrapPostMethod( bv::WrapMethod::repeat );
    FloatInterpolator zt;

    xt.addKey(0.f, -1.f);
    yt.addKey(0.f, -5.f);
    zt.addKey(0.f, -5.f);

    yt.addKey(30.f, 5.f);

    trns->addTranslation( xt, yt, zt );

    trns->addScale( PluginsFactory::CreateConstValueFloat( 1.f ), PluginsFactory::CreateConstValueFloat( 1.f ), PluginsFactory::CreateConstValueFloat( 1.f ) );

    auto transPlugin = PluginsFactory::CreateTransformPlugin( texPlugin, trns );

    root->AddPlugin( transPlugin );

    auto vertexShaderPlugin = PluginsFactory::CreateSimpleVertexShaderPlugin( transPlugin,  "../dep/media/shaders/simpletexture.vert" );

    root->AddPlugin( vertexShaderPlugin );

    auto colorPlugin = PluginsFactory::CreateSimpleColorPlugin( vertexShaderPlugin, color );

    root->AddPlugin( colorPlugin );

    auto ctx = PluginsFactory::CreateDefaultRenderableContext();
    ctx->alphaCtx->blendEnabled = true;
    ctx->depthCtx->enabled = false;

    auto pixelShaderPlugin = PluginsFactory::CreateSimplePixelShaderPlugin( colorPlugin,  "../dep/media/shaders/text.frag", ctx );

    root->AddPlugin( pixelShaderPlugin );

    return root;
}

// ******************************
//
model::BasicNode * Text1Textured()
{
    model::BasicNode * root = new model::BasicNode();

    std::wstring str    =   TextHelper::LoadUtf8FileToString( L"text_example1.txt");

    auto texPlugin      =   PluginsFactory::CreateTextPlugin( str, "../dep/Media/fonts/cour.ttf", 8 );

    root->AddPlugin( texPlugin );

    TransformF *    trns  = new TransformF                ();

    FloatInterpolator xt; xt.setWrapPostMethod( bv::WrapMethod::pingPong );
    FloatInterpolator yt; yt.setWrapPostMethod( bv::WrapMethod::repeat );
    FloatInterpolator zt;

    xt.addKey(0.f, -1.f);
    yt.addKey(0.f, -5.f);
    zt.addKey(0.f, -2.f);

    yt.addKey(30.f, 5.f);

    trns->addTranslation( xt, yt, zt );

    trns->addScale( PluginsFactory::CreateConstValueFloat( 3.f ), PluginsFactory::CreateConstValueFloat( 3.f ), PluginsFactory::CreateConstValueFloat( 1.f ) );

    auto transPlugin = PluginsFactory::CreateTransformPlugin( texPlugin, trns );

    root->AddPlugin( transPlugin );

    std::vector< std::string > textures;
    textures.push_back( "simless_01.jpg" );

    auto texturePlugin = PluginsFactory::CreateTexturePlugin( transPlugin, textures );

    root->AddPlugin( texturePlugin );

    auto vertexShaderPlugin = PluginsFactory::CreateSimpleVertexShaderPlugin( texturePlugin,  "../dep/media/shaders/textured_text.vert" );

    root->AddPlugin( vertexShaderPlugin );

    auto ctx = PluginsFactory::CreateDefaultRenderableContext();
    ctx->alphaCtx->blendEnabled = true;
    ctx->depthCtx->enabled = false;

    auto pixelShaderPlugin = PluginsFactory::CreateSimplePixelShaderPlugin( vertexShaderPlugin,  "../dep/media/shaders/textured_text.frag", ctx );

    root->AddPlugin( pixelShaderPlugin );

    return root;
}

// ******************************
//
model::BasicNode *     Text2()
{
    model::BasicNode * root = new model::BasicNode();

    std::wstring str  = TextHelper::LoadUtf8FileToString( L"text_example.txt");

    Vec4Interpolator color; color.setWrapPostMethod( bv::WrapMethod::pingPong );

    color.addKey(0.f, glm::vec4( 1.f, 0.f, 0.f, 1.f ) );
    color.addKey(1.f, glm::vec4( 0.f, 1.f, 0.f, 1.f ) );
    color.addKey(8.f, glm::vec4( 0.f, 0.f, 1.f, 1.f ) );
    color.addKey(18.f, glm::vec4( 1.f, 1.f, 1.f, 1.f ) );

    auto texPlugin = PluginsFactory::CreateTextPlugin( str, "../dep/Media/fonts/cour.ttf", 128, true, true);

    root->AddPlugin( texPlugin );

    TransformF *    trns  = new TransformF                ();

    trns->addScale( PluginsFactory::CreateConstValueFloat( 2.f ), PluginsFactory::CreateConstValueFloat( 2.f ), PluginsFactory::CreateConstValueFloat( 2.f ) );

    FloatInterpolator xt; xt.setWrapPostMethod( bv::WrapMethod::pingPong );
    FloatInterpolator yt; yt.setWrapPostMethod( bv::WrapMethod::repeat );
    FloatInterpolator zt;

    xt.addKey(0.f, -1.f);
    yt.addKey(0.f, -5.f);
    zt.addKey(0.f, -0.5f);

    yt.addKey(30.f, 5.f);

    trns->addTranslation( xt, yt, zt );

    auto transPlugin = PluginsFactory::CreateTransformPlugin( texPlugin, trns );

    root->AddPlugin( transPlugin );

    auto vertexShaderPlugin = PluginsFactory::CreateSimpleVertexShaderPlugin( transPlugin,  "../dep/media/shaders/simpletexture.vert" );

    root->AddPlugin( vertexShaderPlugin );

    auto colorPlugin = PluginsFactory::CreateSimpleColorPlugin( vertexShaderPlugin, color );

    root->AddPlugin( colorPlugin );

    auto ctx = PluginsFactory::CreateDefaultRenderableContext();
    ctx->alphaCtx->blendEnabled = true;
    ctx->depthCtx->enabled = false;

    auto pixelShaderPlugin = PluginsFactory::CreateSimplePixelShaderPlugin( colorPlugin,  "../dep/media/shaders/text.frag", ctx );

    root->AddPlugin( pixelShaderPlugin );

    return root;
}

// ******************************
//
model::BasicNode *          ExtrudedRedRect()
{
    model::BasicNode * root = new model::BasicNode();

    model::GeometryRectPlugin*          rectPlugin  = PluginsFactory::CreateGeometryRectPlugin(1.f, 1.f);

    model::AnimatedStripComponent *     rect        = model::AnimatedStripComponent::Create( 2.f, 1.f, 10, 0.f, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 );

    model::GeometryChannel *            geomCh      = PluginsFactory::CreateGeometryChannel( rect );

    rectPlugin->SetGeometryChannel( geomCh );


    /// Set Transform Channel
    TransformF *    trans  = new TransformF                ();

    trans->addScale( PluginsFactory::CreateConstValueFloat( 1.f ), PluginsFactory::CreateConstValueFloat( 1.f ), PluginsFactory::CreateConstValueFloat( 1.f ) );
    trans->addTranslation( PluginsFactory::CreateConstValueFloat( -2.f ), PluginsFactory::CreateConstValueFloat( 1.5f ), PluginsFactory::CreateConstValueFloat( 0.f ) );

    auto trasformChannel  = PluginsFactory::CreateTransformChannel( trans );

    rectPlugin->SetTransformChannel( trasformChannel );

    root->AddPlugin(rectPlugin);

    ///////////////////////////// Solid plugin //////////////////////////// 

    auto solidPlugin = PluginsFactory::CreateSolidColorPlugin( rectPlugin, glm::vec4( 1.f, 0.f, 0.f, 1.f ) );

    solidPlugin->SetGeometryShaderChannel ( PluginsFactory::CreateGeometryShaderExtrude( 1.f ) );

    root->AddPlugin(solidPlugin);

    return root;
}

// ******************************
//
model::BasicNode *          ExtrudedTexturedRing()
{
    model::BasicNode * root = new model::BasicNode();

    model::GeometryRingPlugin*          ringPlugin = new model::GeometryRingPlugin();

    /// Set Geometry Channel

    model::RingComponent *      ring        = model::RingComponent::Create(0.f, 1.5f * 3.14f, 1.f, 2.f, 200 );

    model::GeometryChannel *    geomCh      = PluginsFactory::CreateGeometryChannel( ring );
    ringPlugin->SetGeometryChannel( geomCh );


    /// Set Transform Channel
    TransformF *    trans  = new TransformF                ();

    trans->addScale( PluginsFactory::CreateConstValueFloat( 0.25f ), PluginsFactory::CreateConstValueFloat( 0.25f ), PluginsFactory::CreateConstValueFloat( 1.f ) );
    trans->addTranslation( PluginsFactory::CreateConstValueFloat( -1.f ), PluginsFactory::CreateConstValueFloat( -1.f ), PluginsFactory::CreateConstValueFloat( 0.f ) );

    auto trasformChannel  = PluginsFactory::CreateTransformChannel( trans );

    ringPlugin->SetTransformChannel      ( trasformChannel );

    root->AddPlugin(ringPlugin);

    std::vector< std::string > textures;
    textures.push_back( "simless_01.jpg" );

    auto texturePlugin  = PluginsFactory::CreateTexturePlugin( ringPlugin, textures );

    texturePlugin->SetGeometryShaderChannel ( PluginsFactory::CreateGeometryShaderExtrude( 1.f ) );

    root->AddPlugin(texturePlugin);

    return root;
}

} // anonymous


// ******************************
//
model::BasicNode *          TestScenesFactory::AnotherTestScene()
{
    auto root =  Text1();
    root->AddChild( GreenRect() );
    root->AddChild( TexturedRect() );
    //root->AddChild( ExtrudedTexturedRing() ); // To nie dziala na mojej karcie.
    root->AddChild( TexturedRing() );
    root->AddChild( ExtrudedRedRect() );
    root->AddChild( Text1Textured() );
    root->AddChild( Text2() );

    return root;
}

// ******************************
//
model::BasicNode *          TestScenesFactory::XMLTestScene()
{
    /*auto root =  Text1();
    root->AddChild( GreenRect() );
    root->AddChild( TexturedRect() );
    root->AddChild( ExtrudedTexturedRing() ); // To nie dziala na mojej karcie.
    root->AddChild( TexturedRing() );
    root->AddChild( ExtrudedRedRect() );
    root->AddChild( Text2() );
	*/

	TreeBuilder *XMLTree = new TreeBuilder();

	auto root = XMLTree->BuildTree("e:\\temp\\test2.xml");

    return root;
}



// ******************************
//
model::BasicNode *      TestScenesFactory::AnimatedTestScene ()
{
    float w = 5.f;
    float h = 1.f;
    float z = 0.f;
    
    unsigned int numSegments = 100;

    float speedX    = 5.f;
    float speedY    = 1.f;
    float cyclesX   = 1.5f; //5
    float cyclesY   = 0.7f; //3
    float sizeY     = 1.5f;
    float sizeZ     = 2.f; //0.7

    //float 
    return AnimatedSolid( w, h, z, numSegments, speedX, speedY, cyclesX, cyclesY, sizeY, sizeZ );
}

// ******************************
//
model::BasicNode *      TestScenesFactory::TestSceneVariableTopology   ()
{
    float size              = 1.0f;
    float speed             = 1.5f;
    float oscilationSpeed   = 1.f;
    int numSegments         = 10;
    int numComponents       = 4;

    return VariableTopologySolids( size, speed, oscilationSpeed, numSegments, numComponents );
}

// ******************************
//
model::BasicNode *      TestScenesFactory::SequenceAnimationTestScene  ()
{
    std::vector< AnimationSequenceDesc >    animations;

    AnimationSequenceDesc intro;
    intro.ext = "tga";
    intro.baseName = "Alfai";
    intro.path = "../../media/sequences/FullHD/alfai/";
    intro.fps = 100;
    intro.numFrames = 75;

    AnimationSequenceDesc kolarstwo;
    kolarstwo.ext = "tga";
    kolarstwo.baseName = "Alfai";
    kolarstwo.path = "../../media/sequences/FullHD/alfai/";
    kolarstwo.fps = 10;
    kolarstwo.numFrames = 23;

    animations.push_back( intro );
    animations.push_back( kolarstwo );

    return AnimatedSequenceRect( animations );
}

} // bv
