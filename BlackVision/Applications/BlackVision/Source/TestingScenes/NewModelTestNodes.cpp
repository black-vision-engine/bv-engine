#include "stdafxBVApp.h"

#include "NewModelTestNodes.h"

#include "Engine/Models/Plugins/Interfaces/IPluginListFinalized.h"
#include "Engine/Models/Plugins/Manager/PluginsManager.h"
#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"
#include "Engine/Models/Plugins/PluginsFactory.h"
#include "Engine/Models/Plugins/Simple/TextPlugins/DefaultTextPlugin.h"
#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultTexturePlugin.h"
#include "Engine/Models/Plugins/Simple/TextPlugins/DefaultTimerPlugin.h"

#include "Engine/Models/Plugins/Simple/AVStreamDecoder/DefaultAVDecoderPlugin.h"
#include "Engine/Models/Plugins/Simple/AVStreamDecoder/DefaultAudioDecoderPlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultRectPlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultConePlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultCirclePlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultSpherePlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultEllipsePlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultTorusPlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultSpringPlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultCylinderPlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultCubePlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultCogWheelPlugin.h"
#include "Engine/Models/NodeLogics/NodeReplicator/NodeReplicator.h"
#include "Engine/Models/NodeLogics/NodeReplicator/ShiftReplicationModifier.h"

#include "Engine/Models/Plugins/Channels/Geometry/Simple/PrismComponent.h"

#include "Engine/Models/NodeEffects/ModelNodeEffectFactory.h"

#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Models/Timeline/TimelineHelper.h"
#include "Engine/Models/Plugins/PluginUtils.h"

#include "Engine/Models/BasicNode.h"

#include "testai/TestAIManager.h"
#include "Engine/Models/NodeLogics/Scroller/Scroller.h"
#include "Helpers/RectNodeBuilder.h"

#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"
#include "Engine/Models/Plugins/Channels/RendererContext/FillContext.h"

#include "BVConfig.h"

#include "ProjectManager.h"

#include "Serialization/Json/JsonDeserializeObject.h"
#include "Serialization/Json/JsonSerializeObject.h"

#include <fstream>

namespace {

    std::string GSimplePlugins0[] = { "DEFAULT_TRANSFORM", "DEFAULT_RECTANGLE", "DEFAULT_COLOR" };
    std::string GSimplePlugins1[] = { /*"DEFAULT_TRANSFORM",*/ "DEFAULT_RECTANGLE", "DEFAULT_TEXTURE" };
    std::string GSimplePlugins2[] = { "DEFAULT_TRANSFORM", "DEFAULT_RECTANGLE", "DEFAULT_ANIMATION" };
    std::string GSimplePlugins3[] = { "DEFAULT_TRANSFORM", "DEFAULT_COLOR", "DEFAULT_TEXT" };
    std::string GSimplePlugins4[] = { "DEFAULT_TRANSFORM", "DEFAULT_TEXT" };
    std::string GSimplePlugins5[] = { "DEFAULT_TRANSFORM", "DEFAULT_COLOR", "DEFAULT_TIMER" };
    std::string GSimplePlugins6[] = { "DEFAULT_TRANSFORM", "DEFAULT_RECTANGLE", "DEFAULT_HEIGHT_MAP" };
    std::string GSimplePlugins7[] = { "DEFAULT_TRANSFORM", "DEFAULT_RECTANGLE", "DEFAULT_VIDEO_DECODER" };
    std::string GSimplePlugins8[] = { "DEFAULT_TRANSFORM", "DEFAULT_AUDIO_DECODER" };


    // *****************************
    //
    void SetDefaultColorChangeAnim( bv::model::IPluginPtr plugin )
    {
        auto param = plugin->GetParameter( "color" );
        assert( param );

        SetParameter( param, 0.f,  glm::vec4( 0.f, 1.f, 0.f, 1.f ) );
        SetParameter( param, 5.f,  glm::vec4( 1.f, 0.f, 0.f, 1.f ) );
        SetParameter( param, 10.f, glm::vec4( 0.f, 0.f, 1.f, 1.f ) );
        SetParameter( param, 15.f, glm::vec4( 1.f, 1.f, 0.f, 1.f ) );
        SetParameter( param, 20.f, glm::vec4( 0.f, 1.f, 1.f, 1.f ) );
        SetParameter( param, 25.f, glm::vec4( 1.f, 0.f, 1.f, 1.f ) );
        SetParameter( param, 30.f, glm::vec4( 0.f, 1.f, 0.f, 1.f ) );
    }

    // *****************************
    //
    void SetDefaultTransformAnim( bv::model::IPluginPtr plugin )
    {
        auto param = plugin->GetParameter( "simple_transform" );
        assert( param );

        SetParameterScale ( param, 0.0f, glm::vec3( 1.f, 1.f, 1.f ) );
        SetParameterScale ( param, 1.0f, glm::vec3( 0.5f, 0.5f, 1.f ) );
        SetParameterScale ( param, 2.0f, glm::vec3( 1.f, 1.f, 1.f ) );
        SetParameterScale ( param, 3.0f, glm::vec3( 0.2f, 0.2f, 1.f ) );
        SetParameterScale ( param, 4.0f, glm::vec3( 1.f, 1.f, 1.f ) );
        SetParameterScale ( param, 5.0f, glm::vec3( 0.3f, 0.3f, 1.f ) );
        SetParameterScale ( param, 6.0f, glm::vec3( 1.f, 1.f, 1.f ) );
        SetParameterScale ( param, 7.0f, glm::vec3( 0.6f, 0.6f, 1.f ) );
        SetParameterScale ( param, 8.0f, glm::vec3( 1.f, 1.f, 1.f ) );
        SetParameterScale ( param, 9.0f, glm::vec3( 0.4f, 0.4f, 1.f ) );
        SetParameterScale ( param, 10.0f, glm::vec3( 1.f, 1.f, 1.f ) );
        SetParameterScale ( param, 12.0f, glm::vec3( 0.2f, 3.f, 1.f ) );
        SetParameterScale ( param, 16.0f, glm::vec3( 3.f, 0.5f, 1.f ) );
        SetParameterScale ( param, 18.0f, glm::vec3( 1.f, 1.f, 1.f ) );
        SetParameterScale ( param, 21.0f, glm::vec3( 1.f, 1.f, 1.f ) );
        SetParameterScale ( param, 25.0f, glm::vec3( 2.f, 2.f, 1.f ) );

        SetParameterRotation ( param, 0.0f, glm::vec3( 0.f, 0.f, 0.f ) );
        SetParameterRotation ( param, 30.0f, glm::vec3( 0.f, 0.f, 360.f ) );

        SetParameterCenterMass( param, 0.f, glm::vec3( 1.5f, 1.5f, 0.f ) );
        SetParameterCenterMass( param, 10.f, glm::vec3( -.5f, -1.0f, 0.f ) );
        SetParameterCenterMass( param, 15.f, glm::vec3( 0.f, 0.f, 0.f ) );

        SetParameterTranslation( param, 23.f, glm::vec3( 0.f, 0.f, 0.f ) );
        SetParameterTranslation( param, 25.f, glm::vec3( 2.f, 0.f, 0.f ) );
        SetParameterTranslation( param, 27.5f, glm::vec3( -2.f, 0.f, 0.f ) );
        SetParameterTranslation( param, 30.f, glm::vec3( 0.f, 0.f, 0.f ) );
    }

    // *****************************
    //
    void SetDefaultTransformAlphaMaskTex( bv::model::IPluginPtr plugin )
    {
        auto param = plugin->GetParameter( "txAlphaMat" );
        assert( param );

        SetParameterTranslation( param, 0.f, glm::vec3( 0.f, 0.f, 0.f ) );
        SetParameterTranslation( param, 2.f, glm::vec3( 1.f, 0.f, 0.f ) );
        SetParameterTranslation( param, 5.f, glm::vec3( -1.f, 0.f, 0.f ) );
        SetParameterTranslation( param, 6.f, glm::vec3( 0.f, 1.f, 0.f ) );
        SetParameterTranslation( param, 9.f, glm::vec3( 0.f, -1.f, 0.f ) );
        SetParameterTranslation( param, 12.f, glm::vec3( 0.f, 0.f, 0.f ) );
        /*
        SetParameterCenterMass( param, 0.f, glm::vec3( 0.48f, 0.59f, 0.f ) );

        SetParameterRotation( param, 15.f, glm::vec3( 0.f, 0.f, 1.f ), 0.f );
        SetParameterRotation( param, 23.f, glm::vec3( 0.f, 0.f, 1.f ), 360.f);
        SetParameterRotation( param, 30.f, glm::vec3( 0.f, 0.f, 1.f ), 0.f);

        SetParameterScale( param, 0.f, glm::vec3( 1.f, 1.f, 1.f ) );
        SetParameterScale( param, 12.f, glm::vec3( 1.f, 1.f, 1.f ) );
        SetParameterScale( param, 15.f, glm::vec3( 1.f/68.f, 1./68.f, 1.f ) );
        SetParameterScale( param, 25.f, glm::vec3( 1.f/32.f, 1.f/32.f, 1.f ) );
        */
    }

} //anonymous

