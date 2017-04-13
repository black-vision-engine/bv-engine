#include "stdafxBVApp.h"

#include "MockScenes.h"

#include "Engine/Models/Plugins/Interfaces/IPluginListFinalized.h"
#include "Engine/Models/Plugins/Manager/PluginsManager.h"
#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"

#include "Engine/Models/Plugins/PluginsFactory.h"

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Timeline/TimelineManager.h"

#include "ExampleTestScenes.h"
#include "NewModelTestNodes.h"

#include "PieChartNode.h"
#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultTexturePlugin.h"
#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTexturesData.h"
#include "Engine/Models/Plugins/Simple/VideoInput/DefaultVideoInputResourceDescr.h"
#include "VideoInput/ExampleVideoInput.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultCirclePlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultEllipsePlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultTrianglePlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultRoundedRectPlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultConePlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultCubePlugin.h"
#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultBlendTexturePlugin.h"
#include "Engine/Models/Plugins/Simple/GeometryProcessors/DefaultExtrudePlugin.h"

#include "Engine/Models/NodeLogics/MeshLoader/MeshLoader.h"

#include "Engine/Models/Plugins/PluginUtils.h"

#include "Engine/Models/ModelSceneEditor.h"
#include "Engine/Models/BVProject.h"

#include "Engine/Models/NodeEffects/ModelNodeEffectFactory.h"

#include "System/Env.h"
#include "BVConfig.h"

#include "Serialization/Json/JsonSerializeObject.h"
#include "Serialization/Json/JsonDeserializeObject.h"

#include "ProjectManager.h"
#include <fstream>

namespace bv {

namespace {

// *****************************
//
model::BasicNodePtr  DefaultTestNewAPI   ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    std::vector< std::string > uids;

    uids.push_back( "DEFAULT_TRANSFORM" );
    uids.push_back( "DEFAULT_RECTANGLE" );
    uids.push_back( "DEFAULT_COLOR" );

    model::IPluginListFinalizedPtr pluginsList( pluginsManager->CreatePlugins( uids, timeEvaluator ) );

    auto root = model::BasicNode::Create( "Root", timeEvaluator );

    //FIXME: add list to the node
    return root;
}

// *****************************
//
model::BasicNodePtr  DefaultTestNodeNewNodeImpl  ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    auto root = model::BasicNode::Create( "Root", timeEvaluator, pluginsManager );

    bool success = true;

    success &= root->AddPlugin( "DEFAULT_TRANSFORM", "transform", timeEvaluator );  //success &= root->AddPlugin( "DEFAULT_TRANSFORM" ); //uses the default plugin name
    success &= root->AddPlugin( "DEFAULT_RECTANGLE", "rectangle", timeEvaluator );  //success &= root->AddPlugin( "DEFAULT_RECTANGLE" ); //uses the default plugin name
    success &= root->AddPlugin( "DEFAULT_COLOR", "solid color", timeEvaluator );    //success &= root->AddPlugin( "DEFAULT_COLOR" );     //uses the default plugin name

    assert( success );

    return root;
}

// *****************************
//
model::BasicNodePtr  DefaultTestWithValidation   ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    using namespace model;

    //NEW API
    IPluginPtr firstPlugin  = IPluginPtr( pluginsManager->CreatePlugin( "DEFAULT_TRANSFORM", "transform0", nullptr, timeEvaluator ) );

    IPluginPtr secondPlugin = IPluginPtr( pluginsManager->CreatePlugin( "DEFAULT_RECTANGLE", "rect0", firstPlugin, timeEvaluator ) );

    IPluginPtr thirdPlugin  = IPluginPtr( pluginsManager->CreatePlugin( "DEFAULT_COLOR", "col0", secondPlugin, timeEvaluator ) );

    auto root = model::BasicNode::Create( "Root", timeEvaluator );

    root->AddPlugin( firstPlugin );
    root->AddPlugin( secondPlugin );
    root->AddPlugin( thirdPlugin );

    return root;
}

// *****************************
//
model::BasicNodePtr  DefaultTestNoValidation     ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    using namespace model;

    //NEW API
    auto firstPlugin  = pluginsManager->CreatePlugin( "DEFAULT_TRANSFORM", "transform0", nullptr, timeEvaluator );
    auto secondPlugin = pluginsManager->CreatePlugin( "DEFAULT_RECTANGLE", "rect0", firstPlugin, timeEvaluator );
    auto thirdPlugin  = pluginsManager->CreatePlugin( "DEFAULT_COLOR", "col0", secondPlugin, timeEvaluator );

    auto root = model::BasicNode::Create( "Root", timeEvaluator );

    root->AddPlugin( firstPlugin );
    root->AddPlugin( secondPlugin );
    root->AddPlugin( thirdPlugin );

    return root;
}

// *****************************
//
void  QueryRegisteredPluginDescriptors  ( const model::PluginsManager * pluginsManager )
{
    printf( "Registered plugin descriptors\n" );

    for( auto descr : pluginsManager->GetRegisteredDescriptors() )
    {
        printf( "  %s\n", descr->GetPluginTypeUID().c_str() );
    }
}

// *****************************
//
void QueryParametersGeneric    ( const std::vector< model::IParameterPtr > & params )
{
    printf( "Parameters:\n");

    for( auto param : params )
    {
        printf( "%s : %i\n", param->GetName().c_str(), ( Int32 )param->GetType() );
    }
}

// *****************************
//
void QueryValuesGeneric    ( const std::vector< IValueConstPtr > & values )
{
    printf( "Parameters:\n");

    for( auto value : values )
    {
        printf( "%s : %i\n", value->GetName().c_str(), ( Int32 )value->GetType() );
    }
}

// *****************************
//
void QueryModelPropertiesGeneric    ( model::IParamValModelPtr model, const std::string msg )
{
    printf( "%s\n", msg.c_str() );

    if( model )
    {
        QueryParametersGeneric( model->GetParameters() );
        QueryValuesGeneric( model->GetValues() );
    }
    else
    {
        printf( "empty model\n" ); 
    }
}

// *****************************
//
void QueryPluginPropertiesGeneric   ( model::IPluginConstPtr plugin )
{
    printf ( "Plugin: %s %s", plugin->GetTypeUid().c_str(), plugin->GetName().c_str() );

    auto model = plugin->GetPluginParamValModel();

    QueryModelPropertiesGeneric( model->GetPluginModel(), "Plugin Model" );
    QueryModelPropertiesGeneric( model->GetTransformChannelModel(), "Transform Channel Model" );
    QueryModelPropertiesGeneric( model->GetVertexAttributesChannelModel(), "Vertex Attributes Channel Model" );
    QueryModelPropertiesGeneric( model->GetVertexShaderChannelModel(), "VertexShader Channel Model" );
    QueryModelPropertiesGeneric( model->GetPixelShaderChannelModel(), "PixelShader Channel Model" );
    QueryModelPropertiesGeneric( model->GetGeometryShaderChannelModel(), "GeometryShader Channel Model" );
}

// *****************************
//
void  QueryPluginsNodesGeneric      ( model::BasicNodePtr node )
{
    printf( "Inspecting node: %s\n", node->GetName().c_str() );

    printf( "Plugins: \n" );

    auto pluginlist = node->GetPluginList();
    for( unsigned int i = 0; i < pluginlist->NumPlugins(); ++i )
    {
        QueryPluginPropertiesGeneric( pluginlist->GetPlugin( i ) );
    }

    printf( "Layers: \n" );

    //FIXME: add API to iterate over layers
    //for( auto layer : node->GetLayer() )

    printf( "Children: \n" );

    //FIXME: add API to iterate over children nodes (not by names)
    //for( auto layer : node->GetLayer() )

}

// *****************************
//
void  QueryPropertiesDefaultScene   ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    auto  node = DefaultTestNodeNewNodeImpl( pluginsManager, timeEvaluator );

    model::IParameterConstPtr   transform_p  = node->GetPlugin( "transform" )->GetPluginParamValModel()->GetTransformChannelModel()->GetParameter( "simple_transform" );
    IValueConstPtr              transform_v  = node->GetPlugin( "transform" )->GetPluginParamValModel()->GetTransformChannelModel()->GetValue( "simple_transform" );

    //Rectangle plugin nie ma valiusow, a tylko parametry
    model::IParameterConstPtr width_p       = node->GetPlugin( "rectangle" )->GetPluginParamValModel()->GetTransformChannelModel()->GetParameter( "width" );
    model::IParameterConstPtr height_p      = node->GetPlugin( "rectangle" )->GetPluginParamValModel()->GetTransformChannelModel()->GetParameter( "height" );

    model::IParameterConstPtr color_p       = node->GetPlugin( "solid color" )->GetPluginParamValModel()->GetTransformChannelModel()->GetParameter( "color" );
    IValueConstPtr color_v                  = node->GetPlugin( "solid color" )->GetPluginParamValModel()->GetTransformChannelModel()->GetValue( "color" );

    //Oczywiscie mozna pobierac wszystkie parametry/valiusy dla danego pluginu jedna metoda, ale jest tez to API powyzsze do dobierania sie do nich pojedynczo
}

// *****************************
//
void  QueryPropertiesDefaultSceneConvenienceAPI ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    auto  node = DefaultTestNodeNewNodeImpl( pluginsManager, timeEvaluator );

    model::IParameterPtr transform_p    = node->GetPlugin( "transform" )->GetParameter( "simple_transform" );
    IValueConstPtr transform_v          = node->GetPlugin( "transform" )->GetValue( "simple_transform" );

    //Rectangle plugin nie ma valiusow, a tylko parametry
    model::IParameterPtr width_p        = node->GetPlugin( "rectangle" )->GetParameter( "width" );
    model::IParameterPtr height_p       = node->GetPlugin( "rectangle" )->GetParameter( "height" );

    model::IParameterPtr color_p        = node->GetPlugin( "solid color" )->GetParameter( "color" );
    IValueConstPtr color_v              = node->GetPlugin( "solid color" )->GetValue( "color" );
}

// *****************************
//
void  QueryPropertiesDefaultSceneConvenienceAPIParameterSetters ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    using namespace model;

    auto  node = DefaultTestNodeNewNodeImpl( pluginsManager, timeEvaluator );

    IParameterPtr transform_p = node->GetPlugin( "transform" )->GetParameter( "simple_transform" );

    //Rectangle plugin nie ma valiusow, a tylko parametry
    IParameterPtr width_p     = node->GetPlugin( "rectangle" )->GetParameter( "width" );
    IParameterPtr height_p    = node->GetPlugin( "rectangle" )->GetParameter( "height" );

    IParameterPtr color_p     = node->GetPlugin( "solid color" )->GetParameter( "color" );

    bool success = true;

    success &= SetParameterRotation( transform_p, 0.0f, glm::vec3( 0.f, 0.f, 90.f ) );
    success &= SetParameterScale( transform_p, 0.0f, glm::vec3( 1.f, 2.f, 1.f ) );
    success &= SetParameterTranslation( transform_p, 0.0f, glm::vec3( 0.f, 1.f, -2.f ) );

    success &= SetParameter( width_p, 0.f, 1.0f );
    success &= SetParameter( height_p, 0.f, 1.0f );

    success &= SetParameter( color_p, 0.f, glm::vec4( 1.f, 0.f, 1.f, 1.f ) );

    assert( success );
}

// *****************************
//
void  QueryPropertiesDefaultSceneConvenienceAPIParameterSettersNoErrorChecking ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    auto  n = DefaultTestNodeNewNodeImpl( pluginsManager, timeEvaluator );

    SetParameterRotation    ( n->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, 0.f, 90.f ) );
    SetParameterScale       ( n->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 1.f, 2.f, 1.f ) );
    SetParameterTranslation ( n->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, 1.f, -2.f ) );

    SetParameter( n->GetPlugin( "rectangle" )->GetParameter( "width" ), 0.f, 1.0f );
    SetParameter( n->GetPlugin( "rectangle" )->GetParameter( "height" ), 0.f, 1.0f );

    SetParameter( n->GetPlugin( "solid color" )->GetParameter( "color" ), 0.f, glm::vec4( 1.f, 0.f, 1.f, 1.f ) );
}

class A
{
public:

    typedef void (A::*fvptr)();
    typedef void (A::*ffptr)(float);
    typedef int (A::*fiptr)(double);

public:

    static void f(int) {};
    void        f() {};
    void        f(float) {};
    int         f(double) { return 0; };

    void * return_f()
    {
        //fvptr f0 = &A::f;
        //ffptr f1 = &A::f;
        //fiptr f2 = &A::f;

        //Won't compile
        //auto i_chuj = &A::f;

        return nullptr;
    }
};

class AmbigTester
{
public:

    int    move(int dw, int dz);
    int    move(float dw, float dz);
    int    move(double dx, double dy);
    int    move(double dx, float dy);

    static void Test()
    {
        AmbigTester at;

        at.move( 10, 10 );
        at.move( (short)10, (short)10 );
        at.move( (unsigned char)10, (char )10 );
        at.move( 10.f, 10.f );
        at.move( 10.0, 10.0 );
        at.move( 10.0, 10.f );
    }
};

} // anonymous

void test_get_node( model::IModelNodePtr n, const std::string & path, bool res )
{
    printf( "Querying [%s] for [%s] - ", n->GetName().c_str(), path.c_str() );

    auto result = n->GetNode( path );

    if( result )
    {
        if( res )
        {
            printf( " SUCCESS -> node found\n" );
        }
        else
        {
            printf( " FAILURE -> node found\n" );
        }
    }
    else
    {
        if( res )
        {
            printf( " FAILURE -> Failed to query node\n" );
        }
        else
        {
            printf( " SUCCESS -> Node not found \n" );
        }
    }
}

// *****************************
//
void TestQueryNode( model::ITimeEvaluatorPtr timeEvaluator)
{
    //GetNode tester - do not use:
    auto rt = SimpleNodesFactory::CreateGreenRectNode( timeEvaluator, "." );
    auto n0 = SimpleNodesFactory::CreateGreenRectNode( timeEvaluator, "node0" );
    auto n1 = SimpleNodesFactory::CreateGreenRectNode( timeEvaluator, "node01" );
    auto n2 = SimpleNodesFactory::CreateGreenRectNode( timeEvaluator, "node02" );
    auto n3 = SimpleNodesFactory::CreateGreenRectNode( timeEvaluator, "node011" );
    auto n4 = SimpleNodesFactory::CreateGreenRectNode( timeEvaluator, "node012" );
    auto n5 = SimpleNodesFactory::CreateGreenRectNode( timeEvaluator, "node013" );
    auto n6 = SimpleNodesFactory::CreateGreenRectNode( timeEvaluator, "node021" );
    auto n7 = SimpleNodesFactory::CreateGreenRectNode( timeEvaluator, "node022" );
    auto n8 = SimpleNodesFactory::CreateGreenRectNode( timeEvaluator, "node023" );
    auto n9 = SimpleNodesFactory::CreateGreenRectNode( timeEvaluator, "node024" );

    rt->AddChildToModelOnly( n0 );

    n0->AddChildToModelOnly( n1 );
    n0->AddChildToModelOnly( n2 );

    n1->AddChildToModelOnly( n3 );
    n1->AddChildToModelOnly( n4 );
    n1->AddChildToModelOnly( n5 );

    n2->AddChildToModelOnly( n6 );
    n2->AddChildToModelOnly( n7 );
    n2->AddChildToModelOnly( n8 );
    n2->AddChildToModelOnly( n9 );

    test_get_node( rt, "", true );
    test_get_node( rt, "/", true );
    test_get_node( rt, ".", true );
    test_get_node( rt, "./", true );
    test_get_node( rt, "./node0", true );
    test_get_node( rt, "/node0", true );
    test_get_node( rt, "node0", true );
    test_get_node( rt, "node1", false );
    test_get_node( rt, "./node2", false );

    test_get_node( rt, "./node0/node01", true );
    test_get_node( rt, "./node0/node01", true );
    test_get_node( rt, "./node0/node01/node011", true );
    test_get_node( rt, "./node0/node01/node012", true );
    test_get_node( rt, "./node0/node01/node013", true );
    test_get_node( rt, "./node0/node01/node021", false );
    test_get_node( rt, "./node0/node01/node022", false );
    test_get_node( rt, "./node0/node01/node023", false );
    test_get_node( rt, "./node0/node01/node024", false );

    test_get_node( rt, "/node0/node01", true );
    test_get_node( rt, "/node0/node01", true );
    test_get_node( rt, "/node0/node01/node011", true );
    test_get_node( rt, "/node0/node01/node012", true );
    test_get_node( rt, "/node0/node01/node013", true );
    test_get_node( rt, "/node0/node01/node021", false );
    test_get_node( rt, "/node0/node01/node022", false );
    test_get_node( rt, "/node0/node01/node023", false );
    test_get_node( rt, "/node0/node01/node024", false );

    test_get_node( rt, "node0/node01", true );
    test_get_node( rt, "node0/node01", true );
    test_get_node( rt, "node0/node01/node011", true );
    test_get_node( rt, "node0/node01/node012", true );
    test_get_node( rt, "node0/node01/node013", true );
    test_get_node( rt, "node0/node01/node021", false );
    test_get_node( rt, "node0/node01/node022", false );
    test_get_node( rt, "node0/node01/node023", false );
    test_get_node( rt, "node0/node01/node024", false );

    test_get_node( n2, "node02", true );
    test_get_node( n2, "node02/node021", true );
    test_get_node( n2, "/node02/node022", true );
    test_get_node( n2, "node02/node023", true );
    test_get_node( n2, "/node02/node011", false );
    test_get_node( n2, "node01/node022", false );
    test_get_node( n2, "node02/node013", false );
    test_get_node( n2, "node024", true );
    test_get_node( n1, "node024", false );
}

