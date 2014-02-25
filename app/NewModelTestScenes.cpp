#include "MockScenes.h"

#include "Engine/Models/Plugins/Interfaces/IPluginListFinalized.h"
#include "Engine/Models/Plugins/Manager/PluginsManager.h"
#include "Engine/Models/BasicNode.h"

#include "Engine/Models/Plugins/PluginsFactory.h"


namespace bv {

namespace {

// *****************************
//
model::BasicNode *  DefaultTestNewAPI   ( const model::PluginsManager * pluginsManager )
{
    std::vector< std::string > uids;

    uids.push_back( "DEFAULT_TRANSFORM" );
    uids.push_back( "DEFAULT_RECTANGLE" );
    uids.push_back( "DEFAULT_COLOR" );

    model::IPluginListFinalizedPtr pluginsList( pluginsManager->CreatePlugins( uids ) );

    model::BasicNode * root = new model::BasicNode( "Root" );

    //FIXME: add list to the node
    return root;
}

// *****************************
//
model::BasicNode *  DefaultTestNodeNewNodeImpl  ( const model::PluginsManager * pluginsManager )
{
    model::BasicNode * root = new model::BasicNode( "Root", pluginsManager );

    bool success = true;

    success &= root->AddPlugin( "DEFAULT_TRANSFORM", "transform" );  //success &= root->AddPlugin( "DEFAULT_TRANSFORM" ); //uses the default plugin name
    success &= root->AddPlugin( "DEFAULT_RECTANGLE", "rectangle" );  //success &= root->AddPlugin( "DEFAULT_RECTANGLE" ); //uses the default plugin name
    success &= root->AddPlugin( "DEFAULT_COLOR", "solid color" );    //success &= root->AddPlugin( "DEFAULT_COLOR" );     //uses the default plugin name

    assert( success );

    return root;
}

// *****************************
//
model::BasicNode *  DefaultTestWithValidation   ( const model::PluginsManager * pluginsManager )
{
    using namespace model;

    //NEW API
    IPluginPtr firstPlugin  = IPluginPtr( pluginsManager->CreatePlugin( "DEFAULT_TRANSFORM", "transform0", nullptr ) );

    if( !pluginsManager->CanBeAttachedTo( "DEFAULT_RECTANGLE", firstPlugin.get() ) )
    {
        return nullptr;
    }

    IPluginPtr secondPlugin = IPluginPtr( pluginsManager->CreatePlugin( "DEFAULT_RECTANGLE", "rect0", firstPlugin.get() ) );

    if( !pluginsManager->CanBeAttachedTo( "DEFAULT_COLOR", secondPlugin.get() ) )
    {
        return nullptr;
    }

    IPluginPtr thirdPlugin  = IPluginPtr( pluginsManager->CreatePlugin( "DEFAULT_COLOR", "col0", secondPlugin.get() ) );

    BasicNode * root = new BasicNode( "Root" );

    root->AddPlugin( firstPlugin );
    root->AddPlugin( secondPlugin );
    root->AddPlugin( thirdPlugin );

    return root;
}

// *****************************
//
model::BasicNode *  DefaultTestNoValidation     ( const model::PluginsManager * pluginsManager )
{
    using namespace model;

    //NEW API
    auto firstPlugin  = pluginsManager->CreatePlugin( "DEFAULT_TRANSFORM", "transform0", nullptr );
    auto secondPlugin = pluginsManager->CreatePlugin( "DEFAULT_RECTANGLE", "rect0", firstPlugin );
    auto thirdPlugin  = pluginsManager->CreatePlugin( "DEFAULT_COLOR", "col0", secondPlugin );

    BasicNode * root = new BasicNode( "Root" );

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
void QueryParametersGeneric    ( const std::vector< model::IParameter * > & params )
{
    printf( "Parameters:\n");

    for( auto param : params )
    {
        printf( "%s : %s\n", param->GetName(), param->GetType() );
    }
}

// *****************************
//
void QueryValuesGeneric    ( const std::vector< const IValue * > & values )
{
    printf( "Parameters:\n");

    for( auto value : values )
    {
        printf( "%s : %s\n", value->GetName(), value->GetType() );
    }
}

// *****************************
//
void QueryModelPropertiesGeneric    ( model::IParamValModel * model, const std::string msg )
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
void QueryPluginPropertiesGeneric   ( const model::IPlugin * plugin )
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
void  QueryPluginsNodesGeneric      ( model::BasicNode * node )
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
void  QueryPropertiesDefaultScene   ( const model::PluginsManager * pluginsManager )
{
    model::BasicNode *  node = DefaultTestNodeNewNodeImpl( pluginsManager );

    //Teoretycznie plugin juz moglby miec API dostepowe do odpowiednich modeli dla kanalow, ale poniewaz caly model jeszcze moze sie zmieniac
    //wiec lepiej, zeby byl wyizolowany w oddzielnej klasce (IPluginParamValModel), a jak juz nie bedzie wiekszych zmian, to mozna bedzie dodac to API i wtedy wywolanie sie uprosci do
    //node->GetPlugin( "transform" )->GetTransformChannelModel()->GetParameter( "simple_transform" ) //prosciej sie na razie nie da, bo kanaly moga miec parametry o tych samych nazwach

    const model::IParameter * transform_p   = node->GetPlugin( "transform" )->GetPluginParamValModel()->GetTransformChannelModel()->GetParameter( "simple_transform" );
    const IValue * transform_v              = node->GetPlugin( "transform" )->GetPluginParamValModel()->GetTransformChannelModel()->GetValue( "simple_transform" );

    //Rectangle plugin nie ma valiusow, a tylko parametry
    const model::IParameter * width_p       = node->GetPlugin( "rectangle" )->GetPluginParamValModel()->GetTransformChannelModel()->GetParameter( "width" );
    const model::IParameter * height_p      = node->GetPlugin( "rectangle" )->GetPluginParamValModel()->GetTransformChannelModel()->GetParameter( "height" );

    const model::IParameter * color_p       = node->GetPlugin( "solid color" )->GetPluginParamValModel()->GetTransformChannelModel()->GetParameter( "color" );
    const IValue * color_v                  = node->GetPlugin( "solid color" )->GetPluginParamValModel()->GetTransformChannelModel()->GetValue( "color" );

    //Oczywiscie mozna pobierac wszystkie parametry/valiusy dla danego pluginu jedna metoda, ale jest tez to API powyzsze do dobierania sie do nich pojedynczo

    //FIXME: dodac generic setter API dla propertiesow (i moze tez dla valiusow)
    //cos w stylu bool SetProperty( IProperty *, TimeType, typed_value ); //bool, bo typy moga sie nie zgadzac i wtedy properties nie zostanie ustawiony
    //dla valiusow bedzie nieco latwiej bool SetValue( IValue *, typed_value ); //bool tak samo, jak wyzej - ale to API moze nie jest potrzebnem bo u nas valiusy sa ustawiane chyba tylko w evaluatorach, a w pozostalych
    //miejscach tylko oczytywane, wiec nie ma wielkiego problemu

    //FIXME: wszystkie FIXME z tego pliku dodac do pivotala (sety generyczne, api dstepowe do dzieci i layerow w nodzie)
}

// *****************************
//
void  QueryPropertiesDefaultSceneConvenienceAPI ( const model::PluginsManager * pluginsManager )
{
    model::BasicNode *  node = DefaultTestNodeNewNodeImpl( pluginsManager );

    model::IParameter * transform_p = node->GetPlugin( "transform" )->GetParameter( "simple_transform" );
    const IValue * transform_v      = node->GetPlugin( "transform" )->GetValue( "simple_transform" );

    //Rectangle plugin nie ma valiusow, a tylko parametry
    model::IParameter * width_p     = node->GetPlugin( "rectangle" )->GetParameter( "width" );
    model::IParameter * height_p    = node->GetPlugin( "rectangle" )->GetParameter( "height" );

    model::IParameter * color_p     = node->GetPlugin( "solid color" )->GetParameter( "color" );
    const IValue * color_v          = node->GetPlugin( "solid color" )->GetValue( "color" );

    //Oczywiscie mozna pobierac wszystkie parametry/valiusy dla danego pluginu jedna metoda, ale jest tez to API powyzsze do dobierania sie do nich pojedynczo

    //FIXME: dodac generic setter API dla propertiesow (i moze tez dla valiusow)
    //cos w stylu bool SetProperty( IProperty *, TimeType, typed_value ); //bool, bo typy moga sie nie zgadzac i wtedy properties nie zostanie ustawiony
    //dla valiusow bedzie nieco latwiej bool SetValue( IValue *, typed_value ); //bool tak samo, jak wyzej - ale to API moze nie jest potrzebnem bo u nas valiusy sa ustawiane chyba tylko w evaluatorach, a w pozostalych
    //miejscach tylko oczytywane, wiec nie ma wielkiego problemu

    //FIXME: wszystkie FIXME z tego pliku dodac do pivotala (sety generyczne, api dstepowe do dzieci i layerow w nodzie)
}

} // anonymous

// *****************************
//
model::BasicNode *     TestScenesFactory::NewModelTestScene     ( const model::PluginsManager * pluginsManager )
{
    model::BasicNode * root =  DefaultTestNodeNewNodeImpl ( pluginsManager );

    return root;
}

} //bv
