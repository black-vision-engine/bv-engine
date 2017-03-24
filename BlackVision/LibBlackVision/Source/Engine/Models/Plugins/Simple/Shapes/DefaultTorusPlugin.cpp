#include "stdafx.h"

#include "DefaultTorusPlugin.h"

#include "Memory/MemoryLeaks.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"


namespace bv { namespace model {
	
typedef ParamEnum< DefaultTorus::Plugin::OpenAngleMode > ParamEnumOAM;
typedef ParamEnum< DefaultTorus::Plugin::WeightCenter > ParamEnumWC;
typedef ParamEnum< DefaultTorus::Plugin::MappingType > ParamEnumMT;

VoidPtr    ParamEnumOAM::QueryParamTyped  ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}

template<>
static IParameterPtr        ParametersFactory::CreateTypedParameter< DefaultTorus::Plugin::OpenAngleMode >                 ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return CreateParameterEnum< DefaultTorus::Plugin::OpenAngleMode >( name, timeline );
}

VoidPtr    ParamEnumWC::QueryParamTyped  ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}

template<>
static IParameterPtr        ParametersFactory::CreateTypedParameter< DefaultTorus::Plugin::WeightCenter >                 ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return CreateParameterEnum< DefaultTorus::Plugin::WeightCenter >( name, timeline );
}

VoidPtr    ParamEnumMT::QueryParamTyped  ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}

template<>
static IParameterPtr        ParametersFactory::CreateTypedParameter< DefaultTorus::Plugin::MappingType >                 ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return CreateParameterEnum< DefaultTorus::Plugin::MappingType >( name, timeline );
}

	
namespace DefaultTorus {

const std::string PN::OPENANGLE = "open angle";
const std::string PN::OPENANGLEMODE = "open angle mode";
const std::string PN::RADIUS = "radius";
const std::string PN::RADIUSCROSSSECTION = "radius2";
const std::string PN::TESSELATION = "tesselation";
const std::string PN::WEIGHTCENTERX = "weight center x";
const std::string PN::WEIGHTCENTERY = "weight center y";
const std::string PN::WEIGHTCENTERZ = "weight center z";
const std::string PN::MAPPINGTYPE = "mapping type";

PluginDesc::PluginDesc()
    : DefaultGeometryPluginDescBase( UID(), "torus" )
{
}

DefaultPluginParamValModelPtr   PluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper h( timeEvaluator );

    h.SetOrCreateVacModel();
    h.AddSimpleParam( PN::TESSELATION, 10, true, true );
    h.AddSimpleParam( PN::RADIUS, 1.f, true, true );
    h.AddSimpleParam( PN::RADIUSCROSSSECTION, 0.1f, true, true );
    h.AddSimpleParam( PN::OPENANGLE, 0.f, true, true );
    h.AddParam< IntInterpolator, DefaultTorus::Plugin::OpenAngleMode, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumOAM >
        ( DefaultTorus::PN::OPENANGLEMODE, DefaultTorus::Plugin::OpenAngleMode::CW, true, true );
	h.AddParam< IntInterpolator, Plugin::WeightCenter, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumWC >
        ( DefaultTorus::PN::WEIGHTCENTERX, Plugin::WeightCenter::CENTER, true, true );
	h.AddParam< IntInterpolator, Plugin::WeightCenter, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumWC >
        ( DefaultTorus::PN::WEIGHTCENTERY, Plugin::WeightCenter::MIN, true, true );
	h.AddParam< IntInterpolator, Plugin::WeightCenter, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumWC >
        ( DefaultTorus::PN::WEIGHTCENTERZ, Plugin::WeightCenter::CENTER, true, true );
	h.AddParam< IntInterpolator, Plugin::MappingType, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumMT >
		( PN::MAPPINGTYPE, Plugin::MappingType::DOUBLETEXTURE, true, true );

    h.SetOrCreatePSModel();

    return h.GetModel();
}

IPluginPtr                      PluginDesc::CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< Plugin >( name, prev, timeEvaluator );
}

std::string                     PluginDesc::UID                 ()
{
    return "DEFAULT_TORUS";
}