// *****************************
//
model::BasicNodePtr		    TestScenesFactory::CreateSceneFromEnv       ( const std::string& scene, model::ITimeEvaluatorPtr timeline, const model::PluginsManager * pluginsManager )
{
    model::BasicNodePtr node = nullptr;

    if( scene == "TWO_TEXTURED_RECTANGLES" )
    {
        node = TestScenesFactory::CreateTestScene( pluginsManager, timeline, TestScenesFactory::TestSceneSelector::TSS_TWO_TEXTURED_RECTANGLES );
    }
    else if( scene == "ONE_TEXTURED_RECTANGLE" )
    {
        node = TestScenesFactory::CreateTestScene( pluginsManager, timeline, TestScenesFactory::TestSceneSelector::TSS_ONE_TEXTURED_RECTANGLE );
    }
    else if( scene == "OLAF_TEST_SCENE" )
    {
        node = TestScenesFactory::OlafTestScene( pluginsManager, timeline );
    }
    else if( scene == "CREED_TEST_SCENE" )
    {
        //node = TestScenesFactory::CreedPieChartTestScene( pluginsManager, timeline );
        node = TestScenesFactory::CreedPrismTestScene( pluginsManager, timeline );
    }
    else if( scene == "VIDEO_INPUT_TEST_SCENE" )
    {
        node = TestScenesFactory::CreedVideoInputTestScene( pluginsManager, timeline );
    }
    else if( scene == "DEFAULT_TEXT" )
    {
        node = TestScenesFactory::CreateTestScene( pluginsManager, timeline, TestScenesFactory::TestSceneSelector::TSS_TEXT );
    }
    else if( scene == "TSS_ANIMATION_RECTANGLE" )
    {
        node = TestScenesFactory::CreateTestScene( pluginsManager, timeline, TestScenesFactory::TestSceneSelector::TSS_ANIMATION_RECTANGLE );
    }
    else if( scene == "ALL_BASIC_SHAPES_SHOW" )
    {
        node = TestScenesFactory::BasicShapesShowScene( pluginsManager, timeline );
    }
    else if( scene == "BASIC_SHAPES_TEST_SCENE" )
    {
        node = TestScenesFactory::BasicShapesTest(  pluginsManager, timeline );
    }
    else if( scene == "INTERPOLATION_TEST_SCENE" )
    {
        node = TestScenesFactory::CreedCosineDemoScene( pluginsManager, timeline );
    }
    else if( scene == "GLOBAL_EFFECT_05" )
    {
        node = TestScenesFactory::GlobalEffect05( pluginsManager, timeline );
    }
    else if( scene == "TEXT_CACHE_TEST" )
    {
        node = TestScenesFactory::AssetCacheTestScene( pluginsManager, timeline );
    }
    else if( scene == "VIDEO_STREAM_TEST_SCENE" )
    {
        node = SimpleNodesFactory::CreateVideoStreamDecoderRectNode( timeline, false );
    }
    else if( scene == "REMOTE_EVENTS_TEST_SCENE" )
    {
        node = TestScenesFactory::RemoteEventsTestScene( pluginsManager, timeline );
    }
    else if( scene == "LIGHT_SCATTERING_EFFECT" )
    {
        node = TestScenesFactory::LightScatteringTestScene( pluginsManager, timeline );
    }
    else if( scene == "BLUR_EFFECT" )
    {
        node = TestScenesFactory::BlurEffectTestScene( pluginsManager, timeline );
    }
    else if( scene == "SHADOW_EFFECT" )
    {
        node = TestScenesFactory::ShadowEffectTestScene( pluginsManager, timeline );
    }
    else if( scene == "IMAGE_MASK_EFFECT" )
    {
        node = TestScenesFactory::ImageMaskEffectTestScene( pluginsManager, timeline );
    }
    else if( scene == "MULTI_SHADOW_EFFECT" )
    {
        node = TestScenesFactory::MultiShadowEffectTestScene( pluginsManager, timeline );
    }    
    else if( scene == "TIMER_TEST_SCENE" )
    {
        node = TestScenesFactory::TimerTestScene( pluginsManager, timeline );
    }
    else if( scene == "SCROLLER_TEST_SCENE" )
    {
        node = TestScenesFactory::ScrollerNodeTestScene( pluginsManager, timeline );
    }
    else if( scene == "REPLICATOR_TEST_SCENE" )
    {
        node = TestScenesFactory::NodeReplicatorTestScene( pluginsManager, timeline );
    }
    else if( scene == "STARWARS_TEST_SCENE" )
    {
        node = TestScenesFactory::FontTestScene( pluginsManager, timeline );
    }
    else if( scene == "GLOBAL_EFFECT_VIDEO_OUTPUT" )
    {
        node = TestScenesFactory::CreateTestScene( pluginsManager, timeline, TestScenesFactory::TestSceneSelector::TSS_TWO_TEXTURED_RECTANGLES );
    }
    else if( scene == "MULTI_GLOBAL_EFFECT_TEST_SCENE" )
    {
        node = TestScenesFactory::MultiGlobalEffectTestScene( pluginsManager, timeline );
    }
    else if( scene == "MESH_TEST_SCENE" )
    {
        node = TestScenesFactory::MeshTestScene( timeline );
    }
    else if( scene == "ENVIRONMENTAL_MAP_TEST_SCENE" )
    {
        node = TestScenesFactory::EnvMappingTestScene( timeline );
    }
    else if( scene == "BLEND_TEXTURE_TEST_SCENE" )
    {
        node = TestScenesFactory::BlendTextureTestScene( timeline );
    }
    else if( scene == "FADE_RECT_TEST_SCENE" )
    {
        node = TestScenesFactory::FadeRectTestScene( timeline );
    }
    else if( scene == "EXTRUDE_TEST_SCENE" )
    {
        node = TestScenesFactory::ExtrudeTestScene( timeline );
    }
    else if( scene == "LINE_CHART_TEST_SCENE" )
    {
        node = TestScenesFactory::LineChartTestScene( timeline );
    }
    else if( scene == "SVG_TEST_SCENE" )
    {
        node = TestScenesFactory::SVGTestScene( timeline );
    }
    else if( scene == "TEXT_3D_TEST_SCENE" )
    {
        node = TestScenesFactory::Text3DTestScene( timeline );
    }
    else
    {
        printf( "Environment variable %s not set or invalid. Creating default scene.\n", DefaultConfig.DefaultSceneEnvVarName().c_str() );

        //node = TestScenesFactory::CreateTestScene( pluginsManager, timelineManager, timeEvaluator, TestScenesFactory::TestSceneSelector::TSS_SOLID_RECTANGLE );
        //auto node1 = TestScenesFactory::CreateTestScene( pluginsManager, timelineManager, timeEvaluator, TestScenesFactory::TestSceneSelector::TSS_TEXT );
        //node->AddChildToModelOnly( node1 );

        //node = TestScenesFactory::CreateTestScene( pluginsManager, timelineManager, timeEvaluator, TestScenesFactory::TestSceneSelector::TSS_ONE_TEXTURED_RECTANGLE );
        
        //node = TestScenesFactory::CreateTestScene( pluginsManager, timelineManager, timeEvaluator, TestScenesFactory::TestSceneSelector::TSS_ONE_TEXTURED_RECTANGLE );

        //node = TestScenesFactory::CreateTestScene( pluginsManager, timelineManager, timeEvaluator, TestScenesFactory::TestSceneSelector::TSS_ANIMATION_RECTANGLE );

        node = TestScenesFactory::CreateTestScene( pluginsManager, timeline, TestScenesFactory::TestSceneSelector::TSS_TEXT );
    }

    return node;
}

// *****************************
//
model::BasicNodePtr     TestScenesFactory::CreateTestRandomNode        ( const std::string & name,  const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    return TexturedTestRandomRect( name, pluginsManager, timeEvaluator );
}

// *****************************
//
model::BasicNodePtr     TestScenesFactory::CreateTestScene      ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator, TestScenesFactory::TestSceneSelector tss )
{
    switch( tss )
    {
        case TestSceneSelector::TSS_ONE_SOLID_COLOR_RECTANGLE:
            return SolidRect( pluginsManager, timeEvaluator );
        case TestSceneSelector::TSS_TWO_TEXTURED_RECTANGLES:
            return TwoTexturedRectangles( pluginsManager,  timeEvaluator );
        case TestSceneSelector::TSS_ONE_TEXTURED_RECTANGLE:
            return SimpleNodesFactory::CreateTexturedRectNode( timeEvaluator, false );
        case TestSceneSelector::TSS_TEXT:
            return SimpleNodesFactory::CreateTextNode( timeEvaluator, 0, false );
        case TestSceneSelector::TSS_ANIMATION_RECTANGLE:
            return SimpleNodesFactory::CreateTextureAnimationRectNode( timeEvaluator, false );
        case TestSceneSelector::TSS_SOLID_RECTANGLE:
            return SimpleNodesFactory::CreateGreenRectNodeNoAssert( timeEvaluator, false );
        default:
            assert( false );

            return nullptr;
    };
}

// *****************************
//
model::BasicNodePtr     TestScenesFactory::GlobalEffect05           ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    { pluginsManager; }
    return SimpleNodesFactory::CreateGlobalEffectTest( timeEvaluator );
}

// *****************************
//
model::BasicNodePtr     TestScenesFactory::LightScatteringTestScene  ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    { pluginsManager; }
    return SimpleNodesFactory::CreateLightScatteringTest( timeEvaluator );
}

// *****************************
//
model::BasicNodePtr     TestScenesFactory::BlurEffectTestScene  ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    { pluginsManager; }
    return SimpleNodesFactory::CreateBlurTest( timeEvaluator );
}

// *****************************
//
model::BasicNodePtr     TestScenesFactory::ShadowEffectTestScene  ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    { pluginsManager; }
    return SimpleNodesFactory::CreateShadowTest( timeEvaluator );
}

// *****************************
//
model::BasicNodePtr     TestScenesFactory::ImageMaskEffectTestScene  ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    { pluginsManager; }
    return SimpleNodesFactory::CreateImageMaskTest( timeEvaluator );
}

// *****************************
//
model::BasicNodePtr     TestScenesFactory::MultiShadowEffectTestScene  ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    { pluginsManager; }
    return SimpleNodesFactory::CreateMultiShadowTest( timeEvaluator );
}

// *****************************
//
model::BasicNodePtr     TestScenesFactory::TimerTestScene  ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    { pluginsManager; }
    return SimpleNodesFactory::CreateTimerNode( timeEvaluator, 0, false );
}

model::BasicNodePtr     TestScenesFactory::ScrollerNodeTestScene   ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    { pluginsManager; }
    return SimpleNodesFactory::ScrollerNodeTestScene( timeEvaluator );
}

// *****************************
//
model::BasicNodePtr     TestScenesFactory::NodeReplicatorTestScene  ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    { pluginsManager; }
    return SimpleNodesFactory::CreateNodeReplicatorTest( timeEvaluator );
}

// *****************************
//
model::BasicNodePtr     TestScenesFactory::MultiGlobalEffectTestScene( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    { pluginsManager; }
    return SimpleNodesFactory::CreateNodeMultiGlobalEffectTest( timeEvaluator );
}

// *****************************
//
model::BasicNodePtr     TestScenesFactory::NewModelTestScene        ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    { pluginsManager; } // FIXME: suppress unused warning
    //return SimpleNodesFactory::CreateHeightMapNode( timelineManager, timeEvaluator );
    //return SimpleNodesFactory::CreateScrollerNode( timeEvaluator );
    auto node0 = SimpleNodesFactory::CreateBasicShapesTestNode( timeEvaluator );
    //auto node0 = SimpleNodesFactory::CreateTexturedRectNode( timelineManager, timeEvaluator, false );
    //auto node1 = SimpleNodesFactory::CreateTexturedRectNode( timelineManager, timeEvaluator, false );

    return node0;

    //return SimpleNodesFactory::CreateOverrideNodeMaskTest1( timelineManager, timeEvaluator );

    //return SimpleNodesFactory::CreateTextureAnimationRectNode( timelineManager, timeEvaluator, false );
    //return SimpleNodesFactory::CreateTextNode( timelineManager, timeEvaluator, 0, false );

#if 0
    //return SimpleNodesFactory::CreateOverrideAlphaTest( timelineManager, timeEvaluator );

    //TestQueryNode( timelineManager, timeEvaluator ); //FIXME: remove or uncomment after tests
    //auto c0 = SimpleNodesFactory::CreateGreenRectNode( timelineManager, timeEvaluator, "node0" );
    //auto c1 = SimpleNodesFactory::CreateGreenRectNode( timelineManager, timeEvaluator, "node1" );

    ////Plugin list
    //std::vector< std::string > uids;
    //uids.push_back( "DEFAULT_TRANSFORM" );

    //auto rut = new model::BasicNode( ".", timeEvaluator );
    //auto success = rut->AddPlugins( uids, timeEvaluator );
    //assert( success );

    //rut->AddChild( c0 );
    //rut->AddChild( c1 );

    //return SimpleNodesFactory::CreateHeightMapNode( timelineManager, timeEvaluator );

    //return rut;
    //return SimpleNodesFactory::CreateGreenRectNodeNoAssert( timelineManager, timeEvaluator, true );
    //return SimpleNodesFactory::CreateTexturedRectNode( timelineManager, timeEvaluator, false );
    //return SimpleNodesFactory::CreateTextureAnimationRectNode( timelineManager, timeEvaluator, true );
    return SimpleNodesFactory::CreateScrollerNode( timelineManager, timeEvaluator );
    return SimpleNodesFactory::CreateTextNode( timelineManager, timeEvaluator, 0, false );
    //return SimpleNodesFactory::CreateTextWithShadowNode( timelineManager, timeEvaluator, 5, glm::vec3( 0.005f, -0.005f, -0.001f ) );
    auto root = SimpleNodesFactory::CreateTimerNode( timelineManager, timeEvaluator, 0, false );
    auto r0 = SimpleNodesFactory::CreateTimerNode( timelineManager, timeEvaluator, 0, false );
    root->AddChild( r0 );
    auto r1 = SimpleNodesFactory::CreateTimerNode( timelineManager, timeEvaluator, 0, false );
    r0->AddChild( r1 );
    auto r2 = SimpleNodesFactory::CreateTimerNode( timelineManager, timeEvaluator, 0, false );
    r1->AddChild( r2 );
    auto r3 = SimpleNodesFactory::CreateTimerNode( timelineManager, timeEvaluator, 0, false );
    r2->AddChild( r3 );
    auto r4 = SimpleNodesFactory::CreateTimerNode( timelineManager, timeEvaluator, 0, false );
    r3->AddChild( r4 );
    auto r5 = SimpleNodesFactory::CreateTimerNode( timelineManager, timeEvaluator, 0, false );
    r4->AddChild( r5 );
    


    return root;

    //auto parent = SimpleNodesFactory::CreateTexturedRectNode( timelineManager, timeEvaluator, false );
    //auto child = SimpleNodesFactory::CreateTextNode( timelineManager, timeEvaluator, 0, false );
    //parent->AddChild( child );

    //return child;
    //return parent;
