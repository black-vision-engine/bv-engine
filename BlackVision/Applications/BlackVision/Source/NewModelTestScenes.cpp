#include "MockScenes.h"

#include "Engine/Models/Plugins/Interfaces/IPluginListFinalized.h"
#include "Engine/Models/Plugins/Manager/PluginsManager.h"
#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"

#include "Engine/Models/Plugins/PluginsFactory.h"

#include "Engine/Models/BasicNode.h"

#include "NewModelTestNodes.h"

#include "PieChartNode.h"
#include "Engine/Models/Plugins/Simple/DefaultTexturePlugin.h"
#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTexturesData.h"
#include "VideoInput/DefaultVideoInputResourceDescr.h"
#include "VideoInput/ExampleVideoInput.h"

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

    auto root = std::make_shared< model::BasicNode >( "Root", timeEvaluator );

    //FIXME: add list to the node
    return root;
}

// *****************************
//
model::BasicNodePtr  DefaultTestNodeNewNodeImpl  ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    auto root = std::make_shared< model::BasicNode >( "Root", timeEvaluator, pluginsManager );

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

    if( !pluginsManager->CanBeAttachedTo( "DEFAULT_RECTANGLE", firstPlugin ) )
    {
        return nullptr;
    }

    IPluginPtr secondPlugin = IPluginPtr( pluginsManager->CreatePlugin( "DEFAULT_RECTANGLE", "rect0", firstPlugin, timeEvaluator ) );

    if( !pluginsManager->CanBeAttachedTo( "DEFAULT_COLOR", secondPlugin ) )
    {
        return nullptr;
    }

    IPluginPtr thirdPlugin  = IPluginPtr( pluginsManager->CreatePlugin( "DEFAULT_COLOR", "col0", secondPlugin, timeEvaluator ) );

    auto root = std::make_shared< model::BasicNode >( "Root", timeEvaluator );

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

    auto root = std::make_shared< model::BasicNode >( "Root", timeEvaluator );

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
        printf( "%s : %s\n", param->GetName(), param->GetType() );
    }
}

