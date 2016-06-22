#include "stdafx.h"
#include "DefaultExtrudePlugin.h"

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"


namespace bv { namespace model {


const std::string        DefaultExtrudePlugin::PARAMS::ALPHA             = "alpha2";



// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultExtrudePluginDesc::DefaultExtrudePluginDesc                          ()
    : DefaultGeometryProcessorDescBase( UID(), "extrude" )
{}

// *******************************
//
IPluginPtr              DefaultExtrudePluginDesc::CreatePlugin              ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultExtrudePlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultExtrudePluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper helper( timeEvaluator );

    //Create all models
    auto model = helper.GetModel();
    helper.CreateVacModel();

    return model;
}

// *******************************
//
std::string             DefaultExtrudePluginDesc::UID                       ()
{
    return "DEFAULT_EXTRUDE_PLUGIN";
}


// ************************************************************************* PLUGIN *************************************************************************

// *************************************
// 
DefaultExtrudePlugin::DefaultExtrudePlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : DefaultGeometryProcessorBase( name, uid, prev, model )
{
    SetPrevPlugin( prev );
}

// *************************************
// 
DefaultExtrudePlugin::~DefaultExtrudePlugin         ()
{}

// ***********************
//
void        DefaultExtrudePlugin::ProcessConnectedComponent       ( model::ConnectedComponentPtr & currComponent,
                                                                    std::vector< IConnectedComponentPtr > & /*allComponents*/,
                                                                    PrimitiveType topology )
{
    auto positions = std::static_pointer_cast< Float3AttributeChannel >( currComponent->GetAttrChannel( AttributeSemantic::AS_POSITION ) );
    auto normals = std::static_pointer_cast< Float3AttributeChannel >( currComponent->GetAttrChannel( AttributeSemantic::AS_NORMAL ) );
    auto uvs = std::static_pointer_cast< Float2AttributeChannel >( currComponent->GetAttrChannel( AttributeSemantic::AS_TEXCOORD ) );
    
    if( topology == PrimitiveType::PT_TRIANGLE_STRIP )
    {
        auto connComp = ConnectedComponent::Create();
    }
    else
    {
        assert( !"This primitive topology is not supported yet" );
    }
}


} // model
} // bv