#endif
}

// *****************************
//
model::BasicNodePtr     TestScenesFactory::OlafTestScene     ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    { pluginsManager; } // FIXME: suppress unuse warning
    auto rect = SimpleNodesFactory::CreateOlafRectNode( timeEvaluator );
    auto clock = SimpleNodesFactory::CreateTimerNode( timeEvaluator, 0, false );
    rect->AddChildToModelOnly( clock );

    return rect;
}

model::BasicNodePtr    TestScenesFactory::CreedPieChartTestScene     ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    { pluginsManager; } // FIXME: suppress unuse warning
    std::vector< PieChartSubnodeDesc > descs;

    //descs.push_back( PieChartSubnodeDesc( 10.f, 0.f ) );
    descs.push_back( PieChartSubnodeDesc( 20.f, -.1f, PieChartSubnodeDesc::COLORED ) );
    //descs.push_back( PieChartSubnodeDesc( 50.f, .1f ) );

    auto node = new PieChartNode( timeEvaluator, descs );

    return model::BasicNodePtr( node );
}

model::BasicNodePtr    TestScenesFactory::CreedDeprecatedTestScene     ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    { pluginsManager; } // FIXME: suppress unuse warning
    float percents[] = { 10.f, 20.f, 50.f };
    float offsets[] = { 0.f, -.1f, .1f };

    const int noPieces = 3;

    auto node = new PieChartNode( timeEvaluator, std::vector<float>( percents, percents+noPieces ), std::vector<float> (offsets, offsets+noPieces ) );

    return model::BasicNodePtr( node );
}

model::BasicNodePtr    TestScenesFactory::CreedPrimitivePieChartTestScene     ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    { pluginsManager; } // FIXME: suppress unuse warning
    model::BasicNodePtr root = model::BasicNode::Create( "rootNode", timeEvaluator );
    root->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );

    auto simpleTransform = root->GetPlugin( "transform" )->GetParameter( "simple_transform" );
    SetParameterScale( simpleTransform, 0.0f, glm::vec3( 1.f, .2f, 1.f ) );
    SetParameterTranslation( simpleTransform, 0.0f, glm::vec3( 0, 0, -1.f) );
    SetParameterRotation ( simpleTransform, 0.0f, glm::vec3( 20.f, 0.f, 0.f ) );


    auto node1 = SimpleNodesFactory::CreateCreedColoredPieChartNode( timeEvaluator, 0 );
    auto node2 = SimpleNodesFactory::CreateCreedGradedPieChartNode( timeEvaluator, 0 );

    SetParameter( node1->GetPlugin( "piechart" )->GetParameter( "angleStart" ), 0, 0.f );
    SetParameter( node1->GetPlugin( "piechart" )->GetParameter( "angleEnd" ), 0, 3.14f/4 );

    root->AddChildToModelOnly( node1 );
    root->AddChildToModelOnly( node2 );

    return root;
}

model::BasicNodePtr    TestScenesFactory::CreedPrismTestScene     ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    { pluginsManager; } // FIXME: suppress unuse warning
    //auto rect = SimpleNodesFactory::CreateCreedTextNode( timelineManager, timeEvaluator );

    //auto rect = SimpleNodesFactory::CreateTimerNode( timelineManager, timeEvaluator, 0, false );
    //auto rect = SimpleNodesFactory::CreateCreedTimerNode( timelineManager, timeEvaluator, 0, false );
    //rect->AddChild( rect2 );

    //auto rect = SimpleNodesFactory::CreateTextureAnimationRectNode( timelineManager, timeEvaluator, true );
    //auto rect = SimpleNodesFactory::CreateTexturedRectNode( timelineManager, timeEvaluator, true );
    //auto rect = SimpleNodesFactory::CreateTexturedTextNode( timelineManager, timeEvaluator, false );
    //auto rect = SimpleNodesFactory::CreateHeightMapNode( timelineManager, timeEvaluator );
    //auto rect = SimpleNodesFactory::CreateGreenRectNodeNoAssert( timelineManager, timeEvaluator, false );
    //auto rect = SimpleNodesFactory::CreateTextNode(timelineManager, timeEvaluator, 0, false );

    //return rect;

    model::BasicNodePtr root = model::BasicNode::Create( "rootNode", timeEvaluator );
    root->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
//SetParameterRotation( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 50.f, glm::vec3( 1, 0, 0 ), 1000.f );

    //auto root = SimpleNodesFactory::CreateCreedRectNode( timelineManager, timeEvaluator );

    auto prism = SimpleNodesFactory::CreateCreedColoredPrismNode( timeEvaluator, -1.5f );
    auto success = SetParameter( prism->GetPlugin( "prism" )->GetParameter( "n" ), 0.f, 4 );
    { success; }
    assert( success );
    {success;}

    SetParameter( prism->GetPlugin( "prism" )->GetParameter( "n" ), 10.f, 10 );
    //prism->GetPlugin( "prism" )->GetParameter( "n" )->SetInterpolationMethod( model::IParameter::InterpolationMethod::COSINE );

    SetParameterScale( prism->GetPlugin( "transform" )->GetParameter( "simple_transform" ),  5.f, glm::vec3( 0.25f, 1.0f, 0.25f ) );
    SetParameterScale( prism->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 10.f, glm::vec3( 0.25f,  .0f, 0.25f ) );
    SetParameterScale( prism->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 15.f, glm::vec3( 0.25f,  .0f, 0.25f ) );
    SetParameterScale( prism->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 20.f, glm::vec3( 0.25f, 1.0f, 0.25f ) );

    auto prism2 = SimpleNodesFactory::CreateCreedTexturedPrismNode( timeEvaluator,-0.5f );

    SetParameterScale( prism2->GetPlugin( "transform" )->GetParameter( "simple_transform" ),  3.f, glm::vec3( 0.25f, 1.0f, 0.25f ) );
    SetParameterScale( prism2->GetPlugin( "transform" )->GetParameter( "simple_transform" ),  7.f, glm::vec3( 0.25f,  .0f, 0.25f ) );
    SetParameterScale( prism2->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 13.f, glm::vec3( 0.25f,  .0f, 0.25f ) );
    SetParameterScale( prism2->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 17.f, glm::vec3( 0.25f, 1.0f, 0.25f ) );
    SetParameterScale( prism2->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 20.f, glm::vec3( 0.25f, 1.0f, 0.25f ) );

    auto param = prism2->GetPlugin( "transform" )->GetParameter( "simple_transform" );
    //param->SetInterpolationMethod( model::IParameter::InterpolationMethod::COSINE );

//SetParameterRotation( prism2->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 50.f, glm::vec3( 1, 0, 0 ), -10000.f );

    auto prism3 = SimpleNodesFactory::CreateCreedGradedPrismNode( timeEvaluator, 0.5f );
    SetParameter( prism3->GetPlugin( "prism" )->GetParameter( "n" ), 0.f, 12 );
    
    SetParameterScale( prism3->GetPlugin( "transform" )->GetParameter( "simple_transform" ),  7.f, glm::vec3( 0.25f, 1.0f, 0.25f ) );
    SetParameterScale( prism3->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 13.f, glm::vec3( 0.25f,  .0f, 0.25f ) );
    SetParameterScale( prism3->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 17.f, glm::vec3( 0.25f,  .0f, 0.25f ) );
    SetParameterScale( prism3->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 20.f, glm::vec3( 0.25f, 1.0f, 0.25f ) );

//SetParameterRotation( prism3->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 50.f, glm::vec3( 1, 0, 0 ), 10000.f );

    auto prism4 = SimpleNodesFactory::CreateCreedGradedPrismNode( timeEvaluator, 1.5f );
    SetParameter( prism4->GetPlugin( "prism" )->GetParameter( "n" ), 0.f, 36 );
    SetParameter( prism4->GetPlugin( "linear_gradient" )->GetParameter( "color1" ), 5.f, glm::vec4( 1.f, 1.f, 1.f, 1.f ) );
    SetParameter( prism4->GetPlugin( "linear_gradient" )->GetParameter( "color2" ), 5.f, glm::vec4( 0.f, 0.f, 0.f, 1.f ) );
    //prism4->GetPlugin( "linear_gradient" )->GetParameter( "color2" )->SetInterpolationMethod( model::IParameter::InterpolationMethod::COSINE );
    
    SetParameterScale( prism4->GetPlugin( "transform" )->GetParameter( "simple_transform" ),  7.f, glm::vec3( 0.25f,  .0f, 0.25f ) );
    SetParameterScale( prism4->GetPlugin( "transform" )->GetParameter( "simple_transform" ),  8.f, glm::vec3( 0.25f,  .0f, 0.25f ) );
    SetParameterScale( prism4->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 11.f, glm::vec3( 0.25f, 1.0f, 0.25f ) );
    SetParameterScale( prism4->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 20.f, glm::vec3( 0.25f, 1.0f, 0.25f ) );

    // FIXME: nrl - reimplement effects
    //prism->SetNodeEffect( model::ModelNodeEffectFactory::CreateModelNodeEffect( NodeEffectType::NET_BOUNDING_BOX, "bounding box", timeEvaluator ) );
    //prism2->SetNodeEffect( model::ModelNodeEffectFactory::CreateModelNodeEffect( NodeEffectType::NET_BOUNDING_BOX, "bounding box", timeEvaluator ) );
    //prism3->SetNodeEffect( model::ModelNodeEffectFactory::CreateModelNodeEffect( NodeEffectType::NET_BOUNDING_BOX, "bounding box", timeEvaluator ) );
    //prism4->SetNodeEffect( model::ModelNodeEffectFactory::CreateModelNodeEffect( NodeEffectType::NET_BOUNDING_BOX, "bounding box", timeEvaluator ) );

    root->AddChildToModelOnly( prism );
    root->AddChildToModelOnly( prism2 );
    root->AddChildToModelOnly( prism3 );
    root->AddChildToModelOnly( prism4 );

    //return rect;
    return root;
}

model::BasicNodePtr    TestScenesFactory::CreedVideoInputTestScene   ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    {pluginsManager;}

    model::BasicNodePtr root = model::BasicNode::Create( "rootNode", timeEvaluator );
    root->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
    root->AddPlugin( "DEFAULT_RECTANGLE", timeEvaluator );

    root->AddPlugin( "DEFAULT_VIDEOINPUT", timeEvaluator );
    auto plugin = root->GetPlugin( "video input" );
    auto vi = new ExampleVideoInput( 1920, 1080, 1.f );
    auto success = plugin->LoadResource( AssetDescConstPtr( std::make_shared< model::DefaultVideoInputResourceDescr >( vi->GetTexture(), vi ) ) );
    assert(success);
    { success; }
    //auto vi2 = new ExampleVideoInput( 20, 20, 1.f );
    //success = plugin->LoadResource( model::IPluginResourceDescrConstPtr( new model::DefaultVideoInputResourceDescr( vi2->GetTexture(), vi2 ) ) );
    //assert(success);

    model::SetParameter( plugin->GetParameter( "source" ), 0.f, 1.f );

    return root;
}

model::BasicNodePtr    TestScenesFactory::CreedPrismBugTestScene     ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    {pluginsManager;}

    model::BasicNodePtr root = model::BasicNode::Create( "rootNode", timeEvaluator );
    root->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );

    auto prism2 = SimpleNodesFactory::CreateCreedTexturedPrismNode( timeEvaluator, 0 );
    //auto prism2 = SimpleNodesFactory::CreateCreedColoredPrismNode( timelineManager, timeEvaluator, 0 );
    //auto prism2 = SimpleNodesFactory::CreateCreedGradedPrismNode( timelineManager, timeEvaluator, 0 );

    SetParameterScale( prism2->GetPlugin( "transform" )->GetParameter( "simple_transform" ),  0.f, glm::vec3( 1.f, 1.f, 1.f ) );
    SetParameterTranslation( prism2->GetPlugin( "transform" )->GetParameter( "simple_transform" ),  0.f, glm::vec3( 0.f, -.5f, -10.f ) );
    SetParameterRotation ( prism2->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 100.0f, glm::vec3( 10100.f, 0.f, 0.f ) );

    model::SetParameter( prism2->GetPlugin( "prism" )->GetParameter( "n" ), 10.f, 10.f );

    model::SetParameter( prism2->GetPlugin( "texture" )->GetResourceStateModel( "Tex0" )->GetParameter( "borderColor") , 0.f, glm::vec4( 1, 0, 0, 1 ) );

    //SetParameterScale( prism2->GetPlugin( "transform" )->GetParameter( "simple_transform" ),  3.f, glm::vec3( 0.25f, 1.0f, 0.25f ) );
    //SetParameterScale( prism2->GetPlugin( "transform" )->GetParameter( "simple_transform" ),  7.f, glm::vec3( 0.25f,  .0f, 0.25f ) );
    //SetParameterScale( prism2->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 13.f, glm::vec3( 0.25f,  .0f, 0.25f ) );
    //SetParameterScale( prism2->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 17.f, glm::vec3( 0.25f, 1.0f, 0.25f ) );
    //SetParameterScale( prism2->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 20.f, glm::vec3( 0.25f, 1.0f, 0.25f ) );

    root->AddChildToModelOnly( prism2 );

    //return root;
    return prism2;
}

void BoolParamTest()
{
    auto param = model::ParametersFactory::CreateParameterBool( "toBeOrNotToBe", nullptr );
    model::SetParameter( param, 0.f, true );
}

model::BasicNodePtr CosineDemoRect( glm::vec3 offset, model::ITimeEvaluatorPtr timeEvaluator, CurveType type )
{
    model::BasicNodePtr node = model::BasicNode::Create( "rect", timeEvaluator );
    node->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
    node->AddPlugin( "DEFAULT_RECTANGLE", timeEvaluator );
    node->AddPlugin( "DEFAULT_COLOR", timeEvaluator );

    model::SetParameter( node->GetPlugin( "solid color" )->GetParameter( "color" ), 0.f, glm::vec4( 1, 1, 1, 1 ) );

    auto param = node->GetPlugin( "transform" )->GetParameter( "simple_transform" );

    auto qParam = model::QueryTypedParam< model::ParamTransformPtr >( param );
    assert( qParam );
    qParam->SetAddedKeyCurveType( type );

    model::SetParameterTranslation( param, 5.f, offset + glm::vec3( 0, -2, 0 ) );
    model::SetParameterTranslation( param, 1.f, offset );
    model::SetParameterTranslation( param, 0.f, offset );
    model::SetParameterScale( param, 0.f, glm::vec3( 0.25f, 0.25f, 1.f ) );
    model::SetWrapPostMethod( param, WrapMethod::pingPong );

    //param = node->GetPlugin( "rectangle" )->GetParameter( "width" );
    //auto qParam2 = model::QueryTypedParam< model::ParamFloatPtr >( param );
    //qParam2->SetCurveType( CurveType::BEZIER );
    //model::SetParameter( param, 1, 1.f );
    //auto i = qParam2->AccessInterpolator();
    //i.GetInterpolators()[ 0 ]->GetType();
    //i.SetKey1( 0, Key< float, float >( 0, 0 ) );

    return node;
}

