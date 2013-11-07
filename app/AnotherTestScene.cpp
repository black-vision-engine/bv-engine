#include "MockScenes.h"

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
#include "Engine/Models/Plugins/Channels/PixelShader/TexturePixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/VertexShader/TextureVertexShaderChannel.h"
#include "Engine/Models/Plugins/SimpleTextPlugin.h"
#include "Engine/Models/Plugins/Channels/PixelShader/TextPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/GeometryShader/ExtrudeGeometryShaderChannel.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <wchar.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "Engine\Models\Plugins\Channels\Geometry\Simple\AnimatedStripComponent.h"
#include "Engine\Models\Plugins\Channels\Geometry\Simple\GeometryChannelAnimatedVertices.h"
#include "Engine\Models\Plugins\Channels\Geometry\Simple\VariableTopologyStripComponent.h"
#include "Engine\Models\Plugins\Channels\Geometry\Simple\GeometryChannelVariableTopology.h"

namespace bv
{

namespace
{

// *********************************
//
FloatInterpolator                   CreateConstValueFloat               ( float val )
{
    FloatInterpolator inter; inter.setWrapPostMethod( bv::WrapMethod::pingPong );
    inter.addKey( 0.f, val );

    return inter;
}

// *********************************
//
Vec4Interpolator                   CreateConstValueVec4                 ( const glm::vec4& val )
{
    Vec4Interpolator inter; inter.setWrapPostMethod( bv::WrapMethod::pingPong );
    inter.addKey( 0.f, val );

    return inter;
}

// *********************************
//
model::GeometryRectPlugin*          CreateGeometryRectPlugin            ( float w, float h )
{
    FloatInterpolator wi; wi.setWrapPostMethod( bv::WrapMethod::pingPong );
    FloatInterpolator hi; hi.setWrapPostMethod( bv::WrapMethod::pingPong );
    
    wi.addKey(0.f, w);
    hi.addKey(0.f, h);

    return new model::GeometryRectPlugin(wi, hi);
}

// *********************************
//
model::GeometryChannel*             CreateGeometryChannel               (model::IConnectedComponent* connComp)
{
    model::GeometryChannelDescriptor desc;

    for( auto compDesc : connComp->GetVertexAttributeChannels() )
    {
        desc.AddVertexAttrChannelDesc( static_cast< const model::VertexAttributeChannelDescriptor * >( compDesc->GetDescriptor() ) );
    }

    model::GeometryChannel* ret = new model::GeometryChannel( PrimitiveType::PT_TRIANGLE_STRIP, desc );

    ret->AddConnectedComponent(connComp);

    return ret;
}

// *********************************
//
model::ITransformChannel*           CreateTransformChannel              (TransformF* transformation)
{
    model::SimpleTransformChannel*      trasformChannel  = new model::SimpleTransformChannel();
    trasformChannel->AddTransform( transformation );

    return trasformChannel;
}

// *********************************
//
model::SolidColorPlugin*            CreateSolidColorPlugin              (model::IPlugin* prevPlugin, const glm::vec4& color)
{
    auto solidPlugin = new model::SolidColorPlugin( prevPlugin );

    // Set Pixel Shader Channel
    solidPlugin->SetPixelShaderChannel    ( new model::SolidColorShaderChannel( "../dep/media/shaders/solid.frag", CreateConstValueVec4( color ) ) );

    return solidPlugin;
}

// *********************************
//
model::SimpleTexturePlugin*         CreateTexturePlugin                 ( model::IPlugin* prevPlugin, const std::vector< std::string >& texturesPaths )
{
    auto texturePlugin = new model::SimpleTexturePlugin( prevPlugin, texturesPaths );

    // Set Pixel Shader Channel
    std::vector<TransformF> txMat;
    std::vector<FloatInterpolator> alphas;
    texturePlugin->SetPixelShaderChannel( new model::TexturePixelShaderChannel( "../dep/media/shaders/simpletexture.frag"
                                        , alphas
                                        , txMat )
                                        );

    texturePlugin->SetVertexShaderChannel( new model::TextureVertexShaderChannel( "../dep/media/shaders/simpletexture.vert" )
                                        );

    return texturePlugin;
}

// *********************************
//
model::SimpleTextPlugin*            CreateTextPlugin                    ( const std::wstring& text, const std::string& fontFile, int size, const Vec4Interpolator& color, TransformF* trans )
{
    auto texPlugin = new model::SimpleTextPlugin( text, fontFile, size );

    texPlugin->SetPixelShaderChannel     ( new model::TextPixelShaderChannel( "../dep/media/shaders/text.frag", color ) );
    texPlugin->SetVertexShaderChannel    ( new model::TextureVertexShaderChannel( "../dep/media/shaders/simpletexture.vert" ) );

    model::ITransformChannel      * stch  = CreateTransformChannel( trans );

    texPlugin->SetTransformChannel( stch );

    return texPlugin;
}

// *********************************
//
model::IGeometryShaderChannel*      CreateGeometryShaderExtrude         ( float scale )
{
    FloatInterpolator extrudeScale = CreateConstValueFloat( scale );
    return new model::ExtrudeGeometryShaderChannel("../dep/media/shaders/extrude.geom", extrudeScale);
}

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