bool                                Plugin::NeedsTopologyUpdate()
{
    return ParameterChanged( PN::TESSELATION ) ||
        ParameterChanged( PN::RADIUS ) ||
        ParameterChanged( PN::RADIUSCROSSSECTION ) ||
        ParameterChanged( PN::OPENANGLE ) ||
		ParameterChanged( PN::OPENANGLEMODE ) ||
		ParameterChanged( PN::WEIGHTCENTERX ) ||
		ParameterChanged( PN::WEIGHTCENTERY ) ||
		ParameterChanged( PN::WEIGHTCENTERZ ) ||
		ParameterChanged( PN::MAPPINGTYPE );
}

Plugin::Plugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model )
    : DefaultGeometryPluginBase( name, uid, prev, model )
{
    m_tesselation = QueryTypedValue< ValueIntPtr >( GetValue( PN::TESSELATION ) );
    m_radius = QueryTypedValue< ValueFloatPtr >( GetValue( PN::RADIUS ) );
    m_radiusCrossSection = QueryTypedValue< ValueFloatPtr >( GetValue( PN::RADIUSCROSSSECTION ) );
    m_openAngle = QueryTypedValue< ValueFloatPtr >( GetValue( PN::OPENANGLE ) );
	m_openAngleMode = QueryTypedParam< std::shared_ptr< ParamEnum< OpenAngleMode > > >( GetParameter( PN::OPENANGLEMODE ) );

	m_weightCenterX = QueryTypedParam< std::shared_ptr< ParamEnum< WeightCenter > > >( GetParameter( PN::WEIGHTCENTERX ) );
	m_weightCenterY = QueryTypedParam< std::shared_ptr< ParamEnum< WeightCenter > > >( GetParameter( PN::WEIGHTCENTERY ) );
	m_weightCenterZ = QueryTypedParam< std::shared_ptr< ParamEnum< WeightCenter > > >( GetParameter( PN::WEIGHTCENTERZ ) );
	
	m_mappingType = QueryTypedParam< std::shared_ptr< ParamEnum< MappingType > > >( GetParameter( PN::MAPPINGTYPE ) );

	m_pluginParamValModel->Update();
    InitGeometry();
}



#include "Mathematics/Defines.h"
class Generator : public IGeometryNormalsUVsGenerator
{
protected:
    int tesselation;
    float radius, radius2, openangle;
	Plugin::OpenAngleMode open_angle_mode;
	Plugin::WeightCenter weight_centerX;
	Plugin::WeightCenter weight_centerY;
	Plugin::WeightCenter weight_centerZ;
	Plugin::MappingType mapping_type;

	glm::vec3 center_translate;

public:
	Generator( int t, float r, float r2, float oa, Plugin::OpenAngleMode oam, Plugin::WeightCenter wcx, Plugin::WeightCenter wcy, Plugin::WeightCenter wcz, Plugin::MappingType mt )
		: tesselation( t ), radius( r ), radius2( r2), openangle( oa ), open_angle_mode( oam ), weight_centerX( wcx ), weight_centerY( wcy ), weight_centerZ( wcz ), mapping_type( mt ) { }

	glm::vec3 computeWeightCenter( Plugin::WeightCenter centerX, Plugin::WeightCenter centerY, Plugin::WeightCenter centerZ )
	{
		glm::vec3 centerTranslate = glm::vec3( 0.0f, 0.0f, 0.0f );

		if( centerX == Plugin::WeightCenter::MAX )
			centerTranslate += glm::vec3( -radius - radius2, 0.0, 0.0 );
		else if( centerX == Plugin::WeightCenter::CENTER )
			centerTranslate += glm::vec3( 0.0, 0.0, 0.0 );
		else if( centerX == Plugin::WeightCenter::MIN )
			centerTranslate += glm::vec3( radius + radius2, 0.0, 0.0 );
	
		if( centerY == Plugin::WeightCenter::MAX )
			centerTranslate += glm::vec3( 0.0f, -radius - radius2, 0.0f );
		else if( centerY == Plugin::WeightCenter::CENTER )
			centerTranslate += glm::vec3( 0.0f, 0.0, 0.0f );
		else if( centerY == Plugin::WeightCenter::MIN )
			centerTranslate += glm::vec3( 0.0f, radius + radius2, 0.0f );

		if( centerZ == Plugin::WeightCenter::MAX )
			centerTranslate += glm::vec3( 0.0, 0.0, -radius2 );
		else if( centerZ == Plugin::WeightCenter::CENTER )
			centerTranslate += glm::vec3( 0.0, 0.0, 0.0 );
		else if( centerZ == Plugin::WeightCenter::MIN )
			centerTranslate += glm::vec3( 0.0, 0.0, radius2 );
		
		return centerTranslate;
	}