model::BasicNodePtr    TestScenesFactory::CreedCosineDemoScene     ( const model::PluginsManager * , model::ITimeEvaluatorPtr timeEvaluator )
{
    model::BasicNodePtr root = model::BasicNode::Create( "rootNode", timeEvaluator );
    root->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );

    const int total = bv::CurveType::CT_TOTAL;

    for( int i = 0; i < total; i++ )
        root->AddChildToModelOnly( CosineDemoRect( glm::vec3( -2.f + i * 4.0 / total, 1, 0 ) , timeEvaluator, bv::CurveType( i ) ) );

    return root;
}

//model::BasicNodePtr    TestScenesFactory::CreedBasicGeometryTestScene     ( model::ITimeEvaluatorPtr timeEvaluator )
model::BasicNodePtr    /*TestScenesFactory::*/CreedCircleTestScene     ( model::ITimeEvaluatorPtr timeEvaluator )
{
    model::BasicNodePtr root = model::BasicNode::Create( "rootNode", timeEvaluator );
    root->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
    root->AddPlugin( "DEFAULT_CIRCLE", timeEvaluator );
    root->AddPlugin( "DEFAULT_COLOR", timeEvaluator );

    //root->GetPlugin( "solid color" )->GetRendererContext()->cullCtx->enabled = false;

    model::SetParameter( root->GetPlugin( "solid color" )->GetParameter( "color" ), 0.f, glm::vec4( 1, 1, 1, 1 ) );

    //model::SetParameter( root->GetPlugin( "circle" )->GetParameter( "tesselation" ), 100.f, 100 );
    root->GetPlugin( "circle" )->Update(0); // FIXME: to generate geometry only

    auto plugin = root->GetPlugin( "circle" );
    SetParameter( plugin->GetParameter( model::DefaultCirclePlugin::PN_INNER_RADIUS ), 10.f, 0.8f );
    SetParameter( plugin->GetParameter( model::DefaultCirclePlugin::PN_OUTER_RADIUS ), 4.f, 1.f );
    SetParameter( plugin->GetParameter( model::DefaultCirclePlugin::PN_OUTER_RADIUS ), 10.f, 2.f );
    SetParameter( plugin->GetParameter( model::DefaultCirclePlugin::PN_OPEN_ANGLE ), 30.f, 90.f );
    //plugin->GetParameter( model::DefaultCirclePlugin::PN_INNER_RADIUS )->SetInterpolationMethod( model::IParameter::InterpolationMethod::COSINE );
    //plugin->GetParameter( model::DefaultCirclePlugin::PN_OPEN_ANGLE )->SetInterpolationMethod( model::IParameter::InterpolationMethod::COSINE );

    //SetParameter( plugin->GetParameter( model::DefaultCirclePlugin::PN_OPEN_ANGLE_MODE ), 10.f, model::DefaultCirclePlugin::OpenAngleMode::SYMMETRIC );
    auto param = plugin->GetParameter( model::DefaultCirclePlugin::PN_OPEN_ANGLE_MODE );
    auto qParam = model::QueryTypedParam< std::shared_ptr< model::ParamEnum< model::DefaultCirclePlugin::OpenAngleMode > > >( param );
    qParam->SetVal( model::DefaultCirclePlugin::OpenAngleMode::SYMMETRIC, 9.f );

    return root;
}

model::BasicNodePtr    /*TestScenesFactory::*/CreedEllipseTestScene     ( model::ITimeEvaluatorPtr timeEvaluator )
{
    model::BasicNodePtr root = model::BasicNode::Create( "rootNode", timeEvaluator );
    root->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
    root->AddPlugin( "DEFAULT_ELLIPSE", timeEvaluator );
    root->AddPlugin( "DEFAULT_COLOR", timeEvaluator );

    model::SetParameter( root->GetPlugin( "solid color" )->GetParameter( "color" ), 0.f, glm::vec4( 1, 1, 1, 1 ) );

    auto plugin = root->GetPlugin( "ellipse" );
    //SetParameter( plugin->GetParameter( model::DefaultEllipsePlugin::PN_OPEN_ANGLE ), 10.f, 0.8f );
    SetParameter( plugin->GetParameter( model::DefaultEllipsePlugin::PN_OUTER_RADIUS1 ), 0.f, 1.5f );
    SetParameter( plugin->GetParameter( model::DefaultEllipsePlugin::PN_OUTER_RADIUS2 ), 0.f, 0.5f );
    SetParameter( plugin->GetParameter( model::DefaultEllipsePlugin::PN_QUALITY ), 100.f, 100.f );

    plugin->Update(0); // FIXME: to generate geometry only

    ////SetParameter( plugin->GetParameter( model::DefaultCirclePlugin::PN_OPEN_ANGLE_MODE ), 10.f, model::DefaultCirclePlugin::OpenAngleMode::SYMMETRIC );
    //auto param = plugin->GetParameter( model::DefaultCirclePlugin::PN_OPEN_ANGLE_MODE );
    //auto qParam = model::QueryTypedParam< std::shared_ptr< model::ParamEnum< model::DefaultCirclePlugin::OpenAngleMode > > >( param );
    //qParam->SetVal( model::DefaultCirclePlugin::OpenAngleMode::SYMMETRIC, 9.f );

    return root;
}

model::BasicNodePtr    /*TestScenesFactory::*/CreedTriangleTestScene     ( model::ITimeEvaluatorPtr timeEvaluator )
{
    model::BasicNodePtr root = model::BasicNode::Create( "rootNode", timeEvaluator );
    root->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
    root->AddPlugin( "DEFAULT_TRIANGLE", timeEvaluator );
    root->AddPlugin( "DEFAULT_COLOR", timeEvaluator );

    model::SetParameter( root->GetPlugin( "solid color" )->GetParameter( "color" ), 0.f, glm::vec4( 1, 1, 1, 1 ) );

    auto plugin = root->GetPlugin( "triangle" );

    model::SetParameter( plugin->GetParameter( model::DefaultTrianglePlugin::PN_POINTA ), 0.f, glm::vec3( 1, 0, 0 ) );
    model::SetParameter( plugin->GetParameter( model::DefaultTrianglePlugin::PN_POINTB ), 0.f, glm::vec3( 0, 1, 0 ) );
    model::SetParameter( plugin->GetParameter( model::DefaultTrianglePlugin::PN_POINTC ), 0.f, glm::vec3( 0, 0, 1 ) );

    model::SetParameter( plugin->GetParameter( model::DefaultTrianglePlugin::PN_POINTA ), 10.f, glm::vec3( 0, 0, 1 ) );
    model::SetParameter( plugin->GetParameter( model::DefaultTrianglePlugin::PN_POINTB ), 10.f, glm::vec3( 1, 0, 0 ) );
    model::SetParameter( plugin->GetParameter( model::DefaultTrianglePlugin::PN_POINTC ), 10.f, glm::vec3( 0, 1, 0 ) );

    plugin->Update(0); // FIXME: to generate geometry only

    return root;
}

//model::BasicNodePtr    TestScenesFactory::CreedBasicGeometryTestScene     ( model::ITimeEvaluatorPtr timeEvaluator )
model::BasicNodePtr    /*TestScenesFactory::*/CreedRoundedRectTestScene     ( model::ITimeEvaluatorPtr timeEvaluator )
{
    model::BasicNodePtr root = model::BasicNode::Create( "rootNode", timeEvaluator );
    root->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
    root->AddPlugin( "DEFAULT_ROUNDEDRECT", timeEvaluator );
    root->AddPlugin( "DEFAULT_COLOR", timeEvaluator );

    model::SetParameter( root->GetPlugin( "solid color" )->GetParameter( "color" ), 0.f, glm::vec4( 1, 1, 1, 1 ) );

    auto plugin = root->GetPlugin( "rounded rect" );

    model::SetParameter( plugin->GetParameter( model::DefaultRoundedRectPlugin::PN_BEVELS ), 0.f, glm::vec4( 0, 0.1, 0.1, 0 ) );
    model::SetParameter( plugin->GetParameter( model::DefaultRoundedRectPlugin::PN_BEVELS ), 5.f, glm::vec4( 0.1, 0.2, 0, 0.1 ) );

    plugin->Update(0); // FIXME: to generate geometry only

    return root;
}

//model::BasicNodePtr    TestScenesFactory::CreedBasicGeometryTestScene     ( model::ITimeEvaluatorPtr timeEvaluator )
model::BasicNodePtr    /*TestScenesFactory::*/CreedConeTestScene     ( model::ITimeEvaluatorPtr timeEvaluator )
{
    model::BasicNodePtr root = model::BasicNode::Create( "rootNode", timeEvaluator );

    root->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
    root->AddPlugin( "DEFAULT_CONE", timeEvaluator );
    root->AddPlugin( "DEFAULT_TEXTURE", timeEvaluator );

    //model::SetParameterRotation( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 1, 0, 0 ), 0.f );
    model::SetParameterRotation( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 10.f, glm::vec3( -90.f, 0, 0 ) );
    //model::SetParameterRotation( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 1, 0, 0 ), -90.f );
    model::SetParameterTranslation( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 0, 0, -1 ) );

    auto plugin = root->GetPlugin( "cone" );
    assert( plugin );
    model::SetParameter( plugin->GetParameter( model::DefaultCone::PN::TESSELATION ), 10.f, 10 );
    model::SetParameter( plugin->GetParameter( model::DefaultCone::PN::INNERRADIUS ), 0.f, 0.5f );
    model::SetParameter( plugin->GetParameter( model::DefaultCone::PN::INNERHEIGHT ), 0.f, 0.5f );

    //root->GetPlugin( "texture" )->GetRendererContext()->cullCtx->enabled = true;
    model::LoadTexture( root->GetPlugin( "texture" ), "Assets/Textures/time_zones_4.jpg" );

    return root;

    //root->AddPlugin( "DEFAULT_COLOR", timeEvaluator );
    //model::SetParameter( root->GetPlugin( "solid color" )->GetParameter( "color" ), 0.f, glm::vec4( 1, 1, 1, 1 ) );
    //root->GetPlugin( "solid color" )->GetRendererContext()->fillCtx->fillMode = model::FillContext::Mode::M_LINES;
}

model::BasicNodePtr    TestScenesFactory::CreedBasicGeometryTestScene     ( model::ITimeEvaluatorPtr timeEvaluator )
//model::BasicNodePtr    /*TestScenesFactory::*/CreedCubeTestScene     ( model::ITimeEvaluatorPtr timeEvaluator )
{
    model::BasicNodePtr root = model::BasicNode::Create( "rootNode", timeEvaluator );

    root->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
    root->AddPlugin( "DEFAULT_CUBE", timeEvaluator );
    //root->AddPlugin( "DEFAULT_RECTANGLE", timeEvaluator );

    //model::SetParameterRotation( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 1, 0, 0 ), 0.f );
    model::SetParameterRotation( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 1.f, glm::vec3( 0, 45.f, 0 ) );
    //model::SetParameterRotation( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 0, 0, 0 ), -90.f );
    model::SetParameterScale( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 1.5, 1.5, 1.5 ) );
    model::SetParameterTranslation( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 0, 0, -1 ) );

    auto plugin = root->GetPlugin( "cube" );
    assert( plugin );
    model::SetParameter( plugin->GetParameter( model::DefaultCubePlugin::PARAM::BEVEL ), 20.f, 0.4f );
    model::SetParameter( plugin->GetParameter( model::DefaultCubePlugin::PARAM::TESSELATION ), 0.f, 10 );

    root->AddPlugin( "DEFAULT_TEXTURE", timeEvaluator );
    root->GetPlugin( "texture" )->GetRendererContext()->cullCtx->enabled = false;

    model::LoadTexture( root->GetPlugin( "texture" ), "Assets/Textures/time_zones_4.jpg" );
    model::SetParameter( root->GetPlugin( "texture" )->GetResourceStateModel( "Tex0" )->GetParameter( "borderColor" ), 0.f, glm::vec4( 1, 0, 0, 1 ) );

    //root->AddPlugin( "DEFAULT_COLOR", timeEvaluator );
    //model::SetParameter( root->GetPlugin( "solid color" )->GetParameter( "color" ), 0.f, glm::vec4( 1, 1, 1, 1 ) );
    //root->GetPlugin( "solid color" )->GetRendererContext()->fillCtx->fillMode = model::FillContext::Mode::M_LINES;
    //root->GetPlugin( "solid color" )->GetRendererContext()->cullCtx->enabled = false;

    return root;
}