// *****************************
//
void QueryValuesGeneric    ( const std::vector< IValueConstPtr > & values )
{
    printf( "Parameters:\n");

    for( auto value : values )
    {
        printf( "%s : %s\n", value->GetName(), value->GetType() );
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
    printf ( "Plugin: %s %s", plugin->GetTypeUid(), plugin->GetName() );

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
    printf( "Inspecting node: %s\n", node->GetName() );

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

    success &= SetParameterRotation( transform_p, 0, 0.0f, glm::vec3( 0.f, 0.f, 1.f ), 90.f );
    success &= SetParameterScale( transform_p, 0, 0.0f, glm::vec3( 1.f, 2.f, 1.f ) );
    success &= SetParameterTranslation( transform_p, 0, 0.0f, glm::vec3( 0.f, 1.f, -2.f ) );

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

    SetParameterRotation    ( n->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, 0.f, 1.f ), 90.f );
    SetParameterScale       ( n->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 1.f, 2.f, 1.f ) );
    SetParameterTranslation ( n->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, 1.f, -2.f ) );

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
void TestQueryNode(model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator)
{
    //GetNode tester - do not use:
    auto rt = SimpleNodesFactory::CreateGreenRectNode( timelineManager, timeEvaluator, "." );
    auto n0 = SimpleNodesFactory::CreateGreenRectNode( timelineManager, timeEvaluator, "node0" );
    auto n1 = SimpleNodesFactory::CreateGreenRectNode( timelineManager, timeEvaluator, "node01" );
    auto n2 = SimpleNodesFactory::CreateGreenRectNode( timelineManager, timeEvaluator, "node02" );
    auto n3 = SimpleNodesFactory::CreateGreenRectNode( timelineManager, timeEvaluator, "node011" );
    auto n4 = SimpleNodesFactory::CreateGreenRectNode( timelineManager, timeEvaluator, "node012" );
    auto n5 = SimpleNodesFactory::CreateGreenRectNode( timelineManager, timeEvaluator, "node013" );
    auto n6 = SimpleNodesFactory::CreateGreenRectNode( timelineManager, timeEvaluator, "node021" );
    auto n7 = SimpleNodesFactory::CreateGreenRectNode( timelineManager, timeEvaluator, "node022" );
    auto n8 = SimpleNodesFactory::CreateGreenRectNode( timelineManager, timeEvaluator, "node023" );
    auto n9 = SimpleNodesFactory::CreateGreenRectNode( timelineManager, timeEvaluator, "node024" );

    rt->AddChild( n0 );

    n0->AddChild( n1 );
    n0->AddChild( n2 );

    n1->AddChild( n3 );
    n1->AddChild( n4 );
    n1->AddChild( n5 );

    n2->AddChild( n6 );
    n2->AddChild( n7 );
    n2->AddChild( n8 );
    n2->AddChild( n9 );

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
model::BasicNodePtr     TestScenesFactory::NewModelTestScene     ( const model::PluginsManager * pluginsManager, model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    { timelineManager; }
    { pluginsManager; } // FIXME: suppress unuse warning
    //return SimpleNodesFactory::CreateHeightMapNode( timelineManager, timeEvaluator );

    // auto node0 = SimpleNodesFactory::CreateTexturedRectNode( timelineManager, timeEvaluator, false );
    //auto node1 = SimpleNodesFactory::CreateTexturedRectNode( timelineManager, timeEvaluator, false );

    //node0->AddChild( node1 );

    //return node0;

    //return SimpleNodesFactory::CreateOverrideNodeMaskTest1( timelineManager, timeEvaluator );
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
    return SimpleNodesFactory::CreateCrawlerNode( timelineManager, timeEvaluator );
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
    return NULL;
}

// *****************************
//
model::BasicNodePtr     TestScenesFactory::OlafTestScene     ( const model::PluginsManager * pluginsManager, model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    { pluginsManager; } // FIXME: suppress unuse warning
    auto rect = SimpleNodesFactory::CreateOlafRectNode( timelineManager, timeEvaluator );
    auto clock = SimpleNodesFactory::CreateTimerNode( timelineManager, timeEvaluator, 0, false );
    rect->AddChild( clock );

    return rect;
}

model::BasicNodePtr    TestScenesFactory::CreedVideoInputTestScene   ( const model::PluginsManager * pluginsManager, model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    {timelineManager;}
    {pluginsManager;}

    model::BasicNodePtr root = std::make_shared< model::BasicNode >( "rootNode", timeEvaluator );
    root->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
    root->AddPlugin( "DEFAULT_RECTANGLE", timeEvaluator );

    //auto texturePlugin =  std::dynamic_pointer_cast< model::DefaultTexturePlugin >( root->GetPlugin( "texture" ) );
    //((model::DefaultTexturesData*)(texturePlugin->GetPixelShaderChannel()->GetTexturesData().get()))->AddTexture( new model::DefaultVideoInput() );

    root->AddPlugin( "DEFAULT_VIDEOINPUT", timeEvaluator );
    auto plugin = root->GetPlugin( "video input" );

    //root->AddPlugin( "DEFAULT_TEXTURE", timeEvaluator );
    //auto plugin = root->GetPlugin( "texture" );

    //auto vi = new model::DefaultVideoInput( 10, 10, 1.f );
    auto vi = new ExampleVideoInput( 10, 10, 1.f );
    auto success = plugin->LoadResource( model::IPluginResourceDescrConstPtr( new model::DefaultVideoInputResourceDescr( vi->GetTexture(), vi ) ) );
    assert(success);

    return root;
}

model::BasicNodePtr    TestScenesFactory::CreedPieChartTestScene     ( const model::PluginsManager * pluginsManager, model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    { pluginsManager; } // FIXME: suppress unuse warning
    std::vector< PieChartSubnodeDesc > descs;

    //descs.push_back( PieChartSubnodeDesc( 10.f, 0.f ) );
    descs.push_back( PieChartSubnodeDesc( 20.f, -.1f, PieChartSubnodeDesc::COLORED ) );
    //descs.push_back( PieChartSubnodeDesc( 50.f, .1f ) );

    auto node = new PieChartNode( timelineManager, timeEvaluator, descs );

    return model::BasicNodePtr( node );
}

model::BasicNodePtr    TestScenesFactory::CreedDeprecatedTestScene     ( const model::PluginsManager * pluginsManager, model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    { pluginsManager; } // FIXME: suppress unuse warning
    float percents[] = { 10.f, 20.f, 50.f };
    float offsets[] = { 0.f, -.1f, .1f };

    const int noPieces = 3;

    auto node = new PieChartNode( timelineManager, timeEvaluator, std::vector<float>( percents, percents+noPieces ), std::vector<float> (offsets, offsets+noPieces ) );

    return model::BasicNodePtr( node );
}

model::BasicNodePtr    TestScenesFactory::CreedPrimitivePieChartTestScene     ( const model::PluginsManager * pluginsManager, model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    { pluginsManager; } // FIXME: suppress unuse warning
    model::BasicNodePtr root = std::make_shared< model::BasicNode >( "rootNode", timeEvaluator );
    root->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );

    auto simpleTransform = root->GetPlugin( "transform" )->GetParameter( "simple_transform" );
    SetParameterScale( simpleTransform, 0, 0.0f, glm::vec3( 1.f, .2f, 1.f ) );
    SetParameterTranslation( simpleTransform, 0, 0.0f, glm::vec3( 0, 0, -1.f) );
    SetParameterRotation ( simpleTransform, 0, 0.0f, glm::vec3( 1.f, 0.f, 0.f ), 20.f );


    auto node1 = SimpleNodesFactory::CreateCreedColoredPieChartNode( timelineManager, timeEvaluator, 0 );
    auto node2 = SimpleNodesFactory::CreateCreedGradedPieChartNode( timelineManager, timeEvaluator, 0 );

    SetParameter( node1->GetPlugin( "piechart" )->GetParameter( "angleStart" ), 0, 0.f );
    SetParameter( node1->GetPlugin( "piechart" )->GetParameter( "angleEnd" ), 0, 3.14f/4 );

    root->AddChild( node1 );
    root->AddChild( node2 );

    return root;
}