    auto solidPlugin = CreateSolidColorPlugin( geomPlugin, glm::vec4( 1.f, 1.f, 0.f, 1.f ) );

    //// Add plugins to node
    root->AddPlugin( geomPlugin );
    root->AddPlugin( solidPlugin );

    return root;


    ///////////////////////////// Texture plugin //////////////////////////// 
    std::vector< std::string > textures;
    textures.push_back( "simless_00.jpg" );

    auto texturePlugin = CreateTexturePlugin( geomPlugin, textures );

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

    auto solidPlugin = CreateSolidColorPlugin( geomPlugin, glm::vec4( 1.f, 1.f, 0.f, 1.f ) );

    //// Add plugins to node
    root->AddPlugin( geomPlugin );
    root->AddPlugin( solidPlugin );

    return root;
}

// ******************************
//
model::BasicNode *          GreenRect()
{
    model::BasicNode * root = new model::BasicNode();

    model::GeometryRectPlugin*          rectPlugin = CreateGeometryRectPlugin(1.f, 1.f);

    /// Set Geometry Channel
    model::AnimatedStripComponent *     rect        = model::AnimatedStripComponent::Create( 2.f, 1.f, 10, 0.f, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 );

    model::GeometryChannel *            geomCh      = CreateGeometryChannel( rect );

    rectPlugin->SetGeometryChannel( geomCh );


    /// Set Transform Channel
    TransformF *    trans  = new TransformF                ();

    trans->addScale( CreateConstValueFloat( 1.f ), CreateConstValueFloat( 1.f ), CreateConstValueFloat( 1.f ) );
    trans->addTranslation( CreateConstValueFloat( 0.f ), CreateConstValueFloat( 0.f ), CreateConstValueFloat( 0.f ) );

    model::ITransformChannel*           trasformChannel  = CreateTransformChannel( trans );

    rectPlugin->SetTransformChannel( trasformChannel );
    
    // Add plugin to node.
    root->AddPlugin(rectPlugin);

    auto solidPlugin = CreateSolidColorPlugin( rectPlugin, glm::vec4( 0.f, 1.f, 0.f, 1.f ) );

    root->AddPlugin(solidPlugin);

    return root;
}

// ******************************
//
model::BasicNode *          TexturedRect()
{
    model::BasicNode * root = new model::BasicNode();

    ///////////////////////////// Geometry plugin //////////////////////////
    model::GeometryRectPlugin* rectPlugin   = CreateGeometryRectPlugin(1.f, 1.f);

    /// Set Geometry Channel

    model::RectComponent *      rect        = model::RectComponent::Create();

    model::GeometryChannel *    geomCh      = CreateGeometryChannel( rect );

    rectPlugin->SetGeometryChannel( geomCh );


    /// Set Transform Channel
    TransformF *    trans  = new TransformF                ();

    trans->addScale( CreateConstValueFloat( 1.f ), CreateConstValueFloat( 1.f ), CreateConstValueFloat( 1.f ) );
    trans->addTranslation( CreateConstValueFloat( 1.f ), CreateConstValueFloat( 1.f ), CreateConstValueFloat( 0.f ) );

    model::ITransformChannel*   trasformChannel =   CreateTransformChannel( trans );

    rectPlugin->SetTransformChannel      ( trasformChannel );
    
    root->AddPlugin(rectPlugin);

    ///////////////////////////// Texture plugin //////////////////////////// 


    std::vector< std::string > textures;

    textures.push_back( "simless_00.jpg" );

    auto texturePlugin = CreateTexturePlugin( rectPlugin, textures );

    root->AddPlugin(texturePlugin);

    return root;

}


// ******************************
//
model::BasicNode *          TexturedRing()
{
    model::BasicNode * root = new model::BasicNode();

    ///////////////////////////// Geometry plugin //////////////////////////

    model::GeometryRingPlugin*          ringPlugin = new model::GeometryRingPlugin();

    /// Set Geometry Channel

    model::RingComponent *      ring        = model::RingComponent::Create(0.f, 1.5f * 3.14f, 1.f, 2.f, 200 );

    model::GeometryChannel *    geomCh      = CreateGeometryChannel( ring );

    ringPlugin->SetGeometryChannel( geomCh );


    /// Set Transform Channel
    TransformF *    trans  = new TransformF                ();

    trans->addScale( CreateConstValueFloat( 0.25f ), CreateConstValueFloat( 0.25f ), CreateConstValueFloat( 1.f ) );
    trans->addTranslation( CreateConstValueFloat( -1.f ), CreateConstValueFloat( -1.f ), CreateConstValueFloat( 0.f ) );

    model::ITransformChannel*   trasformChannel  = CreateTransformChannel( trans );

    ringPlugin->SetTransformChannel      ( trasformChannel );
    root->AddPlugin(ringPlugin);

    ///////////////////////////// Texture plugin //////////////////////////// 


    std::vector< std::string > textures;

    textures.push_back( "simless_01.jpg" );

    auto texturePlugin = CreateTexturePlugin( ringPlugin, textures );

    root->AddPlugin(texturePlugin);

    return root;

}

// ******************************
//
size_t GetSizeOfFile(const std::wstring& path)
{
    struct _stat fileinfo;
    _wstat(path.c_str(), &fileinfo);
    return fileinfo.st_size;
}

// ******************************
//
std::wstring LoadUtf8FileToString(const std::wstring& filename)
{
    std::wstring buffer;            // stores file contents
    FILE* f = _wfopen(filename.c_str(), L"rtS, ccs=UTF-8"); //FIXME: COMPILER WARNING

    // Failed to open file
    if (f == NULL)
    {
        // ...handle some error...
        return buffer;
    }

    size_t filesize = GetSizeOfFile(filename);

    // Read entire file contents in to memory
    if (filesize > 0)
    {
        buffer.resize(filesize);
        size_t wchars_read = fread(&(buffer.front()), sizeof(wchar_t), filesize, f);
        buffer.resize(wchars_read);
        buffer.shrink_to_fit();
    }

    fclose(f);

    return buffer;
}

// ******************************
//
model::BasicNode *     Text1()
{
    model::BasicNode * root = new model::BasicNode();

    std::wstring str    =   LoadUtf8FileToString( L"text_example.txt");

    Vec4Interpolator color; color.setWrapPostMethod( bv::WrapMethod::pingPong );

    color.addKey(0.f, glm::vec4( 1.f, 0.f, 0.f, 1.f ) );
    color.addKey(3.f, glm::vec4( 0.f, 1.f, 0.f, 1.f ) );
    color.addKey(5.f, glm::vec4( 0.f, 0.f, 1.f, 1.f ) );
    color.addKey(7.f, glm::vec4( 1.f, 1.f, 1.f, 1.f ) );

    TransformF *    trns  = new TransformF                ();

    FloatInterpolator xt; xt.setWrapPostMethod( bv::WrapMethod::pingPong );
    FloatInterpolator yt; yt.setWrapPostMethod( bv::WrapMethod::repeat );
    FloatInterpolator zt;

    xt.addKey(0.f, -1.f);
    yt.addKey(0.f, -5.f);
    zt.addKey(0.f, -5.f);

    yt.addKey(30.f, 5.f);

    trns->addTranslation( xt, yt, zt );

    trns->addScale( CreateConstValueFloat( 1.f ), CreateConstValueFloat( 1.f ), CreateConstValueFloat( 1.f ) );

    auto texPlugin      =   CreateTextPlugin( str, "../dep/Media/fonts/ARIALUNI.TTF", 64, color, trns );

    root->AddPlugin( texPlugin );

    return root;
}

// ******************************
//
model::BasicNode *     Text2()
{
    model::BasicNode * root = new model::BasicNode();

    std::wstring str  = LoadUtf8FileToString( L"text_example.txt");

    TransformF *    trns  = new TransformF                ();

    trns->addScale( CreateConstValueFloat( 2.f ), CreateConstValueFloat( 2.f ), CreateConstValueFloat( 2.f ) );

    FloatInterpolator xt; xt.setWrapPostMethod( bv::WrapMethod::pingPong );
    FloatInterpolator yt; yt.setWrapPostMethod( bv::WrapMethod::repeat );
    FloatInterpolator zt;

    xt.addKey(0.f, -1.f);
    yt.addKey(0.f, -5.f);
    zt.addKey(0.f, -0.5f);

    yt.addKey(30.f, 5.f);

    trns->addTranslation( xt, yt, zt );

    Vec4Interpolator color; color.setWrapPostMethod( bv::WrapMethod::pingPong );

    color.addKey(0.f, glm::vec4( 1.f, 0.f, 0.f, 1.f ) );
    color.addKey(1.f, glm::vec4( 0.f, 1.f, 0.f, 1.f ) );
    color.addKey(8.f, glm::vec4( 0.f, 0.f, 1.f, 1.f ) );
    color.addKey(18.f, glm::vec4( 1.f, 1.f, 1.f, 1.f ) );

    auto texPlugin = CreateTextPlugin( str, "../dep/Media/fonts/times.ttf", 128, color, trns );

    root->AddPlugin( texPlugin );

    return root;
}

// ******************************
//
model::BasicNode *          ExtrudedRedRect()
{
    model::BasicNode * root = new model::BasicNode();

    model::GeometryRectPlugin*          rectPlugin  = CreateGeometryRectPlugin(1.f, 1.f);

    model::AnimatedStripComponent *     rect        = model::AnimatedStripComponent::Create( 2.f, 1.f, 10, 0.f, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 );

    model::GeometryChannel *            geomCh      = CreateGeometryChannel( rect );

    rectPlugin->SetGeometryChannel( geomCh );


    /// Set Transform Channel
    TransformF *    trans  = new TransformF                ();

    trans->addScale( CreateConstValueFloat( 1.f ), CreateConstValueFloat( 1.f ), CreateConstValueFloat( 1.f ) );
    trans->addTranslation( CreateConstValueFloat( -2.f ), CreateConstValueFloat( 1.5f ), CreateConstValueFloat( 0.f ) );

    model::ITransformChannel*      trasformChannel  = CreateTransformChannel( trans );

    rectPlugin->SetTransformChannel( trasformChannel );

    root->AddPlugin(rectPlugin);

    ///////////////////////////// Solid plugin //////////////////////////// 

    auto solidPlugin = CreateSolidColorPlugin( rectPlugin, glm::vec4( 1.f, 0.f, 0.f, 1.f ) );

    solidPlugin->SetGeometryShaderChannel ( CreateGeometryShaderExtrude( 1.f ) );

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

    model::GeometryChannel *    geomCh      = CreateGeometryChannel( ring );
    ringPlugin->SetGeometryChannel( geomCh );


    /// Set Transform Channel
    TransformF *    trans  = new TransformF                ();

    trans->addScale( CreateConstValueFloat( 0.25f ), CreateConstValueFloat( 0.25f ), CreateConstValueFloat( 1.f ) );
    trans->addTranslation( CreateConstValueFloat( -1.f ), CreateConstValueFloat( -1.f ), CreateConstValueFloat( 0.f ) );

    auto trasformChannel  = CreateTransformChannel( trans );

    ringPlugin->SetTransformChannel      ( trasformChannel );

    root->AddPlugin(ringPlugin);

    std::vector< std::string > textures;
    textures.push_back( "simless_01.jpg" );

    auto texturePlugin  = CreateTexturePlugin( ringPlugin, textures );

    texturePlugin->SetGeometryShaderChannel ( CreateGeometryShaderExtrude( 1.f ) );

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
    root->AddChild( Text2() );

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
    int numSegments         = 30;
    int numComponents       = 4;

    return VariableTopologySolids( size, speed, oscilationSpeed, numSegments, numComponents );
}

} // bv