	float computeAngle2Clamped( float angle, float stripe_num )
	{
		float ret_value = angle * ( stripe_num + 1 );
		if( openangle > 0.0 )
		{
			float max_angle = float( TWOPI - TO_RADIANS( openangle ) );
			if( ret_value > max_angle )
				return max_angle;
		}

		return ret_value;
	}

	float computeAngleOffset( Plugin::OpenAngleMode mode, float open_angle )
	{
		float angle_offset = 0.0f;

		if( mode == Plugin::OpenAngleMode::CW )
			angle_offset = 0.0;     //-float( PI /2 );
		else if( mode == Plugin::OpenAngleMode::CCW )
			angle_offset = float( TO_RADIANS( open_angle ) - PI /2 );
		else if( mode == Plugin::OpenAngleMode::SYMMETRIC )
			angle_offset = float( TO_RADIANS( open_angle / 2 ) - PI /2 );
		else
			assert( false );

		return angle_offset;
	}

	glm::vec2 getUV( float phi, float theta )
	{
		float u = static_cast<float>( phi / TWOPI );
		float v = static_cast<float>( theta / TWOPI );

		glm::vec2 uv( u, v );
		return uv;
	}

    void GenerateGeometryNormalsUVs( Float3AttributeChannelPtr verts, Float3AttributeChannelPtr normals, Float2AttributeChannelPtr uvs ) override
    {
        center_translate = computeWeightCenter( weight_centerX, weight_centerY, weight_centerZ );
        float angleOffset = computeAngleOffset( open_angle_mode, openangle );

        int maxLoop;
        if( openangle != 0.0/* && openangle != 360 */ )		// Uncomment if you want to see organ.
            maxLoop = static_cast< int >( ceil( float( ( TWOPI - TO_RADIANS( openangle ) ) / ( TWOPI / tesselation ) ) ) );
        else
            maxLoop = tesselation;

        for( int j = 0; j < maxLoop; j++ )
        {
            for( int i = 0; i <= tesselation; i++ )
            {
                double phi = i * TWOPI / tesselation;
                double theta = j * TWOPI / tesselation + angleOffset;

                verts->AddAttribute( glm::vec3( cos( theta ) * ( radius + radius2 * cos( phi ) ), sin( theta ) * ( radius + radius2 * cos( phi ) ), radius2 * sin( phi ) ) + center_translate );
                uvs->AddAttribute( getUV( ( float )phi, ( float )theta ) );

                phi = i * TWOPI / tesselation;
                theta = computeAngle2Clamped( float( TWOPI / tesselation ), float( j ) ) + angleOffset;

                verts->AddAttribute( glm::vec3( cos( theta ) * ( radius + radius2 * cos( phi ) ), sin( theta ) * ( radius + radius2 * cos( phi ) ), radius2 * sin( phi ) ) + center_translate );
                uvs->AddAttribute( getUV( ( float )phi, ( float )theta ) );
            }
        }

        GeometryGeneratorHelper::GenerateNonWeightedNormalsFromTriangleStrips( verts, normals );
    }

};

class ClosureGenerator : public Generator
{
protected:
	bool rotated;		///< Indicates which closure should we generate.
public:
	ClosureGenerator( int t, float r, float r2, float oa, Plugin::OpenAngleMode oam, Plugin::WeightCenter wcx, Plugin::WeightCenter wcy, Plugin::WeightCenter wcz, Plugin::MappingType mt, bool rot )
		: Generator( t, r, r2, oa, oam, wcx, wcy, wcz, mt ), rotated( rot ) {}

	glm::vec2 computeUV( double phi, double theta, bool center )
	{
		if( mapping_type == Plugin::MappingType::OLDSTYLE )
			return getUV( (float)phi, (float)theta );
		else if( mapping_type == Plugin::MappingType::DOUBLETEXTURE )
		{
			glm::vec2 translate;
			glm::vec2 scale( 0.25, 0.5 );

			if( rotated )
				translate = glm::vec2( 0.25, 0.5 );
			else
				translate = glm::vec2( 0.75, 0.5 );

			if( center )
				return translate;

			glm::vec2 result( cos( phi ), sin( phi ) );
			result = translate + scale * result;

			return result;
		}
		else
			assert( false );
		return glm::vec2(0.0, 0.0);
	}

