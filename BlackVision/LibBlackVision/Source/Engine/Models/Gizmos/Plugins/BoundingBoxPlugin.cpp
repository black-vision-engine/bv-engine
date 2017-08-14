#include "stdafx.h"
#include "BoundingBoxPlugin.h"



namespace bv {
namespace model {


const std::string        BoundingBoxPlugin::PARAMS::WIDTH = "width";



// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
BoundingBoxPluginDesc::BoundingBoxPluginDesc                          ()
    : DefaultGeometryPluginDescBase( UID(), "bounding box" )
{
}

// *******************************
//
IPluginPtr              BoundingBoxPluginDesc::CreatePlugin              ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< BoundingBoxPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   BoundingBoxPluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper h( timeEvaluator );
    h.SetOrCreateVacModel();

    h.SetOrCreatePSModel();

    return h.GetModel();
}

// *******************************
//
std::string             BoundingBoxPluginDesc::UID                       ()
{
    return "BOUNDING_BOX_PLUGIN";
}


// ========================================================================= //
// Generators
// ========================================================================= //


#include "Mathematics/Defines.h"
namespace BoundingBoxPluginGenerator
{

class MainGenerator : public IGeometryNormalsUVsGenerator
{
protected:
public:

    virtual void GenerateGeometryNormalsUVs( Float3AttributeChannelPtr verts, Float3AttributeChannelPtr normals, Float2AttributeChannelPtr uvs ) override
    {

    }

};
}



// ************************************************************************* PLUGIN *************************************************************************


// *************************************
// 
BoundingBoxPlugin::BoundingBoxPlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : DefaultGeometryPluginBase( name, uid, prev, model )
{
    m_pluginParamValModel->Update();
    InitGeometry();
}

// *************************************
// 
BoundingBoxPlugin::~BoundingBoxPlugin         ()
{}


std::vector<IGeometryGeneratorPtr>	BoundingBoxPlugin::GetGenerators()
{
    std::vector<IGeometryGeneratorPtr> gens;
    return gens;
}


// ***********************
//
bool                                BoundingBoxPlugin::NeedsTopologyUpdate()
{
    auto & states = m_pluginParamValModel->GetVertexAttributesChannelModel()->GetStates();
    for( auto iter = states.begin(); iter != states.end(); ++iter )
    {
        if( iter->second->StateChanged() )
        {
            return true;
        }
    }
    return false;
}


} // model
} // bv



