#include "stdafx.h"
#include "BoundingBoxPlugin.h"



namespace bv {
namespace model {


const std::string        BoundingBoxPlugin::PARAMS::SIZE    = "size";



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
    h.AddSimpleParam( BoundingBoxPlugin::PARAMS::SIZE, glm::vec3( 1.0, 1.0, 1.0 ), true, true );

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

    glm::vec3       m_size;

public:

    explicit            MainGenerator( const glm::vec3 & size )
        : m_size( size )
    {}


    virtual void GenerateGeometryNormalsUVs( Float3AttributeChannelPtr verts, Float3AttributeChannelPtr normals, Float2AttributeChannelPtr uvs ) override
    {
        const int layerSize = 4;
        glm::vec3 corners[ 2 * layerSize ];
        glm::vec3 halfSize = m_size / 2.0f;

        corners[ 0 ] = glm::vec3( -halfSize.x, halfSize.y, halfSize.z );
        corners[ 1 ] = glm::vec3( halfSize.x, halfSize.y, halfSize.z );
        corners[ 2 ] = glm::vec3( halfSize.x, -halfSize.y, halfSize.z );
        corners[ 3 ] = glm::vec3( -halfSize.x, -halfSize.y, halfSize.z );

        corners[ 4 ] = glm::vec3( -halfSize.x, halfSize.y, -halfSize.z );
        corners[ 5 ] = glm::vec3( halfSize.x, halfSize.y, -halfSize.z );
        corners[ 6 ] = glm::vec3( halfSize.x, -halfSize.y, -halfSize.z );
        corners[ 7 ] = glm::vec3( -halfSize.x, -halfSize.y, -halfSize.z );

        // Layers (depth) iteration.
        for( int j = 0; j < 2; j++ )
        {
            for( int i = 0; i < layerSize; i++ )
            {
                verts->AddAttribute( corners[ j * layerSize + i ] );
                verts->AddAttribute( corners[ j * layerSize + ( i + 1 ) % layerSize ] );
            }
        }

        // Connect layers
        for( int i = 0; i < 4; ++i )
        {
            verts->AddAttribute( corners[ i ] );
            verts->AddAttribute( corners[ layerSize + i ] );
        }

        for( int i = 0; i < layerSize * 6; ++i )
            normals->AddAttribute( glm::vec3( 0.0, 0.0, 1.0 ) );

        for( int i = 0; i < layerSize * 6; ++i )
            uvs->AddAttribute( glm::vec2( 0.0, 0.0 ) );
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
    InitGeometry( PrimitiveType::PT_LINES );
}

// *************************************
// 
BoundingBoxPlugin::~BoundingBoxPlugin         ()
{}


std::vector<IGeometryGeneratorPtr>	BoundingBoxPlugin::GetGenerators()
{
    std::vector<IGeometryGeneratorPtr> gens;

    auto size = QueryTypedValue< ValueVec3Ptr >( GetValue( PARAMS::SIZE ) );
    gens.push_back( std::make_shared< BoundingBoxPluginGenerator::MainGenerator >( size->GetValue() ) );

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