// *******************************
//
model::BasicNodePtr    TestScenesFactory::MeshTestScene     ( model::ITimeEvaluatorPtr timeEvaluator )
{
    auto root = model::BasicNode::Create( "rootNode", timeEvaluator );

    //auto child1 = model::BasicNode::Create( "child1", timeEvaluator );
    //root->AddChildToModelOnly( child1 );

    //auto child2 = model::BasicNode::Create( "child2", timeEvaluator );
    //root->AddChildToModelOnly( child2 );

    //root->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
    //root->AddPlugin( "DEFAULT_MESH", timeEvaluator );
    //root->AddPlugin( "DEFAULT_MATERIAL", timeEvaluator );
    //root->AddPlugin( "DEFAULT_TEXTURE", timeEvaluator );

    //child1->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
    //child1->AddPlugin( "DEFAULT_MESH", timeEvaluator );
    //child1->AddPlugin( "DEFAULT_MATERIAL", timeEvaluator );
    //child1->AddPlugin( "DEFAULT_TEXTURE", timeEvaluator );
    
    //child2->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
    //child2->AddPlugin( "DEFAULT_MESH", timeEvaluator );
    //child2->AddPlugin( "DEFAULT_MATERIAL", timeEvaluator );

    //model::SetParameterScale( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 0.01f, 0.01f, 0.01f ) );
    //model::SetParameterRotation( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 20.f, 0.f, 0.f ) );
    
    //model::SetParameterScale( child1->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 0.2f, 0.2f, 0.2f ) );
    //model::SetParameterTranslation( child1->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( -400.0f, 400.0f, -500.0f ) );
    //model::SetParameterRotation( child1->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 20.f, 30.f, 0.f ) );

    //model::SetParameterScale( child2->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 4.0f, 4.0f, 4.0f ) );
    //model::SetParameterTranslation( child2->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 500.0f, -300.0f, -200.0f ) );
    //model::SetParameterRotation( child2->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 0.f, 50.f, 0.f ) );

    //auto material = root->GetPlugin( "material" );
    //model::SetParameter( material->GetParameter( "mtlDiffuse" ), 0.0, glm::vec4( 1, 1, 1, 1 ) );
    //model::SetParameter( material->GetParameter( "mtlAmbient" ), 0.0, glm::vec4( 0, 0, 0, 0 ) );
    //model::SetParameter( material->GetParameter( "mtlSpecular" ), 0.0, glm::vec4( 1.0, 1.0, 1.0, 1.0 ) );
    //model::SetParameter( material->GetParameter( "mtlEmission" ), 0.0, glm::vec4( 0, 0, 0, 0 ) );
    //model::SetParameter( material->GetParameter( "mtlShininess" ), 0.0, 128 );
    
//    model::LoadMesh( root->GetPlugin( "mesh" ), "meshes/daria/bunny.obj" );
//    model::LoadTexture( root->GetPlugin( "texture" ), "meshes/daria/bunny-atlas.jpg" );

    //auto meshLoader = nodelogic::MeshLoader::Create( root, timeEvaluator, "meshes/StarWarsFighter/ARC.FBX" );
    //auto meshLoader = nodelogic::MeshLoader::Create( root, timeEvaluator, "meshes/daria/cube.obj" );
    //root->SetLogic( meshLoader );

    //auto rootNode = meshLoader->Load( meshLoader->GetMeshAsset(), timeEvaluator );
    //for( UInt32 i = 0; i < rootNode->GetNumChildren(); ++i )
    //{
    //    root->AddChildToModelOnly( rootNode->GetChild( i ), i );
    //}

    //auto child = root->GetChild( 0 )->GetChild( 0 )->GetChild( 0 );
    
    //child->AddPlugin( "DEFAULT_TEXTURE", "texture", timeEvaluator );
    //model::LoadTexture( child->GetPlugin( "texture" ), "meshes/daria/brickwall.jpg" );
    
    //child->AddPlugin( "DEFAULT_NORMAL_MAP", "normal map", timeEvaluator );
    //model::LoadTexture( child->GetPlugin( "normal map" ), "meshes/daria/brickwall_normal.jpg" );

    //child->AddPlugin( "DEFAULT_PARALLAX_MAP", "parallax map", timeEvaluator );
    //model::LoadTexture( child->GetPlugin( "parallax map" ), "meshes/daria/bricks2_disp.jpg" );

    //child->AddPlugin( "DEFAULT_TEXTURE", "texture", timeEvaluator );
    //model::LoadTexture( child->GetPlugin( "texture" ), "meshes/daria/d.jpg" );
    //
    //child->AddPlugin( "DEFAULT_NORMAL_MAP", "normal map", timeEvaluator );
    //model::LoadTexture( child->GetPlugin( "normal map" ), "meshes/daria/n.jpg" );
    //
    //child->AddPlugin( "DEFAULT_PARALLAX_MAP", "parallax map", timeEvaluator );
    //model::LoadTexture( child->GetPlugin( "parallax map" ), "meshes/daria/h.jpg" );

    //auto child = root->GetChild( 0 )->GetChild( 1 );

    //child->AddPlugin( "DEFAULT_NORMAL_MAP", "normalmap", timeEvaluator );
    //model::LoadTexture( child->GetPlugin( "normalmap" ), "meshes/StarWarsFighter/ARC170_TXT_VERSION_4_N.jpg" );
    
    return root;
}


