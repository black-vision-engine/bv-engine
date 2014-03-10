#include "NewModelTestNodes.h"

#include "Engine/Models/Plugins/Interfaces/IPluginListFinalized.h"
#include "Engine/Models/Plugins/Manager/PluginsManager.h"
#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"

#include "Engine/Models/Plugins/PluginsFactory.h"

#include "Engine/Models/BasicNode.h"


namespace bv {

// *****************************
//
model::BasicNode *  SimpleNodesFactory::CreateGreenRectNode()
{
    std::vector< std::string > uids;

    uids.push_back( "DEFAULT_TRANSFORM" );
    uids.push_back( "DEFAULT_RECTANGLE" );
    uids.push_back( "DEFAULT_COLOR" );

    model::BasicNode * root = new model::BasicNode( "Root" );

    bool success = root->AddPlugins( uids );

    return root;

    //model::BasicNode * root = new model::BasicNode( "GreenRectRoot" );

    /////////////////////////////// Geometry plugin //////////////////////////
    //model::GeometryRectPlugin * rectPlugin  = PluginsFactory::CreateGeometryRectPlugin( 1.f, 1.f );

    //root->AddPlugin( rectPlugin ); 

    /////////////////////////////// Transform plugin //////////////////////////// 
    //TransformF trans;

    //FloatInterpolator angle; angle.SetWrapPostMethod( bv::WrapMethod::pingPong );
    //angle.AddKey( 0.f, 0.f );
    //angle.AddKey( 10.f, 270.f );

    //trans.AddRotation( angle, InterpolatorsHelper::CreateConstValue( 0.f ), InterpolatorsHelper::CreateConstValue( 0.f ), InterpolatorsHelper::CreateConstValue( 1.f ) );
    //trans.AddScale( InterpolatorsHelper::CreateConstValue( 1.f ), InterpolatorsHelper::CreateConstValue( 1.f ), InterpolatorsHelper::CreateConstValue( 1.f ) );
    //trans.AddTranslation( InterpolatorsHelper::CreateConstValue( 0.f ), InterpolatorsHelper::CreateConstValue( 0.f ), InterpolatorsHelper::CreateConstValue( 0.f ) );

    //auto transformPlugin = PluginsFactory::CreateSimpleTransformPlugin( rectPlugin, model::ParametersFactory::CreateParameter( "transform", trans, nullptr, 0 ) );

    //root->AddPlugin( transformPlugin ); // Plugin with transformation

    /////////////////////////////// Material plugin //////////////////////////// 

    //Vec4Interpolator col; col.SetWrapPostMethod( bv::WrapMethod::pingPong );
    //col.AddKey( 0.f, glm::vec4( 0.f, 1.f, 0.f, 1.f ) );
    //col.AddKey( 10.f, glm::vec4( 0.9f, 0.9f, 0.98f, 1.f ) );

    //auto colorPlugin = PluginsFactory::CreateSimpleColorPlugin( transformPlugin, ParametersFactory::CreateParameter( "color", col ) );

    //root->AddPlugin( colorPlugin );

    //auto pixelShaderPlugin = PluginsFactory::CreateSimplePixelShaderPlugin( colorPlugin,  "../dep/media/shaders/solid.frag", RendererContext::CreateDefault() );

    //root->AddPlugin( pixelShaderPlugin );

    //return root;

    return nullptr;
}

} //bv