	void generateClosure( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs, int j, float angleOffset, bool rotated )
	{
        if( !rotated )
            j = 0;

		double theta = computeAngle2Clamped( float( TWOPI / tesselation), float( j - 1 ) ) + angleOffset;
		double cosTheta = cos( theta );
		double sinTheta = sin( theta );

		for( int i = 0; i <= tesselation; ++i )
		{
			double phi = i * TWOPI / tesselation + angleOffset;

            if( rotated )
            {
			    verts->AddAttribute( glm::vec3( cosTheta*( radius + radius2*cos( phi ) ), sinTheta * ( radius + radius2 * cos(phi) ), radius2 * sin(phi) ) + center_translate );
			    uvs->AddAttribute( computeUV( phi, theta, false ) );

			    verts->AddAttribute( glm::vec3( cosTheta* radius , sinTheta * radius, 0.0 ) + center_translate );
			    uvs->AddAttribute( computeUV( phi, theta, true ) );
            }
            else
            {
			    verts->AddAttribute( glm::vec3( cosTheta* radius , sinTheta * radius, 0.0 ) + center_translate );
			    uvs->AddAttribute( computeUV( phi, theta, true ) );

                verts->AddAttribute( glm::vec3( cosTheta*( radius + radius2*cos( phi ) ), sinTheta * ( radius + radius2 * cos(phi) ), radius2 * sin(phi) ) + center_translate );
			    uvs->AddAttribute( computeUV( phi, theta, false ) );
            }
		}
	}

	void GenerateGeometryNormalsUVs( Float3AttributeChannelPtr verts, Float3AttributeChannelPtr normals, Float2AttributeChannelPtr uvs ) override
    {
		center_translate = computeWeightCenter( weight_centerX, weight_centerY, weight_centerZ );
		float angle_offset = computeAngleOffset( open_angle_mode, openangle );

		int max_loop;
		if( openangle != 0.0/* && openangle != 360 */)		// Uncomment if you want to see organ.
			max_loop = static_cast<int>( ceil( float( ( TWOPI - TO_RADIANS( openangle ) ) / ( TWOPI / tesselation ) ) ) );
		else
			max_loop = tesselation;

        generateClosure( verts, uvs, 0, angle_offset, rotated );
    
        GeometryGeneratorHelper::GenerateNonWeightedNormalsFromTriangleStrips( verts, normals );
    }
};

std::vector<IGeometryGeneratorPtr>  Plugin::GetGenerators()
{
    std::vector< IGeometryGeneratorPtr > gens;
    
    gens.push_back( std::make_shared< Generator >( 
        m_tesselation->GetValue(),
        m_radius->GetValue(),
        m_radiusCrossSection->GetValue(),
        m_openAngle->GetValue(),
		m_openAngleMode->Evaluate(),
		m_weightCenterX->Evaluate(),
		m_weightCenterY->Evaluate(),
		m_weightCenterZ->Evaluate(),
		m_mappingType->Evaluate()
        ) );

	if( m_openAngle->GetValue() > 0.0 )
	{
		gens.push_back( std::make_shared< ClosureGenerator >( 
			m_tesselation->GetValue(),
			m_radius->GetValue(),
			m_radiusCrossSection->GetValue(),
			m_openAngle->GetValue(),
			m_openAngleMode->Evaluate(),
			m_weightCenterX->Evaluate(),
			m_weightCenterY->Evaluate(),
			m_weightCenterZ->Evaluate(),
			m_mappingType->Evaluate(),
			false
			) );
		gens.push_back( std::make_shared< ClosureGenerator >( 
			m_tesselation->GetValue(),
			m_radius->GetValue(),
			m_radiusCrossSection->GetValue(),
			m_openAngle->GetValue(),
			m_openAngleMode->Evaluate(),
			m_weightCenterX->Evaluate(),
			m_weightCenterY->Evaluate(),
			m_weightCenterZ->Evaluate(),
			m_mappingType->Evaluate(),
			true
			) );
	}

    return gens;
}


} } }