model::BasicNodePtr     TestScenesFactory::EnvMappingTestScene             ( model::ITimeEvaluatorPtr timeEvaluator )
{
    // Root node
    auto root = model::BasicNode::Create( "rootNode", timeEvaluator );

    root->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
    model::SetParameterTranslation( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 0.0f, 0.0f, -5.0f ) );

    // Sphere node
    auto envSphereNode = model::BasicNode::Create( "sphereEnv", timeEvaluator );
    root->AddChildToModelOnly( envSphereNode );

    envSphereNode->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
    envSphereNode->AddPlugin( "DEFAULT_SPHERE", timeEvaluator );
    envSphereNode->AddPlugin( "DEFAULT_MATERIAL", timeEvaluator );
    envSphereNode->AddPlugin( "DEFAULT_ENVIRONMENTAL_TEXTURE", timeEvaluator );

    model::SetParameterTranslation( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 1.0f, 1.0f, -5.0f ) );
    model::SetParameterRotation( envSphereNode->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 30.f, glm::vec3( 0.0f, 900.0f, 0.0f ) );

    auto material = envSphereNode->GetPlugin( "material" );
    model::SetParameter( material->GetParameter( "mtlDiffuse" ), 0.0, glm::vec4( 1, 1, 1, 1 ) );
    model::SetParameter( material->GetParameter( "mtlAmbient" ), 0.0, glm::vec4( 0, 0, 0, 0 ) );
    model::SetParameter( material->GetParameter( "mtlSpecular" ), 0.0, glm::vec4( 1.0, 1.0, 1.0, 1.0 ) );
    model::SetParameter( material->GetParameter( "mtlEmission" ), 0.0, glm::vec4( 0.5, 0, 0, 0 ) );
    model::SetParameter( material->GetParameter( "mtlShininess" ), 0.0, 128 );

    auto envMap = envSphereNode->GetPlugin( "environmental tex" );
    model::SetParameter( envMap->GetParameter( "reflectivity" ), 0.0, 0.8f );
    
    model::LoadTexture( envSphereNode->GetPlugin( "environmental tex" ), "textures/witek/Env/EnvVillage.jpg" );

    // Cube node
    auto cubeNode = model::BasicNode::Create( "cubeEnvRefl", timeEvaluator );
    root->AddChildToModelOnly( cubeNode );

    cubeNode->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
    cubeNode->AddPlugin( "DEFAULT_CUBE", timeEvaluator );
    cubeNode->AddPlugin( "DEFAULT_MATERIAL", timeEvaluator );
    cubeNode->AddPlugin( "DEFAULT_ENVIRONMENTAL_TEXTURE", timeEvaluator );
    cubeNode->AddPlugin( "DEFAULT_ENV_REFLECTIVITY_MAP", timeEvaluator );

    model::SetParameterTranslation( cubeNode->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( -5.0f, -0.5f, -4.0f ) );
    model::SetParameterRotation( cubeNode->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 30.f, glm::vec3( 0.0f, 900.0f, 0.0f ) );

    material = cubeNode->GetPlugin( "material" );
    model::SetParameter( material->GetParameter( "mtlDiffuse" ), 0.0, glm::vec4( 1, 1, 1, 1 ) );
    model::SetParameter( material->GetParameter( "mtlAmbient" ), 0.0, glm::vec4( 0, 0, 0, 0 ) );
    model::SetParameter( material->GetParameter( "mtlSpecular" ), 0.0, glm::vec4( 1.0, 1.0, 1.0, 1.0 ) );
    model::SetParameter( material->GetParameter( "mtlEmission" ), 0.0, glm::vec4( 0.5, 0, 0, 0 ) );
    model::SetParameter( material->GetParameter( "mtlShininess" ), 0.0, 128 );

    envMap = cubeNode->GetPlugin( "environmental tex" );
    model::SetParameter( envMap->GetParameter( "reflectivity" ), 0.0, 0.9f );
    model::LoadTexture( envMap, "textures/witek/Env/EnvVillage.jpg" );

    auto reflectivityMap = cubeNode->GetPlugin( "env reflectivity map" );
    model::LoadTexture( reflectivityMap, "textures/witek/Env/Reflectivity.jpg" );

    // Sphere texture environemnt node
    auto sphereTexEnv = model::BasicNode::Create( "SphereTexEnv", timeEvaluator );
    root->AddChildToModelOnly( sphereTexEnv );

    sphereTexEnv->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
    sphereTexEnv->AddPlugin( "DEFAULT_SPHERE", timeEvaluator );
    sphereTexEnv->AddPlugin( "DEFAULT_TEXTURE", timeEvaluator );
    sphereTexEnv->AddPlugin( "DEFAULT_ENVIRONMENTAL_TEXTURE", timeEvaluator );

    model::SetParameterTranslation( sphereTexEnv->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( -8.0f, 1.5f, -2.0f ) );
    model::SetParameterRotation( sphereTexEnv->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 30.f, glm::vec3( 0.0f, 900.0f, 0.0f ) );

    envMap = sphereTexEnv->GetPlugin( "environmental tex" );
    model::SetParameter( envMap->GetParameter( "reflectivity" ), 0.0, 0.6f );
    model::LoadTexture( envMap, "textures/witek/Env/EnvVillage.jpg" );
    model::LoadTexture( sphereTexEnv->GetPlugin( "texture" ), "textures/ice.jpg" );

    // Cube texture environemnt reflectivity map node
    auto cubeTexEnvRefl = model::BasicNode::Create( "CubeTexEnvRefl", timeEvaluator );
    root->AddChildToModelOnly( cubeTexEnvRefl );

    cubeTexEnvRefl->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
    cubeTexEnvRefl->AddPlugin( "DEFAULT_CUBE", timeEvaluator );
    cubeTexEnvRefl->AddPlugin( "DEFAULT_TEXTURE", timeEvaluator );
    cubeTexEnvRefl->AddPlugin( "DEFAULT_ENVIRONMENTAL_TEXTURE", timeEvaluator );
    cubeTexEnvRefl->AddPlugin( "DEFAULT_ENV_REFLECTIVITY_MAP", timeEvaluator );

    model::SetParameterTranslation( cubeTexEnvRefl->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 4.0f, -0.5f, -1.0f ) );
    model::SetParameterRotation( cubeTexEnvRefl->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 30.f, glm::vec3( 0.0f, 900.0f, 0.0f ) );

    envMap = cubeTexEnvRefl->GetPlugin( "environmental tex" );
    model::SetParameter( envMap->GetParameter( "reflectivity" ), 0.0, 1.0f );
    model::LoadTexture( envMap, "textures/witek/Env/EnvVillage.jpg" );
    model::LoadTexture( cubeTexEnvRefl->GetPlugin( "texture" ), "textures/poison.jpg" );
    
    reflectivityMap = cubeTexEnvRefl->GetPlugin( "env reflectivity map" );
    model::LoadTexture( reflectivityMap, "textures/witek/Env/Reflectivity.jpg" );

    // Cube material texture environemnt node
    auto cubeMatTexEnv = model::BasicNode::Create( "CubeMatTexEnv", timeEvaluator );
    root->AddChildToModelOnly( cubeMatTexEnv );

    cubeMatTexEnv->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
    cubeMatTexEnv->AddPlugin( "DEFAULT_CUBE", timeEvaluator );
    cubeMatTexEnv->AddPlugin( "DEFAULT_MATERIAL", timeEvaluator );
    cubeMatTexEnv->AddPlugin( "DEFAULT_TEXTURE", timeEvaluator );
    cubeMatTexEnv->AddPlugin( "DEFAULT_ENVIRONMENTAL_TEXTURE", timeEvaluator );

    model::SetParameterTranslation( cubeMatTexEnv->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 1.0f, -2.0f, 2.0f ) );
    model::SetParameterRotation( cubeMatTexEnv->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 30.f, glm::vec3( 0.0f, 900.0f, 0.0f ) );

    material = cubeMatTexEnv->GetPlugin( "material" );
    model::SetParameter( material->GetParameter( "mtlDiffuse" ), 0.0, glm::vec4( 0, 0, 1, 1 ) );
    model::SetParameter( material->GetParameter( "mtlAmbient" ), 0.0, glm::vec4( 0, 0, 0, 0 ) );
    model::SetParameter( material->GetParameter( "mtlSpecular" ), 0.0, glm::vec4( 1.0, 0.0, 0.0, 1.0 ) );
    model::SetParameter( material->GetParameter( "mtlEmission" ), 0.0, glm::vec4( 1.0, 0.0, 0.0, 1.0 ) );
    model::SetParameter( material->GetParameter( "mtlShininess" ), 0.0, 128 );


    envMap = cubeMatTexEnv->GetPlugin( "environmental tex" );
    model::SetParameter( envMap->GetParameter( "reflectivity" ), 0.0, 0.5f );
    model::LoadTexture( envMap, "textures/witek/Env/EnvVillage.jpg" );
    model::LoadTexture( cubeMatTexEnv->GetPlugin( "texture" ), "textures/fire.jpg" );
    

    // Cube material, texture, environment, reflectivity map node
    auto cubeMatTexEnvRefl = model::BasicNode::Create( "CubeMatTexEnvRefl", timeEvaluator );
    root->AddChildToModelOnly( cubeMatTexEnvRefl );

    cubeMatTexEnvRefl->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
    cubeMatTexEnvRefl->AddPlugin( "DEFAULT_CUBE", timeEvaluator );
    cubeMatTexEnvRefl->AddPlugin( "DEFAULT_MATERIAL", timeEvaluator );
    cubeMatTexEnvRefl->AddPlugin( "DEFAULT_TEXTURE", timeEvaluator );
    cubeMatTexEnvRefl->AddPlugin( "DEFAULT_ENVIRONMENTAL_TEXTURE", timeEvaluator );
    cubeMatTexEnvRefl->AddPlugin( "DEFAULT_ENV_REFLECTIVITY_MAP", timeEvaluator );

    model::SetParameterTranslation( cubeMatTexEnvRefl->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( -3.0f, -2.0f, 4.0f ) );
    model::SetParameterRotation( cubeMatTexEnvRefl->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 30.f, glm::vec3( 0.0f, 900.0f, 0.0f ) );

    material = cubeMatTexEnvRefl->GetPlugin( "material" );
    model::SetParameter( material->GetParameter( "mtlDiffuse" ), 0.0, glm::vec4( 0, 0, 1, 1 ) );
    model::SetParameter( material->GetParameter( "mtlAmbient" ), 0.0, glm::vec4( 0, 0, 0, 0 ) );
    model::SetParameter( material->GetParameter( "mtlSpecular" ), 0.0, glm::vec4( 1.0, 0.0, 0.0, 1.0 ) );
    model::SetParameter( material->GetParameter( "mtlEmission" ), 0.0, glm::vec4( 0.2, 1.0, 0.1, 1.0 ) );
    model::SetParameter( material->GetParameter( "mtlShininess" ), 0.0, 128 );


    envMap = cubeMatTexEnvRefl->GetPlugin( "environmental tex" );
    model::SetParameter( envMap->GetParameter( "reflectivity" ), 0.0, 0.5f );
    model::LoadTexture( envMap, "textures/witek/Env/EnvVillage.jpg" );
    model::LoadTexture( cubeMatTexEnvRefl->GetPlugin( "texture" ), "textures/fire.jpg" );

    reflectivityMap = cubeMatTexEnvRefl->GetPlugin( "env reflectivity map" );
    model::LoadTexture( reflectivityMap, "textures/witek/Env/Reflectivity.jpg" );

    // Mesh material, normal map, environement node
    auto meshMatBumpEnv = model::BasicNode::Create( "CubeMatBumpEnv", timeEvaluator );
    root->AddChildToModelOnly( meshMatBumpEnv );

    meshMatBumpEnv->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
    meshMatBumpEnv->AddPlugin( "DEFAULT_MESH", timeEvaluator );
    meshMatBumpEnv->AddPlugin( "DEFAULT_MATERIAL", timeEvaluator );
    meshMatBumpEnv->AddPlugin( "DEFAULT_NORMAL_MAP", timeEvaluator );
    meshMatBumpEnv->AddPlugin( "DEFAULT_ENVIRONMENTAL_TEXTURE", timeEvaluator );

    model::SetParameterTranslation( meshMatBumpEnv->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( -1.0f, -2.8f, 4.0f ) );
    model::SetParameterRotation( meshMatBumpEnv->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 30.f, glm::vec3( 0.0f, 900.0f, 0.0f ) );

    material = meshMatBumpEnv->GetPlugin( "material" );
    model::SetParameter( material->GetParameter( "mtlDiffuse" ), 0.0, glm::vec4( 0, 0, 1, 1 ) );
    model::SetParameter( material->GetParameter( "mtlAmbient" ), 0.0, glm::vec4( 0, 0, 0, 0 ) );
    model::SetParameter( material->GetParameter( "mtlSpecular" ), 0.0, glm::vec4( 1.0, 0.0, 0.0, 1.0 ) );
    model::SetParameter( material->GetParameter( "mtlEmission" ), 0.0, glm::vec4( 0.2, 1.0, 0.1, 1.0 ) );
    model::SetParameter( material->GetParameter( "mtlShininess" ), 0.0, 128 );


    envMap = meshMatBumpEnv->GetPlugin( "environmental tex" );
    model::SetParameter( envMap->GetParameter( "reflectivity" ), 0.0, 1.0f );
    model::LoadTexture( envMap, "textures/witek/Env/EnvVillage.jpg" );

    auto normalMap = meshMatBumpEnv->GetPlugin( "normal map" );
    model::LoadTexture( normalMap, "meshes/daria/bricks2_normal.jpg" );
    
    model::LoadMesh( meshMatBumpEnv->GetPlugin( "mesh" ), "meshes/daria/cube.obj" );

    // Mesh material, normal map, environment, reflectivity map node
    auto meshMatBumpEnvRefl = model::BasicNode::Create( "MeshMatBumpEnvRefl", timeEvaluator );
    root->AddChildToModelOnly( meshMatBumpEnvRefl );

    meshMatBumpEnvRefl->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
    meshMatBumpEnvRefl->AddPlugin( "DEFAULT_MESH", timeEvaluator );
    meshMatBumpEnvRefl->AddPlugin( "DEFAULT_MATERIAL", timeEvaluator );
    meshMatBumpEnvRefl->AddPlugin( "DEFAULT_NORMAL_MAP", timeEvaluator );
    meshMatBumpEnvRefl->AddPlugin( "DEFAULT_ENVIRONMENTAL_TEXTURE", timeEvaluator );
    meshMatBumpEnvRefl->AddPlugin( "DEFAULT_ENV_REFLECTIVITY_MAP", timeEvaluator );

    model::SetParameterTranslation( meshMatBumpEnvRefl->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 3.0f, -2.0f, 4.0f ) );
    model::SetParameterRotation( meshMatBumpEnvRefl->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 30.f, glm::vec3( 0.0f, 900.0f, 0.0f ) );

    material = meshMatBumpEnvRefl->GetPlugin( "material" );
    model::SetParameter( material->GetParameter( "mtlDiffuse" ), 0.0, glm::vec4( 0, 0, 1, 1 ) );
    model::SetParameter( material->GetParameter( "mtlAmbient" ), 0.0, glm::vec4( 0, 0, 0, 0 ) );
    model::SetParameter( material->GetParameter( "mtlSpecular" ), 0.0, glm::vec4( 1.0, 0.0, 0.0, 1.0 ) );
    model::SetParameter( material->GetParameter( "mtlEmission" ), 0.0, glm::vec4( 0.8, 0.1, 0.7, 1.0 ) );
    model::SetParameter( material->GetParameter( "mtlShininess" ), 0.0, 128 );

    reflectivityMap = meshMatBumpEnvRefl->GetPlugin( "env reflectivity map" );
    model::LoadTexture( reflectivityMap, "textures/witek/Env/Reflectivity.jpg" );

    envMap = meshMatBumpEnvRefl->GetPlugin( "environmental tex" );
    model::SetParameter( envMap->GetParameter( "reflectivity" ), 0.0, 1.0f );
    model::LoadTexture( envMap, "textures/witek/Env/EnvVillage.jpg" );

    normalMap = meshMatBumpEnvRefl->GetPlugin( "normal map" );
    model::LoadTexture( normalMap, "meshes/daria/bricks2_normal.jpg" );
    
    model::LoadMesh( meshMatBumpEnvRefl->GetPlugin( "mesh" ), "meshes/daria/cube.obj" );

    // Mesh material, texture, normal map, environment node
    auto meshMatTexBumpEnv = model::BasicNode::Create( "MeshMatTexBumpEnv", timeEvaluator );
    root->AddChildToModelOnly( meshMatTexBumpEnv );

    meshMatTexBumpEnv->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
    meshMatTexBumpEnv->AddPlugin( "DEFAULT_MESH", timeEvaluator );
    meshMatTexBumpEnv->AddPlugin( "DEFAULT_MATERIAL", timeEvaluator );
    meshMatTexBumpEnv->AddPlugin( "DEFAULT_TEXTURE", timeEvaluator );
    meshMatTexBumpEnv->AddPlugin( "DEFAULT_NORMAL_MAP", timeEvaluator );
    meshMatTexBumpEnv->AddPlugin( "DEFAULT_ENVIRONMENTAL_TEXTURE", timeEvaluator );

    model::SetParameterTranslation( meshMatTexBumpEnv->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( -3.5f, 1.8f, -1.0f ) );
    model::SetParameterRotation( meshMatTexBumpEnv->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 30.f, glm::vec3( 0.0f, 900.0f, 0.0f ) );

    material = meshMatTexBumpEnv->GetPlugin( "material" );
    model::SetParameter( material->GetParameter( "mtlDiffuse" ), 0.0, glm::vec4( 0, 0, 1, 1 ) );
    model::SetParameter( material->GetParameter( "mtlAmbient" ), 0.0, glm::vec4( 0, 0, 0, 0 ) );
    model::SetParameter( material->GetParameter( "mtlSpecular" ), 0.0, glm::vec4( 1.0, 0.0, 0.0, 1.0 ) );
    model::SetParameter( material->GetParameter( "mtlEmission" ), 0.0, glm::vec4( 0.2, 1.0, 0.1, 1.0 ) );
    model::SetParameter( material->GetParameter( "mtlShininess" ), 0.0, 128 );


    envMap = meshMatTexBumpEnv->GetPlugin( "environmental tex" );
    model::SetParameter( envMap->GetParameter( "reflectivity" ), 0.0, 0.6f );
    model::LoadTexture( envMap, "textures/witek/Env/EnvVillage.jpg" );
    model::LoadTexture( meshMatTexBumpEnv->GetPlugin( "texture" ), "textures/sand.jpg" );

    normalMap = meshMatTexBumpEnv->GetPlugin( "normal map" );
    model::LoadTexture( normalMap, "meshes/daria/bricks2_normal.jpg" );
    
    model::LoadMesh( meshMatTexBumpEnv->GetPlugin( "mesh" ), "meshes/daria/cube.obj" );

    // Mesh material, texture, normal map, environment, reflectivity map node
    auto meshMatTexBumpEnvRefl = model::BasicNode::Create( "MeshMatTexBumpEnvRefl", timeEvaluator );
    root->AddChildToModelOnly( meshMatTexBumpEnvRefl );

    meshMatTexBumpEnvRefl->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
    meshMatTexBumpEnvRefl->AddPlugin( "DEFAULT_MESH", timeEvaluator );
    meshMatTexBumpEnvRefl->AddPlugin( "DEFAULT_MATERIAL", timeEvaluator );
    meshMatTexBumpEnvRefl->AddPlugin( "DEFAULT_TEXTURE", timeEvaluator );
    meshMatTexBumpEnvRefl->AddPlugin( "DEFAULT_NORMAL_MAP", timeEvaluator );
    meshMatTexBumpEnvRefl->AddPlugin( "DEFAULT_ENVIRONMENTAL_TEXTURE", timeEvaluator );
    meshMatTexBumpEnvRefl->AddPlugin( "DEFAULT_ENV_REFLECTIVITY_MAP", timeEvaluator );

    model::SetParameterTranslation( meshMatTexBumpEnvRefl->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( -5.5f, -2.3f, 3.0f ) );
    model::SetParameterRotation( meshMatTexBumpEnvRefl->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 30.f, glm::vec3( 0.0f, 900.0f, 0.0f ) );

    material = meshMatTexBumpEnvRefl->GetPlugin( "material" );
    model::SetParameter( material->GetParameter( "mtlDiffuse" ), 0.0, glm::vec4( 0, 0, 1, 1 ) );
    model::SetParameter( material->GetParameter( "mtlAmbient" ), 0.0, glm::vec4( 0, 0, 0, 0 ) );
    model::SetParameter( material->GetParameter( "mtlSpecular" ), 0.0, glm::vec4( 1.0, 0.0, 0.0, 1.0 ) );
    model::SetParameter( material->GetParameter( "mtlEmission" ), 0.0, glm::vec4( 0.1, 0.0, 1.0, 1.0 ) );
    model::SetParameter( material->GetParameter( "mtlShininess" ), 0.0, 128 );

    reflectivityMap = meshMatTexBumpEnvRefl->GetPlugin( "env reflectivity map" );
    model::LoadTexture( reflectivityMap, "textures/witek/Env/Reflectivity.jpg" );

    envMap = meshMatTexBumpEnvRefl->GetPlugin( "environmental tex" );
    model::SetParameter( envMap->GetParameter( "reflectivity" ), 0.0, 0.6f );
    model::LoadTexture( envMap, "textures/witek/Env/EnvVillage.jpg" );
    model::LoadTexture( meshMatTexBumpEnvRefl->GetPlugin( "texture" ), "textures/sand.jpg" );

    normalMap = meshMatTexBumpEnvRefl->GetPlugin( "normal map" );
    model::LoadTexture( normalMap, "meshes/daria/bricks2_normal.jpg" );
    
    model::LoadMesh( meshMatTexBumpEnvRefl->GetPlugin( "mesh" ), "meshes/daria/cube.obj" );


    // Mesh material, normal map, paralax, environement node
    auto meshMatBumpParalaxEnv = model::BasicNode::Create( "MeshMatBumpParalaxEnv", timeEvaluator );
    root->AddChildToModelOnly( meshMatBumpParalaxEnv );

    meshMatBumpParalaxEnv->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
    meshMatBumpParalaxEnv->AddPlugin( "DEFAULT_MESH", timeEvaluator );
    meshMatBumpParalaxEnv->AddPlugin( "DEFAULT_MATERIAL", timeEvaluator );
    meshMatBumpParalaxEnv->AddPlugin( "DEFAULT_NORMAL_MAP", timeEvaluator );
    meshMatBumpParalaxEnv->AddPlugin( "DEFAULT_PARALLAX_MAP", timeEvaluator );
    meshMatBumpParalaxEnv->AddPlugin( "DEFAULT_ENVIRONMENTAL_TEXTURE", timeEvaluator );

    model::SetParameterTranslation( meshMatBumpParalaxEnv->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 2.0f, 3.0f, -3.0f ) );
    model::SetParameterRotation( meshMatBumpParalaxEnv->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 30.f, glm::vec3( 0.0f, 900.0f, 0.0f ) );

    material = meshMatBumpParalaxEnv->GetPlugin( "material" );
    model::SetParameter( material->GetParameter( "mtlDiffuse" ), 0.0, glm::vec4( 0, 0, 1, 1 ) );
    model::SetParameter( material->GetParameter( "mtlAmbient" ), 0.0, glm::vec4( 0, 0, 0, 0 ) );
    model::SetParameter( material->GetParameter( "mtlSpecular" ), 0.0, glm::vec4( 1.0, 0.0, 0.0, 1.0 ) );
    model::SetParameter( material->GetParameter( "mtlEmission" ), 0.0, glm::vec4( 0.0, 0.0, 1.0, 1.0 ) );
    model::SetParameter( material->GetParameter( "mtlShininess" ), 0.0, 128 );


    envMap = meshMatBumpParalaxEnv->GetPlugin( "environmental tex" );
    model::SetParameter( envMap->GetParameter( "reflectivity" ), 0.0, 0.7f );
    model::LoadTexture( envMap, "textures/witek/Env/EnvVillage.jpg" );

    normalMap = meshMatBumpParalaxEnv->GetPlugin( "normal map" );
    model::LoadTexture( normalMap, "meshes/daria/bricks2_normal.jpg" );

    model::LoadTexture( meshMatBumpParalaxEnv->GetPlugin( "parallax map" ), "meshes/daria/bricks2_disp.jpg" );
    
    model::LoadMesh( meshMatBumpParalaxEnv->GetPlugin( "mesh" ), "meshes/daria/cube.obj" );

    // Mesh, material, normal map, paralax, environement, reflectivity node
    auto meshMatBumpParalaxEnvRefl = model::BasicNode::Create( "MeshMatBumpParalaxEnvRefl", timeEvaluator );
    root->AddChildToModelOnly( meshMatBumpParalaxEnvRefl );

    meshMatBumpParalaxEnvRefl->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
    meshMatBumpParalaxEnvRefl->AddPlugin( "DEFAULT_MESH", timeEvaluator );
    meshMatBumpParalaxEnvRefl->AddPlugin( "DEFAULT_MATERIAL", timeEvaluator );
    meshMatBumpParalaxEnvRefl->AddPlugin( "DEFAULT_NORMAL_MAP", timeEvaluator );
    meshMatBumpParalaxEnvRefl->AddPlugin( "DEFAULT_PARALLAX_MAP", timeEvaluator );
    meshMatBumpParalaxEnvRefl->AddPlugin( "DEFAULT_ENVIRONMENTAL_TEXTURE", timeEvaluator );
    meshMatBumpParalaxEnvRefl->AddPlugin( "DEFAULT_ENV_REFLECTIVITY_MAP", timeEvaluator );

    model::SetParameterTranslation( meshMatBumpParalaxEnvRefl->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 5.8f, 3.0f, -3.0f ) );
    model::SetParameterRotation( meshMatBumpParalaxEnvRefl->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 30.f, glm::vec3( 0.0f, 900.0f, 0.0f ) );

    material = meshMatBumpParalaxEnvRefl->GetPlugin( "material" );
    model::SetParameter( material->GetParameter( "mtlDiffuse" ), 0.0, glm::vec4( 0, 0, 1, 1 ) );
    model::SetParameter( material->GetParameter( "mtlAmbient" ), 0.0, glm::vec4( 0, 0, 0, 0 ) );
    model::SetParameter( material->GetParameter( "mtlSpecular" ), 0.0, glm::vec4( 1.0, 0.0, 0.0, 1.0 ) );
    model::SetParameter( material->GetParameter( "mtlEmission" ), 0.0, glm::vec4( 1.0, 0.0, 0.0, 1.0 ) );
    model::SetParameter( material->GetParameter( "mtlShininess" ), 0.0, 128 );

    reflectivityMap = meshMatBumpParalaxEnvRefl->GetPlugin( "env reflectivity map" );
    model::LoadTexture( reflectivityMap, "textures/witek/Env/Reflectivity.jpg" );

    envMap = meshMatBumpParalaxEnvRefl->GetPlugin( "environmental tex" );
    model::SetParameter( envMap->GetParameter( "reflectivity" ), 0.0, 0.7f );
    model::LoadTexture( envMap, "textures/witek/Env/EnvVillage.jpg" );

    normalMap = meshMatBumpParalaxEnvRefl->GetPlugin( "normal map" );
    model::LoadTexture( normalMap, "meshes/daria/bricks2_normal.jpg" );

    model::LoadTexture( meshMatBumpParalaxEnvRefl->GetPlugin( "parallax map" ), "meshes/daria/bricks2_disp.jpg" );
    
    model::LoadMesh( meshMatBumpParalaxEnvRefl->GetPlugin( "mesh" ), "meshes/daria/cube.obj" );

    // Mesh, material, texture, normal map, paralax, environement node
    auto meshMatTexBumpParalaxEnv = model::BasicNode::Create( "MeshMatTexBumpParalaxEnv", timeEvaluator );
    root->AddChildToModelOnly( meshMatTexBumpParalaxEnv );

    meshMatTexBumpParalaxEnv->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
    meshMatTexBumpParalaxEnv->AddPlugin( "DEFAULT_MESH", timeEvaluator );
    meshMatTexBumpParalaxEnv->AddPlugin( "DEFAULT_MATERIAL", timeEvaluator );
    meshMatTexBumpParalaxEnv->AddPlugin( "DEFAULT_TEXTURE", timeEvaluator );
    meshMatTexBumpParalaxEnv->AddPlugin( "DEFAULT_NORMAL_MAP", timeEvaluator );
    meshMatTexBumpParalaxEnv->AddPlugin( "DEFAULT_PARALLAX_MAP", timeEvaluator );
    meshMatTexBumpParalaxEnv->AddPlugin( "DEFAULT_ENVIRONMENTAL_TEXTURE", timeEvaluator );

    model::SetParameterTranslation( meshMatTexBumpParalaxEnv->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( -2.5f, -1.0f, -5.0f ) );
    model::SetParameterRotation( meshMatTexBumpParalaxEnv->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 30.f, glm::vec3( 0.0f, 900.0f, 0.0f ) );

    material = meshMatTexBumpParalaxEnv->GetPlugin( "material" );
    model::SetParameter( material->GetParameter( "mtlDiffuse" ), 0.0, glm::vec4( 0, 0, 1, 1 ) );
    model::SetParameter( material->GetParameter( "mtlAmbient" ), 0.0, glm::vec4( 0, 0, 0, 0 ) );
    model::SetParameter( material->GetParameter( "mtlSpecular" ), 0.0, glm::vec4( 1.0, 0.0, 0.0, 1.0 ) );
    model::SetParameter( material->GetParameter( "mtlEmission" ), 0.0, glm::vec4( 0.7, 1.0, 0.7, 1.0 ) );
    model::SetParameter( material->GetParameter( "mtlShininess" ), 0.0, 128 );

    envMap = meshMatTexBumpParalaxEnv->GetPlugin( "environmental tex" );
    model::SetParameter( envMap->GetParameter( "reflectivity" ), 0.0, 0.5f );
    model::LoadTexture( envMap, "textures/witek/Env/EnvVillage.jpg" );

    normalMap = meshMatTexBumpParalaxEnv->GetPlugin( "normal map" );
    model::LoadTexture( normalMap, "meshes/daria/bricks2_normal.jpg" );

    model::LoadTexture( meshMatTexBumpParalaxEnv->GetPlugin( "texture" ), "textures/poison.jpg" );
    model::LoadTexture( meshMatTexBumpParalaxEnv->GetPlugin( "parallax map" ), "meshes/daria/bricks2_disp.jpg" );
    
    model::LoadMesh( meshMatTexBumpParalaxEnv->GetPlugin( "mesh" ), "meshes/daria/cube.obj" );

    // Mesh, material, texture, normal map, paralax, enviroement, reflectivity map node
    auto meshMatTexBumpParalaxEnvRefl = model::BasicNode::Create( "MeshMatTexBumpParalaxEnvRefl", timeEvaluator );
    root->AddChildToModelOnly( meshMatTexBumpParalaxEnvRefl );

    meshMatTexBumpParalaxEnvRefl->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
    meshMatTexBumpParalaxEnvRefl->AddPlugin( "DEFAULT_MESH", timeEvaluator );
    meshMatTexBumpParalaxEnvRefl->AddPlugin( "DEFAULT_MATERIAL", timeEvaluator );
    meshMatTexBumpParalaxEnvRefl->AddPlugin( "DEFAULT_TEXTURE", timeEvaluator );
    meshMatTexBumpParalaxEnvRefl->AddPlugin( "DEFAULT_NORMAL_MAP", timeEvaluator );
    meshMatTexBumpParalaxEnvRefl->AddPlugin( "DEFAULT_PARALLAX_MAP", timeEvaluator );
    meshMatTexBumpParalaxEnvRefl->AddPlugin( "DEFAULT_ENVIRONMENTAL_TEXTURE", timeEvaluator );
    meshMatTexBumpParalaxEnvRefl->AddPlugin( "DEFAULT_ENV_REFLECTIVITY_MAP", timeEvaluator );

    model::SetParameterTranslation( meshMatTexBumpParalaxEnvRefl->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( -8.5f, -0.8f, -5.0f ) );
    model::SetParameterRotation( meshMatTexBumpParalaxEnvRefl->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 30.f, glm::vec3( 0.0f, 900.0f, 0.0f ) );

    material = meshMatTexBumpParalaxEnvRefl->GetPlugin( "material" );
    model::SetParameter( material->GetParameter( "mtlDiffuse" ), 0.0, glm::vec4( 0, 0, 1, 1 ) );
    model::SetParameter( material->GetParameter( "mtlAmbient" ), 0.0, glm::vec4( 0, 0, 0, 0 ) );
    model::SetParameter( material->GetParameter( "mtlSpecular" ), 0.0, glm::vec4( 1.0, 0.0, 0.0, 1.0 ) );
    model::SetParameter( material->GetParameter( "mtlEmission" ), 0.0, glm::vec4( 0.7, 1.0, 0.7, 1.0 ) );
    model::SetParameter( material->GetParameter( "mtlShininess" ), 0.0, 128 );

    reflectivityMap = meshMatTexBumpParalaxEnvRefl->GetPlugin( "env reflectivity map" );
    model::LoadTexture( reflectivityMap, "textures/witek/Env/Reflectivity.jpg" );

    envMap = meshMatTexBumpParalaxEnvRefl->GetPlugin( "environmental tex" );
    model::SetParameter( envMap->GetParameter( "reflectivity" ), 0.0, 0.5f );
    model::LoadTexture( envMap, "textures/witek/Env/EnvVillage.jpg" );

    normalMap = meshMatTexBumpParalaxEnvRefl->GetPlugin( "normal map" );
    model::LoadTexture( normalMap, "meshes/daria/bricks2_normal.jpg" );

    model::LoadTexture( meshMatTexBumpParalaxEnvRefl->GetPlugin( "texture" ), "textures/water.jpg" );
    model::LoadTexture( meshMatTexBumpParalaxEnvRefl->GetPlugin( "parallax map" ), "meshes/daria/bricks2_disp.jpg" );
    
    model::LoadMesh( meshMatTexBumpParalaxEnvRefl->GetPlugin( "mesh" ), "meshes/daria/cube.obj" );

    return root;
}

