#include "MockScenes.h"

#include "Engine/Models/Plugins/Interfaces/IPluginListFinalized.h"
#include "Engine/Models/Plugins/Manager/PluginsManager.h"
#include "Engine/Models/BasicNode.h"

#include "Engine/Models/Plugins/PluginsFactory.h"


namespace bv {

namespace {

// *****************************
//
model::BasicNode *  DefaultTesNewAPI   ( const model::PluginsManager * pluginsManager )
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

} // anonymous

// *****************************
//
model::BasicNode *     TestScenesFactory::NewModelTestScene    ( const model::PluginsManager * pluginsManager )
{
    model::BasicNode * root =  DefaultTestWithValidation ( pluginsManager );

    return root;
}

} //bv