namespace bv {

// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateSolidRectNode      ( const std::string & name, float w, float h, const glm::vec3 & pos, const glm::vec4 col, model::ITimeEvaluatorPtr timeEvaluator )
{
    //Plugin list
    std::vector< std::string > uids;

    uids.push_back( "DEFAULT_TRANSFORM" );
    uids.push_back( "DEFAULT_RECTANGLE" );
    uids.push_back( "DEFAULT_COLOR" );

    //Create a model
    model::BasicNodePtr root = model::BasicNode::Create( name, timeEvaluator );
    bool success = root->AddPlugins( uids, timeEvaluator );
    assert( success );

    auto wp = root->GetPlugin( "rectangle" )->GetParameter( "width" );
    auto hp = root->GetPlugin( "rectangle" )->GetParameter( "height" );
    auto color = root->GetPlugin( "solid color" )->GetParameter( "color" );
    auto tr = root->GetPlugin( "transform" )->GetParameter( "simple_transform" );

    success &= SetParameter( wp, 0.f, w );
    success &= SetParameter( hp, 0.f, h );
    success &= SetParameter( color, 0.f, col );
    success &= SetParameterTranslation( tr, 0.0f, pos );

    assert( success );

    return root;
}

// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateTexturedRectNode   ( const std::string & name, float w, float h, const glm::vec3 & pos, const std::string & txFileName, model::ITimeEvaluatorPtr timeEvaluator )
{
    //Plugin stuff
    std::vector< std::string > GSimplePluginsUIDS( GSimplePlugins1, GSimplePlugins1 + 3 );

    auto root = model::BasicNode::Create( name, timeEvaluator );
    auto success = root->AddPlugins( GSimplePluginsUIDS, timeEvaluator );
    assert( success );

    auto wp = root->GetPlugin( "rectangle" )->GetParameter( "width" );
    auto hp = root->GetPlugin( "rectangle" )->GetParameter( "height" );
    auto tr = root->GetPlugin( "transform" )->GetParameter( "simple_transform" );

    success &= SetParameter( wp, 0.f, w );
    success &= SetParameter( hp, 0.f, h );
    success &= SetParameterTranslation( tr, 0.0f, pos );

    success = model::LoadTexture( root->GetPlugin( "texture" ), txFileName );
    assert( success );

    return root;    
}

// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateGlobalEffectTest      ( model::ITimeEvaluatorPtr timeEvaluator )
{
    SolidRectNodeBuilder bSolid( timeEvaluator, glm::vec4( 0.f, 1.f, 0.f, 1.f ), 3.4f, 1.9f );
    TexturedRectNodeBuilder bTex( timeEvaluator, "rsrcy/simless_01.jpg", false, 3.4f, 0.7f );

    // ROOT
    auto root = bSolid.CreateNode( "root", true );

    // Vanilla parent
    bSolid.SetW( 2.8f );
    bSolid.SetH( 1.6f );
    bSolid.SetColor( 1.f, 0.f, 0.f, 1.f );
    auto v  = bSolid.CreateNode( "vanilla", true );

    // Vanilla solid
    bSolid.SetW( 1.1f );
    bSolid.SetH( 1.0f );
    bSolid.SetColor( 0.f, 0.f, 1.f, 1.f );
    bSolid.SetPosition( 0.6f, 0.f, .1f );
    auto vs = bSolid.CreateNode( "vanilla_solid", true );
    
    // Vanilla texture
    bTex.SetW( 1.1f );
    bTex.SetH( 1.0f );
    bTex.SetPosition( -0.6f, 0.f, .1f );
    bTex.SetTextureFile( "rsrcy/simless_00.jpg", true );
    auto vt = bTex.CreateNode( "vanilla_tex", true );

    // Alpha mask root
    bTex.SetW( 2.6f );
    bTex.SetH( 1.5f );
    bTex.SetTextureFile( "rsrcy/simless_00.jpg", true );
    auto am = bTex.CreateNode( "alpha_mask", true );

    // Node mask root
    bSolid.SetW( 2.75f );
    bSolid.SetH( 1.55f );
    bSolid.SetColor( 1.f, 0.f, 1.f, 1.f );
    auto nm = bSolid.CreateNode( "node_mask", true );

    // Overlay alpha node root
    bTex.SetW( 2.6f );
    bTex.SetH( 1.35f );
    bTex.SetPosition( 0.0f, 0.f, .1f );
    bTex.SetTextureFile( "rsrcy/simless_01.jpg", true );
    auto ov = bTex.CreateNode( "overlay_alpha", true );

    // Overlay alpha solid
    bSolid.SetW( 1.1f );
    bSolid.SetH( 1.0f );
    bSolid.SetColor( 0.f, 0.f, 1.f, 1.f );
    bSolid.SetPosition( 0.6f, 0.f, .1f );
    auto ov_s = bSolid.CreateNode( "overlay_alpha_solid", true );
    
    // Overlay alpha texture
    bTex.SetW( 1.1f );
    bTex.SetH( 1.0f );
    bTex.SetPosition( -0.6f, 0.f, .1f );
    bTex.SetTextureFile( "rsrcy/simless_00.jpg", true );
    auto ov_t = bTex.CreateNode( "overlay_alpha_tex", true );

    // Node mask background
    bTex.SetW( 2.4f );
    bTex.SetH( 1.0f );
    bTex.SetPosition( 0.0f, 0.f, .1f );
    bTex.SetTextureFile( "rsrcy/simless_00.jpg", true );
    auto nm_bg = bTex.CreateNode( "node_mask_bg", true );

    // Node mask background color child
    bSolid.SetW( .8f );
    bSolid.SetH( .7f );
    bSolid.SetColor( 1.f, 1.f, 0.f, 0.5f );
    bSolid.SetPosition( 0.5f, 0.f, .0f );
    auto nm_bg_c = bSolid.CreateNode( "node_mask_bg_col", true );

    // Node mask foreground
    bTex.SetW( 2.35f );
    bTex.SetH( 0.95f );
    bTex.SetPosition( 0.0f, 0.f, .1f );
    bTex.SetTextureFile( "rsrcy/hm_background.png", true );
    auto nm_fg = bTex.CreateNode( "node_mask_fg", true );

    // Build the scene
    root->AddChildToModelOnly( v );
    v->AddChildToModelOnly( vs );
    v->AddChildToModelOnly( vt );

    root->AddChildToModelOnly( am );

    root->AddChildToModelOnly( nm );
    nm->AddChildToModelOnly( nm_bg );
    nm_bg->AddChildToModelOnly( nm_bg_c );
    nm->AddChildToModelOnly( nm_fg );

    root->AddChildToModelOnly( ov );
    ov->AddChildToModelOnly( ov_s );
    ov->AddChildToModelOnly( ov_t );

    return root;

    /*

    root->AddChildToModelOnly( am );

    root->AddChildToModelOnly( nm );
    nm_bg->AddChildToModelOnly( nm_bg_c );
    nm->AddChildToModelOnly( nm_fg );

    return root;*/
}

// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateLightScatteringTest      ( model::ITimeEvaluatorPtr timeEvaluator )
{
    // FIXME: nrl - reimplement effects
    //TexturedRectNodeBuilder bTex( timeEvaluator, "rsrcy/5.png", false, 3.f, 3.f );

    // // ROOT
    //auto root = bTex.CreateNode( "root", true );

    //root->SetNodeEffect( model::ModelNodeEffectFactory::CreateModelNodeEffect( NodeEffectType::NET_LIGHT_SCATTERING, "light scattering", timeEvaluator ) );

    //return root;

    return nullptr;
}

// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateBlurTest               ( model::ITimeEvaluatorPtr timeEvaluator )
{
    //TexturedRectNodeBuilder bTex( timeEvaluator, "rsrcy/butterfly1.png", false, 3.f, 3.f );
    ////bTex.SetScale( 5.f, 5.f, 1.f, 0.f );
    ////bTex.SetScale( 1.f, 1.f, 1.f, 5.f );
    ////bTex.SetScale( 0.3f, 0.3f, 1.f, 20.f );
    //auto root = bTex.CreateNode( "root", true );

    ////auto text = CreateTextNode( timeEvaluator, 0, false );

    ////root->AddChildToModelOnly( text );

    //root->SetNodeEffect( model::ModelNodeEffectFactory::CreateModelNodeEffect( NodeEffectType::NET_BLUR, "blur", timeEvaluator ) );

    //return root;

    // FIXME: nrl - implement in NNodeFactory
    {timeEvaluator;}
    return nullptr;
}

// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateShadowTest               ( model::ITimeEvaluatorPtr timeEvaluator )
{
    //TexturedRectNodeBuilder bTex( timeEvaluator, "rsrcy/butterfly1.png", false, 3.f, 3.f );
    ////bTex.SetScale( 5.f, 5.f, 1.f, 0.f );
    ////bTex.SetScale( 1.f, 1.f, 1.f, 5.f );
    ////bTex.SetScale( 0.3f, 0.3f, 1.f, 20.f );
    //auto root = bTex.CreateNode( "root", true );

    //root->SetNodeEffect( model::ModelNodeEffectFactory::CreateModelNodeEffect( NodeEffectType::NET_SHADOW, "shadow", timeEvaluator ) );

    ////auto text = CreateTextNode( timeEvaluator, 0, false );

    ////root->AddChildToModelOnly( text );

    ////text->SetNodeEffect( model::ModelNodeEffectFactory::CreateModelNodeEffect( NodeEffectType::NET_SHADOW, "shadow", timeEvaluator ) );
    ////text->SetNodeEffect( model::ModelNodeEffectFactory::CreateModelNodeEffect( NodeEffectType::NET_BLUR, "blur", timeEvaluator ) );

    //return root;

    // FIXME: nrl - implement in NNodeFactory
    {timeEvaluator;}
    return nullptr;
}


// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateImageMaskTest               ( model::ITimeEvaluatorPtr timeEvaluator )
{
    // FIXME: nrl - reimplement effects
    ////auto root = CreateTextNode( timeEvaluator, 0, false );

    //auto root = CreateTextureAnimationRectNode( timeEvaluator, false );

    ////auto effect = model::ModelNodeEffectFactory::CreateModelNodeEffect( NodeEffectType::NET_IMAGE_MASK, "image mask", timeEvaluator );

    //auto pm = ProjectManager::GetInstance();

    //auto desc = pm->GetAssetDesc( "", "textures", "butterfly1.png" );

    //effect->AddAsset( desc, 0 );

    //root->SetNodeEffect( effect );

    //return root;
    return nullptr;
}

// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateMultiShadowTest          ( model::ITimeEvaluatorPtr timeEvaluator )
{
    TexturedRectNodeBuilder bTex( timeEvaluator, "rsrcy/butterfly1.png", false, 3.f, 3.f );
    //bTex.SetScale( 5.f, 5.f, 1.f, 0.f );
    //bTex.SetScale( 1.f, 1.f, 1.f, 5.f );
    //bTex.SetScale( 0.3f, 0.3f, 1.f, 20.f );
    auto root = bTex.CreateNode( "root", true );

    auto n1 = CreateShadowTest( timeEvaluator );

    auto simpleTransform = n1->GetPlugin( "transform" )->GetParameter( "simple_transform" );
    simpleTransform->SetTimeEvaluator( timeEvaluator );

    SetParameterTranslation( simpleTransform, 0.0f, glm::vec3( 0.f, 0.2f, 0.f ) );

    root->AddChildToModelOnly( n1 );

    auto n2 = CreateShadowTest( timeEvaluator );

    simpleTransform = n2->GetPlugin( "transform" )->GetParameter( "simple_transform" );
    simpleTransform->SetTimeEvaluator( timeEvaluator );

    SetParameterTranslation( simpleTransform, 0.0f, glm::vec3( 0.f, 0.4f, 0.f ) );

    root->AddChildToModelOnly( n2 );
    
    auto n3 = CreateShadowTest( timeEvaluator );

    simpleTransform = n3->GetPlugin( "transform" )->GetParameter( "simple_transform" );
    simpleTransform->SetTimeEvaluator( timeEvaluator );

    SetParameterTranslation( simpleTransform, 0.0f, glm::vec3( 0.f, 0.6f, 0.f ) );

    root->AddChildToModelOnly( n3 );

    return root;
}

namespace 
{

model::BasicNodePtr NewEmptyNodeWithTransform( model::ITimeEvaluatorPtr timeEvaluator )
{
    std::vector< std::string > uids;

    uids.push_back( "DEFAULT_TRANSFORM" );

    auto node = model::BasicNode::Create( "", timeEvaluator );
    assert( node );
    
    auto success = node->AddPlugins( uids, timeEvaluator );
    { success; } // FIXME: suppress unused warning
    assert( success );

    return node;
}

model::BasicNodePtr CreateNodeInPos( TexturedRectNodeBuilder & bTex, Float32 X, Float32 Y, std::string name, model::BasicNodePtr root )
{
    bTex.SetPosition( X, Y, 1.0f );
    auto node = bTex.CreateNode( "root", true );
    root->AddChildToModelOnly( node );

    return node;
}

} // anonymous

// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateNodeMultiGlobalEffectTest    ( model::ITimeEvaluatorPtr timeEvaluator )
{
    //auto root = NewEmptyNodeWithTransform( timeEvaluator );

    //TexturedRectNodeBuilder bTex( timeEvaluator, "textures/flagi_faliste/australia.tif", false, 0.6f, 0.6f );

    //auto posX = -1.9f;
    //auto posY = 1.0f;

    //// ROOT
    //auto e0node = CreateNodeInPos( bTex, posX, posY, "e0node", root );

    //posX += 0.7f;
    //CreateNodeInPos( bTex, posX, posY, "e1node", root );

    //posX += 0.7f;
    //CreateNodeInPos( bTex, posX, posY, "e2node", root );

    //posX += 0.7f;
    //CreateNodeInPos( bTex, posX, posY, "e3node", root );

    //posX += 0.7f;
    //CreateNodeInPos( bTex, posX, posY, "e4node", root );

    //posX += 0.7f;
    //CreateNodeInPos( bTex, posX, posY, "e5node", root );

    //posX = -1.9f;
    //posY -= 0.6f;
    //CreateNodeInPos( bTex, posX, posY, "e6node", root );

    //posX += 0.7f;
    //CreateNodeInPos( bTex, posX, posY, "e7node", root );

    //posX += 0.7f;
    //CreateNodeInPos( bTex, posX, posY, "e8node", root );

    //posX += 0.7f;
    //CreateNodeInPos( bTex, posX, posY, "e9node", root );

    //posX += 0.7f;
    //CreateNodeInPos( bTex, posX, posY, "e10node", root );

    //posX += 0.7f;
    //CreateNodeInPos( bTex, posX, posY, "e11node", root );

    //posX = -1.9f;
    //posY -= 0.6f;
    //CreateNodeInPos( bTex, posX, posY, "e12node", root );

    //posX += 0.7f;
    //CreateNodeInPos( bTex, posX, posY, "e13node", root );

    //posX += 0.7f;
    //CreateNodeInPos( bTex, posX, posY, "e14node", root );

    //posX += 0.7f;
    //CreateNodeInPos( bTex, posX, posY, "e15node", root );

    //posX += 0.7f;
    //CreateNodeInPos( bTex, posX, posY, "e16node", root );

    //posX += 0.7f;
    //CreateNodeInPos( bTex, posX, posY, "e17node", root );

    //posX = -1.9f;
    //posY -= 0.6f;
    //CreateNodeInPos( bTex, posX, posY, "e18node", root );

    //posX += 0.7f;
    //auto e19node = CreateNodeInPos( bTex, posX, posY, "e19node", root );
    //auto eff = model::ModelNodeEffectFactory::CreateModelNodeEffect( NodeEffectType::NET_IMAGE_MASK, "shadow", timeEvaluator );

    //auto pm = ProjectManager::GetInstance();

    //auto desc = pm->GetAssetDesc( "", "textures", "butterfly1.png" );

    //eff->AddAsset( desc, 0 );
    //e19node->SetNodeEffect( eff );

    //posX += 0.7f;
    //auto e20node = CreateNodeInPos( bTex, posX, posY, "e20node", root );
    //e20node->SetNodeEffect( model::ModelNodeEffectFactory::CreateModelNodeEffect( NodeEffectType::NET_SHADOW, "shadow", timeEvaluator ) );

    //posX += 0.7f;
    //auto e21node = CreateNodeInPos( bTex, posX, posY, "e21node", root );
    //e21node->SetNodeEffect( model::ModelNodeEffectFactory::CreateModelNodeEffect( NodeEffectType::NET_BLUR, "blur", timeEvaluator ) );

    //posX += 0.7f;
    //auto e22node = CreateNodeInPos( bTex, posX, posY, "e22node", root );
    //e22node->SetNodeEffect( model::ModelNodeEffectFactory::CreateModelNodeEffect( NodeEffectType::NET_BLUR, "blur", timeEvaluator ) );

    //posX += 0.7f;
    //auto e23node = CreateNodeInPos( bTex, posX, posY, "e23node", root );
    //e23node->SetNodeEffect( model::ModelNodeEffectFactory::CreateModelNodeEffect( NodeEffectType::NET_BLUR, "blur", timeEvaluator ) );


    //// NET_IMAGE_MASK
    //// NET_LIGHT_SCATTERING,
    //// NET_SHADOW,
    //// NET_BLUR,

    ////root->SetNodeEffect( model::ModelNodeEffectFactory::CreateModelNodeEffect( NodeEffectType::NET_LIGHT_SCATTERING, "light scattering", timeEvaluator ) );

    //return root;

    // FIXME: nrl - implement in NNodeFactory
    {timeEvaluator;}
    return nullptr;
}

// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateNodeReplicatorTest       ( model::ITimeEvaluatorPtr timeEvaluator )
{
    // FIXME: nrl - reimplement effects
    //TexturedRectNodeBuilder bTex( timeEvaluator, "rsrcy/rus.jpg", false, 0.6f, 0.6f );

    //// ROOT
    //auto root = bTex.CreateNode( "root", true );

    //auto shiftRepMod = nodelogic::ShiftReplicationModifier::Create();

    //nodelogic::ParamValDelta delta;

    //delta.deltaTime = 0.f;
    //delta.startTime = 0.f;
    //auto v = std::make_shared< ValueVec3 >( "" );
    //delta.delta = v;
    //v->SetValue( glm::vec3( 0.4f, 0.4f, 0.0 ) );

    //shiftRepMod->AddParamShift( "transform", "translation", delta );

    //auto repLogic = nodelogic::NodeReplicator::Create( root, 5, shiftRepMod );

    //auto image = bTex.CreateNode( "piateczka", true );

    //root->AddChildToModelOnly( image );

    //root->SetLogic( repLogic );

    //root->SetNodeEffect( model::ModelNodeEffectFactory::CreateModelNodeEffect( NodeEffectType::NET_LIGHT_SCATTERING, "light scattering", timeEvaluator ) );

    //return root;

    return nullptr;
}

// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateOverrideAlphaTest  ( model::ITimeEvaluatorPtr timeEvaluator )
{
    TexturedRectNodeBuilder bTex( timeEvaluator, "rsrcy/simless_01.jpg", false, 3.4f, 0.7f );
    SolidRectNodeBuilder bSolid( timeEvaluator, glm::vec4( 0.f, 1.f, 1.f, 0.75f ), .85f, 0.31f );

    bTex.SetPosition( 0.f, -.6f, 0.f );
    auto root = bTex.CreateNode( ".", true );

    bTex.SetW( .85f );
    bTex.SetH( 0.31f );
    bTex.SetPosition( 1.3f, -0.17f, 0.01f, 0.0f );
    bTex.SetTextureFile( "rsrcy/simless_00.jpg", false );
    bTex.SetPosition( -1.f,  -0.17f, 0.01f, 0.0f );
    bTex.SetPosition( -1.1f, -0.17f, 0.01f, 2.0f );
    bTex.SetPosition( -0.2f, -0.17f, 0.01f, 4.0f );
    bTex.SetPosition( 0.5f,  -0.17f, 0.01f, 6.0f );
    bTex.SetPosition( 1.2f,  -0.17f, 0.01f, 9.0f );
    auto c0 = bTex.CreateNode( "node0", true );

    //bSolid.SetColor( 1.f, 0.f, 1.f, 0.2f, 0.f );
    //bSolid.SetColor( 1.f, 0.f, 1.f, 1.0f, 5.f );
    //bSolid.SetColor( 1.f, 0.f, 1.f, 0.1f, 10.f );
    //bSolid.SetColor( 1.f, 1.f, 1.f, 0.8f, 15.f );
    bSolid.SetPosition( 1.3f, 0.17f, 0.01f, 0.0f );
    bSolid.SetPosition( 0.6f, 0.17f, 0.01f, 4.0f );
    bSolid.SetPosition( -0.5f, 0.17f, 0.01f, 7.0f );
    bSolid.SetPosition( -1.2f, 0.17f, 0.01f, 8.0f );
    bSolid.SetPosition( -0.3f, 0.17f, 0.01f, 9.0f );
    bSolid.SetPosition(  1.2f, 0.17f, 0.01f, 14.0f );
    auto c1 = bSolid.CreateNode( "node1" );

    //bSolid.SetColor( 0.f, 1.f, 0.f, 1.0f, 0.f );
    bSolid.SetW( 0.25f );
    bSolid.SetH( 0.25f );
    bSolid.SetPosition( -0.2f, 0.f, 0.01f );
    auto c00 = bSolid.CreateNode( "node01" );

    bSolid.SetColor( 0.f, 0.f, 1.f, 1.0f, 0.f );
    bSolid.SetW( 0.28f );
    bSolid.SetH( 0.28f );
    bSolid.SetPosition( 0.2f, 0.f, 0.01f );
    auto c01 = bSolid.CreateNode( "node02" );

    bTex.SetTextureFile( "rsrcy/simless_01.jpg", false );
    bTex.SetW( .15f );
    bTex.SetH( 0.15f );
    bTex.SetPosition( 0.f, 0.f, 0.01f );
    bTex.SetRotation( 0.f, 0.f, 0.0f, 0.f );
    bTex.SetRotation( 0.f, 0.f, 360.0f, 15.f );
    auto c010 = bTex.CreateNode( "node010", true );

    root->AddChildToModelOnly( c0 );
    root->AddChildToModelOnly( c1 );
    c0->AddChildToModelOnly( c00 );
    c0->AddChildToModelOnly( c01 );
    c01->AddChildToModelOnly( c010 );

    auto ai = TestAIManager::Instance().GetAIPreset( 3, root );
    { ai; } // FIXME: suppress unused warning

    return root;
}

// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateOverrideNodeMaskTest ( model::ITimeEvaluatorPtr timeEvaluator )
{
    TexturedRectNodeBuilder bTex( timeEvaluator, "rsrcy/simless_00.jpg", false, 3.4f, 1.7f );
    SolidRectNodeBuilder bSolid( timeEvaluator, glm::vec4( 0.f, 1.f, 1.f, 0.75f ), .85f, 0.31f );

    bTex.SetPosition( 0.f, -.6f, 0.f );
    auto root = bTex.CreateNode( ".", true );

    bTex.SetH( 0.7f );
    bTex.SetW( 3.0f );
    bTex.SetTextureFile( "rsrcy/simless_01.jpg", false );
    auto cm1 = bTex.CreateNode( "nodem1", true );

    bSolid.SetColor( 0.f, 1.f, 1.f, 0.75f, 0.f );
    bSolid.SetColor( 0.f, 1.f, 1.f, 1.f, 5.f );
    bSolid.SetColor( 0.f, 1.f, 1.f, 1.f, 9.f );
    bSolid.SetColor( 0.f, 1.f, 1.f, 0.7f, 10.f );
    bSolid.SetColor( 0.f, 1.f, 1.f, 0.95f, 12.f );
    bSolid.SetPosition( 1.3f, 0.17f, 0.01f, 0.0f );
    bSolid.SetPosition( 0.6f, 0.17f, 0.01f, 4.0f );
    bSolid.SetPosition( -0.5f, 0.17f, 0.01f, 7.0f );
    bSolid.SetPosition( -1.2f, 0.17f, 0.01f, 8.0f );
    bSolid.SetPosition( -0.3f, 0.17f, 0.01f, 9.0f );
    bSolid.SetPosition(  1.2f, 0.17f, 0.01f, 14.0f );
    auto c0 = bSolid.CreateNode( "node0" );

    root->AddChildToModelOnly( cm1 );
    cm1->AddChildToModelOnly( c0 );

    auto ai = TestAIManager::Instance().GetAIPreset( 6, root );
    { ai; } // FIXME: suppress unuse warning

    return root;
}

// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateOverrideNodeMaskTest1 ( model::ITimeEvaluatorPtr timeEvaluator )
{
    TexturedRectNodeBuilder bTex( timeEvaluator, "rsrcy/simless_00.jpg", false, 3.4f, 1.7f );
    SolidRectNodeBuilder bSolid( timeEvaluator, glm::vec4( 0.f, 1.f, 1.f, 0.75f ), .85f, 0.31f );

    bTex.SetPosition( 0.f, -.6f, 0.f );
    auto root = bTex.CreateNode( "nodem1", true );

    bTex.SetH( 0.7f );
    bTex.SetW( 3.0f );
    bTex.SetTextureFile( "rsrcy/simless_01.jpg", false );
    auto cm1 = bTex.CreateNode( "nodem2", true );

    bSolid.SetColor( 1.f, 0.f, 1.f, 1.f, 0.f );
    bSolid.SetColor( 1.f, 0.f, 1.f, 1.f, 5.f );
    bSolid.SetColor( 1.f, 0.f, 0.f, 1.f, 9.f );
    bSolid.SetColor( 1.f, 0.f, 1.f, 1.f, 10.f );
    bSolid.SetColor( 1.f, 0.f, 1.f, 1.f, 12.f );
    bSolid.SetPosition( 1.3f, 0.17f, 0.01f, 0.2f );
    bSolid.SetPosition( 0.6f, 0.17f, 0.01f, 3.0f );
    bSolid.SetPosition( -0.5f, 0.17f, 0.01f, 5.0f );
    bSolid.SetPosition( -1.2f, 0.17f, 0.01f, 7.0f );
    bSolid.SetPosition( -0.3f, 0.17f, 0.01f, 8.0f );
    bSolid.SetPosition(  1.2f, 0.17f, 0.01f, 12.0f );
    auto cm2 = bSolid.CreateNode( "nodem22" );

    bSolid.SetW( 1.f );
    bSolid.SetH( 0.2f );
    bSolid.SetColor( 0.f, 1.f, 1.f, 0.75f, 0.f );
    bSolid.SetColor( 0.f, 1.f, 1.f, 1.f, 5.f );
    bSolid.SetColor( 0.f, 1.f, 1.f, 1.f, 9.f );
    bSolid.SetColor( 0.f, 1.f, 1.f, 0.7f, 10.f );
    bSolid.SetColor( 0.f, 1.f, 1.f, 0.95f, 12.f );
    bSolid.SetPosition( 1.3f, 0.17f, 0.01f, 0.0f );
    bSolid.SetPosition( 0.6f, 0.17f, 0.01f, 4.0f );
    bSolid.SetPosition( -0.5f, 0.17f, 0.01f, 7.0f );
    bSolid.SetPosition( -1.2f, 0.17f, 0.01f, 8.0f );
    bSolid.SetPosition( -0.3f, 0.17f, 0.01f, 9.0f );
    bSolid.SetPosition(  1.2f, 0.17f, 0.01f, 14.0f );
    auto c0 = bSolid.CreateNode( "nodem3" );

    root->AddChildToModelOnly( cm1 );
    cm1->AddChildToModelOnly( cm2 );
    root->AddChildToModelOnly( c0 );

    auto ai = TestAIManager::Instance().GetAIPreset( 6, root );
    { ai; } // FIXME: suppress unuse warning

    return root;
}

// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateGreenRectNode( model::ITimeEvaluatorPtr timeEvaluator, const std::string & nodeName )
{
    //Plugin list
    std::vector< std::string > uids;

    uids.push_back( "DEFAULT_TRANSFORM" );
    uids.push_back( "DEFAULT_RECTANGLE" );
    uids.push_back( "DEFAULT_COLOR" );

    //Create a model
    model::BasicNodePtr root = model::BasicNode::Create( nodeName, timeEvaluator );

    bool success = root->AddPlugins( uids, timeEvaluator );
    assert( success );

    //Set some values to make it look like a scene
    //SetDefaultTransformAnim( root->GetPlugin( "transform" ) );

    auto color = root->GetPlugin( "solid color" )->GetParameter( "color" );
    assert( color );

    auto w = root->GetPlugin( "rectangle" )->GetParameter( "width" );
    auto h = root->GetPlugin( "rectangle" )->GetParameter( "height" );

    success &= SetParameter( w, 0.f, 2.f );
    success &= SetParameter( h, 0.f, 2.f );

    //success &= SetParameter( w, 20.f, 1.f );
    //success &= SetParameter( h, 20.f, 2.f );

    success &= SetParameter( color, 0.f, glm::vec4( 0.f, 1.f, 0.f, 1.f ) );

    assert( success );

    return root;
}

// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateGreenRectNodeNoAssert( model::ITimeEvaluatorPtr timeEvaluator, bool useAlphaMask )
{
        //Timeline stuff
    auto someTimelineWithEvents = model::TimelineHelper::CreateDefaultTimeline( "evt timeline", TimeType( 20.0 ), TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_CLAMP );
    model::TimelineManager::GetInstance()->AddStopEventToTimeline( someTimelineWithEvents, "stop0", TimeType( 5.0 ) );
    model::TimelineManager::GetInstance()->AddStopEventToTimeline( someTimelineWithEvents, "stop1", TimeType( 10.0 ) );

    auto localTimeline = model::TimelineHelper::CreateOffsetTimeEvaluator( "timeline0" , TimeType( 1.0 ) );

    someTimelineWithEvents->AddChild( localTimeline );
    timeEvaluator->AddChild( someTimelineWithEvents );

    std::vector< std::string > GSimplePluginsUIDS( GSimplePlugins0, GSimplePlugins0 + 3 );

    if( useAlphaMask )
    {
        GSimplePluginsUIDS.push_back( "DEFAULT_ALPHA_MASK" );
    }

    
    auto node = model::BasicNode::Create( "Root", timeEvaluator );
    node->AddPlugins( GSimplePluginsUIDS, timeEvaluator );

    SetDefaultColorChangeAnim( node->GetPlugin( "solid color" ) );

    node->GetPlugin( "solid color" )->GetParameter( "color" )->SetTimeEvaluator( localTimeline );

    //SetDefaultTransformAnim( node->GetPlugin( "transform" ) );

    model::SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "height" ), TimeType( 0.f ), 0.2f );
    model::SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "width" ), TimeType( 0.f ),  0.2f );

    if( useAlphaMask )
    {
        bool success = model::LoadTexture( node->GetPlugin( "alpha_mask" ), "bar_mask_red.png" );
        { success; } // FIXME: suppress unused warning
        assert( success );

        //SetDefaultTransformAlphaMaskTex( node->GetPlugin( "alpha_mask" ) );
    }

    return node;
}

// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateOlafRectNode( model::ITimeEvaluatorPtr timeEvaluator)
{
    auto offset5Timeline = model::TimelineHelper::CreateOffsetTimeEvaluator( "5secoffset", TimeType( 5.0 ) ); 
    auto offset3Timeline  = model::TimelineHelper::CreateOffsetTimeEvaluator( "3secoffset", TimeType( 3.0 ) );
    timeEvaluator->AddChild(offset5Timeline);
    timeEvaluator->AddChild(offset3Timeline);

    //Plugin list
    std::vector< std::string > uids;

    uids.push_back( "DEFAULT_TRANSFORM" );
    uids.push_back( "DEFAULT_RECTANGLE" );
    uids.push_back( "DEFAULT_COLOR" );

    //Create a model
    model::BasicNodePtr root = model::BasicNode::Create( "rectNode", timeEvaluator );

    bool success = root->AddPlugins( uids, timeEvaluator );
    assert( success );

    auto simpleTransform = root->GetPlugin( "transform" )->GetParameter( "simple_transform" );
    simpleTransform->SetTimeEvaluator(offset3Timeline);

    SetParameterRotation ( simpleTransform, 0.0f, glm::vec3( 0.f, 0.f, 0.f ) );
    SetParameterRotation ( simpleTransform, 2.0f, glm::vec3( 0.f, 0.f, 360.f ) );

    auto color = root->GetPlugin( "solid color" )->GetParameter( "color" );
    assert( color );

    auto w = root->GetPlugin( "rectangle" )->GetParameter( "width" );
    auto h = root->GetPlugin( "rectangle" )->GetParameter( "height" );

    h->SetTimeEvaluator(offset5Timeline);
    w->SetTimeEvaluator(offset5Timeline);

    success &= SetParameter( w, 0.f, 2.f );
    success &= SetParameter( h, 0.f, 1.f );

    success &= SetParameter( w, 7.f, 1.f );
    success &= SetParameter( h, 7.f, 2.f );

    success &= SetParameter( color, 0.f, glm::vec4( 0.5f, 0.f, 0.f, 1.f ) );
    success &= SetParameter( color, 5.f, glm::vec4( 0.f, 0.f,  0.5f, 1.f) );

    assert( success );

    return root;
}

// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateCreedPrismNode( model::ITimeEvaluatorPtr timeEvaluator, float offset )
{
    auto offset5Timeline = model::TimelineHelper::CreateOffsetTimeEvaluator( "5secoffset", TimeType( 5.0 ) ); 
    auto offset3Timeline  = model::TimelineHelper::CreateOffsetTimeEvaluator( "3secoffset", TimeType( 3.0 ) );
    timeEvaluator->AddChild(offset5Timeline);
    timeEvaluator->AddChild(offset3Timeline);

    //Plugin list
    std::vector< std::string > uids;

    uids.push_back( "DEFAULT_TRANSFORM" );
    uids.push_back( "DEFAULT_PRISM" );
    //uids.push_back( "DEFAULT_COLOR" );
    //uids.push_back( "DEFAULT_TEXTURE" );
    uids.push_back( "DEFAULT_LINEAR_GRADIENT" );

    //Create a model
    model::BasicNodePtr root = model::BasicNode::Create( "rectNode", timeEvaluator );

    bool success = root->AddPlugins( uids, timeEvaluator );
    assert( success );

    //auto n = root->GetPlugin( "prism" )->GetParameter( "n" );
    //success &= SetParameter( n, 0.f, 5.f );
    //assert( success );

    auto simpleTransform = root->GetPlugin( "transform" )->GetParameter( "simple_transform" );
    //simpleTransform->SetTimeEvaluator(offset3Timeline);

// TEXTURE plugin
    if( root->GetPlugin( "texture" ) )
    {
        success = model::LoadTexture( root->GetPlugin( "texture" ), "caption_white.png" );
        success = model::LoadTexture( root->GetPlugin( "texture" ), "time_zones_4.jpg" );
        assert( success );
    }

// COLOR plugin
    if( root->GetPlugin( "solid color" ) )
    {
        auto color = root->GetPlugin( "solid color" )->GetParameter( "color" );
        success &= SetParameter( color, 0.f, glm::vec4( 0.5f, 0.f, 0.f, 1.f ) );
        assert( success );
    }

// TRANSFORM plugin

    SetParameterScale( simpleTransform, 0.0f, glm::vec3( 0.25f, 1.0f, 0.25f ) );
    SetParameterTranslation( simpleTransform, 0.0f, glm::vec3( offset, -.3f, 0.f) );
    //SetParameterTranslation( simpleTransform, 10.0f, glm::vec3( 1.f, 1.0f, -100.f) );
    SetParameterRotation ( simpleTransform, 0.0f, glm::vec3( 20.f, 0.f, 0.f ) );
    //SetParameterRotation ( simpleTransform, 10.0f, glm::vec3( 1.f, 0.f, 0.f ), 360.f );
    //SetParameterRotation ( simpleTransform, 2.0f, glm::vec3( 0.f, 0.f, 1.f ), 360.f );

// LINEAR GRADIENT plugin
    if( root->GetPlugin( "linear_gradient" ) )
    {
        auto color1 = root->GetPlugin( "linear_gradient" )->GetParameter( "color1" );
        assert( color1 );
        success &= SetParameter( color1, 0.f, glm::vec4( 1.0f, 0.f, 0.f, 1.f ) );
        //success &= SetParameter( color1, 5.f, glm::vec4( 0.f, 0.f,  0.5f, 1.f) );
        assert( success );
        auto color2 = root->GetPlugin( "linear_gradient" )->GetParameter( "color2" );
        assert( color2 );
        success &= SetParameter( color2, 0.f, glm::vec4( 0.f, 1.0f, 1.f, 1.f ) );
        //success &= SetParameter( color2, 5.f, glm::vec4( 0.5f, 0.f,  0.5f, 0.5f) );
        assert( success );

        root->GetPlugin( "linear_gradient" )->GetRendererContext()->cullCtx->enabled = false;
    }

    return root;
}

// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateCreedColoredPrismNode( model::ITimeEvaluatorPtr timeEvaluator, float offset )
{
    //Plugin list
    std::vector< std::string > uids;

    uids.push_back( "DEFAULT_TRANSFORM" );
    uids.push_back( "DEFAULT_PRISM" );
    uids.push_back( "DEFAULT_COLOR" );

    //Create a model
    model::BasicNodePtr root = model::BasicNode::Create( "prismNode", timeEvaluator );

    bool success = root->AddPlugins( uids, timeEvaluator );
    assert( success );

    auto simpleTransform = root->GetPlugin( "transform" )->GetParameter( "simple_transform" );

// TEXTURE plugin
    if( root->GetPlugin( "texture" ) )
    {
        success = model::LoadTexture( root->GetPlugin( "texture" ), "textures/sand.jpg" );
        //success = model::LoadTexture( root->GetPlugin( "texture" ), "time_zones_4.jpg" );
        assert( success );
    }

// COLOR plugin
    if( root->GetPlugin( "solid color" ) )
    {
        auto color = root->GetPlugin( "solid color" )->GetParameter( "color" );
        success &= SetParameter( color, 0.f, glm::vec4( 0.5f, 0.f, 0.f, 1.f ) );
        assert( success );
    }

// TRANSFORM plugin

    SetParameterScale( simpleTransform, 0.0f, glm::vec3( 0.25f, 1.0f, 0.25f ) );
    SetParameterTranslation( simpleTransform, 0.0f, glm::vec3( offset, -.3f, 0.f) );
    SetParameterRotation ( simpleTransform, 0.0f, glm::vec3( 20.f, 0.f, 0.f ) );

// LINEAR GRADIENT plugin
    if( root->GetPlugin( "linear_gradient" ) )
    {
        auto color1 = root->GetPlugin( "linear_gradient" )->GetParameter( "color1" );
        assert( color1 );
        success &= SetParameter( color1, 0.f, glm::vec4( 1.0f, 0.f, 0.f, 1.f ) );
        //success &= SetParameter( color1, 5.f, glm::vec4( 0.f, 0.f,  0.5f, 1.f) );
        assert( success );
        auto color2 = root->GetPlugin( "linear_gradient" )->GetParameter( "color2" );
        assert( color2 );
        success &= SetParameter( color2, 0.f, glm::vec4( 0.f, 1.0f, 1.f, 1.f ) );
        //success &= SetParameter( color2, 5.f, glm::vec4( 0.5f, 0.f,  0.5f, 0.5f) );
        assert( success );

        root->GetPlugin( "linear_gradient" )->GetRendererContext()->cullCtx->enabled = false;
    }

    return root;
}

// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateCreedGradedPrismNode( model::ITimeEvaluatorPtr timeEvaluator, float offset )
{
    //Plugin list
    std::vector< std::string > uids;

    uids.push_back( "DEFAULT_TRANSFORM" );
    uids.push_back( "DEFAULT_PRISM" );
    uids.push_back( "DEFAULT_LINEAR_GRADIENT" );

    //Create a model
    model::BasicNodePtr root = model::BasicNode::Create( "prismNode", timeEvaluator );

    bool success = root->AddPlugins( uids, timeEvaluator );
    assert( success );

    auto simpleTransform = root->GetPlugin( "transform" )->GetParameter( "simple_transform" );

// TEXTURE plugin
    if( root->GetPlugin( "texture" ) )
    {
        success = model::LoadTexture( root->GetPlugin( "texture" ), "textures/sand.jpg" );
        //success = model::LoadTexture( root->GetPlugin( "texture" ), "time_zones_4.jpg" );
        assert( success );
    }

// COLOR plugin
    if( root->GetPlugin( "solid color" ) )
    {
        auto color = root->GetPlugin( "solid color" )->GetParameter( "color" );
        success &= SetParameter( color, 0.f, glm::vec4( 0.5f, 0.f, 0.f, 1.f ) );
        assert( success );
    }

// TRANSFORM plugin

    SetParameterScale( simpleTransform, 0.0f, glm::vec3( 0.25f, 1.0f, 0.25f ) );
    SetParameterTranslation( simpleTransform, 0.0f, glm::vec3( offset, -.3f, 0.f) );
    SetParameterRotation ( simpleTransform, 0.0f, glm::vec3( 20.f, 0.f, 0.f ) );

// LINEAR GRADIENT plugin
    if( root->GetPlugin( "linear_gradient" ) )
    {
        auto param = root->GetPlugin( "prism" )->GetParameter( "uv_type" );
        assert( param );
        SetParameter( param, 0.f, float( model::PrismComponent::PrismUVType::LINGRADED ) );

        auto color1 = root->GetPlugin( "linear_gradient" )->GetParameter( "color1" );
        assert( color1 );
        success &= SetParameter( color1, 0.f, glm::vec4( 0.0f, 0.f, 1.f, 1.f ) );
        //success &= SetParameter( color1, 5.f, glm::vec4( 0.f, 0.f,  0.5f, 1.f) );
        assert( success );
        auto color2 = root->GetPlugin( "linear_gradient" )->GetParameter( "color2" );
        assert( color2 );
        success &= SetParameter( color2, 0.f, glm::vec4( 0.f, 1.0f, 0.f, 1.f ) );
        //success &= SetParameter( color2, 5.f, glm::vec4( 0.5f, 0.f,  0.5f, 0.5f) );
        assert( success );

        root->GetPlugin( "linear_gradient" )->GetRendererContext()->cullCtx->enabled = false;
    }

    return root;
}

// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateCreedTexturedPrismNode( model::ITimeEvaluatorPtr timeEvaluator, float offset )
{
    //Plugin list
    std::vector< std::string > uids;

    uids.push_back( "DEFAULT_TRANSFORM" );
    uids.push_back( "DEFAULT_PRISM" );
    uids.push_back( "DEFAULT_TEXTURE" );

    //Create a model
    model::BasicNodePtr root = model::BasicNode::Create( "prismNode", timeEvaluator );

    bool success = root->AddPlugins( uids, timeEvaluator );
    assert( success );

    auto simpleTransform = root->GetPlugin( "transform" )->GetParameter( "simple_transform" );

// TEXTURE plugin
    if( root->GetPlugin( "texture" ) )
    {
        success = model::LoadTexture( root->GetPlugin( "texture" ), "textures/sand.jpg" );
        //success = model::LoadTexture( root->GetPlugin( "texture" ), "Assets/Textures/time_zones_4.jpg" );
        SetParameter( root->GetPlugin( "texture" )->GetResourceStateModel( "Tex0" )->GetParameter( "borderColor" ), 0.f, glm::vec4( 1, 1, 0, 1 ) );
        root->GetPlugin( "texture" )->GetRendererContext()->cullCtx->enabled = false;
        assert( success );
    }

// COLOR plugin
    if( root->GetPlugin( "solid color" ) )
    {
        auto color = root->GetPlugin( "solid color" )->GetParameter( "color" );
        success &= SetParameter( color, 0.f, glm::vec4( 0.5f, 0.f, 0.f, 1.f ) );
        assert( success );
    }

// TRANSFORM plugin

    SetParameterScale( simpleTransform, 0.0f, glm::vec3( 0.25f, 1.0f, 0.25f ) );
    SetParameterTranslation( simpleTransform, 0.0f, glm::vec3( offset, -.3f, 0.f) );
    SetParameterRotation ( simpleTransform, 0.0f, glm::vec3( 20.f, 0.f, 0.f ) );

// LINEAR GRADIENT plugin
    if( root->GetPlugin( "linear_gradient" ) )
    {
        auto color1 = root->GetPlugin( "linear_gradient" )->GetParameter( "color1" );
        assert( color1 );
        success &= SetParameter( color1, 0.f, glm::vec4( 1.0f, 0.f, 0.f, 1.f ) );
        //success &= SetParameter( color1, 5.f, glm::vec4( 0.f, 0.f,  0.5f, 1.f) );
        assert( success );
        auto color2 = root->GetPlugin( "linear_gradient" )->GetParameter( "color2" );
        assert( color2 );
        success &= SetParameter( color2, 0.f, glm::vec4( 0.f, 1.0f, 1.f, 1.f ) );
        //success &= SetParameter( color2, 5.f, glm::vec4( 0.5f, 0.f,  0.5f, 0.5f) );
        assert( success );

        root->GetPlugin( "linear_gradient" )->GetRendererContext()->cullCtx->enabled = false;
    }

    return root;
}

// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateCreedColoredPieChartNode( model::ITimeEvaluatorPtr timeEvaluator, float offset )
{
    { offset; } // FIXME: suppress unuse warning
    //Plugin list
    std::vector< std::string > uids;

    uids.push_back( "DEFAULT_TRANSFORM" );
    uids.push_back( "DEFAULT_PIECHART" );
    uids.push_back( "DEFAULT_COLOR" );

    //Create a model
    model::BasicNodePtr root = model::BasicNode::Create( "pieNode", timeEvaluator );

    bool success = root->AddPlugins( uids, timeEvaluator );
    assert( success );

    auto simpleTransform = root->GetPlugin( "transform" )->GetParameter( "simple_transform" );

// TEXTURE plugin
    if( root->GetPlugin( "texture" ) )
    {
        success = model::LoadTexture( root->GetPlugin( "texture" ), "caption_white.png" );
        success = model::LoadTexture( root->GetPlugin( "texture" ), "time_zones_4.jpg" );
        assert( success );
    }

// COLOR plugin
    if( root->GetPlugin( "solid color" ) )
    {
        auto color = root->GetPlugin( "solid color" )->GetParameter( "color" );
        success &= SetParameter( color, 0.f, glm::vec4( 0.5f, 0.f, 0.f, 1.f ) );
        assert( success );
        root->GetPlugin( "solid color" )->GetRendererContext()->cullCtx->enabled = false;
    }

// TRANSFORM plugin

// TRANSFORM plugin

    //SetParameterScale( simpleTransform, 0.0f, glm::vec3( 1.f, .2f, 1.f ) );
    //SetParameterTranslation( simpleTransform, 0.0f, glm::vec3( offset, 0, -1.f) );
    //SetParameterRotation ( simpleTransform, 0.0f, glm::vec3( 1.f, 0.f, 0.f ), 20.f );

    ////SetParameterScale( simpleTransform, 0.0f, glm::vec3( 0.25f, 1.0f, 0.25f ) );
    //SetParameterTranslation( simpleTransform, 0.0f, glm::vec3( offset, -.3f, 0.f) );
    //SetParameterRotation ( simpleTransform, 0.0f, glm::vec3( 1.f, 0.f, 0.f ), 20.f );

// LINEAR GRADIENT plugin
    if( root->GetPlugin( "linear_gradient" ) )
    {
        auto color1 = root->GetPlugin( "linear_gradient" )->GetParameter( "color1" );
        assert( color1 );
        success &= SetParameter( color1, 0.f, glm::vec4( 1.0f, 0.f, 0.f, 1.f ) );
        //success &= SetParameter( color1, 5.f, glm::vec4( 0.f, 0.f,  0.5f, 1.f) );
        assert( success );
        auto color2 = root->GetPlugin( "linear_gradient" )->GetParameter( "color2" );
        assert( color2 );
        success &= SetParameter( color2, 0.f, glm::vec4( 0.f, 1.0f, 1.f, 1.f ) );
        //success &= SetParameter( color2, 5.f, glm::vec4( 0.5f, 0.f,  0.5f, 0.5f) );
        assert( success );

        root->GetPlugin( "linear_gradient" )->GetRendererContext()->cullCtx->enabled = false;
    }

    return root;
}

// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateCreedGradedPieChartNode( model::ITimeEvaluatorPtr timeEvaluator, float offset )
{
    { offset; } // FIXME: suppress unuse warning
    //Plugin list
    std::vector< std::string > uids;

    uids.push_back( "DEFAULT_TRANSFORM" );
    uids.push_back( "DEFAULT_PIECHART" );
    uids.push_back( "DEFAULT_LINEAR_GRADIENT" );

    //Create a model
    model::BasicNodePtr root = model::BasicNode::Create( "pieNode", timeEvaluator );

    bool success = root->AddPlugins( uids, timeEvaluator );
    assert( success );

    auto simpleTransform = root->GetPlugin( "transform" )->GetParameter( "simple_transform" );

    //auto aeParam = root->GetPlugin( "piechart" )->GetParameter( "angleEnd" );
    //SetParameter( aeParam, 1.f, 3.14f );

// TEXTURE plugin
    if( root->GetPlugin( "texture" ) )
    {
        success = model::LoadTexture( root->GetPlugin( "texture" ), "caption_white.png" );
        success = model::LoadTexture( root->GetPlugin( "texture" ), "time_zones_4.jpg" );
        assert( success );
    }

// COLOR plugin
    if( root->GetPlugin( "solid color" ) )
    {
        auto color = root->GetPlugin( "solid color" )->GetParameter( "color" );
        success &= SetParameter( color, 0.f, glm::vec4( 0.5f, 0.f, 0.f, 1.f ) );
        assert( success );
        root->GetPlugin( "solid color" )->GetRendererContext()->cullCtx->enabled = false;
    }

// TRANSFORM plugin

    //SetParameterScale( simpleTransform, 0.0f, glm::vec3( 1.f, .2f, 1.f ) );
    //SetParameterTranslation( simpleTransform, 0.0f, glm::vec3( offset, 0, -1.f) );
    //SetParameterRotation ( simpleTransform, 0.0f, glm::vec3( 1.f, 0.f, 0.f ), 20.f );

// LINEAR GRADIENT plugin
    if( root->GetPlugin( "linear_gradient" ) )
    {
        auto color1 = root->GetPlugin( "linear_gradient" )->GetParameter( "color1" );
        assert( color1 );
        success &= SetParameter( color1, 0.f, glm::vec4( 0.0f, 0.f, 0.f, 1.f ) );
        //success &= SetParameter( color1, 5.f, glm::vec4( 0.f, 0.f,  0.5f, 1.f) );
        assert( success );
        auto color2 = root->GetPlugin( "linear_gradient" )->GetParameter( "color2" );
        assert( color2 );
        success &= SetParameter( color2, 0.f, glm::vec4( 1.f, 1.0f, 1.f, 1.f ) );
        //success &= SetParameter( color2, 0.f, glm::vec4( 10.f, 10.0f, 10.f, 1.f ) );
        //success &= SetParameter( color2, 5.f, glm::vec4( 0.5f, 0.f,  0.5f, 0.5f) );
        assert( success );

        root->GetPlugin( "linear_gradient" )->GetRendererContext()->cullCtx->enabled = false;
    }

    return root;
}

// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateCreedRectNode( model::ITimeEvaluatorPtr timeEvaluator)
{
    auto offset5Timeline = model::TimelineHelper::CreateOffsetTimeEvaluator( "5secoffset", TimeType( 5.0 ) ); 
    auto offset3Timeline  = model::TimelineHelper::CreateOffsetTimeEvaluator( "3secoffset", TimeType( 3.0 ) );
    timeEvaluator->AddChild(offset5Timeline);
    timeEvaluator->AddChild(offset3Timeline);

    //Plugin list
    std::vector< std::string > uids;

    //uids.push_back( "DEFAULT_TRANSFORM" );
    //uids.push_back( "DEFAULT_RECTANGLE" );
    //uids.push_back( "DEFAULT_LINEAR_GRADIENT" );

    //Create a model
    model::BasicNodePtr root = model::BasicNode::Create( "rectNode", timeEvaluator );

    bool success = root->AddPlugins( uids, timeEvaluator );
    assert( success );

    auto simpleTransform = root->GetPlugin( "transform" )->GetParameter( "simple_transform" );
    simpleTransform->SetTimeEvaluator(offset3Timeline);
    //SetParameterScale( simpleTransform, 0.f, glm::vec3( 0, 0, 0 ) );

// RECT plugin

    auto w = root->GetPlugin( "rectangle" )->GetParameter( "width" );
    auto h = root->GetPlugin( "rectangle" )->GetParameter( "height" );

    success &= SetParameter( w, 0.f, 0.f );
    success &= SetParameter( h, 0.f, 0.f );

// LINEAR GRADIENT plugin

 //   auto color1 = root->GetPlugin( "linear_gradient" )->GetParameter( "color1" );
 //   assert( color1 );
 //   success &= SetParameter( color1, 0.f, glm::vec4( 0.5f, 0.f, 0.f, 1.f ) );
    //success &= SetParameter( color1, 5.f, glm::vec4( 0.f, 0.f,  0.5f, 1.f) );
 //   assert( success );
    //auto color2 = root->GetPlugin( "linear_gradient" )->GetParameter( "color2" );
 //   assert( color2 );
 //   success &= SetParameter( color2, 0.f, glm::vec4( 0.f, 0.5f, 0.f, 1.f ) );
    //success &= SetParameter( color2, 5.f, glm::vec4( 0.5f, 0.f,  0.5f, 0.9f) );
 //   assert( success );


    //root->GetPlugin( "linear_gradient" )->GetRendererContext()->alphaCtx->blendEnabled = true;
    //root->GetPlugin( "linear_gradient" )->GetRendererContext()->->blendEnabled = true;

    return root;
}

// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateCreedTextNode( model::ITimeEvaluatorPtr timeEvaluator)
{
    auto offset5Timeline = model::TimelineHelper::CreateOffsetTimeEvaluator( "5secoffset", TimeType( 5.0 ) ); 
    auto offset3Timeline  = model::TimelineHelper::CreateOffsetTimeEvaluator( "3secoffset", TimeType( 3.0 ) );
    timeEvaluator->AddChild(offset5Timeline);
    timeEvaluator->AddChild(offset3Timeline);

    //Plugin list
    std::vector< std::string > uids;

    uids.push_back( "DEFAULT_TRANSFORM" );
    //uids.push_back( "DEFAULT_RECTANGLE" );
    //uids.push_back( "DEFAULT_COLOR" );
    uids.push_back( "DEFAULT_TEXT" );
    uids.push_back( "DEFAULT_LINEAR_GRADIENT" );
    //uids.push_back( "DEFAULT_ALPHA_MASK" );
    //uids.push_back( "DEFAULT_TEXTURE" );

    //Create a model
    model::BasicNodePtr root = model::BasicNode::Create( "rectNode", timeEvaluator );

    bool success = root->AddPlugins( uids, timeEvaluator );
    assert( success );

    auto simpleTransform = root->GetPlugin( "transform" )->GetParameter( "simple_transform" );
    simpleTransform->SetTimeEvaluator(offset3Timeline);

// TRANSFORM plugin

    //SetParameterRotation ( simpleTransform, 0.0f, glm::vec3( 0.f, 0.f, 1.f ), 0.f );
    //SetParameterRotation ( simpleTransform, 2.0f, glm::vec3( 0.f, 0.f, 1.f ), 360.f );

// RECT plugin

    //auto w = root->GetPlugin( "rectangle" )->GetParameter( "width" );
    //auto h = root->GetPlugin( "rectangle" )->GetParameter( "height" );

    //h->SetTimeEvaluator(offset5Timeline);
    //w->SetTimeEvaluator(offset5Timeline);

    //success &= SetParameter( w, 0.f, 2.f );
    //success &= SetParameter( h, 0.f, 1.f );

    //success &= SetParameter( w, 7.f, 1.f );
    //success &= SetParameter( h, 7.f, 2.f );

// COLOR plugin

    //SetParameter( root->GetPlugin( "solid color" )->GetParameter( "color" ), 0.f, glm::vec4( 0.f, 0.5f, 0.f, 1.f ) );

// TEXT plugin

    auto node = root;
    //SetParameter( node->GetPlugin( "text" )->GetParameter( "fontSize" ), TimeType( 0.0 ), 423.0f );
    //SetParameter( node->GetPlugin( "text" )->GetParameter( "blurSize" ), TimeType( 0.0 ), float( 0 ) );
    SetParameter( node->GetPlugin( "text" )->GetParameter( "spacing" ), TimeType( 0.0 ), 0.0f );
    SetParameter( node->GetPlugin( "text" )->GetParameter( "alignment" ), TimeType( 0.0 ), float( TextAlignmentType::Left ) );
    SetParameter( node->GetPlugin( "text" )->GetParameter( "maxTextLenght" ), TimeType( 0.0 ), 0.0f );
    success = model::LoadFont( node->GetPlugin( "text" ), "../dep/Media/fonts/arial.TTF", 423, 0, 0, 0, false );
    assert( success );

    SetParameter( node->GetPlugin("text")->GetParameter( "text" ), 0.0, std::wstring( L"1238" ) );

//return root;
// LINEAR GRADIENT plugin

    auto color1 = root->GetPlugin( "linear_gradient" )->GetParameter( "color1" );
    assert( color1 );
    success &= SetParameter( color1, 0.f, glm::vec4( 0.5f, 0.f, 0.f, 1.f ) );
    success &= SetParameter( color1, 5.f, glm::vec4( 0.f, 0.f,  0.5f, 1.f) );
    assert( success );
    auto color2 = root->GetPlugin( "linear_gradient" )->GetParameter( "color2" );
    assert( color2 );
    success &= SetParameter( color2, 0.f, glm::vec4( 0.f, 0.5f, 0.f, 1.f ) );
    success &= SetParameter( color2, 5.f, glm::vec4( 0.5f, 0.f,  0.5f, 1.f) );
    assert( success );
    node->GetPlugin( "linear_gradient" )->Update( TimeType( 0.f ) ); //Regenerate all necessary geometry and channels



    return root;
}

namespace 
{
    template< class PluginType >
    std::shared_ptr< PluginType > QuaryPluginTyped( model::IPluginPtr plugin )
    {
        return std::dynamic_pointer_cast< PluginType >( plugin );
    }
}

// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateTexturedRectNode( model::ITimeEvaluatorPtr timeEvaluator, bool useAlphaMask )
{
    //Timeline stuff
    auto someTimelineWithEvents = model::TimelineHelper::CreateDefaultTimeline( "evt timeline", TimeType( 20.0 ), TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_CLAMP );
    model::TimelineManager::GetInstance()->AddStopEventToTimeline( someTimelineWithEvents, "stop0", TimeType( 5.0 ) );
    model::TimelineManager::GetInstance()->AddStopEventToTimeline( someTimelineWithEvents, "stop1", TimeType( 10.0 ) );
    
    auto localTimeline = model::TimelineHelper::CreateOffsetTimeEvaluator( "timeline0" , TimeType( 1.0 ) );

    //someTimelineWithEvents->AddChild( localTimeline );
    timeEvaluator->AddChild( localTimeline );

    //Plugin stuff
    std::vector< std::string > GSimplePluginsUIDS( GSimplePlugins1, GSimplePlugins1 + 2 );

    if( useAlphaMask )
    {
        GSimplePluginsUIDS.push_back( "DEFAULT_ALPHA_MASK" );
    }

    auto node = model::BasicNode::Create( "Root", timeEvaluator );

    auto success = node->AddPlugins( GSimplePluginsUIDS, localTimeline );
    assert( success );

#if 0
    SetParameterScale ( node->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.05f, 0.05f, 1.f ) );
    //SetParameterRotation( node->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 0.f, 1.f, 0.f ), 0.f );
    //SetParameterRotation( node->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 5.f, glm::vec3( 0.f, 1.f, 0.f ), 70.f );
    //SetParameterRotation( node->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 100.f, glm::vec3( 0.f, 1.f, 0.f ), -70.f );
    //SetParameterScale ( node->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 20.0f, glm::vec3( 1.f/15.f, 1.f/15.f, 1.f ) );
    //SetParameterScale ( node->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 40.0f, glm::vec3( 1.f, 1.f, 1.f ) );

    node->GetPlugin( "transform" )->GetParameter( "simple_transform" )->SetTimeEvaluator( timeEvaluator );

    node->GetPlugin( "transform" )->GetParameter( "simple_transform" )->SetTimeEvaluator( timeEvaluator );
    auto plugin = node->GetPlugin( "transform" );
    //auto param = plugin->GetParameter( "simple_transform" );
#endif

    //SetParameterTranslation( param, 0, 0.0f, glm::vec3( 0.0f, 0.f, 0.f ) );

    /*
    SetParameterTranslation( node->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 0.f, 0.f, 0.0f ) );
    SetParameterTranslation( node->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 2.f, glm::vec3( 0.f, 0.f, 0.0f ) );
    SetParameterTranslation( node->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 5.f, glm::vec3( 0.f, 0.f, -2.f ) );
    SetParameterTranslation( node->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 6.f, glm::vec3( 0.f, 0.f, -5.f ) );
    SetParameterTranslation( node->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 12.f, glm::vec3( 0.f, 0.f, 0.f ) );

    SetParameterRotation( node->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 0.f, 1.f, 1.f ), 0.f );
    SetParameterRotation( node->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 4.f, glm::vec3( 1.f, 1.f, 0.f ), 90.f );
    SetParameterRotation( node->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 8.f, glm::vec3( 1.f, 0.f, 1.f ), 180.f );
    SetParameterRotation( node->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 12.f, glm::vec3( 1.f, 0.f, 0.f ), 270.f );
    SetParameterRotation( node->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 16.f, glm::vec3( 0.f, 0.f, 1.f ), 360.f );
    */
    node->GetPlugin( "texture" )->GetParameter( "alpha" )->SetTimeEvaluator( timeEvaluator );

    /*
    model::SetParameter( node->GetPlugin( "texture" )->GetParameter( "alpha" ), TimeType( 0.f ),   0.5f );
    model::SetParameter( node->GetPlugin( "texture" )->GetParameter( "alpha" ), TimeType( 1.f ),   1.f );
    model::SetParameter( node->GetPlugin( "texture" )->GetParameter( "alpha" ), TimeType( 2.f ),   .2f );
    model::SetParameter( node->GetPlugin( "texture" )->GetParameter( "alpha" ), TimeType( 3.f ),   1.f );
    */

    //success = model::LoadTexture( node->GetPlugin( "texture" ), "4float.exr" );
    //success = model::LoadTexture( node->GetPlugin( "texture" ), "4float.exr", MipMapFilterType::BILINEAR );
    success = model::LoadTexture( node->GetPlugin( "texture" ), "textures/sand.jpg", MipMapFilterType::BILINEAR );
    //success = model::LoadTexture( node->GetPlugin( "texture" ), "Desert.jpg", MipMapFilterType::BILINEAR );
    //success = model::LoadTexture( node->GetPlugin( "texture" ), "64bit.png" );

    //success = model::LoadTexture( node->GetPlugin( "texture" ), "sand.jpg" );

    //success = model::LoadTexture( node->GetPlugin( "texture" ), "0level.bmp", MipMapFilterType::BILINEAR );
    //success = model::LoadTexture( node->GetPlugin( "texture" ), "0level.bmp" );

    //success = model::LoadTexture( node->GetPlugin( "texture" ), "Untitled drawing.png" );
    assert( success );

    auto texturePlugin =  QuaryPluginTyped< model::DefaultTexturePlugin >( node->GetPlugin( "texture" ) );
    if( texturePlugin )
    {
        auto width   = texturePlugin->GetTextureWidth();
        auto height  = texturePlugin->GetTextureHeight();

        //float wf = float( width ) / float( height );
        //float scl = 1.f;

        //model::SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "height" ), TimeType( 0.f ),   2.f * scl );
        //model::SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "width" ), TimeType( 0.f ), wf * 2.f * scl );

        auto rectPlugin = node->GetPlugin( "rectangle" );
        SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "height" ), TimeType( 0.f ),  float(2 * height)/1080.f );
        SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "width" ), TimeType( 0.f ),   float(2 * width)/1080.f );

        rectPlugin->Update( TimeType( 0.f ) );
    }

    if( useAlphaMask )
    {
        success = model::LoadTexture( node->GetPlugin( "alpha_mask" ), "bar_mask_red.png" );
        assert( success );

        node->GetPlugin( "alpha_mask" )->GetParameter( "txAlphaMat" )->SetTimeEvaluator( timeEvaluator );

        SetDefaultTransformAlphaMaskTex( node->GetPlugin( "alpha_mask" ) );
    }

    //auto ai = TestAIManager::Instance().GetAIPreset( 2 );
    //ai->SetTimeline( someTimelineWithEvents );

    return node;    
}

// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateTexturedTextNode( model::ITimeEvaluatorPtr timeEvaluator, bool useAlphaMask )
{
    //Timeline stuff
    auto someTimelineWithEvents = model::TimelineHelper::CreateDefaultTimeline( "evt timeline", TimeType( 20.0 ), TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_CLAMP );
    model::TimelineManager::GetInstance()->AddStopEventToTimeline( someTimelineWithEvents, "stop0", TimeType( 5.0 ) );
    model::TimelineManager::GetInstance()->AddStopEventToTimeline( someTimelineWithEvents, "stop1", TimeType( 10.0 ) );
    
    auto localTimeline = model::TimelineHelper::CreateOffsetTimeEvaluator( "timeline0" , TimeType( 1.0 ) );

    //someTimelineWithEvents->AddChild( localTimeline );
    timeEvaluator->AddChild( localTimeline );

    //Plugin stuff
    //std::vector< std::string > GSimplePluginsUIDS( GSimplePlugins1, GSimplePlugins1 + 3 );

    //if( useAlphaMask )
    //{
    //    GSimplePluginsUIDS.push_back( "DEFAULT_ALPHA_MASK" );
    //}

    //Plugin list
    std::vector< std::string > GSimplePluginsUIDS;

    GSimplePluginsUIDS.push_back( "DEFAULT_TRANSFORM" );
    //uids.push_back( "DEFAULT_RECTANGLE" );
    //uids.push_back( "DEFAULT_COLOR" );
    GSimplePluginsUIDS.push_back( "DEFAULT_TEXT" );
    //uids.push_back( "DEFAULT_LINEAR_GRADIENT" );
    //uids.push_back( "DEFAULT_ALPHA_MASK" );
    GSimplePluginsUIDS.push_back( "DEFAULT_TEXTURE" );


    auto node = model::BasicNode::Create( "Root", timeEvaluator );

    auto success = node->AddPlugins( GSimplePluginsUIDS, localTimeline );
    assert( success );

    //SetDefaultTransformAnim     ( node->GetPlugin( "transform" ) );

    node->GetPlugin( "transform" )->GetParameter( "simple_transform" )->SetTimeEvaluator( timeEvaluator );

        node->GetPlugin( "transform" )->GetParameter( "simple_transform" )->SetTimeEvaluator( timeEvaluator );
    auto plugin = node->GetPlugin( "transform" );
    auto param = plugin->GetParameter( "simple_transform" );

    SetParameterTranslation( param, 0.0f, glm::vec3( 0.0f, 0.f, 0.f ) );

    /*
    SetParameterTranslation( node->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 0.f, 0.f, 0.0f ) );
    SetParameterTranslation( node->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 2.f, glm::vec3( 0.f, 0.f, 0.0f ) );
    SetParameterTranslation( node->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 5.f, glm::vec3( 0.f, 0.f, -2.f ) );
    SetParameterTranslation( node->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 6.f, glm::vec3( 0.f, 0.f, -5.f ) );
    SetParameterTranslation( node->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 12.f, glm::vec3( 0.f, 0.f, 0.f ) );

    SetParameterRotation( node->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 0.f, 1.f, 1.f ), 0.f );
    SetParameterRotation( node->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 4.f, glm::vec3( 1.f, 1.f, 0.f ), 90.f );
    SetParameterRotation( node->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 8.f, glm::vec3( 1.f, 0.f, 1.f ), 180.f );
    SetParameterRotation( node->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 12.f, glm::vec3( 1.f, 0.f, 0.f ), 270.f );
    SetParameterRotation( node->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 16.f, glm::vec3( 0.f, 0.f, 1.f ), 360.f );
    */
    node->GetPlugin( "texture" )->GetParameter( "alpha" )->SetTimeEvaluator( timeEvaluator );

    /*
    model::SetParameter( node->GetPlugin( "texture" )->GetParameter( "alpha" ), TimeType( 0.f ),   0.5f );
    model::SetParameter( node->GetPlugin( "texture" )->GetParameter( "alpha" ), TimeType( 1.f ),   1.f );
    model::SetParameter( node->GetPlugin( "texture" )->GetParameter( "alpha" ), TimeType( 2.f ),   .2f );
    model::SetParameter( node->GetPlugin( "texture" )->GetParameter( "alpha" ), TimeType( 3.f ),   1.f );
    */

    success = model::LoadTexture( node->GetPlugin( "texture" ), "caption_white.png" );
    //success = model::LoadTexture( node->GetPlugin( "texture" ), "Untitled drawing.png" );
    assert( success );

    auto texturePlugin =  QuaryPluginTyped< model::DefaultTexturePlugin >( node->GetPlugin( "texture" ) );
    if( texturePlugin )
    {
        auto width   = texturePlugin->GetTextureWidth();
        auto height  = texturePlugin->GetTextureHeight();

        //float wf = float( width ) / float( height );
        //float scl = 1.f;

        //model::SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "height" ), TimeType( 0.f ),   2.f * scl );
        //model::SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "width" ), TimeType( 0.f ), wf * 2.f * scl );

        SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "height" ), TimeType( 0.f ),  float(height)/540.f );
        SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "width" ), TimeType( 0.f ),   float(width)/540.f );
    }

    if( useAlphaMask )
    {
        success = model::LoadTexture( node->GetPlugin( "alpha_mask" ), "bar_mask_red.png" );
        assert( success );

        node->GetPlugin( "alpha_mask" )->GetParameter( "txAlphaMat" )->SetTimeEvaluator( timeEvaluator );

        SetDefaultTransformAlphaMaskTex( node->GetPlugin( "alpha_mask" ) );
    }

    //auto ai = TestAIManager::Instance().GetAIPreset( 2 );
    //ai->SetTimeline( someTimelineWithEvents );

    return node;    
}

// *****************************
//
model::BasicNodePtr SimpleNodesFactory::CreateTextureAnimationRectNode( model::ITimeEvaluatorPtr timeEvaluator, bool useAlphaMask )
{
    //Timeline stuff
    auto someTimelineWithEvents = model::TimelineHelper::CreateDefaultTimeline( "evt timeline", TimeType( 20.0 ), TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_CLAMP );
    model::TimelineManager::GetInstance()->AddStopEventToTimeline( someTimelineWithEvents, "stop0", TimeType( 2.0 ) );
    model::TimelineManager::GetInstance()->AddStopEventToTimeline( someTimelineWithEvents, "stop1", TimeType( 10.0 ) );
    
    auto localTimeline = model::TimelineHelper::CreateOffsetTimeEvaluator( "timeline0" , TimeType( 0.0 ) );

    someTimelineWithEvents->AddChild( localTimeline );
    timeEvaluator->AddChild( someTimelineWithEvents );

    //Plugin stuf
    std::vector< std::string > GSimplePluginsUIDS( GSimplePlugins2, GSimplePlugins2 + 3 );

    if( useAlphaMask )
    {
        GSimplePluginsUIDS.push_back( "DEFAULT_ALPHA_MASK" );
    }

    auto node = model::BasicNode::Create( "Root", timeEvaluator );

    auto success = node->AddPlugins( GSimplePluginsUIDS, localTimeline );
    assert( success );

    SetParameter( node->GetPlugin( "animation" )->GetParameter( "frameNum" ), TimeType( 0.f ), 0.f );
    SetParameter( node->GetPlugin( "animation" )->GetParameter( "frameNum" ), TimeType( 10.f ), 10.f );

    node->GetPlugin( "animation" )->GetParameter( "frameNum" )->SetTimeEvaluator( timeEvaluator );

    node->GetPlugin( "transform" )->GetParameter( "simple_transform" )->SetTimeEvaluator( timeEvaluator );

    SetParameterTranslation( node->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.2f, 0.3f, 0.f ) );
    //SetParameterTranslation( node->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 10.0f, glm::vec3( 0.0f, 2.f, 0.f ) );

    model::SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "height" ), TimeType( 0.f ), 1.f );
    model::SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "width" ), TimeType( 0.f ), 2.5f );

    success = model::LoadAnimation( node->GetPlugin( "animation" ), "proj02/FullHD/alfai", ".*tga" );      // @fixme Set proper regular expression for filter
    //success = model::LoadAnimation( node->GetPlugin( "animation" ), "d:/src/media/sequences/FullHD/alfai/", "*.tga" );
    assert( success );

    if( useAlphaMask )
    {
        success = model::LoadTexture( node->GetPlugin( "alpha_mask" ), "bar_mask_red.png" );
        assert( success );

        node->GetPlugin( "alpha_mask" )->GetParameter( "txAlphaMat" )->SetTimeEvaluator( timeEvaluator );

        SetDefaultTransformAlphaMaskTex( node->GetPlugin( "alpha_mask" ) );
    }

    auto ai = TestAIManager::Instance().GetAIPreset( 2 );
    ai->SetTimeline( someTimelineWithEvents );

    return node;    
}


// *****************************
//
model::BasicNodePtr SimpleNodesFactory::CreateVideoStreamDecoderRectNode( model::ITimeEvaluatorPtr timeEvaluator, bool useAlphaMask )
{
#define USE_AV
//#define USE_AUDIO

    { useAlphaMask; }
    //Timeline stuff
    auto localTimeline = model::TimelineHelper::CreateDefaultTimeline( "timeline0", 400.0f, TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_REPEAT );
    timeEvaluator->AddChild( localTimeline );

#ifdef USE_AV
    std::vector< std::string > GSimplePluginsUIDS( GSimplePlugins7, GSimplePlugins7 + 3 );
#endif

#ifdef USE_AUDIO
    std::vector< std::string > GSimplePluginsUIDS( GSimplePlugins8, GSimplePlugins8 + 2 );
#endif

    auto node = model::BasicNode::Create( "video_node", timeEvaluator );
    //auto node2 = model::BasicNode::Create( "video_node2", timeEvaluator );
    //node->AddChildToModelOnly( node2 );

    auto success = node->AddPlugins( GSimplePluginsUIDS, localTimeline );
    assert( success );
    //success = node2->AddPlugins( GSimplePluginsUIDS, localTimeline );
    //assert( success );


#ifdef USE_AV
    model::SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "height" ), TimeType( 0.f ), 2.f );
    model::SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "width" ), TimeType( 0.f ), 2.5f );

    //http://samples.ffmpeg.org/game-formats/bink/ActivisionLogo.bik
    //success = model::LoadAVStream( node->GetPlugin( "video_decoder" ), "rsrcy/ActivisionLogo.bik", TextureFormat::F_A8R8G8B8 );

    //http://www.cinemartin.com/cinec/_Sample_Videos/Samsung_Galaxy_Note_3/20140117_142047_CINEC_ProRes4444.mov
    //success = model::LoadAVStream( node->GetPlugin( "video_decoder" ), "rsrcy/20140117_142047_CINEC_ProRes4444.mov", TextureFormat::F_A8R8G8B8 );

    //http://download.openbricks.org/sample/H264/big_buck_bunny_480p_H264_AAC_25fps_1800K_short.MP4
    success = model::LoadAVStream( node->GetPlugin( "video_decoder" ), "rsrcy/big_buck_bunny_480p_H264_AAC_25fps_1800K_short.MP4", TextureFormat::F_A8R8G8B8 );
    //success = model::LoadAVStream( node->GetPlugin( "video_decoder" ), "rsrcy/H264_test7_voiceclip_mp4_480x360.mp4", TextureFormat::F_A8R8G8B8 );
    //success = model::LoadAVStream( node->GetPlugin( "video_decoder" ), "adYze0j_460sv.mp4", TextureFormat::F_A8R8G8B8 );
    
    //http://trace.eas.asu.edu/yuv/akiyo/akiyo_cif.7z
    //success = model::LoadAVStream( node->GetPlugin( "video_decoder" ), "rsrcy/akiyo_cif.yuv", TextureFormat::F_A8R8G8B8, 352, 288, 25.0, VideoPixelFormat::VPF_YUV420P );

    //SetParameter( node->GetPlugin( "video_decoder" )->GetParameter( "offset" ), 0.5f, glm::vec2( 2.f, 0.f ) );
    SetParameter( node->GetPlugin( "video_decoder" )->GetParameter( "state" ), 1.0f, model::DefaultAVDecoderPlugin::DecoderMode::PLAY );
    //SetParameter( node->GetPlugin( "video_decoder" )->GetParameter( "state" ), 5.0f, model::DefaultAVDecoderPlugin::DecoderMode::PAUSE );
    //SetParameter( node->GetPlugin( "video_decoder" )->GetParameter( "state" ), 6.0f, model::DefaultAVDecoderPlugin::DecoderMode::PLAY );
    SetParameter( node->GetPlugin( "video_decoder" )->GetParameter( "state" ), 3.0f, model::DefaultAVDecoderPlugin::DecoderMode::STOP );
    SetParameter( node->GetPlugin( "video_decoder" )->GetParameter( "state" ), 4.0f, model::DefaultAVDecoderPlugin::DecoderMode::PLAY );
    //SetParameter( node->GetPlugin( "video_decoder" )->GetParameter( "state" ), 15.0f, model::DefaultAVDecoderPlugin::DecoderMode::STOP );
    //SetParameter( node->GetPlugin( "video_decoder" )->GetParameter( "loopEnabled" ), 0.0f, true );

    //SetParameter( node->GetPlugin( "video_decoder" )->GetParameter( "offset" ), 0.0f, glm::vec2( 5.f, 0.f ) );

    //SetParameter( node->GetPlugin( "video_decoder" )->GetParameter( "offset" ), 5.0f, glm::vec2( 0.f, 0.f ) );
   // SetParameter( node->GetPlugin( "video_decoder" )->GetParameter( "state" ), 5.0f, model::DefaultAVDecoderPlugin::DecoderMode::PLAY );
    //SetParameter( node->GetPlugin( "video_decoder" )->GetParameter( "offset" ), 10.0f, glm::vec2( 7.f, 0.f ) );
    //SetParameter( node->GetPlugin( "video_decoder" )->GetParameter( "state" ), 15.0f, model::DefaultAVDecoderPlugin::DecoderMode::PLAY );
    //SetParameter( node->GetPlugin( "video_decoder" )->GetParameter( "offset" ), 15.0f, glm::vec2( 0.f, 0.f ) );

    //SetParameter( node->GetPlugin( "video_decoder" )->GetParameter( "offset" ), 15.0f, glm::vec2( 0.f, 0.f ) );
#endif

#ifdef USE_AUDIO
    //success = model::LoadAVStream( node->GetPlugin( "audio_decoder" ), "rsrcy/stereo.wav", TextureFormat::F_A8R8G8B8 );
    //success = model::LoadAVStream( node->GetPlugin( "audio_decoder" ), "audio/HelixMobileProducer_test1_MPEG2_Mono_CBR_40kbps_16000Hz.mp3", TextureFormat::F_A8R8G8B8 );
    //success = model::LoadAVStream( node->GetPlugin( "audio_decoder" ), "HelixMobileProducer_test2_MPEG1_Stereo_CBR_96kbps_32000Hz.mp3", TextureFormat::F_A8R8G8B8 );
    success = model::LoadAVStream( node->GetPlugin( "audio_decoder" ), "HelixMobileProducer_test3_MPEG1_Stereo_CBR_256kbps_44100Hz.mp3", TextureFormat::F_A8R8G8B8 );
    //success = model::LoadAVStream( node->GetPlugin( "audio_decoder" ), "NeroSmartTrax_test2_MPEG1_Mono_CBR_64kbps_44100Hz.mp3", TextureFormat::F_A8R8G8B8 );
    //success = model::LoadAVStream( node->GetPlugin( "audio_decoder" ), "QuickTime_test1_LPCM_Mono_CBR_8SS_12000Hz.wav", TextureFormat::F_A8R8G8B8 );
    
    //success = model::LoadAVStream( node->GetPlugin( "audio_decoder" ), "rsrcy/big_buck_bunny_480p_H264_AAC_25fps_1800K_short.MP4", TextureFormat::F_A8R8G8B8 );
    
    SetParameter( node->GetPlugin( "audio_decoder" )->GetParameter( "state" ), 0.0f, model::DefaultAudioDecoderPlugin::DecoderMode::PLAY );
    //SetParameter( node->GetPlugin( "audio_decoder" )->GetParameter( "state" ), 0.5, model::DefaultAudioDecoderPlugin::DecoderMode::PAUSE );
    //SetParameter( node->GetPlugin( "audio_decoder" )->GetParameter( "offset" ), 1.0f, glm::vec2( 55.0f, 0.f ) );
    //SetParameter( node->GetPlugin( "audio_decoder" )->GetParameter( "state" ), 1.5f, model::DefaultAudioDecoderPlugin::DecoderMode::PLAY );
    SetParameter( node->GetPlugin( "audio_decoder" )->GetParameter( "state" ), 5.0f, model::DefaultAudioDecoderPlugin::DecoderMode::PAUSE );
    SetParameter( node->GetPlugin( "audio_decoder" )->GetParameter( "state" ), 6.0f, model::DefaultAudioDecoderPlugin::DecoderMode::PLAY );
    SetParameter( node->GetPlugin( "audio_decoder" )->GetParameter( "state" ), 10.0f, model::DefaultAudioDecoderPlugin::DecoderMode::STOP );
    SetParameter( node->GetPlugin( "audio_decoder" )->GetParameter( "state" ), 11.0f, model::DefaultAudioDecoderPlugin::DecoderMode::PLAY );
    //SetParameter( node->GetPlugin( "audio_decoder" )->GetParameter( "state" ), 15.0f, model::DefaultAudioDecoderPlugin::DecoderMode::STOP );
    //SetParameter( node->GetPlugin( "audio_decoder" )->GetParameter( "loopEnabled" ), 0.0f, true );
    
    //success = model::LoadAVStream( node2->GetPlugin( "audio_decoder" ), "HelixMobileProducer_test2_MPEG1_Stereo_CBR_96kbps_32000Hz.mp3", TextureFormat::F_A8R8G8B8 );
    //SetParameter( node2->GetPlugin( "audio_decoder" )->GetParameter( "state" ), 0.0f, model::DefaultAVDecoderPlugin::DecoderMode::PLAY );
#endif
    
    localTimeline->Play();

    assert( success );

    return node;    
}

// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateTextNode( model::ITimeEvaluatorPtr timeEvaluator, unsigned int blurSize, bool useAlphaMask )
{
    //Timeline stuff
    auto someTimelineWithEvents = model::TimelineHelper::CreateDefaultTimeline( "evt timeline", TimeType( 20.0 ), TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_CLAMP );
    model::TimelineManager::GetInstance()->AddStopEventToTimeline( someTimelineWithEvents, "stop0", TimeType( 5.0 ) );
    model::TimelineManager::GetInstance()->AddStopEventToTimeline( someTimelineWithEvents, "stop1", TimeType( 10.0 ) );
    
    auto localTimeline = model::TimelineHelper::CreateOffsetTimeEvaluator( "timeline0" , TimeType( 0.0 ) );

    someTimelineWithEvents->AddChild( localTimeline );
    timeEvaluator->AddChild( someTimelineWithEvents );

    //Plugin stuff
    //std::vector< std::string > GSimplePluginsUIDS( GSimplePlugins4, GSimplePlugins4 + 2 );
    std::vector< std::string > GSimplePluginsUIDS( GSimplePlugins3, GSimplePlugins3 + 3 );

    if( useAlphaMask )
    {
        GSimplePluginsUIDS.push_back( "DEFAULT_ALPHA_MASK" );
    }

    auto node = model::BasicNode::Create( "Text", timeEvaluator );

    

    auto success = node->AddPlugins( GSimplePluginsUIDS, localTimeline );
    assert( success );

    //SetDefaultTransformAnim     ( node->GetPlugin( "transform" ) );
    node->GetPlugin( "transform" )->GetParameter( "simple_transform" )->SetTimeEvaluator( timeEvaluator );
    auto plugin = node->GetPlugin( "transform" );
    auto param = plugin->GetParameter( "simple_transform" );

    //SetParameterTranslation( param, 0, 0.0f, glm::vec3( 0.f, 0.f, 0.f ) );
    //SetParameterTranslation( param, 0, 0.0f, glm::vec3( 0.1f, 0.f, 0.f ) );

    //SetParameterScale ( param, 0, 0.0f, glm::vec3( 2.f, 2.f, 1.f ) );
    //SetParameterScale ( param, 0, 30.0f, glm::vec3( 12.f, 12.f, 1.f ) );
    //SetParameterScale ( param, 0, 15.0f, glm::vec3( 1.f, 1.f, 1.f ) );
    ////SetParameterScale ( param, 0, 30.0f, glm::vec3( 1.5f, 1.5f, 1.f ) );
    //SetParameterScale ( param, 0, 7.5f, glm::vec3( 0.1f, 0.1f, 1.f ) );
    //SetParameterScale ( param, 0, 0.0f, glm::vec3( 0.002f, 0.002f, 1.f ) );


    //SetParameterRotation ( param, 0, 10.0f, glm::vec3( 0.0f, 0.0f, 1.f ), 360 );

    node->GetPlugin( "solid color" )->GetParameter( "color" )->SetTimeEvaluator( timeEvaluator );
    node->GetPlugin( "text" )->GetParameter( "outlineColor" )->SetTimeEvaluator( timeEvaluator );

    SetParameter( node->GetPlugin( "solid color" )->GetParameter( "color" ), TimeType( 0.0 ), glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ) );
    //SetParameter( node->GetPlugin( "text" )->GetParameter( "outlineColor" ), TimeType( 0.0 ), glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f ) );
    //SetParameter( node->GetPlugin( "solid color" )->GetParameter( "color" ), TimeType( 10.0 ), glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f ) );
    //SetParameter( node->GetPlugin( "text" )->GetParameter( "outlineColor" ), TimeType( 10.0 ), glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f ) );

    SetParameter( node->GetPlugin( "text" )->GetParameter( "spacing" ), TimeType( 0.0 ), 0.f );

    //SetParameter( node->GetPlugin( "text" )->GetParameter( "alignment" ), TimeType( 0.0 ), float( TextAlignmentType::Center ) );
    node->GetPlugin( "text" )->GetParameter( "maxTextLenght" )->SetTimeEvaluator( timeEvaluator );
    //SetParameter( node->GetPlugin( "text" )->GetParameter( "maxTextLenght" ), TimeType( 0.0 ), 0.2f );
    //SetParameter( node->GetPlugin( "text" )->GetParameter( "maxTextLenght" ), TimeType( 5.0 ), 0.1f );
    //SetParameter( node->GetPlugin( "text" )->GetParameter( "maxTextLenght" ), TimeType( 10.0 ), 0.5f );


    //success = model::LoadFont( node->GetPlugin( "text" ), "../dep/Media/fonts/courbi.ttf" );
    //success = model::LoadFont( node->GetPlugin( "text" ), "../dep/Media/fonts/cour.ttf" );
    success = model::LoadFont( node->GetPlugin( "text" ), "fonts/Astera.TTF", 50, blurSize, 0, 0, true );
    //success = model::LoadFont( node->GetPlugin( "text" ), "../dep/Media/fonts/ARIALUNI.TTF" );
    assert( success );

    //model::SetTextPluginContent( node->GetPlugin( "text" ), L"bla bla" );
    //model::SetTextPluginContent( node->GetPlugin( "text" ), L"AAAAAAAA\nBBBBCCCC\nDDDDDDDDD" );
    //model::SetTextPluginContent( node->GetPlugin( "text" ), L"AV::11A-AAAA\nBBBBCCCC\nDDD333DD88\nAAAAAAAA\nB3BBCCCC\nDDDD888DDD" );
//    model::SetTextPluginContent( node->GetPlugin( "text" ), L"AAAAAABBBBCCCCDDDD" );
    //model::DefaultTextPlugin::SetText( node->GetPlugin( "text" ), L"AV::11A-AAAABBBBCCCCDDD333DD88AAAAAAAAB3BBCCCCDDDD888DDD" );
    SetParameter( node->GetPlugin("text")->GetParameter( "text" ), 0.0, std::wstring( L"1234567890000000000000000000000000000000000" ) );

    if( useAlphaMask )
    {
        success = model::LoadTexture( node->GetPlugin( "alpha_mask" ), "bar_mask_red.png" );
        assert( success );

        node->GetPlugin( "alpha_mask" )->GetParameter( "txAlphaMat" )->SetTimeEvaluator( timeEvaluator );

        SetDefaultTransformAlphaMaskTex( node->GetPlugin( "alpha_mask" ) );

        node->GetPlugin( "alpha_mask" )->Update( TimeType( 0.f ) ); //Regenerate all necessary geometry and channels
    }

    auto ai = TestAIManager::Instance().GetAIPreset( 2 );
    ai->SetTimeline( someTimelineWithEvents );

    return node;    
}

// *****************************
//
model::BasicNodePtr	SimpleNodesFactory::ScrollerNodeTestScene( model::ITimeEvaluatorPtr timeEvaluator )
{
    //model::BasicNode::Create( "Root", timeEvaluator );
    //node->AddPlugin( "DEFAULT_TRANSFORM", "transform", timeEvaluator );

    auto node = CreateGreenRectNode( timeEvaluator, "green rect"); 

    auto Scroller = nodelogic::Scroller::Create( node, mathematics::Rect::Create( -1.f, -1.f, 1.f, 1.f ), nullptr );

    node->SetLogic( Scroller );

    auto texture = CreateTexturedRectNode( timeEvaluator, false );
    texture->AddChildToModelOnly( CreateTextNode( timeEvaluator, 0, false ) );
    Scroller->AddNext( texture );
    texture = CreateTexturedRectNode( timeEvaluator, false );
    texture->AddChildToModelOnly( CreateTextNode( timeEvaluator, 0, false ) );
    Scroller->AddNext( texture );
    texture = CreateTexturedRectNode( timeEvaluator, false );
    texture->AddChildToModelOnly( CreateTextNode( timeEvaluator, 0, false ) );
    Scroller->AddNext( texture );
    texture = CreateTexturedRectNode( timeEvaluator, false );
    texture->AddChildToModelOnly( CreateTextNode( timeEvaluator, 0, false ) );
    Scroller->AddNext( texture );
    //Scroller->AddNext( CreateTextNode( timeEvaluator, 0, false ) );
    //Scroller->AddNext( CreateTextNode( timeEvaluator, 0, false ) );
    //Scroller->AddNext( CreateTextNode( timeEvaluator, 0, false ) );
    //Scroller->AddNext( CreateTextNode( timeEvaluator, 0, false ) );
    //Scroller->AddNext( CreateTextNode( timeEvaluator, 0, false ) );
    //Scroller->AddNext( CreateTextNode( timeEvaluator, 0, false ) );

    Scroller->SetSpeed( 0.1f );
    Scroller->SetInterspace( 0.4f );

    Scroller->Finalize();
    Scroller->Start();

    return node;
}

// *****************************
//
model::BasicNodePtr SimpleNodesFactory::CreateTextWithShadowNode(   model::ITimeEvaluatorPtr timeEvaluator,
                                                                    unsigned int blurSize,
                                                                    const glm::vec3 shadowTranslation )
{
    auto shadowNode =  SimpleNodesFactory::CreateTextNode( timeEvaluator, blurSize, false );
    auto transPlugin = shadowNode->GetPlugin( "transform" );

    auto param = transPlugin->GetParameter( "simple_transform" );
    assert( param );

    model::SetParameterTranslation ( param, 0.0f, shadowTranslation );

    auto node =  SimpleNodesFactory::CreateTextNode( timeEvaluator, 0, false );

    transPlugin = node->GetPlugin( "transform" );

    param = transPlugin->GetParameter( "simple_transform" );
    assert( param );

    model::SetParameterTranslation ( param, 0.0f, -shadowTranslation );

  /*  auto tx =  SimpleNodesFactory::CreateTextNode( timelineManager, timeEvaluator, 0 );

    auto plugin = tx->GetPlugin( "transform" );
    param = plugin->GetParameter( "simple_transform" );
    auto plo = tx->GetPlugin( "solid color" );
    auto sp = plo->GetParameter( "color" );
    assert( sp );

    SetParameter( sp, TimeType( 0.0 ), glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f ) );*/

    //auto st = -shadowTranslation;
    //st.z += 0.001f;
    //SetParameterTranslation( param, 0, 0.0f, st );

    shadowNode->AddChildToModelOnly( node );
    //shadowNode->AddChildToModelOnly( tx );

    return shadowNode;
}

// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateTimerNode( model::ITimeEvaluatorPtr timeEvaluator, unsigned int blurSize, bool useAlphaMask )
{
    //Timeline stuff
    auto someTimelineWithEvents = model::TimelineHelper::CreateDefaultTimeline( "evt timeline", TimeType( 20.0 ), TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_CLAMP );
    timeEvaluator->AddChild( someTimelineWithEvents );
    model::TimelineManager::GetInstance()->AddStopEventToTimeline( someTimelineWithEvents, "stop0", TimeType( 5.0 ) );
    model::TimelineManager::GetInstance()->AddStopEventToTimeline( someTimelineWithEvents, "stop1", TimeType( 10.0 ) );
    
    auto localTimeline = model::TimelineHelper::CreateOffsetTimeEvaluator( "timeline0" , TimeType( 3.0 ) );

    someTimelineWithEvents->AddChild( localTimeline );
    timeEvaluator->AddChild( someTimelineWithEvents );

    //Plugin stuff
    std::vector< std::string > GSimplePluginsUIDS( GSimplePlugins5, GSimplePlugins5 + 3 );

    if( useAlphaMask )
    {
        GSimplePluginsUIDS.push_back( "DEFAULT_ALPHA_MASK" );
    }

    auto node = model::BasicNode::Create( "Root", timeEvaluator );

    auto success = node->AddPlugins( GSimplePluginsUIDS, localTimeline );
    assert( success );

    //SetDefaultTransformAnim     ( node->GetPlugin( "transform" ) );

    auto plugin = node->GetPlugin( "transform" );
    auto param = plugin->GetParameter( "simple_transform" );

    SetParameterTranslation( param, 0.0f, glm::vec3( 0.f, 0.1f, 0.f ) );

    SetParameter( node->GetPlugin( "solid color" )->GetParameter( "color" ), TimeType( 0.0 ), glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ) );
    SetParameter( node->GetPlugin( "timer" )->GetParameter( "alignment" ), TimeType( 0.0 ), float( TextAlignmentType::Right ) );

    SetParameter( node->GetPlugin( "timer" )->GetParameter( "spacing" ), TimeType( 0.0 ), 4.f / 1080.f );

    success = model::LoadFont( node->GetPlugin( "timer" ), "fonts/proj00/arial.ttf", 127, blurSize, 0, 0, false );
    assert( success );

    SetTimeTimerPlugin( node->GetPlugin( "timer" ), 0.0f );

    StartTimerPlugin( node->GetPlugin( "timer" ) );
    //StopTimerPlugin( node->GetPlugin( "timer" ) );

    //model::SetTextPluginContent( node->GetPlugin( "text" ), L"bla bla" );
    //model::SetTextPluginContent( node->GetPlugin( "text" ), L"AAAAAAAA\nBBBBCCCC\nDDDDDDDDD" );
    //model::SetTextPluginContent( node->GetPlugin( "text" ), L"AAAAAAAA\nBBBBCCCC\nDDD333DD88\nAAAAAAAA\nB3BBCCCC\nDDDD888DDD" );
//    model::SetTextPluginContent( node->GetPlugin( "text" ), L"AAAAAABBBBCCCCDDDD" );
//    model::SetTextPluginContent( node->GetPlugin( "text" ), L"A" );

    if( useAlphaMask )
    {
        success = model::LoadTexture( node->GetPlugin( "alpha_mask" ), "bar_mask_red.png" );
        assert( success );

        node->GetPlugin( "alpha_mask" )->GetParameter( "txAlphaMat" )->SetTimeEvaluator( timeEvaluator );

        SetDefaultTransformAlphaMaskTex( node->GetPlugin( "alpha_mask" ) );

        node->GetPlugin( "alpha_mask" )->Update( TimeType( 0.f ) ); //Regenerate all necessary geometry and channels
    }

    auto ai = TestAIManager::Instance().GetAIPreset( 2 );
    ai->SetTimeline( someTimelineWithEvents );

    return node;    
}

// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateCreedTimerNode( model::ITimeEvaluatorPtr timeEvaluator, unsigned int blurSize, bool useAlphaMask )
{
    //Timeline stuff
    auto someTimelineWithEvents = model::TimelineHelper::CreateDefaultTimeline( "evt timeline", TimeType( 20.0 ), TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_CLAMP );
    model::TimelineManager::GetInstance()->AddStopEventToTimeline( someTimelineWithEvents, "stop0", TimeType( 5.0 ) );
    model::TimelineManager::GetInstance()->AddStopEventToTimeline( someTimelineWithEvents, "stop1", TimeType( 10.0 ) );
    
    auto localTimeline = model::TimelineHelper::CreateOffsetTimeEvaluator( "timeline0" , TimeType( 3.0 ) );

    someTimelineWithEvents->AddChild( localTimeline );
    timeEvaluator->AddChild( someTimelineWithEvents );

    //Plugin stuff
    std::string plugins[] = { "DEFAULT_TRANSFORM", "DEFAULT_TIMER", "DEFAULT_LINEAR_GRADIENT" };
    //std::string plugins[] = { "DEFAULT_TRANSFORM", "DEFAULT_LINEAR_GRADIENT", "DEFAULT_TIMER" };
    std::vector< std::string > GSimplePluginsUIDS( plugins, plugins+3 );

    if( useAlphaMask )
    {
        GSimplePluginsUIDS.push_back( "DEFAULT_ALPHA_MASK" );
    }

    auto node = model::BasicNode::Create( "Root", timeEvaluator );

    auto success = node->AddPlugins( GSimplePluginsUIDS, localTimeline );
    assert( success );

    //SetDefaultTransformAnim     ( node->GetPlugin( "transform" ) );

    auto plugin = node->GetPlugin( "transform" );
    auto param = plugin->GetParameter( "simple_transform" );

    SetParameterTranslation( param, 0.0f, glm::vec3( 0.f, -1.0f, 0.f ) );

    //SetParameter( node->GetPlugin( "solid color" )->GetParameter( "color" ), TimeType( 0.0 ), glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ) );
    SetParameter( node->GetPlugin( "timer" )->GetParameter( "alignment" ), TimeType( 0.0 ), float( TextAlignmentType::Right ) );

    SetParameter( node->GetPlugin( "timer" )->GetParameter( "spacing" ), TimeType( 0.0 ), 4.f / 1080.f );

    success = model::LoadFont( node->GetPlugin( "timer" ), "fonts/proj00/arial.ttf", 127, blurSize, 0, 0, false );
    assert( success );

    SetTimeTimerPlugin( node->GetPlugin( "timer" ), 0.0f );

    StartTimerPlugin( node->GetPlugin( "timer" ) );

    if( useAlphaMask )
    {
        success = model::LoadTexture( node->GetPlugin( "alpha_mask" ), "bar_mask_red.png" );
        assert( success );

        node->GetPlugin( "alpha_mask" )->GetParameter( "txAlphaMat" )->SetTimeEvaluator( timeEvaluator );

        SetDefaultTransformAlphaMaskTex( node->GetPlugin( "alpha_mask" ) );

        node->GetPlugin( "alpha_mask" )->Update( TimeType( 0.f ) ); //Regenerate all necessary geometry and channels
    }

    auto ai = TestAIManager::Instance().GetAIPreset( 2 );
    ai->SetTimeline( someTimelineWithEvents );

// LINEAR GRADIENT plugin
    auto root = node;
    auto color1 = root->GetPlugin( "linear_gradient" )->GetParameter( "color1" );
    assert( color1 );
    success &= SetParameter( color1, 0.f, glm::vec4( 0.5f, 0.f, 0.f, 1.f ) );
    success &= SetParameter( color1, 5.f, glm::vec4( 0.f, 0.f,  0.5f, 1.f) );
    assert( success );
    auto color2 = root->GetPlugin( "linear_gradient" )->GetParameter( "color2" );
    assert( color2 );
    success &= SetParameter( color2, 0.f, glm::vec4( 0.f, 0.5f, 0.f, 1.f ) );
    success &= SetParameter( color2, 5.f, glm::vec4( 0.5f, 0.f,  0.5f, 1.f) );
    assert( success );
    
    node->GetPlugin( "linear_gradient" )->Update( TimeType( 0.f ) ); //Regenerate all necessary geometry and channels

    return node;    
}

// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateHeightMapNode( model::ITimeEvaluatorPtr timeEvaluator )
{
    //Plugin stuff
    std::vector< std::string > GSimplePluginsUIDS( GSimplePlugins6, GSimplePlugins6 + 3 );

    auto node = model::BasicNode::Create( ".", timeEvaluator );

    auto success = node->AddPlugins( GSimplePluginsUIDS, timeEvaluator );
    assert( success );

    //HeightMap
    success = model::LoadTexture( node->GetPlugin( "height map" ), "bukovina.png" );
    assert( success );

    //HillTexture
    success = model::LoadTexture( node->GetPlugin( "height map" ), "full_hm.png" ); //"hill_tex.jpg" );
    assert( success );

    //CoveredDistanceTexture
    success = model::LoadTexture( node->GetPlugin( "height map" ), "covered_dist_tex_hm.png" );
    assert( success );

    //BackgroundTexture
    success = model::LoadTexture( node->GetPlugin( "height map" ), "MAPA_WYSOKOSCI_LIVE__EDYTOWALNA.jpg" );//"hm_background.png" );
    assert( success );

    model::SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "height" ), TimeType( 0.f ),   2.f );
    model::SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "width" ), TimeType( 0.f ), 2.f * 1920.f / 1080.f );
    
    
    //Intro - slide up
    model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "hmOffsetYInPixels" ), TimeType( 0.f ), -195.f );
    model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "hmOffsetYInPixels" ), TimeType( 0.5f ), -195.f );
    model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "hmOffsetYInPixels" ), TimeType( 1.2f ), 98.f );


    //Intro show covered dist
    model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "coveredDistShowFactor" ), TimeType( 0.f ), 0.f );
    model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "coveredDistShowFactor" ), TimeType( 1.2f ), 0.0f );
    model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "coveredDistShowFactor" ), TimeType( 3.5 ), 1.f );

    model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "startDistInMeters" ), TimeType( 0.f ), 8000.f );
    model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "endDistInMeters" ), TimeType( 0.f ), 166227.f );

    model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "totalDistanceInMeters" ), TimeType( 0.f ), 174227.f );

    //FIXME: REMOVE
    //model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "totalDistanceInMeters" ), TimeType( 0.f ), 174227.f );
    //model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "curDistanceInMeters" ), TimeType( 0.f ), 35000.f );

    //model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "coveredDistShowFactor" ), TimeType( 1.2f ), 0.0f );
    //model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "coveredDistShowFactor" ), TimeType( 3.5 ), 1.f );
    ////model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "coveredDistShowFactor" ), TimeType( 8.5 ), 1.f );

    //model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "scale" ), TimeType( 3.5f ), glm::vec2( 1.f, 1.f ) );
    //model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "scale" ), TimeType( 6.f ), glm::vec2( 12.f, 12.f ) );
    //FIXME: REMOVE

    //Zoom to interesting point
    model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "scale" ), TimeType( 4.1f ), glm::vec2( 1.f, 1.f ) );
    //model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "scale" ), TimeType( 5.f ), glm::vec2( 2 * 2 * 16 * 25.f, 2 * 2 * 8 * 10.f ) );
    //model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "scale" ), TimeType( 5.f ), glm::vec2( 25.f, 10.f ) );
    model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "scale" ), TimeType( 8.f ), glm::vec2( 120.f, 12.f ) );

    model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "scale" ), TimeType( 8.f ), glm::vec2( 12.f, 12.f ) );
    model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "curDistanceInMeters" ), TimeType( 0.f ), 38000.f );

    return node;