// ***********************
//
model::BasicNodePtr     TestScenesFactory::BlendTextureTestScene           ( model::ITimeEvaluatorPtr timeEvaluator )
{
    // Root node
    auto root = model::BasicNode::Create( "rootNode", timeEvaluator );
    root->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );

    // Rectangle node
    auto rectNode = model::BasicNode::Create( "RectNode", timeEvaluator );
    root->AddChildToModelOnly( rectNode );

    rectNode->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
    rectNode->AddPlugin( "DEFAULT_RECTANGLE", timeEvaluator );
    rectNode->AddPlugin( "DEFAULT_TEXTURE", timeEvaluator );
    rectNode->AddPlugin( "DEFAULT_BLEND_TEXTURE", timeEvaluator );

    auto rectangle = rectNode->GetPlugin( "rectangle" );
    model::SetParameter( rectangle->GetParameter( "width" ), 0.0f, 4.0f );
    model::SetParameter( rectangle->GetParameter( "height" ), 0.0f, 4.0f );

    auto blendTex = rectNode->GetPlugin( "blend texture" );
    model::SetParameter( blendTex->GetParameter( "blendingMode" ), 0.0f, model::BlendHelper::BlendMode::BM_Luminosity );

    model::LoadTexture( rectNode->GetPlugin( "texture" ), "textures/poison.jpg" );
    model::LoadTexture( rectNode->GetPlugin( "blend texture" ), "textures/lens_flares/1.png" );
    

    return root;
}


//model::BasicNodePtr    TestScenesFactory::CreedBasicGeometryTestScene     ( model::ITimeEvaluatorPtr timeEvaluator )
model::BasicNodePtr    /*TestScenesFactory::*/CreedTorusBasicGeometryTestScene     ( model::ITimeEvaluatorPtr timeEvaluator )
{
    model::BasicNodePtr root = model::BasicNode::Create( "rootNode", timeEvaluator );

    root->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
    root->AddPlugin( "DEFAULT_TORUS", timeEvaluator );

    //model::SetParameterRotation( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 1, 0, 0 ), 0.f );
    model::SetParameterRotation( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 5.f, glm::vec3( 90.f, 0, 0 ) );
    //model::SetParameterRotation( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 1, 0, 0 ), -90.f );
    model::SetParameterScale( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 1.5, 1.5, 1.5 ) );
    SetParameterTranslation( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 0, 0, -4 ) );

    auto plugin = root->GetPlugin( "torus" );
    assert( plugin );
    //model::SetParameter( plugin->GetParameter( model::DefaultCube::PN::BEVEL ), 5.f, 0.4f );
    //model::SetParameter( plugin->GetParameter( model::DefaultCube::PN::TESSELATION ), 0.f, 10 );

    root->AddPlugin( "DEFAULT_TEXTURE", timeEvaluator );
    root->GetPlugin( "texture" )->GetRendererContext()->cullCtx->enabled = false;

    model::LoadTexture( root->GetPlugin( "texture" ), "Assets/Textures/time_zones_4.jpg" );
    model::SetParameter( root->GetPlugin( "texture" )->GetResourceStateModel( "Tex0" )->GetParameter( "borderColor" ), 0.f, glm::vec4( 1, 0, 0, 1 ) );

    //root->AddPlugin( "DEFAULT_COLOR", timeEvaluator );
    //model::SetParameter( root->GetPlugin( "solid color" )->GetParameter( "color" ), 0.f, glm::vec4( 1, 1, 1, 1 ) );
    ////root->GetPlugin( "solid color" )->GetRendererContext()->fillCtx->fillMode = model::FillContext::Mode::M_LINES;
    //root->GetPlugin( "solid color" )->GetRendererContext()->cullCtx->enabled = false;

    return root;
}

//model::BasicNodePtr    TestScenesFactory::CreedBasicGeometryTestScene     ( model::ITimeEvaluatorPtr timeEvaluator )
model::BasicNodePtr    /*TestScenesFactory::*/CreedBasicGeometryTestScene     ( model::ITimeEvaluatorPtr timeEvaluator )
{
    model::BasicNodePtr root = model::BasicNode::Create( "rootNode", timeEvaluator );

    root->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
    root->AddPlugin( "DEFAULT_SPRING", timeEvaluator );

    //model::SetParameterRotation( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 1, 0, 0 ), 0.f );
    model::SetParameterRotation( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 5.f, glm::vec3( 90.f, 0, 0 ) );
    //model::SetParameterRotation( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 1, 0, 0 ), -90.f );
    model::SetParameterScale( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 1, 2, 1 ) );
    SetParameterTranslation( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.f, glm::vec3( 0, -1, -1 ) );

    auto plugin = root->GetPlugin( "spring" );
    assert( plugin );
    //model::SetParameter( plugin->GetParameter( model::DefaultCube::PN::BEVEL ), 5.f, 0.4f );
    //model::SetParameter( plugin->GetParameter( model::DefaultCube::PN::TESSELATION ), 0.f, 10 );

    root->AddPlugin( "DEFAULT_TEXTURE", timeEvaluator );
    root->GetPlugin( "texture" )->GetRendererContext()->cullCtx->enabled = false;

    model::LoadTexture( root->GetPlugin( "texture" ), "Assets/Textures/time_zones_4.jpg" );
    model::SetParameter( root->GetPlugin( "texture" )->GetResourceStateModel( "Tex0" )->GetParameter( "borderColor" ), 0.f, glm::vec4( 1, 0, 0, 1 ) );

    //root->AddPlugin( "DEFAULT_COLOR", timeEvaluator );
    //model::SetParameter( root->GetPlugin( "solid color" )->GetParameter( "color" ), 0.f, glm::vec4( 1, 1, 1, 1 ) );
    ////root->GetPlugin( "solid color" )->GetRendererContext()->fillCtx->fillMode = model::FillContext::Mode::M_LINES;
    //root->GetPlugin( "solid color" )->GetRendererContext()->cullCtx->enabled = false;

    return root;
}

    //static  model::BasicNodePtr     CreateSerializedTestScene       ( const model::PluginsManager * pluginsManager );

/**All basic shapes in one scene*/
model::BasicNodePtr		TestScenesFactory::BasicShapesShowScene		( const model::PluginsManager* pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    pluginsManager;

    auto node0 = SimpleNodesFactory::CreateBasicShapeShow( timeEvaluator, "DEFAULT_CONE", glm::vec3( 0.0, -0.5, -4.0 ), "textures/sand.jpg" );
    auto node1 = SimpleNodesFactory::CreateBasicShapeShow( timeEvaluator, "DEFAULT_CUBE", glm::vec3( 0.0, 2.0, 4.0 ), "textures/sand.jpg" );
    auto node2 = SimpleNodesFactory::CreateBasicShapeShow( timeEvaluator, "DEFAULT_CIRCLE", glm::vec3( -2.0, 0.0, 3.0 ), "textures/water.jpg" );
    auto node3 = SimpleNodesFactory::CreateBasicShapeShow( timeEvaluator, "DEFAULT_SPHERE", glm::vec3( -4.0, -3.0, 1.0 ), "textures/sand.jpg" );
    auto node4 = SimpleNodesFactory::CreateBasicShapeShow( timeEvaluator, "DEFAULT_ROUNDEDRECT", glm::vec3( -3.0, -2.0, -3.0 ), "textures/sand.jpg" );
    auto node5 = SimpleNodesFactory::CreateBasicShapeShow( timeEvaluator, "DEFAULT_TORUS", glm::vec3( 1.0, 0.0, -3.0 ), "textures/sand.jpg" );
    auto node6 = SimpleNodesFactory::CreateBasicShapeShow( timeEvaluator, "DEFAULT_SPRING", glm::vec3( 5.0, -5.0, 0.0 ), "textures/water.jpg" );
    auto node7 = SimpleNodesFactory::CreateBasicShapeShow( timeEvaluator, "DEFAULT_TRIANGLE", glm::vec3( 1.0, -0.7, -3.0 ), "textures/sand.jpg" );
    auto node8 = SimpleNodesFactory::CreateBasicShapeShow( timeEvaluator, "DEFAULT_CYLINDER", glm::vec3( -3.0, 2.0, -3.0 ), "textures/water.jpg" );
    auto node9 = SimpleNodesFactory::CreateBasicShapeShow( timeEvaluator, "DEFAULT_ELLIPSE", glm::vec3( 0.0, -3.0, 4.0 ), "textures/sand.jpg" );
    
    auto node10 = SimpleNodesFactory::CreateCreedTimerNode( timeEvaluator, 0, false );

    node0->AddChildToModelOnly( node1 );
    node0->AddChildToModelOnly( node2 );
    node0->AddChildToModelOnly( node3 );
    node0->AddChildToModelOnly( node4 );
    node0->AddChildToModelOnly( node5 );
    node0->AddChildToModelOnly( node6 );
    node0->AddChildToModelOnly( node7 );
    node0->AddChildToModelOnly( node8 );
    node0->AddChildToModelOnly( node9 );

    node0->AddChildToModelOnly( node10 );

    //node0->SetNodeEffect( model::ModelNodeEffectFactory::CreateModelNodeEffect( NodeEffectType::NET_BOUNDING_BOX, "bounding box", timeEvaluator ) );

    return node0;
}

model::BasicNodePtr		TestScenesFactory::BasicShapesTest		( const model::PluginsManager* pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    pluginsManager;

    auto node0 = SimpleNodesFactory::CreateBasicShapesTestNode( timeEvaluator );
    return node0;
}

