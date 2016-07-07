#include "stdafx.h"
#include "DefaultLineChartPlugin.h"



namespace bv { namespace model {


const std::string        DefaultLineChartPlugin::PARAMS::WIDTH             = "width";



// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultLineChartPluginDesc::DefaultLineChartPluginDesc                          ()
    : DefaultGeometryPluginDescBase( UID(), "line chart" )
{
}

// *******************************
//
IPluginPtr              DefaultLineChartPluginDesc::CreatePlugin              ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultLineChartPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultLineChartPluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper h( timeEvaluator );
    h.CreateVacModel();

    h.CreatePSModel();

    return h.GetModel();
}

// *******************************
//
std::string             DefaultLineChartPluginDesc::UID                       ()
{
    return "DEFAULT_LINE_CHART";
}


// ========================================================================= //
// Generators
// ========================================================================= //


#include "Mathematics/Defines.h"
namespace DefaultLineChartPluginGenerator
{

    class MainGenerator : public IGeometryNormalsUVsGenerator
    {
    protected:
    public:

        virtual void GenerateGeometryNormalsUVs( Float3AttributeChannelPtr verts, Float3AttributeChannelPtr normals, Float2AttributeChannelPtr uvs ) override
        {
            verts->AddAttribute( glm::vec3( -1.0, 0.0, 0.0 ) );
            verts->AddAttribute( glm::vec3( -0.9, 0.8, 0.0 ) );

            verts->AddAttribute( glm::vec3( -0.9, 0.8, 0.0 ) );
            verts->AddAttribute( glm::vec3( -0.2, 0.5, 0.0 ) );

            verts->AddAttribute( glm::vec3( -0.2, 0.5, 0.0 ) );
            verts->AddAttribute( glm::vec3( 0.2, 0.0, 0.0 ) );

            verts->AddAttribute( glm::vec3( 0.2, 0.0, 0.0 ) );
            verts->AddAttribute( glm::vec3( 0.5, 1.0, 0.0 ) );

            verts->AddAttribute( glm::vec3( 0.5, 1.0, 0.0 ) );
            verts->AddAttribute( glm::vec3( 1.0, 0.1, 0.0 ) );

            for( unsigned int i = 0; i < verts->GetNumEntries(); ++i )
            {
                normals->AddAttribute( glm::vec3( 0.0, 0.0, 0.0 ) );
                uvs->AddAttribute( glm::vec2( 0.0, 0.0 ) );
            }
        }

    };
}


// ************************************************************************* PLUGIN *************************************************************************


// *************************************
// 
DefaultLineChartPlugin::DefaultLineChartPlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
: DefaultGeometryPluginBase( name, uid, prev, model )
{
    m_pluginParamValModel->Update();
    InitGeometry( PrimitiveType::PT_LINES );
}

// *************************************
// 
DefaultLineChartPlugin::~DefaultLineChartPlugin         ()
{}


std::vector<IGeometryGeneratorPtr>	DefaultLineChartPlugin::GetGenerators()
{
    std::vector<IGeometryGeneratorPtr> gens;
    
    gens.push_back( std::make_shared< DefaultLineChartPluginGenerator::MainGenerator >() );
    return gens;
}


// ***********************
//
bool                                DefaultLineChartPlugin::NeedsTopologyUpdate()
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