#if 0
    //Current state (distance)
    model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "totalDistanceInMeters" ), TimeType( 0.f ), 174227.f );
    model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "curDistanceInMeters" ), TimeType( 0.f ), 86000.f );

    //Some animation examples
    //1. Show some fast riding
    model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "curDistanceInMeters" ), TimeType( 8.5f ), 86000.f );
    model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "curDistanceInMeters" ), TimeType( 46.f ), 87100.f );

    //2. Zoom out afterwards
    model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "scale" ), TimeType( 46.f ), glm::vec2( 120.f, 12.f ) );
    model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "scale" ), TimeType( 47.5f ), glm::vec2( 1.f, 1.f ) );

    //3. FF movement
    model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "curDistanceInMeters" ), TimeType( 47.5f ), 87100.f );
    model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "curDistanceInMeters" ), TimeType( 57.5f ), 127100.f );

    //4. Slow down again
    model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "curDistanceInMeters" ), TimeType( 57.5f ), 127100.f );
    model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "curDistanceInMeters" ), TimeType( 90.5f ), 129100.f );

    //5. Zoom in again
    model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "scale" ), TimeType( 58.f ), glm::vec2( 1.f, 1.f ) );
    model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "scale" ), TimeType( 61.5f ), glm::vec2( 120.f, 12.f ) );

    //6. And zoom out
    model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "scale" ), TimeType( 87.f ), glm::vec2( 120.f, 12.f ) );
    model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "scale" ), TimeType( 97.5f ), glm::vec2( 1.f, 1.f ) );

    //model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "centerX" ), TimeType( 3.f ), .5f );
    //model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "centerX" ), TimeType( 5.f ), .62f );
    //model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "centerY" ), TimeType( 3.f ), .5f );
    //model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "centerY" ), TimeType( 5.f ), .35f );

    //model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "hmShadowOffsetInPixels" ), TimeType( 0.f ), glm::vec2( 0.f, 0.f ) );
    //model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "hmShadowOffsetInPixels" ), TimeType( 2.f ), glm::vec2( -6.f, 6.f ) );
    //model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "hmShadowOffsetInPixels" ), TimeType( 4.f ), glm::vec2( 0.f, 2.f ) );
    //model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "hmShadowOffsetInPixels" ), TimeType( 6.f ), glm::vec2( 6.f, 6.f ) );
    
//    model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "hmGroundLevelHeight" ), TimeType( 0.f ), 0.f );
//    model::SetParameter( node->GetPlugin( "height map" )->GetParameter( "hmGroundLevelHeight" ), TimeType( 10.f ), 300.f );

    return node;
#endif
}

// *****************************
//
model::BasicNodePtr	SimpleNodesFactory::CreateBasicShapesTestNode( model::ITimeEvaluatorPtr timeEvaluator )
{

#define VERSION_TEXTURE
//#define NO_PERSPECTIVE
//#define VERSION_COLOR
#define VERSION_MATERIAL

//#define SHOW_CUBE
//#define SHOW_CYLINDER
//#define SHOW_CONE
//#define SHOW_SPHERE
//#define SHOW_CIRCLE
//#define SHOW_ELLIPSE
//#define SHOW_ROUNDEDRECT
//#define SHOW_TRIANGLE
//#define SHOW_TORUS
//#define SHOW_SPRING
//#define SHOW_GEOSPHERE
//#define SHOW_SIMPLE_CUBE
#define SHOW_COGWHEEL
//#define SHOW_WALL

      //Timeline stuff
    auto someTimelineWithEvents = model::TimelineHelper::CreateDefaultTimeline( "evt timeline", TimeType( 20.0 ), TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_CLAMP );
    model::TimelineManager::GetInstance()->AddStopEventToTimeline( someTimelineWithEvents, "stop0", TimeType( 5.0 ) );
    model::TimelineManager::GetInstance()->AddStopEventToTimeline( someTimelineWithEvents, "stop1", TimeType( 10.0 ) );
    
    auto localTimeline = model::TimelineHelper::CreateOffsetTimeEvaluator( "timeline0" , TimeType( 1.0 ) );

    //someTimelineWithEvents->AddChild( localTimeline );
    timeEvaluator->AddChild( localTimeline );

// ============================================ //
// Plugins stuff
    std::vector< std::string > uids;
    uids.push_back( "DEFAULT_TRANSFORM" );
    
// ============================================ //
// Geometry plugins
#ifdef SHOW_SPHERE
    uids.push_back( "DEFAULT_SPHERE" );
#endif
#ifdef SHOW_GEOSPHERE
    uids.push_back( "DEFAULT_GEOSPHERE" );
#endif
#ifdef SHOW_SIMPLE_CUBE
    uids.push_back( "DEFAULT_SIMPLE_CUBE" );
#endif
#ifdef SHOW_CIRCLE
    uids.push_back( "DEFAULT_CIRCLE" );
#endif
#ifdef SHOW_CUBE
    uids.push_back( "DEFAULT_CUBE" );
#endif
#ifdef SHOW_CYLINDER
    uids.push_back( "DEFAULT_CYLINDER" );
#endif
#ifdef SHOW_CONE
    uids.push_back( "DEFAULT_CONE" );
#endif
#ifdef SHOW_ELLIPSE
    uids.push_back( "DEFAULT_ELLIPSE" );
#endif
#ifdef SHOW_ROUNDEDRECT
    uids.push_back( "DEFAULT_ROUNDEDRECT" );
#endif
#ifdef SHOW_TRIANGLE
    uids.push_back( "DEFAULT_TRIANGLE" );
#endif
#ifdef SHOW_TORUS
    uids.push_back( "DEFAULT_TORUS" );
#endif
#ifdef SHOW_SPRING
    uids.push_back( "DEFAULT_SPRING" );
#endif
#ifdef SHOW_COGWHEEL
    uids.push_back( "DEFAULT_COGWHEEL" );
#endif
#ifdef SHOW_WALL
    uids.push_back( "DEFAULT_WALL" );
#endif

// ============================================ //
// Color or texture plugin
#ifdef VERSION_COLOR
    uids.push_back( "DEFAULT_COLOR" );
#endif

#ifdef VERSION_MATERIAL
    uids.push_back( "DEFAULT_MATERIAL" );
#endif

#ifdef VERSION_TEXTURE
    #ifdef NO_PERSPECTIVE
        uids.push_back( "DEFAULT_NO_PERSPECTIVE_TEXTURE" );
    #else
        uids.push_back( "DEFAULT_TEXTURE" );
    #endif
#endif

    auto root = model::BasicNode::Create( "Root", timeEvaluator );

    auto success = root->AddPlugins( uids, localTimeline );
    assert( success );  { success;  }

// ============================================ //
// Tranformations
    glm::vec3 rotation_axis2( -1.f, 1.f, -1.f );

    SetParameterScale ( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 2.f, 2.f, 2.f ) );
    SetParameterRotation( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 0.f, 0.0f, 0.0f ) );
    SetParameterRotation( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 200.f, glm::vec3( 7200.f, 0.0f, 0.0f ) );
    //SetParameterRotation( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 40.f, rotation_axis2, -720.f );
    SetParameterTranslation( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0, glm::vec3( 0.0, 0.0, -2.0f ) );


// ============================================ //
// Geometry parameters
#ifdef SHOW_CIRCLE
    auto plugin = root->GetPlugin( "circle" );
    //model::SetParameter( plugin->GetParameter( "open angle mode" ), 0.0f, bv::model::DefaultCirclePlugin::CW );
    model::SetParameter( plugin->GetParameter( "open angle mode" ), 0.0f, bv::model::DefaultCirclePlugin::CCW );
    //model::SetParameter( plugin->GetParameter( "open angle mode" ), 0.0f, bv::model::DefaultCirclePlugin::SYMMETRIC );
    model::SetParameter( plugin->GetParameter( "outer radius" ), 0.0f, 1.0f );
    model::SetParameter( plugin->GetParameter( "inner radius" ), 0.0f, 0.3f );
    model::SetParameter( plugin->GetParameter( "tesselation" ), 0.0f, 20 );
    model::SetParameter( plugin->GetParameter( "open angle" ), 0.0f, 360.0f );
#endif
#ifdef SHOW_CONE
    auto plugin = root->GetPlugin( "cone" );
    model::SetParameter( plugin->GetParameter( "tesselation" ), 0.0f, 16 );
    model::SetParameter( plugin->GetParameter( "rounded tip height" ), 0.0f, 0.0f );
    model::SetParameter( plugin->GetParameter( "rounded tip height" ), 10.0f, 0.3f );
    model::SetParameter( plugin->GetParameter( "inner radius" ), 0.0f, 0.2f );
    model::SetParameter( plugin->GetParameter( "outer radius" ), 0.0f, 0.8f );
    model::SetParameter( plugin->GetParameter( "inner height" ), 0.0f, 0.2f );
    model::SetParameter( plugin->GetParameter( "height" ), 0.0f, 0.8f );
    model::SetParameter( plugin->GetParameter( "bevel tesselation" ), 0.0f, 4 );
    model::SetParameter( plugin->GetParameter( "open angle" ), 0.0f, 00.0f );

    // Enums
    model::SetParameter( plugin->GetParameter( "weight center x" ), 0.0, bv::model::DefaultCone::DefaultConePlugin::WeightCenter::CENTER );
    model::SetParameter( plugin->GetParameter( "weight center y" ), 0.0, bv::model::DefaultCone::DefaultConePlugin::WeightCenter::CENTER );
    model::SetParameter( plugin->GetParameter( "weight center z" ), 0.0, bv::model::DefaultCone::DefaultConePlugin::WeightCenter::CENTER );

    model::SetParameter( plugin->GetParameter( "open angle mode" ), 0.0, bv::model::DefaultCone::DefaultConePlugin::OpenAngleMode::SYMMETRIC );
    //model::SetParameter( plugin->GetParameter( "open angle mode" ), 0.0, bv::model::DefaultCone::DefaultConePlugin::OpenAngleMode::CCW );
    //model::SetParameter( plugin->GetParameter( "open angle mode" ), 0.0, bv::model::DefaultCone::DefaultConePlugin::OpenAngleMode::CW );

    model::SetParameter( plugin->GetParameter( "mapping type" ), 0.0, bv::model::DefaultCylinder::DefaultPlugin::MappingType::GOODMAPPING );
#endif
#ifdef SHOW_CYLINDER
    auto plugin = root->GetPlugin( "cylinder" );
    model::SetParameter( plugin->GetParameter( "tesselation" ), 0.0f, 16 );
    model::SetParameter( plugin->GetParameter( "inner radius" ), 0.0f, 0.4f );
    //model::SetParameter( plugin->GetParameter( "inner radius" ), 10.0f, 0.78f );
    model::SetParameter( plugin->GetParameter( "outer radius" ), 0.0f, 0.8f );
    model::SetParameter( plugin->GetParameter( "height" ), 0.0f, 0.8f );
    model::SetParameter( plugin->GetParameter( "open angle" ), 0.0f, 120.0f );
    //model::SetParameter( plugin->GetParameter( "open angle" ), 10.0f, 360.0f );

    model::SetParameter( plugin->GetParameter( "open angle mode" ), 0.0, bv::model::DefaultCone::DefaultConePlugin::OpenAngleMode::SYMMETRIC );
    //model::SetParameter( plugin->GetParameter( "open angle mode" ), 0.0, bv::model::DefaultCone::DefaultConePlugin::OpenAngleMode::CCW );
    //model::SetParameter( plugin->GetParameter( "open angle mode" ), 0.0, bv::model::DefaultCone::DefaultConePlugin::OpenAngleMode::CW );

    model::SetParameter( plugin->GetParameter( "weight center x" ), 0.0, bv::model::DefaultCylinder::DefaultPlugin::WeightCenter::CENTER );
    model::SetParameter( plugin->GetParameter( "weight center y" ), 0.0, bv::model::DefaultCylinder::DefaultPlugin::WeightCenter::CENTER );
    model::SetParameter( plugin->GetParameter( "weight center z" ), 0.0, bv::model::DefaultCylinder::DefaultPlugin::WeightCenter::CENTER );

    model::SetParameter( plugin->GetParameter( "mapping type" ), 0.0, bv::model::DefaultCylinder::DefaultPlugin::MappingType::GOODMAPPING );
#endif
#ifdef SHOW_SPHERE
    auto plugin = root->GetPlugin( "sphere" );
    model::SetParameter( plugin->GetParameter( "vertical stripes" ), 0.0f, 30 );
    model::SetParameter( plugin->GetParameter( "horizontal stripes" ), 0.0f, 30 );
    model::SetParameter( plugin->GetParameter( "radius" ), 0.0f, 0.5f );
    model::SetParameter( plugin->GetParameter( "open angle" ), 0.0f, 0.0f );
    model::SetParameter( plugin->GetParameter( "open angle" ), 10.0f, 360.0f );

    model::SetParameter( plugin->GetParameter( "open angle mode" ), 0.0, bv::model::DefaultCone::DefaultConePlugin::OpenAngleMode::SYMMETRIC );
    //model::SetParameter( plugin->GetParameter( "open angle mode" ), 0.0, bv::model::DefaultCone::DefaultConePlugin::OpenAngleMode::CCW );
    //model::SetParameter( plugin->GetParameter( "open angle mode" ), 0.0, bv::model::DefaultCone::DefaultConePlugin::OpenAngleMode::CW );
#endif
#ifdef SHOW_ELLIPSE
    auto plugin = root->GetPlugin( "ellipse" );
    model::SetParameter( plugin->GetParameter( "outer radius 1" ), 0.0f, 1.0f );
    model::SetParameter( plugin->GetParameter( "tesselation" ), 0.0f, 1.0f );
    model::SetParameter( plugin->GetParameter( "outer radius 2" ), 0.0f, 3.0f );
    model::SetParameter( plugin->GetParameter( "open angle" ), 0.0f, 0.0f );
    model::SetParameter( plugin->GetParameter( "open angle mode" ), 0.0f, bv::model::DefaultEllipsePlugin::OpenAngleMode::CCW );
    //model::SetParameter( plugin->GetParameter( "open angle mode" ), 0.0f, bv::model::DefaultEllipsePlugin::OpenAngleMode::CW );
    //model::SetParameter( plugin->GetParameter( "open angle mode" ), 0.0f, bv::model::DefaultEllipsePlugin::OpenAngleMode::SYMMETRIC );
#endif
#ifdef SHOW_CUBE
    auto plugin = root->GetPlugin( "cube" );
    model::SetParameter( plugin->GetParameter( "bevel" ), 0.0f, 0.2f );
    model::SetParameter( plugin->GetParameter( "dimensions" ), 0.0f, glm::vec3( 1.0, 1.0, 1.0 ) );
    model::SetParameter( plugin->GetParameter( "tesselation" ), 0.0f, 20 );
    //model::SetParameter( plugin->GetParameter( "tesselation" ), 100.0f, 10 );

    model::SetParameter( plugin->GetParameter( "weight center x" ), 0.0, bv::model::DefaultCube::Plugin::WeightCenter::CENTER );
    model::SetParameter( plugin->GetParameter( "weight center y" ), 0.0, bv::model::DefaultCube::Plugin::WeightCenter::CENTER );
    model::SetParameter( plugin->GetParameter( "weight center z" ), 0.0, bv::model::DefaultCube::Plugin::WeightCenter::CENTER );
#endif
#ifdef SHOW_ROUNDEDRECT
    auto plugin = root->GetPlugin( "rounded rect" );
    model::SetParameter( plugin->GetParameter( "bevels" ), 0.0f, glm::vec4( 0.4f, 0.2f, 0.2f, 0.2f ) );
    model::SetParameter( plugin->GetParameter( "size" ), 0.0f, glm::vec2( 2.0, 0.9 ) );
#endif
#ifdef SHOW_TRIANGLE
    auto plugin = root->GetPlugin( "triangle" );
    model::SetParameter( plugin->GetParameter( "point a" ), 0.0f, glm::vec3( 0.0, 1.0, 0.0 ) );
    model::SetParameter( plugin->GetParameter( "point b" ), 0.0f, glm::vec3( -1.0, 0.0, 0.0 ) );
    model::SetParameter( plugin->GetParameter( "point c" ), 0.0f, glm::vec3( 1.0, .0, 0.0 ) );
#endif
#ifdef SHOW_TORUS
    auto plugin = root->GetPlugin( "torus" );
    model::SetParameter( plugin->GetParameter( "radius" ), 0.0f, 0.4f );
    model::SetParameter( plugin->GetParameter( "radius2" ), 0.0f, 0.1f );
    model::SetParameter( plugin->GetParameter( "open angle" ), 0.0f, 0.0f );
    model::SetParameter( plugin->GetParameter( "open angle" ), 10.0f, 360.0f );
    model::SetParameter( plugin->GetParameter( "tesselation" ), 0.0f, 15 );

    model::SetParameter( plugin->GetParameter( "weight center x" ), 0.0, bv::model::DefaultTorus::Plugin::WeightCenter::CENTER );
    model::SetParameter( plugin->GetParameter( "weight center y" ), 0.0, bv::model::DefaultTorus::Plugin::WeightCenter::CENTER );
    model::SetParameter( plugin->GetParameter( "weight center z" ), 0.0, bv::model::DefaultTorus::Plugin::WeightCenter::CENTER );

    model::SetParameter( plugin->GetParameter( "open angle mode" ), 0.0, bv::model::DefaultTorus::Plugin::OpenAngleMode::SYMMETRIC );
    //model::SetParameter( plugin->GetParameter( "open angle mode" ), 0.0, bv::model::DefaultTorus::Plugin::OpenAngleMode::CCW );
    //model::SetParameter( plugin->GetParameter( "open angle mode" ), 0.0, bv::model::DefaultTorus::Plugin::OpenAngleMode::CW );
#endif
#ifdef SHOW_SPRING
    auto plugin = root->GetPlugin( "spring" );
    model::SetParameter( plugin->GetParameter( "radius" ), 0.0f, 0.5f );
    model::SetParameter( plugin->GetParameter( "radius2" ), 0.0f, 0.1f );
    model::SetParameter( plugin->GetParameter( "tesselation" ), 0.0f, 100 );
    model::SetParameter( plugin->GetParameter( "tesselation2" ), 0.0f, 15 );
    model::SetParameter( plugin->GetParameter( "delta" ), 0.0f, 1.0f );
    model::SetParameter( plugin->GetParameter( "turns" ), 0.0f, 6 );

    model::SetParameter( plugin->GetParameter( "weight center x" ), 0.0, bv::model::DefaultSpring::Plugin::WeightCenter::CENTER );
    model::SetParameter( plugin->GetParameter( "weight center y" ), 0.0, bv::model::DefaultSpring::Plugin::WeightCenter::CENTER );
    model::SetParameter( plugin->GetParameter( "weight center z" ), 0.0, bv::model::DefaultSpring::Plugin::WeightCenter::CENTER );

    model::SetParameter( plugin->GetParameter( "mapping type" ), 0.0, bv::model::DefaultSpring::Plugin::MappingType::DOUBLETEXTURE );