model::BasicNodePtr     TestScenesFactory::AssetCacheTestScene         ( const model::PluginsManager* pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    pluginsManager;

    auto node0 = SimpleNodesFactory::CreateBasicShapeShow( timeEvaluator, "DEFAULT_CONE", glm::vec3( 0.0, 0.0, -4.0 ), "textures/sand.jpg" );
    auto node1 = SimpleNodesFactory::CreateBasicShapeShow( timeEvaluator, "DEFAULT_CUBE", glm::vec3( 0.0, 2.0, 4.0 ), "textures/sand.jpg" );
    auto node2 = SimpleNodesFactory::CreateBasicShapeShow( timeEvaluator, "DEFAULT_CYLINDER", glm::vec3( -3.0, 2.0, -3.0 ), "textures/water.jpg" );
    auto node3 = SimpleNodesFactory::CreateBasicShapeShow( timeEvaluator, "DEFAULT_CUBE", glm::vec3( 5.0, -5.0, 0.0 ), "textures/water.jpg" );

    auto node4 = SimpleNodesFactory::CreateTextCacheTest( timeEvaluator, "Text", glm::vec3( 0.0, -0.4, 0.0 ), glm::vec4( 1.0, 0.7, 0.0, 1.0 ), L"Astera tekst 1", "fonts/Astera.TTF" );
    auto node5 = SimpleNodesFactory::CreateTextCacheTest( timeEvaluator, "Text", glm::vec3( -3.0, 0.0, -3.0 ), glm::vec4( 1.0, 0.7, 0.0, 1.0 ), L"Astera tekst 2", "fonts/Astera.TTF" );
    auto node6 = SimpleNodesFactory::CreateTextCacheTest( timeEvaluator, "Text", glm::vec3( -4.0, -3.0, 1.0 ), glm::vec4( 1.0, 0.7, 0.0, 1.0 ), L"Courbi tekst 1", "fonts/courbi.ttf" );
    auto node7 = SimpleNodesFactory::CreateTextCacheTest( timeEvaluator, "Text", glm::vec3( 0.0, 0.0, 4.0 ), glm::vec4( 1.0, 0.7, 0.0, 1.0 ), L"Courbi tekst 2", "fonts/courbi.ttf" );
    auto node8 = SimpleNodesFactory::CreateTextCacheTest( timeEvaluator, "Text", glm::vec3( 1.0, 0.0, -3.0 ), glm::vec4( 1.0, 0.7, 0.0, 1.0 ), L"Cour tekst 1", "fonts/cour.ttf" );
    auto node9 = SimpleNodesFactory::CreateTextCacheTest( timeEvaluator, "Text", glm::vec3( -4.0, -3.0, 1.0 ), glm::vec4( 1.0, 0.7, 0.0, 1.0 ), L"Cour tekst 2", "fonts/cour.ttf" );

    node0->AddChildToModelOnly( node1 );
    node0->AddChildToModelOnly( node2 );
    node0->AddChildToModelOnly( node3 );

    node0->AddChildToModelOnly( node4 );
    node0->AddChildToModelOnly( node5 );
    node0->AddChildToModelOnly( node6 );
    node0->AddChildToModelOnly( node7 );
    node0->AddChildToModelOnly( node8 );
    node0->AddChildToModelOnly( node9 );

    return node0;
}


model::BasicNodePtr TestScenesFactory::RemoteEventsTestScene( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    pluginsManager;
    auto node0 = SimpleNodesFactory::CreateBasicShapeShow( timeEvaluator, "DEFAULT_CONE", glm::vec3( 0.0, 0.0, -4.0 ), "textures/water.jpg" );
    auto node1 = SimpleNodesFactory::CreateBasicShapeShow( timeEvaluator, "DEFAULT_CUBE", glm::vec3( 0.0, 2.0, 4.0 ), "textures/sand.jpg" );
    auto node2 = SimpleNodesFactory::CreateTextCacheTest( timeEvaluator, "Text", glm::vec3( 0.0, -0.4, 0.0 ), glm::vec4( 1.0, 0.7, 0.0, 1.0 ), L"Long time ago in a galaxy", "fonts/StarWars.ttf" );

    // FIXME: nrl - reimplement effects
    //node0->SetNodeEffect( model::ModelNodeEffectFactory::CreateModelNodeEffect( NodeEffectType::NET_BOUNDING_BOX, "bb", timeEvaluator ) );
    //node1->SetNodeEffect( model::ModelNodeEffectFactory::CreateModelNodeEffect( NodeEffectType::NET_BOUNDING_BOX, "bb", timeEvaluator ) );
    //node2->SetNodeEffect( model::ModelNodeEffectFactory::CreateModelNodeEffect( NodeEffectType::NET_BOUNDING_BOX, "bb", timeEvaluator ) );

    node0->AddChildToModelOnly( node1 );
    node0->AddChildToModelOnly( node2 );

    return node0;
}

model::BasicNodePtr TestScenesFactory::FontTestScene( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    pluginsManager;

    glm::vec3 nodeTranslation( 0.0, -2.0, -1.0 );
    glm::vec3 nextTranslation( 0.0, -0.1, 0.0 );
    glm::vec4 color( 1.0, 0.7, 0.0, 1.0 );

    auto node0 = SimpleNodesFactory::CreateScrollerTestNode( timeEvaluator );
    //auto node0 = SimpleNodesFactory::CreateTextCacheTest( timeEvaluator, "Dummy", nodeTranslation, color, L"", "fonts/StarWars.ttf" );

    //auto rootPath = ProjectManager::GetInstance()->GetRootDir();
    //std::string textPath = "other/FontTestText.txt";
    //rootPath = rootPath.Join( textPath );
    //
    //std::string nextLine;
    //std::string line = "line";
    //unsigned int numLines = 0;

    //std::ifstream file( rootPath.Str() );
    //if( !file.fail() )
    //{
    //    while( !file.eof() )
    //    {
    //        numLines++;
    //        nodeTranslation += nextTranslation;

    //        std::getline( file, nextLine );
    //        auto newNode = SimpleNodesFactory::CreateTextCacheTest( timeEvaluator, line + std::to_string( numLines ), nodeTranslation, color, std::wstring( nextLine.begin(), nextLine.end() ), "fonts/StarWars.ttf" );
    //        node0->AddChildToModelOnly( newNode );
    //    }
    //}

    return node0;
}

// ***********************
//
model::BasicNodePtr     TestScenesFactory::FadeRectTestScene               ( model::ITimeEvaluatorPtr timeEvaluator )
{
    // Root node
    auto root = model::BasicNode::Create( "rootNode", timeEvaluator );

    root->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
    root->AddPlugin( "DEFAULT_RECTANGLE", timeEvaluator );
    root->AddPlugin( "DEFAULT_COLOR", timeEvaluator );
    root->AddPlugin( "DEFAULT_FADE_PLUGIN", timeEvaluator );

    auto rectPlugin = root->GetPlugin( "rectangle" );
    model::SetParameter( rectPlugin->GetParameter( "width" ), 0.0, 3.0f );
    model::SetParameter( rectPlugin->GetParameter( "height" ), 0.0, 3.0f );
    
    //model::LoadTexture( root->GetPlugin( "texture" ), "textures/water.jpg" );
    
    //auto gradientPlugin = root->GetPlugin( "linear_gradient" );
    //model::SetParameter( gradientPlugin->GetParameter( "color1" ), 0.0, glm::vec4( 1.0, 0.0, 0.0, 1.0 ) );
    //model::SetParameter( gradientPlugin->GetParameter( "color2" ), 0.0, glm::vec4( 0.0, 0.0, 1.0, 1.0 ) );

    
    auto fadePlugin = root->GetPlugin( "fade" );
    model::SetParameter( fadePlugin->GetParameter( "FadeAlpha1" ), 0.0, 1.0f );
    model::SetParameter( fadePlugin->GetParameter( "FadeAlpha1" ), 3.0, 0.3f );
    model::SetParameter( fadePlugin->GetParameter( "FadeAlpha1" ), 4.0, 0.8f );

    model::SetParameter( fadePlugin->GetParameter( "FadeAlpha2" ), 0.0, 1.0f );
    model::SetParameter( fadePlugin->GetParameter( "FadeAlpha2" ), 4.0, 1.0f );
    model::SetParameter( fadePlugin->GetParameter( "FadeAlpha2" ), 6.0, 0.5f );

    model::SetParameter( fadePlugin->GetParameter( "FadeAlpha3" ), 0.0, 1.0f );
    model::SetParameter( fadePlugin->GetParameter( "FadeAlpha3" ), 3.0, 0.1f );

    model::SetParameter( fadePlugin->GetParameter( "FadeAlpha4" ), 0.0, 1.0f );

    return root;
}

// ***********************
//
model::BasicNodePtr     TestScenesFactory::ExtrudeTestScene                ( model::ITimeEvaluatorPtr timeEvaluator )
{
    // Root node
    auto root = model::BasicNode::Create( "rootNode", timeEvaluator );

    root->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
    root->AddPlugin( "DEFAULT_WALL", timeEvaluator );
    root->AddPlugin( "DEFAULT_EXTRUDE_PLUGIN", timeEvaluator );
    root->AddPlugin( "DEFAULT_MATERIAL", timeEvaluator );

    auto material = root->GetPlugin( "material" );
    model::SetParameter( material->GetParameter( "mtlDiffuse" ), 0.0, glm::vec4( 1, 1, 1, 1 ) );
    model::SetParameter( material->GetParameter( "mtlAmbient" ), 0.0, glm::vec4( 0, 0, 0, 0 ) );
    model::SetParameter( material->GetParameter( "mtlSpecular" ), 0.0, glm::vec4( 1.0, 0.0, 0.0, 1.0 ) );
    model::SetParameter( material->GetParameter( "mtlEmission" ), 0.0, glm::vec4( 0.1, 0.0, 0.2, 1.0 ) );
    model::SetParameter( material->GetParameter( "mtlShininess" ), 0.0, 128 );

    //auto rectPlugin = root->GetPlugin( "rectangle" );
    //model::SetParameter( rectPlugin->GetParameter( "width" ), 0.0, 3.0f );
    //model::SetParameter( rectPlugin->GetParameter( "height" ), 0.0, 2.0f );

    return root;
}

// ***********************
//
model::BasicNodePtr     TestScenesFactory::LineChartTestScene                ( model::ITimeEvaluatorPtr timeEvaluator )
{
    // Root node
    auto root = model::BasicNode::Create( "rootNode", timeEvaluator );

    root->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
    root->AddPlugin( "DEFAULT_LINE_CHART", timeEvaluator );
    root->AddPlugin( "DEFAULT_COLOR", timeEvaluator );

    return root;
}


// ***********************
//
model::BasicNodePtr     TestScenesFactory::SVGTestScene                    ( model::ITimeEvaluatorPtr timeEvaluator )
{
    // Root node
    auto root = model::BasicNode::Create( "rootNode", timeEvaluator );

    root->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
    root->AddPlugin( "DEFAULT_MESH", timeEvaluator );

    auto plugin = root->GetPlugin( "mesh" );
    assert( plugin );
    auto assetDesc = ProjectManager::GetInstance()->GetAssetDesc( "", "svgs", "kupa_twarz.svg" );
    assert( assetDesc );
    plugin->LoadResource( assetDesc );
    
    root->AddPlugin( "TRIANGULATE", timeEvaluator );

    auto success = root->AddPlugin( "DEFAULT_EXTRUDE_PLUGIN", timeEvaluator ); success;
    assert( success );
    auto extrude = root->GetPlugin( "extrude" );
    assert( extrude );
    model::SetParameter( extrude->GetParameter( model::DefaultExtrudePlugin::PARAMS::EXTRUDE_CURVE ), 0, model::DefaultExtrudePlugin::ExtrudeCurveType::Circle );    
    model::SetParameter( extrude->GetParameter( model::DefaultExtrudePlugin::PARAMS::SMOOTH_THRESHOLD_ANGLE ), 0, 160.0f );    
    model::SetParameter( extrude->GetParameter( model::DefaultExtrudePlugin::PARAMS::CURVE_SCALE ), 0, 0.01f );
    model::SetParameter( extrude->GetParameter( model::DefaultExtrudePlugin::PARAMS::CURVE_SCALE ), 3, 0.2f );
    
    static_cast< model::DefaultTimeline* >( timeEvaluator.get() )->SetWrapBehavior( TimelineWrapMethod::TWM_MIRROR, TimelineWrapMethod::TWM_MIRROR );
    static_cast< model::DefaultTimeline* >( timeEvaluator.get() )->SetDuration( 3.0f );

    //root->AddPlugin( "DEFAULT_COLOR", timeEvaluator );
    //success = model::SetParameter( root->GetPlugin( "solid color" )->GetParameter( "color" ), 0.f, glm::vec4( 1, 1, 1, 1 ) );
    //assert( success );

    root->AddPlugin( "DEFAULT_MATERIAL", timeEvaluator );

    auto material = root->GetPlugin( "material" );
    model::SetParameter( material->GetParameter( "mtlDiffuse" ), 0.0, glm::vec4( 1, 1, 1, 1 ) );
    model::SetParameter( material->GetParameter( "mtlAmbient" ), 0.0, glm::vec4( 0, 0, 0, 0 ) );
    model::SetParameter( material->GetParameter( "mtlSpecular" ), 0.0, glm::vec4( 1.0, 0.0, 0.0, 1.0 ) );
    model::SetParameter( material->GetParameter( "mtlEmission" ), 0.0, glm::vec4( 0.1, 0.0, 0.2, 1.0 ) );
    model::SetParameter( material->GetParameter( "mtlShininess" ), 0.0, 128 );

//    root->GetPlugin( "solid color" )->GetRendererContext()->fillCtx->fillMode = model::FillContext::Mode::M_LINES;

    return root;
}

// ***********************
//
model::BasicNodePtr     TestScenesFactory::Text3DTestScene                 ( model::ITimeEvaluatorPtr timeEvaluator )
{
    //Plugin stuff
    std::vector< std::string > uids;
    uids.push_back( "DEFAULT_TRANSFORM" );
    uids.push_back( "DEFAULT_TEXT3D" );
    uids.push_back( "DEFAULT_EXTRUDE_PLUGIN" );
    uids.push_back( "DEFAULT_MATERIAL" );


    auto node = model::BasicNode::Create( "Text3D", timeEvaluator );
    auto success = node->AddPlugins( uids, timeEvaluator );
    assert( success );


    auto material = node->GetPlugin( "material" );
    model::SetParameter( material->GetParameter( "mtlDiffuse" ), 0.0, glm::vec4( 1, 0.5, 1, 1 ) );
    model::SetParameter( material->GetParameter( "mtlAmbient" ), 0.0, glm::vec4( 0, 0, 0, 0 ) );
    model::SetParameter( material->GetParameter( "mtlSpecular" ), 0.0, glm::vec4( 1.0, 0.0, 0.0, 1.0 ) );
    model::SetParameter( material->GetParameter( "mtlEmission" ), 0.0, glm::vec4( 0.1, 0.0, 0.2, 1.0 ) );
    model::SetParameter( material->GetParameter( "mtlShininess" ), 0.0, 128 );



    //SetParameterScale ( node->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.001f, 0.001f, 0.001f ) );

    SetParameter( node->GetPlugin( "text3d" )->GetParameter( "spacing" ), TimeType( 0.0 ), 0.f );
    SetParameter( node->GetPlugin( "text3d" )->GetParameter( "fontSize" ), TimeType( 0.0 ), 200.0f );
    SetParameter( node->GetPlugin( "text3d" )->GetParameter( "text" ), 0.0, std::wstring( L"pi¿mowó³ z¿era\nz¿ó³k³¹\nœciêt¹ kiœæ!?!\n%%@#$^&*()" ) );
    //SetParameter( node->GetPlugin( "text" )->GetParameter( "alignment" ), TimeType( 0.0 ), float( TextAlignmentType::Center ) );

    SetParameter( node->GetPlugin( "extrude" )->GetParameter( "extrude vector" ), 0.0, glm::vec3( 0.0, 0.0, -0.3f ) );

    success = model::LoadFont( node->GetPlugin( "text3d" ), "fonts/proj00/arial.ttf", 30, 0, 0, 0, true );
    assert( success );

    return node;    
}

} //bv
