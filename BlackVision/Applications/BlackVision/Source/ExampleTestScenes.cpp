#include "ExampleTestScenes.h"

#include <vector>
#include <string>
#include <cstdarg>

#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"
#include "Engine/Models/Plugins/PluginUtils.h"

namespace bv {

// *****************************
//
model::BasicNodePtr CreateSingleTestNode( const std::string & nodeName, model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator, unsigned int numPlugins, ... )
{
    auto node = model::BasicNode::Create( nodeName, timeEvaluator, pluginsManager );

    va_list args;
    va_start( args, numPlugins );

    for ( unsigned int i = 0; i < numPlugins; ++i )
    {
        std::string uid         = va_arg( args, const char * );
        std::string pluginName  = va_arg( args, const char * );        
    
        auto success = node->AddPlugin( uid, pluginName, timeEvaluator );

        if( !success )
        {
            return nullptr;
        }
    }

    va_end( args );

    return node;
}

// *****************************
//
model::BasicNodePtr CreateSingleTestNodeUidsOnly( const std::string & nodeName, const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator, unsigned int numPlugins, ... )
{
    auto node = model::BasicNode::Create( nodeName, timeEvaluator, pluginsManager );

    va_list args;
    va_start( args, numPlugins );

    for ( unsigned int i = 0; i < numPlugins; ++i )
    {
        std::string uid         = va_arg( args, const char * );
    
        auto success = node->AddPlugin( uid, timeEvaluator );

        if( !success )
        {
            return nullptr;
        }
    }

    va_end( args );

    return node;
}

// *****************************
//
model::BasicNodePtr TwoTexturedRectangles       ( const model::PluginsManager * pluginsManager, model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    { timelineManager; } // FIXME: suppress unused warning

    auto rootNode =     CreateSingleTestNodeUidsOnly( "root", pluginsManager, timeEvaluator, 3,
                                                      "DEFAULT_TRANSFORM", 
                                                      "DEFAULT_RECTANGLE", 
                                                      "DEFAULT_TEXTURE" );
    
    auto childNode =    CreateSingleTestNodeUidsOnly( "root", pluginsManager, timeEvaluator, 3,
                                                      "DEFAULT_TRANSFORM", 
                                                      "DEFAULT_RECTANGLE", 
                                                      "DEFAULT_TEXTURE" );

    assert( rootNode && childNode );

    auto rootTransform  = rootNode->GetPlugin( "transform" )->GetParameter( "simple_transform" );
    auto childTransform = childNode->GetPlugin( "transform" )->GetParameter( "simple_transform" );

    bool success = true;

    success &= model::SetParameterTranslation( rootTransform, 0, 0.0f, glm::vec3( -1.f, 0.f, -1.f ) );
    success &= model::SetParameterTranslation( childTransform, 0, 0.0f, glm::vec3( 2.f, 0.f, -1.f ) );

    assert( success );

    std::string txFileName = "rsrcy/simless_00.jpg";

    success &= model::LoadTexture( rootNode->GetPlugin( "texture" ), txFileName );
    success &= model::LoadTexture( childNode->GetPlugin( "texture" ), txFileName );

    assert( success );

    rootNode->AddChildToModelOnly( childNode );

    return rootNode;
}

} //bv
