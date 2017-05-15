#include "stdafx.h"

#include "DefaultRoundedRectPlugin.h"

#include "Mathematics/Defines.h"

#include "Memory/MemoryLeaks.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"


namespace bv { namespace model {






// ***********************
//
DefaultRoundedRectPluginDesc::DefaultRoundedRectPluginDesc() 
    : DefaultGeometryPluginDescBase( UID(), "rounded rect" ) { }


// ***********************
//
DefaultPluginParamValModelPtr   DefaultRoundedRectPluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper h( timeEvaluator );

    h.SetOrCreateVacModel();
    h.AddSimpleParam( DefaultRoundedRectPlugin::PARAMS::SIZE, glm::vec2( 1, 1 ), true, true );
    h.AddSimpleParam( DefaultRoundedRectPlugin::PARAMS::BEVELS, glm::vec4( 0.1, 0.1, 0.1, 0.1 ), true, true );
    h.AddSimpleParam( DefaultRoundedRectPlugin::PARAMS::TESSELATION, 10, true, true );
    h.AddSimpleParam( DefaultRoundedRectPlugin::PARAMS::STRETCH, 0.0f, true, true );
    h.AddSimpleParam( DefaultRoundedRectPlugin::PARAMS::USE_OUTLINE, false, true, true );
    h.AddSimpleParam( DefaultRoundedRectPlugin::PARAMS::OUTLINE_WIDTH, 0.1f, true, true );
    h.AddSimpleParam( DefaultRoundedRectPlugin::PARAMS::CROP, false, true, true );
    h.AddSimpleParam( DefaultRoundedRectPlugin::PARAMS::CROP_FACES, glm::vec4( -3.0, 3.0, -3.0, 3.0 ), true, true );

    h.SetOrCreatePSModel();

    return h.GetModel();
}


// ***********************
//
IPluginPtr                      DefaultRoundedRectPluginDesc::CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultRoundedRectPlugin >( name, prev, timeEvaluator );
}

// ***********************
//
std::string                     DefaultRoundedRectPluginDesc::UID                 ()
{
    return "DEFAULT_ROUNDEDRECT";
}



const std::string DefaultRoundedRectPlugin::PN_SIZE = "size";
const std::string DefaultRoundedRectPlugin::PN_BEVELS = "bevels";
const std::string DefaultRoundedRectPlugin::PN_TESSELATION = "tesselation";


const std::string       DefaultRoundedRectPlugin::PARAMS::SIZE          = "size";
const std::string       DefaultRoundedRectPlugin::PARAMS::BEVELS        = "bevels";
const std::string       DefaultRoundedRectPlugin::PARAMS::TESSELATION   = "tesselation";
const std::string       DefaultRoundedRectPlugin::PARAMS::STRETCH       = "stretch";
const std::string       DefaultRoundedRectPlugin::PARAMS::USE_OUTLINE   = "use outline";
const std::string       DefaultRoundedRectPlugin::PARAMS::OUTLINE_WIDTH = "outline width";
const std::string       DefaultRoundedRectPlugin::PARAMS::CROP          = "crop";
const std::string       DefaultRoundedRectPlugin::PARAMS::CROP_FACES    = "crop faces";


// ***********************
//
class RoundedRectGenerator : public IGeometryNormalsGenerator
{
private:

    glm::vec2       m_size;
    glm::vec4       m_bevels;
    int             m_tesselation;
    float           m_stretch;
    float           m_outlineWidth;
    bool            m_useOutline;
    bool            m_crop;
    glm::vec4       m_clippingPlanes;



public:

    RoundedRectGenerator( glm::vec2 s, glm::vec4 b, int tesselation )
        :   m_size( s )
        ,   m_bevels( b )
        ,   m_tesselation( tesselation )
    {}

private:

    float sizeX, sizeY;
    glm::vec3 centers[4];

    
public:
    void GenerateGeometryNormals( Float3AttributeChannelPtr verts, Float3AttributeChannelPtr normals )
    {
        sizeX = m_size[ 0 ] / 2;
        sizeY = m_size[ 1 ] / 2;

        centers[ 0 ] = glm::vec3( sizeX - m_bevels[ 0 ], sizeY - m_bevels[ 0 ], 0 );
        centers[ 1 ] = glm::vec3( -sizeX + m_bevels[ 1 ], sizeY - m_bevels[ 1 ], 0 );
        centers[ 2 ] = glm::vec3( -sizeX + m_bevels[ 2 ], -sizeY + m_bevels[ 2 ], 0 );
        centers[ 3 ] = glm::vec3( sizeX - m_bevels[ 3 ], -sizeY + m_bevels[ 3 ], 0 );

        int nPoints = GetNPoints(), i, j;
        for( i = 0, j = nPoints - 1; i < j; i++, j-- )
        {
            verts->AddAttribute( GetPoint( j ) );
            verts->AddAttribute( GetPoint( i ) );
        }
        if( i == j )
            assert( false );

        GeometryGeneratorHelper::GenerateNonWeightedNormalsFromTriangleStrips( verts, normals );
    }

private:

    int GetNPoints()
    {
        return 4 * m_tesselation;
    }

    glm::vec3 GetPoint( int i )
    {
        assert( i < 4 * m_tesselation );
        int nCenter = i / m_tesselation;
        glm::vec3 center = centers[ nCenter ];

        double angle = ( i - nCenter ) * 2 * PI / ( ( m_tesselation - 1 ) * 4 );

        return center + glm::vec3( cos( angle ), sin( angle ), 0 ) * m_bevels[ nCenter ];
    }


//    void            Generate

};


// ***********************
//
DefaultRoundedRectPlugin::DefaultRoundedRectPlugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model ) 
    : DefaultGeometryPluginBase( name, uid, prev, model )
{
	m_size = GetValueParamState< glm::vec2 >( GetPluginParamValModel()->GetVertexAttributesChannelModel().get(), PARAMS::SIZE );
	m_bevels = GetValueParamState< glm::vec4 >( GetPluginParamValModel()->GetVertexAttributesChannelModel().get(), PARAMS::BEVELS );
	m_tesselations = GetValueParamState< Int32 >( GetPluginParamValModel()->GetVertexAttributesChannelModel().get(), PARAMS::TESSELATION );

	InitGeometry();
}

// ***********************
//
std::vector< IGeometryGeneratorPtr >        DefaultRoundedRectPlugin::GetGenerators()
{
    return std::vector< IGeometryGeneratorPtr >( 1, std::make_shared< RoundedRectGenerator >( GetSize(), GetBevels(), GetTesselation() ) );
}

// ***********************
//
bool                                        DefaultRoundedRectPlugin::NeedsTopologyUpdate()
{
    if( m_pluginParamValModel->GetVertexAttributesChannelModel() )
    {
        auto & states = m_pluginParamValModel->GetVertexAttributesChannelModel()->GetStates();
        for( auto iter = states.begin(); iter != states.end(); ++iter )
        {
            if( iter->second->StateChanged() )
                return true;
        }
    }
    return false;
}

// ***********************
//
glm::vec2                                   DefaultRoundedRectPlugin::GetSize()
{
    auto param = GetParameter( PARAMS::SIZE );
    auto tParam = QueryTypedParam< ParamVec2Ptr >( param );
    return tParam->Evaluate();
}

// ***********************
//
glm::vec4                                   DefaultRoundedRectPlugin::GetBevels()
{
    auto param = GetParameter( PARAMS::BEVELS );
    auto tParam = QueryTypedParam< ParamVec4Ptr >( param );
    return tParam->Evaluate();
}

// ***********************
//
int                                         DefaultRoundedRectPlugin::GetTesselation()
{
    auto param = GetParameter( PARAMS::TESSELATION );
    auto tParam = QueryTypedParam< ParamIntPtr >( param );
    return tParam->Evaluate();
}

} }