model::BasicNodePtr    TestScenesFactory::CreedPrismTestScene     ( const model::PluginsManager * pluginsManager, model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator )
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

    model::BasicNodePtr root = std::make_shared< model::BasicNode >( "rootNode", timeEvaluator );
    root->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );

//SetParameterRotation( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 50.f, glm::vec3( 1, 0, 0 ), 1000.f );

    //auto root = SimpleNodesFactory::CreateCreedRectNode( timelineManager, timeEvaluator );

    auto prism = SimpleNodesFactory::CreateCreedColoredPrismNode( timelineManager, timeEvaluator, -1.5f );
    auto success = SetParameter( prism->GetPlugin( "prism" )->GetParameter( "n" ), 0.f, 4 );
    assert( success );
    SetParameter( prism->GetPlugin( "prism" )->GetParameter( "n" ), 10.f, 10 );
    //prism->GetPlugin( "prism" )->GetParameter( "n" )->SetInterpolationMethod( model::IParameter::InterpolationMethod::COSINE );

    SetParameterScale( prism->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0,  5.f, glm::vec3( 0.25f, 1.0f, 0.25f ) );
    SetParameterScale( prism->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 10.f, glm::vec3( 0.25f,  .0f, 0.25f ) );
    SetParameterScale( prism->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 15.f, glm::vec3( 0.25f,  .0f, 0.25f ) );
    SetParameterScale( prism->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 20.f, glm::vec3( 0.25f, 1.0f, 0.25f ) );

    auto prism2 = SimpleNodesFactory::CreateCreedTexturedPrismNode( timelineManager, timeEvaluator,-0.5f );

    SetParameterScale( prism2->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0,  3.f, glm::vec3( 0.25f, 1.0f, 0.25f ) );
    SetParameterScale( prism2->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0,  7.f, glm::vec3( 0.25f,  .0f, 0.25f ) );
    SetParameterScale( prism2->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 13.f, glm::vec3( 0.25f,  .0f, 0.25f ) );
    SetParameterScale( prism2->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 17.f, glm::vec3( 0.25f, 1.0f, 0.25f ) );
    SetParameterScale( prism2->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 20.f, glm::vec3( 0.25f, 1.0f, 0.25f ) );

    auto param = prism2->GetPlugin( "transform" )->GetParameter( "simple_transform" );
    param->SetInterpolationMethod( model::IParameter::InterpolationMethod::COSINE );

