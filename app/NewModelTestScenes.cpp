#include "MockScenes.h"

#include "Engine/Models/Plugins/Simple/DefaultTransformPlugin.h"
#include "Engine/Models/Plugins/DefaultRectPlugin.h"
#include "Engine/Models/BasicNode.h"

#include "Engine/Models/Plugins/PluginsFactory.h"

namespace bv {

namespace {

// *****************************
//
model::BasicNode *  DefaultTransformPluginOnly  ()
{
    //NEW API
    auto firstPlugin    = model::DefaultTransformPluginDesc::CreatePlugin( nullptr, true );
    auto secondPlugin   = model::DefaultRectPluginDesc::CreatePlugin( firstPlugin, true );
   
    model::BasicNode * root = new model::BasicNode( "Root" );

    root->AddPlugin( firstPlugin );
    root->AddPlugin( secondPlugin ); 

    //OLD API - FIXME: implement shader channels in new model
    ///////////////////////////// Material plugin //////////////////////////// 

    Vec4Interpolator col; col.SetWrapPostMethod( bv::WrapMethod::pingPong );
    col.AddKey( 0.f, glm::vec4( 0.f, 1.f, 0.f, 1.f ) );
    col.AddKey( 10.f, glm::vec4( 0.9f, 0.9f, 0.98f, 1.f ) );

    auto colorPlugin = model::PluginsFactory::CreateSimpleColorPlugin( secondPlugin, model::ParametersFactory::CreateParameter( "color", col ) );

    root->AddPlugin( colorPlugin );

    auto pixelShaderPlugin = model::PluginsFactory::CreateSimplePixelShaderPlugin( colorPlugin,  "../dep/media/shaders/solid.frag", model::RendererContext::CreateDefault() );

    root->AddPlugin( pixelShaderPlugin );

    return root;
}

} // anonymous

// *****************************
//
model::BasicNode *     TestScenesFactory::NewModelTestScene    ()
{
    auto root =  DefaultTransformPluginOnly ();

    return root;
}

} //bv
