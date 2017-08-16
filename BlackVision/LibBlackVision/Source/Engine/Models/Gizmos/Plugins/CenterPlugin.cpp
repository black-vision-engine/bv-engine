#include "stdafx.h"
#include "CenterPlugin.h"




namespace bv {
namespace model {


const std::string        CenterPlugin::PARAMS::SIZE = "size";



// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
CenterPluginDesc::CenterPluginDesc                          ()
    : DefaultGeometryPluginDescBase( UID(), "center" )
{
}

// *******************************
//
IPluginPtr              CenterPluginDesc::CreatePlugin              ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< CenterPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   CenterPluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper h( timeEvaluator );

    h.SetOrCreateVacModel();
    h.AddSimpleParam( CenterPlugin::PARAMS::SIZE, 1.0f, true, true );

    h.SetOrCreatePSModel();

    return h.GetModel();
}

// *******************************
//
std::string             CenterPluginDesc::UID                       ()
{
    return "CENTER_PLUGIN";
}


// ========================================================================= //
// Generators
// ========================================================================= //


#include "Mathematics/Defines.h"
namespace CenterPluginGenerator
{

class MainGenerator : public IGeometryNormalsUVsGenerator
{
protected:

    float       m_size;

public:

    MainGenerator( float size )
        : m_size( size / 2.0f )
    {}


    virtual void GenerateGeometryNormalsUVs( Float3AttributeChannelPtr verts, Float3AttributeChannelPtr normals, Float2AttributeChannelPtr uvs ) override
    {
        verts->AddAttribute( glm::vec3( -m_size, 0.0, 0.0 ) );
        verts->AddAttribute( glm::vec3( m_size, 0.0, 0.0 ) );

        verts->AddAttribute( glm::vec3( 0.0, -m_size, 0.0 ) );
        verts->AddAttribute( glm::vec3( 0.0, m_size, 0.0 ) );

        verts->AddAttribute( glm::vec3( 0.0, 0.0, -m_size ) );
        verts->AddAttribute( glm::vec3( 0.0, 0.0, m_size ) );

        normals->AddAttribute( glm::vec3( 0.0, 0.0, 1.0 ) );
        normals->AddAttribute( glm::vec3( 0.0, 0.0, 1.0 ) );

        normals->AddAttribute( glm::vec3( 0.0, 0.0, 1.0 ) );
        normals->AddAttribute( glm::vec3( 0.0, 0.0, 1.0 ) );

        normals->AddAttribute( glm::vec3( 0.0, 0.0, 1.0 ) );
        normals->AddAttribute( glm::vec3( 0.0, 0.0, 1.0 ) );

        uvs->AddAttribute( glm::vec2( 0.0, 0.0 ) );
        uvs->AddAttribute( glm::vec2( 0.0, 0.0 ) );

        uvs->AddAttribute( glm::vec2( 0.0, 0.0 ) );
        uvs->AddAttribute( glm::vec2( 0.0, 0.0 ) );

        uvs->AddAttribute( glm::vec2( 0.0, 0.0 ) );
        uvs->AddAttribute( glm::vec2( 0.0, 0.0 ) );
    }

};
}



// ************************************************************************* PLUGIN *************************************************************************


// *************************************
// 
CenterPlugin::CenterPlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : DefaultGeometryPluginBase( name, uid, prev, model )
{
    m_pluginParamValModel->Update();
    InitGeometry( PrimitiveType::PT_LINES );
}

// *************************************
// 
CenterPlugin::~CenterPlugin         ()
{}


std::vector<IGeometryGeneratorPtr>	CenterPlugin::GetGenerators()
{
    std::vector<IGeometryGeneratorPtr> gens;

    auto size = QueryTypedValue< ValueFloatPtr >( GetValue( PARAMS::SIZE ) );
    gens.push_back( std::make_shared< CenterPluginGenerator::MainGenerator >( size->GetValue() ) );

    return gens;
}


// ***********************
//
bool                                CenterPlugin::NeedsTopologyUpdate()
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