//SetParameterRotation( prism2->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 50.f, glm::vec3( 1, 0, 0 ), -10000.f );

    auto prism3 = SimpleNodesFactory::CreateCreedGradedPrismNode( timelineManager, timeEvaluator, 0.5f );
    SetParameter( prism3->GetPlugin( "prism" )->GetParameter( "n" ), 0.f, 12 );
    
    SetParameterScale( prism3->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0,  7.f, glm::vec3( 0.25f, 1.0f, 0.25f ) );
    SetParameterScale( prism3->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 13.f, glm::vec3( 0.25f,  .0f, 0.25f ) );
    SetParameterScale( prism3->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 17.f, glm::vec3( 0.25f,  .0f, 0.25f ) );
    SetParameterScale( prism3->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 20.f, glm::vec3( 0.25f, 1.0f, 0.25f ) );

//SetParameterRotation( prism3->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 50.f, glm::vec3( 1, 0, 0 ), 10000.f );

    auto prism4 = SimpleNodesFactory::CreateCreedGradedPrismNode( timelineManager, timeEvaluator, 1.5f );
    SetParameter( prism4->GetPlugin( "prism" )->GetParameter( "n" ), 0.f, 36 );
    SetParameter( prism4->GetPlugin( "linear_gradient" )->GetParameter( "color1" ), 5.f, glm::vec4( 1.f, 1.f, 1.f, 1.f ) );
    SetParameter( prism4->GetPlugin( "linear_gradient" )->GetParameter( "color2" ), 5.f, glm::vec4( 0.f, 0.f, 0.f, 1.f ) );
    //prism4->GetPlugin( "linear_gradient" )->GetParameter( "color2" )->SetInterpolationMethod( model::IParameter::InterpolationMethod::COSINE );
    
    SetParameterScale( prism4->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0,  7.f, glm::vec3( 0.25f,  .0f, 0.25f ) );
    SetParameterScale( prism4->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0,  8.f, glm::vec3( 0.25f,  .0f, 0.25f ) );
    SetParameterScale( prism4->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 11.f, glm::vec3( 0.25f, 1.0f, 0.25f ) );
    SetParameterScale( prism4->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 20.f, glm::vec3( 0.25f, 1.0f, 0.25f ) );

    root->AddChild( prism );
    root->AddChild( prism2 );
    root->AddChild( prism3 );
    root->AddChild( prism4 );

    //return rect;
    return root;
}

model::BasicNodePtr    TestScenesFactory::CreedPrismBugTestScene     ( const model::PluginsManager * pluginsManager, model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    {pluginsManager;}

    model::BasicNodePtr root = std::make_shared< model::BasicNode >( "rootNode", timeEvaluator );
    root->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );

    auto prism2 = SimpleNodesFactory::CreateCreedTexturedPrismNode( timelineManager, timeEvaluator, 0 );
    //auto prism2 = SimpleNodesFactory::CreateCreedColoredPrismNode( timelineManager, timeEvaluator, 0 );
    //auto prism2 = SimpleNodesFactory::CreateCreedGradedPrismNode( timelineManager, timeEvaluator, 0 );

    SetParameterScale( prism2->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0,  0.f, glm::vec3( 1.f, 1.f, 1.f ) );
    SetParameterTranslation( prism2->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0,  0.f, glm::vec3( 0.f, -.5f, -10.f ) );
    SetParameterRotation ( prism2->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 100.0f, glm::vec3( 1.f, 0.f, 0.f ), 10100.f );

    model::SetParameter( prism2->GetPlugin( "prism" )->GetParameter( "n" ), 10.f, 10.f );

    model::SetParameter( prism2->GetPlugin( "texture" )->GetParameter( "borderColor") , 0.f, glm::vec4( 1, 0, 0, 1 ) );

    //SetParameterScale( prism2->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0,  3.f, glm::vec3( 0.25f, 1.0f, 0.25f ) );
    //SetParameterScale( prism2->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0,  7.f, glm::vec3( 0.25f,  .0f, 0.25f ) );
    //SetParameterScale( prism2->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 13.f, glm::vec3( 0.25f,  .0f, 0.25f ) );
    //SetParameterScale( prism2->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 17.f, glm::vec3( 0.25f, 1.0f, 0.25f ) );
    //SetParameterScale( prism2->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 20.f, glm::vec3( 0.25f, 1.0f, 0.25f ) );

    root->AddChild( prism2 );

    //return root;
    return prism2;
}

} //bv
