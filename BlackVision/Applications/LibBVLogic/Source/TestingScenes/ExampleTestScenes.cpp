#include "stdafxBVApp.h"

#include "ExampleTestScenes.h"

#include <vector>
#include <string>
#include <cstdarg>
#include <cstdlib>
#include <ctime>

#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"
#include "Engine/Models/Plugins/PluginUtils.h"

namespace bv {

namespace {

// *****************************
//
float simple_rnd( float a, float b )
{
    float t = float(double(std::rand()) / double(RAND_MAX));

    return a + ( b - a ) * t;
}

} // anonymous

// *****************************
//
model::BasicNodePtr CreateSingleTestNode( const std::string & nodeName, model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator, unsigned int numPlugins, ... )
{
    auto node = model::BasicNode::Create( nodeName, pluginsManager );

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
    auto node = model::BasicNode::Create( nodeName, pluginsManager );

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
model::BasicNodePtr SolidRect( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    auto rootNode =     CreateSingleTestNodeUidsOnly( "root", pluginsManager, timeEvaluator, 3,
                                                      "DEFAULT_TRANSFORM", 
                                                      "DEFAULT_RECTANGLE", 
                                                      "DEFAULT_COLOR" );
    assert( rootNode );

    auto color = rootNode->GetPlugin( "solid color" )->GetParameter( "color" );
    assert( color );

    auto success = SetParameter( color, 0.f, glm::vec4( 0.f, 1.f, 0.f, 1.f ) );
    assert( success );
	{ success; }

    auto simpleTransform = rootNode->GetPlugin( "transform" )->GetParameter( "simple_transform" );

    SetParameterRotation ( simpleTransform, 0.0f, glm::vec3( 0.f, 0.f, 0.f ));
    SetParameterRotation ( simpleTransform, 2.0f, glm::vec3( 0.f, 0.f, 20000.f ) );

    return rootNode;
}

// *****************************
//
model::BasicNodePtr TexturedTestRandomRect( const std::string & name, const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    auto node =     CreateSingleTestNodeUidsOnly( name, pluginsManager, timeEvaluator, 3, "DEFAULT_TRANSFORM",  "DEFAULT_RECTANGLE",  "DEFAULT_TEXTURE" );
    assert( node );

    auto transform  = node->GetPlugin( "transform" )->GetParameter( "simple_transform" );

    bool success = true;

    float xt = simple_rnd( -1.f, 1.f );
    float yt = simple_rnd( -1.f, 1.f );
    float zt = simple_rnd( -1.f, -5.f );

    success &= model::SetParameterTranslation( transform, 0.0f, glm::vec3( xt, yt, zt ) );

    std::string txFileName = simple_rnd( -1.f, 1.f ) > 0.f ? "rsrcy/simless_01.jpg" : "rsrcy/simless_00.jpg";

    success &= model::LoadTexture( node->GetPlugin( "texture" ), txFileName );

    assert( success );

    return node;
}

// *****************************
//
model::BasicNodePtr TwoTexturedRectangles       ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    auto rootNode =     CreateSingleTestNodeUidsOnly( "root", pluginsManager, timeEvaluator, 3,
                                                      "DEFAULT_TRANSFORM", 
                                                      "DEFAULT_RECTANGLE", 
                                                      "DEFAULT_TEXTURE" );

    auto childNode =    CreateSingleTestNodeUidsOnly( "child0", pluginsManager, timeEvaluator, 3,
                                                      "DEFAULT_TRANSFORM", 
                                                      "DEFAULT_RECTANGLE", 
                                                      "DEFAULT_TEXTURE" );

    assert( rootNode && childNode );

    auto rootTransform  = rootNode->GetPlugin( "transform" )->GetParameter( "simple_transform" );
    auto childTransform = childNode->GetPlugin( "transform" )->GetParameter( "simple_transform" );

    bool success = true;

    success &= model::SetParameterTranslation( rootTransform, 0.0f, glm::vec3( -1.f, 0.f, -1.f ) );
    success &= model::SetParameterRotation( rootTransform, 100.0f, glm::vec3( 0.f, 0.f, 5000.f ) );
    success &= model::SetParameterTranslation( childTransform, 0.0f, glm::vec3( 2.f, 0.f, 0.f ) );
    success &= model::SetParameterRotation( childTransform, 100.0f, glm::vec3( 0.f, 0.f, 36000.f ) );

    assert( success );

    std::string txFileName = "sand.jpg";

    success &= model::LoadTexture( rootNode->GetPlugin( "texture" ), txFileName );
    success &= model::LoadTexture( childNode->GetPlugin( "texture" ), txFileName );

    assert( success );

    rootNode->AddChildToModelOnly( childNode );

    return rootNode;
}

} //bv