#endif
#ifdef SHOW_SIMPLE_CUBE
    auto plugin = root->GetPlugin( "simple cube" );
    model::SetParameter( plugin->GetParameter( "dimensions" ), 0.0f, glm::vec3( 1.0, 1.0, 1.0 ) );
#endif
#ifdef SHOW_COGWHEEL
    auto plugin = root->GetPlugin( "cog wheel" );
    model::SetParameter( plugin->GetParameter( "tesselation" ), 0.0f, 1 );
    model::SetParameter( plugin->GetParameter( "inner radius" ), 0.0f, 0.2f );
    model::SetParameter( plugin->GetParameter( "outer radius" ), 0.0f, 0.8f );
    model::SetParameter( plugin->GetParameter( "height" ), 0.0f, 0.5f );
    model::SetParameter( plugin->GetParameter( "bevel" ), 0.0f, 0.1f );
    model::SetParameter( plugin->GetParameter( "tooth height" ), 0.0f, 0.2f );
    model::SetParameter( plugin->GetParameter( "tooth base length" ), 0.0f, 0.20f );
    model::SetParameter( plugin->GetParameter( "tooth top length" ), 0.0f, 0.02f );
    model::SetParameter( plugin->GetParameter( "teeth number" ), 0.0f, 8 );

    model::SetParameter( plugin->GetParameter( "weight center x" ), 0.0, bv::model::DefaultCogWheel::Plugin::WeightCenter::CENTER );
    model::SetParameter( plugin->GetParameter( "weight center y" ), 0.0, bv::model::DefaultCogWheel::Plugin::WeightCenter::CENTER );
    model::SetParameter( plugin->GetParameter( "weight center z" ), 0.0, bv::model::DefaultCogWheel::Plugin::WeightCenter::CENTER );
#endif
#ifdef SHOW_WALL
    auto plugin = root->GetPlugin( "wall" );
    model::SetParameter( plugin->GetParameter( "tesselation" ), 0.0f, 20 );
    model::SetParameter( plugin->GetParameter( "height" ), 0.0f, 1.0f );
    model::SetParameter( plugin->GetParameter( "width" ), 0.0f, 1.0f );
    model::SetParameter( plugin->GetParameter( "depth" ), 0.0f, 1.0f );
    model::SetParameter( plugin->GetParameter( "angle" ), 0.0f, 80.0f );
    model::SetParameter( plugin->GetParameter( "front" ), 0.0f, true );

#endif

// ============================================ //
// Color, texture plugins parameters
#ifdef VERSION_COLOR
    auto color = root->GetPlugin( "solid color" )->GetParameter( "color" );
    SetParameter( color, 0.f, glm::vec4( 0.5f, 0.f, 1.f, 1.f ) );
    //root->GetPlugin( "solid color" )->GetRendererContext()->cullCtx->isCCWOrdered = false;
    //root->GetPlugin( "solid color" )->GetRendererContext()->cullCtx->enabled = false;
#endif
    
#ifdef VERSION_TEXTURE
    auto texDesc = TextureAssetDesc::Create( "textures/sand.jpg", MipMapFilterType::BILINEAR, true );
    root->GetPlugin( "texture" )->LoadResource( std::static_pointer_cast<const AssetDesc>( texDesc ) );

    model::SetParameter( root->GetPlugin( "texture" )->GetResourceStateModel( "Tex0" )->GetParameter( "wrapModeX" ), 0.0, (float) TextureWrappingMode::TWM_MIRROR );
    model::SetParameter( root->GetPlugin( "texture" )->GetResourceStateModel( "Tex0" )->GetParameter( "wrapModeY" ), 0.0, (float) TextureWrappingMode::TWM_MIRROR );

    //success = model::LoadTexture( root->GetPlugin( "texture" ), "sand.jpg", MipMapFilterType::BILINEAR );	//, MipMapFilterType::BOX
#endif

#ifdef VERSION_MATERIAL
    auto material = root->GetPlugin( "material" );
    model::SetParameter( material->GetParameter( "mtlDiffuse" ), 0.0, glm::vec4( 1, 0, 0, 1 ) );
    model::SetParameter( material->GetParameter( "mtlAmbient" ), 0.0, glm::vec4( 0.2, 0, 0, 1 ) );
    model::SetParameter( material->GetParameter( "mtlSpecular" ), 0.0, glm::vec4( 1.0, 1.0, 1.0, 1.0 ) );
    model::SetParameter( material->GetParameter( "mtlEmission" ), 0.0, glm::vec4( 0.2, 0, 0, 1 ) );
    model::SetParameter( material->GetParameter( "mtlShininess" ), 0.0, 128.0f );
#endif
    
    return root;
    
#undef VERSION_TEXTURE
#undef NO_PERSPECTIVE
#undef VERSION_COLOR
#undef VERSION_MATERIAL
}

// *****************************
//
model::BasicNodePtr	SimpleNodesFactory::CreateBasicShapeShow( model::ITimeEvaluatorPtr timeEvaluator, const std::string& uid, glm::vec3 translation, std::string texturePath )
{
    
#define VERSION_TEXTURE
//#define NO_PERSPECTIVE
//#define VERSION_COLOR
    
        //Timeline stuff
    auto someTimelineWithEvents = model::TimelineHelper::CreateDefaultTimeline( "evt timeline", TimeType( 1000.0 ), TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_CLAMP );

      //Timeline stuff
    //auto localTimeline = model::TimelineHelper::CreateOffsetTimeEvaluator( "timeline0" , TimeType( 0.0 ) );
    
    timeEvaluator->AddChild( someTimelineWithEvents );

// ============================================ //
// Plugins stuff
    std::vector< std::string > uids;
    uids.push_back( "DEFAULT_TRANSFORM" );

// ============================================ //
//  Geometry plugin
    uids.push_back( uid );

// ============================================ //
// Color or texture plugin
#ifdef VERSION_COLOR
    uids.push_back( "DEFAULT_COLOR" );
#endif
#ifdef VERSION_TEXTURE
    #ifdef NO_PERSPECTIVE
        uids.push_back( "DEFAULT_NO_PERSPECTIVE_TEXTURE" );
    #else
        uids.push_back( "DEFAULT_TEXTURE" );
    #endif
#endif

    auto root = model::BasicNode::Create( uid, timeEvaluator );

    auto success = root->AddPlugins( uids, someTimelineWithEvents );
    assert( success );

// ============================================ //
// Tranformations
    glm::vec3 rotation_axis( 0.f, 1.f, 0.f );
    glm::vec3 rotation_axis2( -1.f, 1.f, 0.f );

    SetParameterScale ( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 2.f, 2.f, 2.f ) );
    SetParameterRotation( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 0.f, 0.f, 0.f ) );
    SetParameterRotation( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 100.f, glm::vec3( 0.f, -7200.f, 0.f ) );
    //SetParameterRotation( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 40.f, rotation_axis2, 50.f );
    SetParameterTranslation( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0, translation );

// ============================================ //
// Color, texture plugins parameters
#ifdef VERSION_COLOR
    auto color = root->GetPlugin( "solid color" )->GetParameter( "color" );
    SetParameter( color, 0.f, glm::vec4( 0.5f, 0.f, 1.f, 1.f ) );
    //root->GetPlugin( "solid color" )->GetRendererContext()->cullCtx->isCCWOrdered = false;
    root->GetPlugin( "solid color" )->GetRendererContext()->cullCtx->enabled = false;
#endif
    
#ifdef VERSION_TEXTURE
    success = model::LoadTexture( root->GetPlugin( "texture" ), texturePath, MipMapFilterType::BILINEAR );
    assert( success );
    auto texturePlugin =  root->GetPlugin( "texture" );
    model::SetParameter( texturePlugin->GetResourceStateModel( "Tex0" )->GetParameter( "wrapModeX" ), 0.0, (float) TextureWrappingMode::TWM_MIRROR );
    model::SetParameter( texturePlugin->GetResourceStateModel( "Tex0" )->GetParameter( "wrapModeY" ), 0.0, (float) TextureWrappingMode::TWM_MIRROR );
    //root->GetPlugin( "texture" )->GetRendererContext()->cullCtx->isCCWOrdered = false;
#endif

    return root;


#undef VERSION_TEXTURE
#undef NO_PERSPECTIVE
#undef VERSION_COLOR
}

model::BasicNodePtr SimpleNodesFactory::CreateTextCacheTest         ( model::ITimeEvaluatorPtr timeEvaluator, const std::string& nodeName, glm::vec3 translation, glm::vec4 color, const std::wstring text, const std::string& fontName )
{
    //Timeline stuff
    auto localTimeline = model::TimelineHelper::CreateOffsetTimeEvaluator( "timeline0" , TimeType( 0.0 ) );
    timeEvaluator->AddChild( localTimeline );

    //Plugin stuff
    std::vector< std::string > uids;
    uids.push_back( "DEFAULT_TRANSFORM" );
    uids.push_back( "DEFAULT_COLOR" );
    uids.push_back( "DEFAULT_TEXT" );


    auto node = model::BasicNode::Create( nodeName, timeEvaluator );
    auto success = node->AddPlugins( uids, localTimeline );
    assert( success );

    SetParameterTranslation( node->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, translation );
    SetParameterScale ( node->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 2.f, 2.f, 1.f ) );
    

    //node->GetPlugin( "solid color" )->GetParameter( "color" )->SetTimeEvaluator( timeEvaluator );
    //node->GetPlugin( "text" )->GetParameter( "outlineColor" )->SetTimeEvaluator( timeEvaluator );

    SetParameter( node->GetPlugin( "solid color" )->GetParameter( "color" ), TimeType( 0.0 ), color );
    SetParameter( node->GetPlugin( "text" )->GetParameter( "outlineColor" ), TimeType( 0.0 ), glm::vec4( 1.0f, 1.0f, 0.0f, 1.0f ) );
    SetParameter( node->GetPlugin( "text" )->GetParameter( "spacing" ), TimeType( 0.0 ), 0.f );
    SetParameter( node->GetPlugin( "text" )->GetParameter( "alignment" ), TimeType( 0.0 ), float( TextAlignmentType::Center ) );

    success = model::LoadFont( node->GetPlugin( "text" ), fontName, 30, 0, 0, 0, true );
    assert( success );

    SetParameter( node->GetPlugin("text")->GetParameter( "text" ), 0.0, text );

    return node;    
}



model::BasicNodePtr  SimpleNodesFactory::CreateScrollerTestNode       ( model::ITimeEvaluatorPtr timeEvaluator )
{
    glm::vec3 nodeTranslation( 0.0, 0.0, 0.0 );
    glm::vec4 color( 1.0, 0.7, 0.0, 1.0 );

    auto node = CreateTextCacheTest( timeEvaluator, "Dummy0", nodeTranslation, color, L"", "fonts/StarWars.ttf" );

    //auto Scroller = nodelogic::Scroller::Create( node.get(), mathematics::Rect::Create( -1.f, -1.f, 1.f, 1.f ) );

    //node->SetLogic( Scroller );

    
    //Scroller->AddNext( CreateTextCacheTest( timeEvaluator, "Text", nodeTranslation, color, L"Teskt1", "fonts/StarWars.ttf" ) );
    //Scroller->AddNext( CreateTextCacheTest( timeEvaluator, "Dummy2", nodeTranslation, color, L"Teskt2", "fonts/StarWars.ttf" ) );
    //Scroller->AddNext( CreateTextCacheTest( timeEvaluator, "Dummy3", nodeTranslation, color, L"Teskt3", "fonts/StarWars.ttf" ) );
    //Scroller->AddNext( CreateTextCacheTest( timeEvaluator, "Dummy4", nodeTranslation, color, L"Teskt4", "fonts/StarWars.ttf" ) );
    //Scroller->AddNext( CreateTextCacheTest( timeEvaluator, "Dummy5", nodeTranslation, color, L"Teskt5", "fonts/StarWars.ttf" ) );
    //Scroller->AddNext( CreateTextCacheTest( timeEvaluator, "Dummy6", nodeTranslation, color, L"Teskt6", "fonts/StarWars.ttf" ) );

    auto texture = CreateTexturedRectNode( timeEvaluator, false );
    texture->AddChildToModelOnly( CreateTextCacheTest( timeEvaluator, "Text", nodeTranslation, color, L"Teskt1", "fonts/StarWars.ttf" ) );
    //Scroller->AddNext( texture );
    node->AddChildToModelOnly( texture );

    texture = CreateTexturedRectNode( timeEvaluator, false );
    texture->AddChildToModelOnly( CreateTextCacheTest( timeEvaluator, "Text", nodeTranslation, color, L"Teskt2", "fonts/StarWars.ttf" ) );
    //Scroller->AddNext( texture );
    node->AddChildToModelOnly( texture );

    texture = CreateTexturedRectNode( timeEvaluator, false );
    texture->AddChildToModelOnly( CreateTextCacheTest( timeEvaluator, "Text", nodeTranslation, color, L"Teskt3", "fonts/StarWars.ttf" ) );
    //Scroller->AddNext( texture );
    node->AddChildToModelOnly( texture );

    texture = CreateTexturedRectNode( timeEvaluator, false );
    texture->AddChildToModelOnly( CreateTextCacheTest( timeEvaluator, "Text", nodeTranslation, color, L"Teskt4", "fonts/StarWars.ttf" ) );
    //Scroller->AddNext( texture );
    node->AddChildToModelOnly( texture );



 //   Scroller->AddMessage( L"Message 1" );
 //   Scroller->AddMessage( L"Message 2" );
 //   Scroller->AddMessage( L"Message 3" );
 //   Scroller->AddMessage( L"Message 4" );
 //   Scroller->AddMessage( L"Message 5" );
 //   Scroller->AddMessage( L"Message 6" );

    //Scroller->SetSpeed( 400000.0f );
    //Scroller->SetInterspace( 1.0f );

    //Scroller->Finalize();
    //Scroller->Start();

    return node;
}

// *****************************
//
void					SimpleNodesFactory::TestTypedParamQueries	( model::ITimeEvaluatorPtr timeEvaluator )
{
    assert( false ); timeEvaluator;
    //auto node = CreateSolidRectNode( "rect_node", 1.0f, 1.0f, glm::vec3( 0.f, 0.f, 0.f ), glm::vec4( 1.f, 1.f, 1.f, 1.f ), timelineManager, timeEvaluator );

    /////////////////////////////////////////////////////
    //// Acessing simple interpolator and time evaluator for float parmameter
 //   auto paramWidth					= node->GetPlugin( "rectangle" )->GetParameter( "width" );
    //auto paramWidth_Float			= model::QueryTypedParam< model::ParamFloatPtr >( paramWidth );
    //auto & paramWidth_Interpolator	= paramWidth_Float->AccessInterpolator(); // auto & is necessary as AccessInterpolator returns reference to the interpolator
    //auto paramWidt_timeline			= paramWidth_Float->GetTimeEvaluator();
    //
    //paramWidth_Interpolator.AddKey( 0.f, 3.f );
    //auto & keys = paramWidth_Interpolator.AccessKeys(); // returns const reference to keys, to change keys use: paramWidth_Interpolator.AddKey(...)

    //auto & key_val	= keys.at( 0 ); // equal to: keys[ 0 ]
    //TimeType time	= key_val.t;
    //float value		= key_val.val;

    /////////////////////////////////////////////////////
    //// Acessing simple interpolator for vec4 parmameter
 //   auto paramColor					= node->GetPlugin( "solid color" )->GetParameter( "color" );
    //auto paramColor_Float			= model::QueryTypedParam< model::ParamVec4Ptr >( paramColor );
    //auto & paramColor_Interpolator	= paramColor_Float->AccessInterpolator();

    //auto & paramColor_keys			= paramColor_Interpolator.AccessKeys();
    //
    //// inspect the first key (TimeType, glm::vec4)
    //TimeType paramColor_keys_0_t	= paramColor_keys[ 0 ].t;
    //glm::vec4 paramColor_keys_0_val = paramColor_keys[ 0 ].val;


    /////////////////////////////////////////////////////
    //// Acessing simple interpolator for transform parameter
    //auto paramTrans					= node->GetPlugin( "transform" )->GetParameter( "simple_transform" );
    //auto paramTrans_TV				= model::QueryTypedParam< model::ParamTransformVecPtr >( paramTrans );
    //
    //auto & transform				= paramTrans_TV->Transform( 0 );
    //auto translation				= transform[ 1 ];								// translation
    //auto rotation					= static_cast<RotationF *>( transform[ 2 ] );	// rotation transform[ 2 ] == SimpleTransformF * but can be cast to RotationF, as it is a rotation by default

    //auto & transform_x_interpolator = translation->GetP0MotylaNoga();
    //auto & transform_y_interpolator = translation->GetP1MotylaNoga();
    //auto & transform_z_interpolator = translation->GetP2MotylaNoga();

    //auto & t_x_keys					= transform_x_interpolator.AccessKeys();

    //// inspect the first key (TimeType, float)
    //TimeType t_x_0_key_time			= t_x_keys[ 0 ].t;
    //float	 t_x_0_key_val			= t_x_keys[ 0 ].val;

    //// Make sure that vec4 interpolator is used for rotation axis interpolation
    //assert( rotation->IsAxisVec3() );

    //auto & rot_angle_interpolator	= rotation->AccessAngle();
    //auto & rot_angle_keys			= rot_angle_interpolator.AccessKeys();

    //// inspect the first key (TimeType, float)
    //TimeType rot_angle_key_0_time	= rot_angle_keys[ 0 ].t; 
    //float rot_angle_key_0_value		= rot_angle_keys[ 0 ].val; 
    //
    //auto & rot_axis_interpolator	= rotation->AccessRotAxis();
    //auto & rot_axis_keys			= rot_axis_interpolator.AccessKeys();

    //// inspect the first key (TimeType, glm::vec3)
    //TimeType rot_axis_key_0_time	= rot_axis_keys[ 0 ].t; 
    //glm::vec3 rot_axis_key_0_value	= rot_axis_keys[ 0 ].val; 
    //
    //// suppress warnings
    //{ transform_y_interpolator; }
    //{ transform_z_interpolator; }

    //{ time; }
    //{ value; }

    //{ t_x_0_key_time; }
    //{ t_x_0_key_val; }

    //{ paramColor_keys_0_t; }
    //{ paramColor_keys_0_val; }

    //{ rot_angle_key_0_time; }
    //{ rot_angle_key_0_value; }

    //{ rot_axis_key_0_time; }
    //{ rot_axis_key_0_value; }
}

} //bv
