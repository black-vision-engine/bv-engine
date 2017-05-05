#include "stdafxBVApp.h"

//#include "MockScenes.h"
//
//#include <vector>
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <stdio.h>
//#include <stddef.h>
//#include <stdlib.h>
//#include <wchar.h>
//
//#include "Engine/Models/BasicNode.h"
//#include "Engine/Models/Plugins/GeometryPluginRing.h"
//
//#include "Engine/Models/Plugins/GeometryPlugin.h"
//#include "Engine/Models/Plugins/GeometryPluginRect.h"
//#include "Engine/Models/Plugins/Channels/Transform/SimpleTransformChannel.h"
//#include "Engine/Models/Plugins/Channels/Geometry/Simple/RectComponent.h"
//#include "Engine/Models/Plugins/Channels/Geometry/Simple/AnimatedStripComponent.h"
//#include "Engine/Models/Plugins/Channels/Geometry/Simple/RingComponent.h"
//#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannelDescriptor.h"
//#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"
//#include "Engine/Models/Plugins/Interfaces/IAttributeChannel.h"
//#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"
//#include "Engine/Models/Plugins/Simple/TextPlugins/DefaultTimerPlugin.h"
//#include "Engine/Models/Plugins/Channels/Geometry/Simple/AnimatedStripComponent.h"
//#include "Engine/Models/Plugins/Channels/Geometry/Simple/VariableTopologyStripComponent.h"
//#include "Engine/Models/Plugins/GeometryMultiRectPlugin.h"
//#include "Engine/Models/Plugins/Channels/ChannelsFactory.h"
//
//#include "Engine/Models/Timeline/TimelineManager.h"
//#include "Engine/Models/Plugins/PluginsFactory.h"
//#include "Assets/AssetDescsWithUIDs.h"
//
//#include "Engine/Models/BVProject.h"
//#include "System/Path.h"
//
//#include "Serialization/BV/XML/BVXMLDeserializer.h"
//#include "Serialization/Json/JsonDeserializeObject.h"
//#include "Serialization/SerializationHelper.h"
//#include "Serialization/BV/CloneViaSerialization.h"
//
//#include "Application/WindowedApplication.h"
//#include "Engine/Graphics/Renderers/Renderer.h"
//
//#define _USE_MATH_DEFINES
//#include <math.h>
//
//
//namespace bv
//{
//
//namespace
//{
//
//struct AnimationSequenceDesc
//{
//    FloatInterpolator frameCounter;
//    unsigned int numFrames;
//
//    std::string path;
//    std::string baseName;
//    std::string ext;
//
//};
//
//using namespace model;
//
////// ******************************
//////
////model::BasicNode * AnimatedSequenceRect( const std::vector< AnimationSequenceDesc > & animations )
////{
////    unsigned int numAnimations = animations.size();
////
////    assert( numAnimations >= 1 );
////
////    model::BasicNode * root = new model::BasicNode( "AnimatedSequenceRoot" );
////
////    float hds = 1.7778f;
////
////    ///////////////////////////// Geometry plugin //////////////////////////
////    model::GeometryRectPlugin * rectPlugin   = PluginsFactory::CreateGeometryRectPlugin( hds, 1.f );
////
////    root->AddPlugin( rectPlugin );
////
////    ///////////////////////////// Transform plugin //////////////////////////// 
////    TransformF trans;
////
////    float scl = 1.95f / float(numAnimations);
////    FloatInterpolator sx; sx.SetWrapPostMethod(bv::WrapMethod::pingPong);
////    FloatInterpolator sy; sy.SetWrapPostMethod(bv::WrapMethod::pingPong);
////
////    //sx.AddKey( 0.f, scl * 1.777777778f );
////    //sy.AddKey( 0.f, scl * 1.777777778f );
////    sx.AddKey( 0.f, 1.2f );
////    sy.AddKey( 0.f, 1.2f );
////
////    trans.AddScale( sx, sy, InterpolatorsHelper::CreateConstValue( 1.f ) );
////    trans.AddTranslation( InterpolatorsHelper::CreateConstValue( -float(numAnimations - 1) * hds / 2.f ), InterpolatorsHelper::CreateConstValue( 0.f ), InterpolatorsHelper::CreateConstValue( 0.f ) );
////
////    auto transformPlugin = PluginsFactory::CreateSimpleTransformPlugin( rectPlugin, model::ParametersFactory::CreateParameter( "transformation", trans, nullptr, 0 ) );
////
////    root->AddPlugin( transformPlugin ); // Plugin with transformation
////    
////    ///////////////////////////// Material plugin //////////////////////////// 
////
////    FloatInterpolator fc = animations[ 0 ].frameCounter;
////    ParamFloat frameCounter = ParametersFactory::CreateParameter( std::string( "animation" ), fc );
////
////    unsigned int numFrames = animations[ 0 ].numFrames;
////    const std::string & path = animations[ 0 ].path;
////    const std::string & baseName = animations[ 0 ].baseName;
////    const std::string & ext = animations[ 0 ].ext;
////
////    std::vector< std::string > textures;
////
////    for( unsigned int i = 0; i < numFrames; ++i )
////    {
////        char buffer[10];
////        sprintf_s( buffer, 10, "%02d", i );
////        
////        std::string txName( path + baseName + std::string( buffer ) + "." + ext );        
////        textures.push_back( txName );
////    }
////
////    //FIXME: renderer context can be specified here
////    auto animationPlugin = PluginsFactory::CreateSimpleAnimationPlugin( transformPlugin, textures, frameCounter );
////
////    root->AddPlugin( animationPlugin );
////
////    for( unsigned int i = 1; i < numAnimations; ++i )
////    {
////        model::BasicNode * root1 = new model::BasicNode( "FIXME:" );
////
////        ///////////////////////////// Geometry plugin //////////////////////////
////        model::GeometryRectPlugin * rectPlugin   = PluginsFactory::CreateGeometryRectPlugin( 1.77778f * 1.f, 1.f );
////
////        root1->AddPlugin( rectPlugin );
////
////        ///////////////////////////// Transform plugin //////////////////////////// 
////        TransformF trans;
////
////        FloatInterpolator sx; sx.SetWrapPostMethod(bv::WrapMethod::pingPong);
////        FloatInterpolator sy; sy.SetWrapPostMethod(bv::WrapMethod::pingPong);
////
////        sx.AddKey( 0.f, 1.f );
////        sy.AddKey( 0.f, 1.f );
////
////        trans.AddScale( sx, sy, InterpolatorsHelper::CreateConstValue( 1.f ) );
////        trans.AddTranslation( InterpolatorsHelper::CreateConstValue( float(i) * hds * 1.02f ), InterpolatorsHelper::CreateConstValue( 0.f ), InterpolatorsHelper::CreateConstValue( 0.1f ) );
////
////        auto transformPlugin = PluginsFactory::CreateSimpleTransformPlugin( rectPlugin, model::ParametersFactory::CreateParameter( "transformation", trans, nullptr, 0 ) );
////
////        root1->AddPlugin( transformPlugin ); // Plugin with transformation
////    
////        ///////////////////////////// Material plugin //////////////////////////// 
////
////        FloatInterpolator fc = animations[ i ].frameCounter;
////        ParamFloat frameCounter = ParametersFactory::CreateParameter( std::string( "animation" ), fc );
////
////        unsigned int numFrames = animations[ i ].numFrames;
////        const std::string & path = animations[ i ].path;
////        const std::string & baseName = animations[ i ].baseName;
////        const std::string & ext = animations[ i ].ext;
////
////        std::vector< std::string > textures;
////
////        for( unsigned int i = 0; i < numFrames; ++i )
////        {
////            char buffer[40];
////            sprintf_s( buffer, 40, "%02d", i );
////        
////            std::string txName( path + baseName + std::string( buffer ) + "." + ext );        
////            textures.push_back( txName );
////        }
////
////        auto animationPlugin = PluginsFactory::CreateSimpleAnimationPlugin( transformPlugin, textures, frameCounter );
////
////        root1->AddPlugin( animationPlugin );
////
////        root->AddChild( root1 );
////    }
////
////    return root;
////}
////
////// ******************************
//////
////model::BasicNode * NonGeometryParentRects   ( TimeType start0, TimeType stop0, TimeType start1, TimeType stop1, TimeType start2, TimeType stop2 ,TimeType start3, TimeType stop3 )
////{
////    model::BasicNode * root = new model::BasicNode( "NonGeometryRoot" );
////
////    model::BasicNode * child1 = new model::BasicNode( "NonGeometryC0" );
////    model::BasicNode * child2 = new model::BasicNode( "NonGeometryC1" );
////    model::BasicNode * child3 = new model::BasicNode( "NonGeometryC2" );
////    model::BasicNode * child4 = new model::BasicNode( "NonGeometryC3" );
////
////    model::SimpleTransformPlugin * plugin00 = model::SimpleTransformPlugin::Create( nullptr, model::ParametersFactory::CreateParameter( "transformation", TransformF(), nullptr, 0 ) );
////
////    root->AddPlugin( plugin00 );
////
////    model::GeometryRectPlugin *  rectPlugin11  = PluginsFactory::CreateGeometryRectPlugin(1.f, 1.f);
////    model::GeometryRectPlugin *  rectPlugin12  = PluginsFactory::CreateGeometryRectPlugin(1.f, 1.f);
////    model::GeometryRectPlugin *  rectPlugin13  = PluginsFactory::CreateGeometryRectPlugin(1.f, 1.f);
////    model::GeometryRectPlugin *  rectPlugin14  = PluginsFactory::CreateGeometryRectPlugin(1.f, 1.f);
////
////    TransformF trans11;
////    TransformF trans12;
////    TransformF trans13;
////    TransformF trans14;
////
////    trans11.AddTranslation( InterpolatorsHelper::CreateConstValue( -0.6f ), InterpolatorsHelper::CreateConstValue( -0.6f ), InterpolatorsHelper::CreateConstValue( 0.f ) );
////    trans12.AddTranslation( InterpolatorsHelper::CreateConstValue( 0.6f ), InterpolatorsHelper::CreateConstValue( -0.6f ), InterpolatorsHelper::CreateConstValue( 0.f ) );
////    trans13.AddTranslation( InterpolatorsHelper::CreateConstValue( -0.6f ), InterpolatorsHelper::CreateConstValue( 0.6f ), InterpolatorsHelper::CreateConstValue( 0.f ) );
////    trans14.AddTranslation( InterpolatorsHelper::CreateConstValue( 0.6f ), InterpolatorsHelper::CreateConstValue( 0.6f ), InterpolatorsHelper::CreateConstValue( 0.f ) );
////
////    model::SimpleTransformPlugin * tplugin11 = model::SimpleTransformPlugin::Create( rectPlugin11, model::ParametersFactory::CreateParameter( "transformation", trans11, nullptr, 0 ) );
////    model::SimpleTransformPlugin * tplugin12 = model::SimpleTransformPlugin::Create( rectPlugin12, model::ParametersFactory::CreateParameter( "transformation", trans12, nullptr, 0 ) );
////    model::SimpleTransformPlugin * tplugin13 = model::SimpleTransformPlugin::Create( rectPlugin13, model::ParametersFactory::CreateParameter( "transformation", trans13, nullptr, 0 ) );
////    model::SimpleTransformPlugin * tplugin14 = model::SimpleTransformPlugin::Create( rectPlugin14, model::ParametersFactory::CreateParameter( "transformation", trans14, nullptr, 0 ) );
////
////    model::Timeline * timeline0 = new model::Timeline( nullptr, start0, stop0, "timeline0" );
////    model::Timeline * timeline1 = new model::Timeline( nullptr, start1, stop1, "timeline1" );
////    model::Timeline * timeline2 = new model::Timeline( nullptr, start2, stop2, "timeline2" );
////    model::Timeline * timeline3 = new model::Timeline( nullptr, start3, stop3, "timeline3" );
////
////    glm::vec4 col11( 1.f/255.f, 167.f/255.f, 193.f/255.f, 1.f ); 
////    glm::vec4 col12( 226.f/255.f, 169.f/255.f, 36.f/255.f, 1.f );
////    glm::vec4 col13( 0.898f, 0.25f, 0.f, 1.f );
////    glm::vec4 col14( 0.435f, 172.f / 255.f, 19.f/255.f, 1.f );
////
////    glm::vec4 colb( 0.f, 0.f, 0.f, 1.f );
////    glm::vec4 colw( 1.f, 1.f, 1.f, 1.f );
////
////    Vec4Interpolator colc11; colc11.SetWrapPostMethod( bv::WrapMethod::pingPong ); colc11.AddKey( TimeType( 0.0 ), col11 ); colc11.AddKey( TimeType( 2.0 ), colb );
////    Vec4Interpolator colc12; colc12.SetWrapPostMethod( bv::WrapMethod::pingPong ); colc12.AddKey( TimeType( 0.0 ), col12 ); colc12.AddKey( TimeType( 2.0 ), colw );
////    Vec4Interpolator colc13; colc13.SetWrapPostMethod( bv::WrapMethod::pingPong ); colc13.AddKey( TimeType( 0.0 ), col13 ); colc13.AddKey( TimeType( 2.0 ), colb );
////    Vec4Interpolator colc14; colc14.SetWrapPostMethod( bv::WrapMethod::pingPong ); colc14.AddKey( TimeType( 0.0 ), col14 ); colc14.AddKey( TimeType( 2.0 ), colw );
////
////    ParamVec4 paramCol11 = ParametersFactory::CreateParameter( "color0", colc11, timeline0 );
////    ParamVec4 paramCol12 = ParametersFactory::CreateParameter( "color1", colc12, timeline1 );
////    ParamVec4 paramCol13 = ParametersFactory::CreateParameter( "color2", colc13, timeline2 );
////    ParamVec4 paramCol14 = ParametersFactory::CreateParameter( "color3", colc14, timeline3 );
////
////    auto colorPlugin11 = PluginsFactory::CreateSimpleColorPlugin( tplugin11, paramCol11 );
////    auto colorPlugin12 = PluginsFactory::CreateSimpleColorPlugin( tplugin12, paramCol12 );
////    auto colorPlugin13 = PluginsFactory::CreateSimpleColorPlugin( tplugin13, paramCol13 );
////    auto colorPlugin14 = PluginsFactory::CreateSimpleColorPlugin( tplugin14, paramCol14 );
////
////    auto psp11 = PluginsFactory::CreateSimplePixelShaderPlugin( colorPlugin11,  "Assets/Shaders/Deprecated/solid.frag", RendererContext::CreateDefault() );
////    auto psp12 = PluginsFactory::CreateSimplePixelShaderPlugin( colorPlugin12,  "Assets/Shaders/Deprecated/solid.frag", RendererContext::CreateDefault() );
////    auto psp13 = PluginsFactory::CreateSimplePixelShaderPlugin( colorPlugin13,  "Assets/Shaders/Deprecated/solid.frag", RendererContext::CreateDefault() );
////    auto psp14 = PluginsFactory::CreateSimplePixelShaderPlugin( colorPlugin14,  "Assets/Shaders/Deprecated/solid.frag", RendererContext::CreateDefault() );
////
////    child1->AddPlugin( rectPlugin11 );
////    child1->AddPlugin( tplugin11 );
////    child1->AddPlugin( colorPlugin11 );
////    child1->AddPlugin( psp11 );
////
////    child2->AddPlugin( rectPlugin12 );
////    child2->AddPlugin( tplugin12 );
////    child2->AddPlugin( colorPlugin12 );
////    child2->AddPlugin( psp12 );
////
////    child3->AddPlugin( rectPlugin13 );
////    child3->AddPlugin( tplugin13 );
////    child3->AddPlugin( colorPlugin13 );
////    child3->AddPlugin( psp13 );
////
////    child4->AddPlugin( rectPlugin14 );
////    child4->AddPlugin( tplugin14 );
////    child4->AddPlugin( colorPlugin14 );
////    child4->AddPlugin( psp14 );
////
////    ///////////////////////////// Transform plugin //////////////////////////// 
////
////    root->AddPlugin( plugin00 );
////
////    root->AddChild( child1 );
////    root->AddChild( child2 );
////    root->AddChild( child3 );
////    root->AddChild( child4 );
////
////    return root;
////}
////
////// ******************************
//////
////model::BasicNode *          GreenRect()
////{
////    model::BasicNode * root = new model::BasicNode( "GreenRectRoot" );
////
////    ///////////////////////////// Geometry plugin //////////////////////////
////    model::GeometryRectPlugin * rectPlugin  = PluginsFactory::CreateGeometryRectPlugin( 1.f, 1.f );
////
////    root->AddPlugin( rectPlugin ); 
////
////    ///////////////////////////// Transform plugin //////////////////////////// 
////    TransformF trans;
////
////    FloatInterpolator angle; angle.SetWrapPostMethod( bv::WrapMethod::pingPong );
////    angle.AddKey( 0.f, 0.f );
////    angle.AddKey( 10.f, 270.f );
////
////    trans.AddRotation( angle, InterpolatorsHelper::CreateConstValue( 0.f ), InterpolatorsHelper::CreateConstValue( 0.f ), InterpolatorsHelper::CreateConstValue( 1.f ) );
////    trans.AddScale( InterpolatorsHelper::CreateConstValue( 1.f ), InterpolatorsHelper::CreateConstValue( 1.f ), InterpolatorsHelper::CreateConstValue( 1.f ) );
////    trans.AddTranslation( InterpolatorsHelper::CreateConstValue( 0.f ), InterpolatorsHelper::CreateConstValue( 0.f ), InterpolatorsHelper::CreateConstValue( 0.f ) );
////
////    auto transformPlugin = PluginsFactory::CreateSimpleTransformPlugin( rectPlugin, model::ParametersFactory::CreateParameter( "transform", trans, nullptr, 0 ) );
////
////    root->AddPlugin( transformPlugin ); // Plugin with transformation
////
////    ///////////////////////////// Material plugin //////////////////////////// 
////
////    Vec4Interpolator col; col.SetWrapPostMethod( bv::WrapMethod::pingPong );
////    col.AddKey( 0.f, glm::vec4( 0.f, 1.f, 0.f, 1.f ) );
////    col.AddKey( 10.f, glm::vec4( 0.9f, 0.9f, 0.98f, 1.f ) );
////
////    auto colorPlugin = PluginsFactory::CreateSimpleColorPlugin( transformPlugin, ParametersFactory::CreateParameter( "color", col ) );
////
////    root->AddPlugin( colorPlugin );
////
////    auto pixelShaderPlugin = PluginsFactory::CreateSimplePixelShaderPlugin( colorPlugin,  "Assets/Shaders/Deprecated/solid.frag", RendererContext::CreateDefault() );
////
////    root->AddPlugin( pixelShaderPlugin );
////
////    return root;
////}
////
////// ******************************
//////
////model::BasicNode *          TexturedRect()
////{
////    model::BasicNode * root = new model::BasicNode( "TxRectRoot" );
////
////    ///////////////////////////// Geometry plugin //////////////////////////
////    model::GeometryRectPlugin* rectPlugin   = PluginsFactory::CreateGeometryRectPlugin(1.f, 1.f);
////
////    root->AddPlugin( rectPlugin );
////
////    ///////////////////////////// Transform plugin //////////////////////////// 
////    TransformF trans;
////    FloatInterpolator ang; ang.SetWrapPostMethod( bv::WrapMethod::pingPong );
////    ang.AddKey( 0.f, 0.f );
////    ang.AddKey( 9.f, 359.f );
////    FloatInterpolator s; s.SetWrapPostMethod( bv::WrapMethod::pingPong );
////    s.AddKey( 0.f, 1.f );
////    s.AddKey( 8.f, 2.f );
////
////    trans.AddRotation( ang, InterpolatorsHelper::CreateConstValue( 0.f ), InterpolatorsHelper::CreateConstValue( 0.f ), InterpolatorsHelper::CreateConstValue( 1.f ) );
////    trans.AddScale( s, s, InterpolatorsHelper::CreateConstValue( 1.f ) );
////    trans.AddTranslation( InterpolatorsHelper::CreateConstValue( 0.f ), InterpolatorsHelper::CreateConstValue( 0.f ), InterpolatorsHelper::CreateConstValue( 0.f ) );
////
////    auto transformPlugin = PluginsFactory::CreateSimpleTransformPlugin( rectPlugin, model::ParametersFactory::CreateParameter( "transformation", trans, nullptr, 0 ) );
////
////    root->AddPlugin( transformPlugin ); // Plugin with transformation
////    
////    ///////////////////////////// Material plugin //////////////////////////// 
////    
////    FloatInterpolator angle; angle.SetWrapPostMethod( bv::WrapMethod::pingPong );
////    angle.AddKey( 0.f, 0.f );
////    angle.AddKey( 6.5f, 270.f );
////
////    TransformF txTrans;
////    txTrans.AddTranslation( InterpolatorsHelper::CreateConstValue( 0.5f ), InterpolatorsHelper::CreateConstValue( 0.5f ), InterpolatorsHelper::CreateConstValue( 0.f ) );
////    txTrans.AddRotation( angle, InterpolatorsHelper::CreateConstValue( 0.f ), InterpolatorsHelper::CreateConstValue( 0.f ), InterpolatorsHelper::CreateConstValue( 1.f ) );
////    txTrans.AddTranslation( InterpolatorsHelper::CreateConstValue( -0.5f ), InterpolatorsHelper::CreateConstValue( -0.5f ), InterpolatorsHelper::CreateConstValue( 0.f ) );
////
////    std::vector< const model::TextureDescriptor > textures;
////        
////    textures.push_back( model::TextureDescriptor( "pliczek_z_kwiatkiem.jpg", ParametersFactory::CreateParameter( "texTransform", txTrans ) ) );
////
////    auto texturePlugin = PluginsFactory::CreateSimpleTexturePlugin( transformPlugin, textures );
////
////    root->AddPlugin( texturePlugin );
////
////    return root;
////
////}
////
////
////// ******************************
//////
////model::BasicNode *          TexturedRing()
////{
////    model::BasicNode * root = new model::BasicNode( "TxRingRoot" );
////
////    ///////////////////////////// Geometry plugin //////////////////////////
////
////    model::GeometryRingPlugin*          ringPlugin = PluginsFactory::CreateGeometryRingPlugin( 0.f, 1.5f * 3.14f, 1.f, 2.f, 200 );
////
////    root->AddPlugin(ringPlugin);
////
////    ///////////////////////////// Transform plugin //////////////////////////// 
////
////    TransformF trans;
////
////    trans.AddScale( InterpolatorsHelper::CreateConstValue( 0.25f ), InterpolatorsHelper::CreateConstValue( 0.25f ), InterpolatorsHelper::CreateConstValue( 1.f ) );
////    trans.AddTranslation( InterpolatorsHelper::CreateConstValue( -1.f ), InterpolatorsHelper::CreateConstValue( -1.f ), InterpolatorsHelper::CreateConstValue( 0.001f ) );
////
////    auto transformPlugin = PluginsFactory::CreateSimpleTransformPlugin( ringPlugin, model::ParametersFactory::CreateParameter( "transformation", trans, nullptr, 0 ) );
////    
////    root->AddPlugin( transformPlugin );
////
////    ///////////////////////////// Material plugin //////////////////////////// 
////
////    std::vector< const model::TextureDescriptor > textures;
////        
////    textures.push_back( model::TextureDescriptor( "simless_01.jpg" ) );
////
////    auto texturePlugin = PluginsFactory::CreateSimpleTexturePlugin( transformPlugin, textures );
////
////    root->AddPlugin(texturePlugin);
////
////    return root;
////}
////
////// ******************************
//////
////model::BasicNode *     NaiveTimer()
////{
////    model::BasicNode * root = new model::BasicNode( "NaiveTimerRoot" );
////
////    Vec4Interpolator color; color.SetWrapPostMethod( bv::WrapMethod::pingPong );
////    color.AddKey(0.f, glm::vec4( 1.f, 0.f, 0.f, 1.f ) );
////    color.AddKey(3.f, glm::vec4( 0.f, 1.f, 0.f, 1.f ) );
////    color.AddKey(5.f, glm::vec4( 0.f, 0.f, 1.f, 1.f ) );
////    color.AddKey(7.f, glm::vec4( 1.f, 1.f, 1.f, 1.f ) );
////
////    std::wstring str    =   L"0123456789:.";
////    auto texPlugin      =   PluginsFactory::CreateSimpleTextPlugin( str, "../dep/Media/fonts/cour.ttf", 20 );
////    root->AddPlugin( texPlugin );
////
////    TransformF     trns;
////    trns.AddTranslation( InterpolatorsHelper::CreateConstValue( 0.f ), InterpolatorsHelper::CreateConstValue( 0.f ), InterpolatorsHelper::CreateConstValue( 0.f ) );
////    FloatInterpolator xt; xt.SetWrapPostMethod( bv::WrapMethod::pingPong );
////
////    auto transPlugin = PluginsFactory::CreateSimpleTransformPlugin( texPlugin, model::ParametersFactory::CreateParameter( "transformation", trns, nullptr, 0 ) );
////    root->AddPlugin( transPlugin );
////
////    auto vertexShaderPlugin = PluginsFactory::CreateSimpleVertexShaderPlugin( transPlugin,  "Assets/Shaders/Deprecated/simpletexture.vert" );
////    root->AddPlugin( vertexShaderPlugin );
////
////    auto colorPlugin = PluginsFactory::CreateSimpleColorPlugin( vertexShaderPlugin, ParametersFactory::CreateParameter( "color", color ) );
////    root->AddPlugin( colorPlugin );
////
////    auto ctx = RendererContext::CreateDefault();
////    //ctx->alphaCtx->blendEnabled = true;
////    //ctx->depthCtx->enabled = false;
////    ctx->cullCtx->enabled = false;
////    auto pixelShaderPlugin = PluginsFactory::CreateSimplePixelShaderPlugin( colorPlugin,  "Assets/Shaders/Deprecated/text.frag", ctx );
////    root->AddPlugin( pixelShaderPlugin );
////
////    return root;
////}
////
////// ******************************
//////
////model::BasicNode *     Text1()
////{
////    model::BasicNode * root = new model::BasicNode( "Tex1Root" );
////
////    std::wstring str    =   TextHelper::LoadUtf8FileToString( L"text_example.txt");
////
////    Vec4Interpolator color; color.SetWrapPostMethod( bv::WrapMethod::pingPong );
////
////    color.AddKey(0.f, glm::vec4( 1.f, 1.f, 1.f, 1.f ) );
////    color.AddKey(3.f, glm::vec4( 0.f, 1.f, 0.f, 1.f ) );
////    color.AddKey(5.f, glm::vec4( 0.f, 0.f, 1.f, 1.f ) );
////    color.AddKey(7.f, glm::vec4( 1.f, 1.f, 1.f, 1.f ) );
////
////    auto texPlugin      =   PluginsFactory::CreateSimpleTextPlugin( L"DUPA", "../dep/Media/fonts/ARIALUNI.TTF", 64 );
////
////    root->AddPlugin( texPlugin );
////
////    TransformF trans;
////
////    FloatInterpolator angle; angle.SetWrapPostMethod( bv::WrapMethod::pingPong );
////
////    angle.AddKey( 0.f, 0.f );
////    angle.AddKey( 10.f, 270.f );
////
////    trans.AddRotation( angle, InterpolatorsHelper::CreateConstValue( 0.f ), InterpolatorsHelper::CreateConstValue( 0.f ), InterpolatorsHelper::CreateConstValue( 1.f ) );
////    trans.AddScale( InterpolatorsHelper::CreateConstValue( 1.f ), InterpolatorsHelper::CreateConstValue( 1.f ), InterpolatorsHelper::CreateConstValue( 1.f ) );
////    trans.AddTranslation( InterpolatorsHelper::CreateConstValue( -1.f ), InterpolatorsHelper::CreateConstValue( 0.f ), InterpolatorsHelper::CreateConstValue( 0.f ) );
////
////    auto transPlugin = PluginsFactory::CreateSimpleTransformPlugin( texPlugin, model::ParametersFactory::CreateParameter( "transformation", trans, nullptr, 0 ) );
////
////    root->AddPlugin( transPlugin );
////
////    auto vertexShaderPlugin = PluginsFactory::CreateSimpleVertexShaderPlugin( transPlugin,  "Assets/Shaders/Deprecated/text.vert" );
////
////    root->AddPlugin( vertexShaderPlugin );
////
////    auto colorPlugin = PluginsFactory::CreateSimpleColorPlugin( vertexShaderPlugin, ParametersFactory::CreateParameter( "color", color ) );
////
////    root->AddPlugin( colorPlugin );
////
////    auto ctx = RendererContext::CreateDefault();
////    ctx->alphaCtx->blendEnabled = true;
////    ctx->depthCtx->enabled = false;
////
////    auto pixelShaderPlugin = PluginsFactory::CreateSimplePixelShaderPlugin( colorPlugin,  "Assets/Shaders/Deprecated/text.frag", ctx );
////
////    root->AddPlugin( pixelShaderPlugin );
////
////    return root;
////}
////
////// ******************************
//////
////model::BasicNode *     Timer()
////{
////    model::BasicNode * root = new model::BasicNode( "TimerRoot" );
////
////    std::wstring str    =   TextHelper::LoadUtf8FileToString( L"text_example.txt");
////
////    FloatInterpolator time; time.SetWrapPostMethod( bv::WrapMethod::pingPong );
////
////    time.AddKey(0.f, 0.f );
////    time.AddKey(10.f, 10.f );
////
////    auto timerPlugin      =   PluginsFactory::CreateTimerPlugin( model::ParametersFactory::CreateParameter("time", time ), 64 );
////
////    root->AddPlugin( timerPlugin );
////
////    TransformF trans;
////
////    FloatInterpolator angle; angle.SetWrapPostMethod( bv::WrapMethod::pingPong );
////
////    angle.AddKey( 0.f, 0.f );
////    angle.AddKey( 10.f, 270.f );
////
////    //trans.AddRotation( angle, InterpolatorsHelper::CreateConstValue( 0.f ), InterpolatorsHelper::CreateConstValue( 0.f ), InterpolatorsHelper::CreateConstValue( 1.f ) );
////    trans.AddScale( InterpolatorsHelper::CreateConstValue( 0.3f ), InterpolatorsHelper::CreateConstValue( 0.3f ), InterpolatorsHelper::CreateConstValue( 1.f ) );
////    trans.AddTranslation( InterpolatorsHelper::CreateConstValue( -1.f ), InterpolatorsHelper::CreateConstValue( 0.f ), InterpolatorsHelper::CreateConstValue( 0.f ) );
////    
////    auto transPlugin = PluginsFactory::CreateSimpleTransformPlugin( timerPlugin, model::ParametersFactory::CreateParameter( "transformation", trans, nullptr, 0 ) );
////
////    root->AddPlugin( transPlugin ); // Plugin with transformation
////
////    auto vertexShaderPlugin = PluginsFactory::CreateSimpleVertexShaderPlugin( transPlugin,  "Assets/Shaders/Deprecated/text.vert" );
////
////    root->AddPlugin( vertexShaderPlugin );
////
////    Vec4Interpolator color; color.SetWrapPostMethod( bv::WrapMethod::pingPong );
////
////    color.AddKey(0.f, glm::vec4( 1.f, 1.f, 1.f, 1.f ) );
////
////    auto colorPlugin = PluginsFactory::CreateSimpleColorPlugin( vertexShaderPlugin, ParametersFactory::CreateParameter( "color", color ) );
////
////    root->AddPlugin( colorPlugin );
////
////    auto ctx = RendererContext::CreateDefault();
////    ctx->alphaCtx->blendEnabled = true;
////    ctx->depthCtx->enabled = true;
////
////    auto pixelShaderPlugin = PluginsFactory::CreateSimplePixelShaderPlugin( colorPlugin,  "Assets/Shaders/Deprecated/text.frag", ctx );
////
////    root->AddPlugin( pixelShaderPlugin );
////
////    return root;
////}
////
////
////// ******************************
//////
////model::BasicNode * Text1Textured()
////{
////    model::BasicNode * root = new model::BasicNode( "Tex1TexRoot" );
////
////    std::wstring str    =   TextHelper::LoadUtf8FileToString( L"text_example1.txt");
////
////    auto texPlugin      =   PluginsFactory::CreateSimpleTextPlugin( str, "../dep/Media/fonts/cour.ttf", 8 );
////
////    root->AddPlugin( texPlugin );
////
////    TransformF     trns;
////
////    FloatInterpolator xt; xt.SetWrapPostMethod( bv::WrapMethod::pingPong );
////    FloatInterpolator yt; yt.SetWrapPostMethod( bv::WrapMethod::repeat );
////    FloatInterpolator zt;
////
////    xt.AddKey(0.f, -1.f);
////    yt.AddKey(0.f, -5.f);
////    zt.AddKey(0.f, -2.f);
////
////    yt.AddKey(30.f, 5.f);
////
////    trns.AddTranslation( xt, yt, zt );
////
////    trns.AddScale( InterpolatorsHelper::CreateConstValue( 3.f ), InterpolatorsHelper::CreateConstValue( 3.f ), InterpolatorsHelper::CreateConstValue( 1.f ) );
////
////    auto transPlugin = PluginsFactory::CreateSimpleTransformPlugin( texPlugin, model::ParametersFactory::CreateParameter( "transformation", trns, nullptr, 0 ) );
////
////    root->AddPlugin( transPlugin );
////
////    std::vector< const model::TextureDescriptor > textures;
////        
////    textures.push_back( model::TextureDescriptor( "simless_01.jpg" ) );
////
////    auto texturePlugin = PluginsFactory::CreateSimpleTexturePlugin( transPlugin, textures );
////
////    root->AddPlugin( texturePlugin );
////
////    auto vertexShaderPlugin = PluginsFactory::CreateSimpleVertexShaderPlugin( texturePlugin,  "Assets/Shaders/Deprecated/textured_text.vert" );
////
////    root->AddPlugin( vertexShaderPlugin );
////
////    auto ctx = RendererContext::CreateDefault();
////    ctx->alphaCtx->blendEnabled = true;
////    ctx->depthCtx->enabled = false;
////
////    auto pixelShaderPlugin = PluginsFactory::CreateSimplePixelShaderPlugin( vertexShaderPlugin,  "Assets/Shaders/Deprecated/textured_text.frag", ctx );
////
////    root->AddPlugin( pixelShaderPlugin );
////
////    return root;
////}
////
////// ******************************
//////
////model::BasicNode *     Text2()
////{
////    model::BasicNode * root = new model::BasicNode( "Tx2root" );
////
////    std::wstring str  = TextHelper::LoadUtf8FileToString( L"text_example.txt" );
////
////    Vec4Interpolator color; color.SetWrapPostMethod( bv::WrapMethod::pingPong );
////
////    color.AddKey( 0.f, glm::vec4( 1.f, 0.f, 0.f, 1.f ) );
////    color.AddKey( 1.f, glm::vec4( 0.f, 1.f, 0.f, 1.f ) );
////    color.AddKey( 8.f, glm::vec4( 0.f, 0.f, 1.f, 1.f ) );
////    color.AddKey( 18.f, glm::vec4( 1.f, 1.f, 1.f, 1.f ) );
////
////    auto texPlugin = PluginsFactory::CreateSimpleTextPlugin( str, "../dep/Media/fonts/cour.ttf", 128, true, true );
////
////    root->AddPlugin( texPlugin );
////
////    TransformF     trns;
////
////    trns.AddScale( InterpolatorsHelper::CreateConstValue( 2.f ), InterpolatorsHelper::CreateConstValue( 2.f ), InterpolatorsHelper::CreateConstValue( 2.f ) );
////
////    FloatInterpolator xt; xt.SetWrapPostMethod( bv::WrapMethod::pingPong );
////    FloatInterpolator yt; yt.SetWrapPostMethod( bv::WrapMethod::repeat );
////    FloatInterpolator zt;
////
////    xt.AddKey(0.f, -1.f);
////    yt.AddKey(0.f, -5.f);
////    zt.AddKey(0.f, -0.5f);
////
////    yt.AddKey(30.f, 5.f);
////
////    trns.AddTranslation( xt, yt, zt );
////
////    auto transPlugin = PluginsFactory::CreateSimpleTransformPlugin( texPlugin, model::ParametersFactory::CreateParameter( "transformation", trns, nullptr, 0 ) );
////
////    root->AddPlugin( transPlugin );
////
////    auto vertexShaderPlugin = PluginsFactory::CreateSimpleVertexShaderPlugin( transPlugin,  "Assets/Shaders/Deprecated/simpletexture.vert" );
////
////    root->AddPlugin( vertexShaderPlugin );
////
////    auto colorPlugin = PluginsFactory::CreateSimpleColorPlugin( vertexShaderPlugin, ParametersFactory::CreateParameter( "color", color ) );
////
////    root->AddPlugin( colorPlugin );
////
////    auto ctx = RendererContext::CreateDefault();
////    ctx->alphaCtx->blendEnabled = true;
////    ctx->depthCtx->enabled = false;
////
////    auto pixelShaderPlugin = PluginsFactory::CreateSimplePixelShaderPlugin( colorPlugin,  "Assets/Shaders/Deprecated/text.frag", ctx );
////
////    root->AddPlugin( pixelShaderPlugin );
////
////    return root;
////}
////
////// ******************************
//////
////model::BasicNode *          ExtrudedRedRect()
////{
////    model::BasicNode * root = new model::BasicNode( "ExtrudedRectRoot" );
////
////    auto rectPlugin  = PluginsFactory::CreateGeometryRectPlugin(2.f, 1.f);
////
////    root->AddPlugin(rectPlugin);
////
////    /// Set Transform Channel
////    TransformF trans;
////
////    trans.AddScale( InterpolatorsHelper::CreateConstValue( 1.f ), InterpolatorsHelper::CreateConstValue( 1.f ), InterpolatorsHelper::CreateConstValue( 1.f ) );
////    trans.AddTranslation( InterpolatorsHelper::CreateConstValue( -2.f ), InterpolatorsHelper::CreateConstValue( 1.5f ), InterpolatorsHelper::CreateConstValue( 0.f ) );
////
////    auto trasformPlugin  = PluginsFactory::CreateSimpleTransformPlugin( rectPlugin, model::ParametersFactory::CreateParameter( "transformation", trans, nullptr, 0 ) );
////
////    root->AddPlugin( trasformPlugin );
////
////    ///////////////////////////// Solid plugin //////////////////////////// 
////
////    auto solidPlugin = PluginsFactory::CreateSolidColorPlugin( root, trasformPlugin, glm::vec4( 1.f, 0.f, 0.f, 1.f ) );
////
////    root->AddPlugin(solidPlugin);
////
////    auto geomShaderPlugin = PluginsFactory::CreateSimpleGeometryShaderPlugin( solidPlugin, "Assets/Shaders/Deprecated/extrude.geom" );
////
////    root->AddPlugin(geomShaderPlugin);
////
//////    solidPlugin->SetGeometryShaderChannel ( PluginsFactory::CreateGeometryShaderExtrude( 1.f ) );
////
////    
////
////    return root;
////}
////
////// ******************************
//////
////model::BasicNode *          ExtrudedTexturedRing()
////{
////    model::BasicNode * root = new model::BasicNode( "ExtrudedRingRoot" );
////
////    model::GeometryRingPlugin*          ringPlugin = model::PluginsFactory::CreateGeometryRingPlugin( 0.f, 1.5f * 3.14f, 1.f, 2.f, 200 );
////
////    /// Set Transform Channel
////    TransformF trans;
////
////    trans.AddScale( InterpolatorsHelper::CreateConstValue( 0.25f ), InterpolatorsHelper::CreateConstValue( 0.25f ), InterpolatorsHelper::CreateConstValue( 1.f ) );
////    trans.AddTranslation( InterpolatorsHelper::CreateConstValue( -1.f ), InterpolatorsHelper::CreateConstValue( -1.f ), InterpolatorsHelper::CreateConstValue( 0.f ) );
////
////    auto trasformChannel  = ChannelsFactory::CreateTransformChannel( model::ParametersFactory::CreateParameter( "transformation", trans, nullptr, 0 ) );
////
////    ringPlugin->SetTransformChannel      ( trasformChannel );
////
////    root->AddPlugin(ringPlugin);
////
////    std::vector< const model::TextureDescriptor > textures;
////        
////    textures.push_back( model::TextureDescriptor( "simless_01.jpg" ) );
////
////
////    auto texturePlugin  = PluginsFactory::CreateSimpleTexturePlugin( ringPlugin, textures );
////
////    texturePlugin->SetGeometryShaderChannel ( ChannelsFactory::CreateGeometryShaderExtrude( 1.f ) );
////
////    root->AddPlugin(texturePlugin);
////
////    return root;
////}
//
//} // anonymous
//
//
//// ******************************
////
//model::BasicNodePtr          TestScenesFactory::AnotherTestScene()
//{
//    auto root = nullptr;
//    //auto root = GreenRect();
//    //auto root =  Timer();
//    //root->AddChild( Text1() );
//    //root->AddChild( GreenRect() );
//    //root->AddChild( TexturedRect() );
//    //root->AddChild( ExtrudedTexturedRing() ); // To nie dziala na mojej karcie.
//    //root->AddChild( TexturedRing() );
//    //root->AddChild( ExtrudedRedRect() );
//    //root->AddChild( Text1Textured() );
//    //root->AddChild( Text2() );
//
//    return root;
//}
//
//// ******************************
////
//model::BasicNodePtr      TestScenesFactory::GreenRectTestScene          ()
//{
//    //return GreenRect();
//    return nullptr;
//}
//
//// ******************************
////
//model::BasicNodePtr      TestScenesFactory::TexturedRectTestScene       ()
//{
//    //return TexturedRect();
//    return nullptr;
//}
//
//// ******************************
////
//model::BasicNodePtr      TestScenesFactory::NaiveTimerTestScene         ()
//{
//    //return NaiveTimer();
//    return nullptr;
//}
//
//// ******************************
////
//model::BasicNodePtr      TestScenesFactory::SequenceAnimationTestScene  ()
//{
//    std::vector< AnimationSequenceDesc >    animations;
//
//    FloatInterpolator anim0; anim0.SetWrapPostMethod( WrapMethod::pingPong ); anim0.SetWrapPreMethod( WrapMethod::clamp );
//    FloatInterpolator anim1; anim1.SetWrapPostMethod( WrapMethod::pingPong ); anim1.SetWrapPreMethod( WrapMethod::clamp );
//
//    anim0.AddKey( TimeType( 5.0 ), 0.0f );
//    anim0.AddKey( TimeType( 6.5 ), 50.0f );
//    anim0.AddKey( TimeType( 8.0 ), 75.0f );
//
//    anim1.AddKey( TimeType( 2.0 ), 75.0f );
//    anim1.AddKey( TimeType( 5.0 ), 0.0f );
//    anim1.AddKey( TimeType( 9.0 ), 60.0f );
//    anim1.AddKey( TimeType( 11.0 ), 0.0f );
//
//    AnimationSequenceDesc intro;
//    intro.ext = "tga";
//    intro.baseName = "Alfai";
//    intro.path = "../../media/sequences/FullHD/alfai/";
//    intro.frameCounter = anim0;
//    intro.numFrames = 75;
//
//    AnimationSequenceDesc alfai;
//    alfai.ext = "tga";
//    alfai.baseName = "alfai";
//    alfai.path = "../../media/sequences/FullHD/alfai/";
//    alfai.frameCounter = anim1;
//    alfai.numFrames = 100;
//
//    animations.push_back( intro );
//    animations.push_back( alfai );
//
//    //return AnimatedSequenceRect( animations );
//    return nullptr;
//}
//
//model::SceneModelPtr LoadSceneFromFile( std::string filename )
//{
//    if( !Path::Exists( filename ) )
//    {
//        std::cout << "[ERROR] File " << filename << " does not exist" << std::endl;
//        return nullptr;
//    }
//
//    //JsonDeserializeObject deser;
//    //deser.Load( filename );
//    BVXMLDeserializer deser( filename, nullptr, nullptr );
//
//    auto model = SerializationHelper::DeserializeObject< SceneModel >( deser, "scene" );
//
//	//auto teClone = CloneViaSerialization::Clone( model::TimelineManager::GetInstance(), "timelines" );
//    //auto plClone = model->GetRootNode()->GetPlugins()->GetPlugin( 2 )->Clone();
//
//    return model;
//}
//
//model::SceneModelPtr     TestScenesFactory::CreateSerializedTestScene       ( const model::PluginsManager * )
//{
//    //return LoadSceneFromFile( "Assets/07_Results.xml", pluginsManager, timelineManager );
//    return LoadSceneFromFile( "test.xml" );
//}
//
//} // bv
