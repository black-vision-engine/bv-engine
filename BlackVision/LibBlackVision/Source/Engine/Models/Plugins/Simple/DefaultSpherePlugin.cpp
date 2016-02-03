#include "stdafx.h"

#include "DefaultSpherePlugin.h"
#include "Mathematics/Defines.h"

namespace bv { namespace model {
	
	
typedef ParamEnum< DefaultSphere::Plugin::MappingType> ParamEnumMT;
typedef ParamEnum< DefaultSphere::Plugin::OpenAngleMode > ParamEnumOAM;

VoidPtr    ParamEnumOAM::QueryParamTyped  ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}

template<>
static IParameterPtr        ParametersFactory::CreateTypedParameter< DefaultSphere::Plugin::OpenAngleMode >                 ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return CreateParameterEnum< DefaultSphere::Plugin::OpenAngleMode >( name, timeline );
}

VoidPtr    ParamEnumMT::QueryParamTyped  ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}

template<>
static IParameterPtr        ParametersFactory::CreateTypedParameter< DefaultSphere::Plugin::MappingType >                 ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return CreateParameterEnum< DefaultSphere::Plugin::MappingType >( name, timeline );
}
	
#include "Engine/Models/Plugins/ParamValModel/SimpleParamValEvaluator.inl"
	
namespace DefaultSphere {


const std::string PN::VERTICAL_STRIPES = "vertical stripes";
const std::string PN::HORIZONTAL_STRIPES = "horizontal stripes";
const std::string PN::RADIUS = "radius";
const std::string PN::OPEN_ANGLE = "open angle";
const std::string PN::OPEN_ANGLE_MODE = "open angle mode";
const std::string PN::MAPPING_TYPE = "mappig type";

PluginDesc::PluginDesc()
    : DefaultGeometryPluginDescBase( UID(), "sphere" )
{
}


IPluginPtr                      PluginDesc::CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< Plugin >( name, prev, timeEvaluator );
}

std::string PluginDesc::UID()
{
    return "DEFAULT_SPHERE";
}


DefaultPluginParamValModelPtr   PluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper h( timeEvaluator );

    h.CreateVacModel();
    h.AddSimpleParam<int>( PN::VERTICAL_STRIPES, 10, true, true );
	h.AddSimpleParam<int>( PN::HORIZONTAL_STRIPES, 10, true, true );
	h.AddSimpleParam<float>( PN::RADIUS, 1.0f, true, true );
	h.AddSimpleParam<float>( PN::OPEN_ANGLE, 240.0f, true, true );
	h.AddParam< IntInterpolator, Plugin::OpenAngleMode, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumOAM >
		( DefaultSphere::PN::OPEN_ANGLE_MODE, Plugin::OpenAngleMode::CW, true, true );
	h.AddParam< IntInterpolator, Plugin::MappingType, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumMT >
		( DefaultSphere::PN::MAPPING_TYPE, Plugin::MappingType::DOUBLETEXTURE, true, true );

    h.CreatePSModel();

    return h.GetModel();
}

//====================================================//
// Generator
//====================================================//
namespace Generator
{
	unsigned int		horizontal_stripes;
	unsigned int		vertical_stripes;
	float				radius;
	float				open_angle;
	Plugin::OpenAngleMode		open_angle_mode;
	Plugin::MappingType			mapping_type;

	float computeAngleOffset( Plugin::OpenAngleMode mode, float open_angle )
	{
		mode;
		open_angle;
		float angle_offset = 0.0f;

		if( mode == Plugin::OpenAngleMode::CW )
			angle_offset = 0.0;
		else if( mode == Plugin::OpenAngleMode::CCW )
			angle_offset = (float)TO_RADIANS( open_angle );
		else if( mode == Plugin::OpenAngleMode::SYMMETRIC )
			angle_offset = (float)TO_RADIANS( open_angle / 2 );
		else
			assert( false );

		return angle_offset;
	}
	
	float computeAngle2Clamped( float angle, float stripe_num )
	{
		float ret_value = angle * ( stripe_num + 1 );
		if( open_angle > 0.0 )
		{
			float max_angle = float( TWOPI - TO_RADIANS( open_angle ) );
			if( ret_value > max_angle )
				return max_angle;
		}

		return ret_value;
	}

	/**@brief Generates sphere built as strips.
	
	One instance of class SimpleCubeGenerator generates stripe of a sphere
	from north pole to south pole. Contructor takes in parameter number of
	stripe that it has to process (starting from 0).*/
	class SphereGenerator : public IGeometryAndUVsGenerator
	{
	private:
		float stripe_num;			///<Number of stripe 
	public:

		SphereGenerator( int stripe ) { stripe_num = static_cast<float>( stripe );}
		~SphereGenerator(){}

		Type GetType() { return Type::GEOMETRY_AND_UVS; }


		glm::vec2 generateUV( float horizontal_angle, float vertical_angle )
		{
			///UVs are working good in texture mirror mode
			const glm::vec2 bottomUV( 0.0, 0.0 );		// UV clipping
			const glm::vec2 topUV( 1.0, 1.0 );			// UV clipping

			glm::vec2 returnUV = glm::vec2( horizontal_angle, vertical_angle / PI );

			if( mapping_type == Plugin::MappingType::SINGLETEXTURE )
			{
				glm::vec2 scale_factor = glm::vec2( 2.0 / 3.0, 1.0 );
				returnUV = returnUV * scale_factor;
			}

			returnUV = glm::clamp( returnUV, bottomUV, topUV );

			return returnUV;
		}

		void GenerateGeometryAndUVs( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs ) override
        {
			assert( stripe_num < horizontal_stripes );
			assert( horizontal_stripes >= 3 );
			assert( vertical_stripes >= 3 );

			float angle_offset = computeAngleOffset( open_angle_mode, open_angle );

			// Vertex on north pole ( we need two verticies )
			verts->AddAttribute( glm::vec3( 0.0, 1.0, 0.0 ) * radius );
			verts->AddAttribute( glm::vec3( 0.0, 1.0, 0.0 ) * radius );

			float vert_delta_angle = float( PI ) / float(vertical_stripes);			// Added each loop
			float hor_delta_angle = float( TWOPI ) / float(horizontal_stripes);		// Added each loop

			// We compute cos and sin of angles we need (we are processing stripe, so we must
			// take values of theese functions for left and right verticies.
			float cos_hor_angle1 = radius * cos( hor_delta_angle * stripe_num + angle_offset );
			float sin_hor_angle1 = radius * sin( hor_delta_angle * stripe_num + angle_offset );

			float hor_angle2_clamped = computeAngle2Clamped( hor_delta_angle, stripe_num );

			float cos_hor_angle2 = radius * cos( hor_angle2_clamped + angle_offset );
			float sin_hor_angle2 = radius * sin( hor_angle2_clamped + angle_offset );

			float alfa = float( PI ) / float(2) - vert_delta_angle;		// Angle counting from north pole

			for( unsigned int i = 0; i < vertical_stripes; ++i )
			{
				float cos_alfa = cos( alfa );
				float sin_alfa = sin( alfa );

				float x = sin_hor_angle1 * cos_alfa;
				float y = radius * sin_alfa;
				float z = cos_hor_angle1 * cos_alfa;

				verts->AddAttribute( glm::vec3( x, y, z ) );

				x = sin_hor_angle2 * cos_alfa;
				z = cos_hor_angle2 * cos_alfa;
				verts->AddAttribute( glm::vec3( x, y, z ) );

				alfa -= vert_delta_angle;
			}
			

			float horizontal_angle2;
			horizontal_angle2 = ( hor_angle2_clamped + angle_offset ) / float( TWOPI );		//transformed to interval [0.0 , 1.0]

			float horizontal_angle1 = ( hor_delta_angle * stripe_num + angle_offset ) / float( TWOPI );				//transformed to interval [0.0 , 1.0]
			float vertical_angle = float( PI ) - vert_delta_angle;


			uvs->AddAttribute( /*glm::clamp( glm::vec2( horizontal_angle1, 1.0 ), bottomUV, topUV )*/generateUV( horizontal_angle1, static_cast<float>(PI) ) );
			uvs->AddAttribute( /*glm::clamp( glm::vec2( horizontal_angle2, 1.0 ), bottomUV, topUV )*/generateUV( horizontal_angle2, static_cast<float>(PI) ) );
			for( unsigned int i = 0; i < vertical_stripes - 1; ++i )
			{
				uvs->AddAttribute( /*glm::clamp( glm::vec2( horizontal_angle1, vertical_angle / PI ),  bottomUV, topUV )*/generateUV( horizontal_angle1, vertical_angle ) );
				uvs->AddAttribute( /*glm::clamp( glm::vec2( horizontal_angle2, vertical_angle / PI ),  bottomUV, topUV )*/generateUV( horizontal_angle2, vertical_angle ) );

				vertical_angle -= vert_delta_angle;
			}
			uvs->AddAttribute( /*glm::clamp( glm::vec2( horizontal_angle1, vertical_angle / PI ),  bottomUV, topUV )*/generateUV( horizontal_angle1, vertical_angle ) );
			vertical_angle -= vert_delta_angle;
			uvs->AddAttribute( /*glm::clamp( glm::vec2( horizontal_angle2, vertical_angle / PI ),  bottomUV, topUV )*/generateUV( horizontal_angle2, vertical_angle ) );
		}

	};

	class SphereClosureGenerator : public IGeometryAndUVsGenerator
	{
	private:
		float number_of_stripes;
	public:

		SphereClosureGenerator( int stripe ) { number_of_stripes = static_cast<float>( stripe );}
		~SphereClosureGenerator(){}

		Type GetType() { return Type::GEOMETRY_AND_UVS; }

		glm::vec2 computeClosureUV( glm::vec2 normalized_pos, bool invert )
		{
			if( invert )
				normalized_pos.x = -normalized_pos.x;

			glm::vec2 scale_factor;
			glm::vec2 closureRegion;

			if( mapping_type == Plugin::MappingType::SINGLETEXTURE )
			{
				scale_factor = glm::vec2( 1.0 / 3.0, 1.0 );
				closureRegion = glm::vec2( 2.0 / 3.0, 0.0 );
			}
			else if( mapping_type == Plugin::MappingType::DOUBLETEXTURE )
			{
				scale_factor = glm::vec2( 1.0, 1.0 );
				closureRegion = glm::vec2( 0.0, 0.0 );
			}

			return closureRegion + scale_factor * ( glm::vec2( 0.5, 0.5 ) + normalized_pos * glm::vec2( 0.5, 0.5 ) );
		}

		void GenerateGeometryAndUVs( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs ) override
        {
			float angle_offset = computeAngleOffset( open_angle_mode, open_angle );

			float vert_delta_angle = float( PI ) / float(vertical_stripes);
			float hor_delta_angle = float( TWOPI ) / float(horizontal_stripes);
			
			float hor_angle_clamped = computeAngle2Clamped( hor_delta_angle, number_of_stripes );

			float cos_hor_angle = radius * cos( angle_offset );
			float sin_hor_angle = radius * sin( angle_offset );

			// ===================================================== //
			// The first face
			float U_coord = ( angle_offset ) / float( TWOPI );
			float alfa = float( PI ) / float(2) - vert_delta_angle;		// Angle counting from north pole

			verts->AddAttribute( glm::vec3( 0.0, 1.0, 0.0 ) * radius );
			uvs->AddAttribute( /*glm::vec2( 0.0, 1.0 )*/computeClosureUV( glm::vec2(0.0, 1.0), false ) );
			for( unsigned int i = 0; i < vertical_stripes; ++i )
			{
				//float V_coord = float( ( PI / 2 + alfa ) / PI );
				float cos_alfa = cos( alfa );
				float sin_alfa = sin( alfa );

				float x = 0.0;
				float y = radius * sin_alfa;
				float z = 0.0;
				verts->AddAttribute( glm::vec3( x, y, z ) );
				uvs->AddAttribute( /*glm::vec2( 1.0, V_coord ) */computeClosureUV( glm::vec2(0.0, sin_alfa), false ) );

				x = sin_hor_angle * cos_alfa;		// Radius already multiplied
				z = cos_hor_angle * cos_alfa;		// Radius already multiplied
				verts->AddAttribute( glm::vec3( x, y, z ) );
				uvs->AddAttribute( /*glm::vec2( U_coord, V_coord )*/computeClosureUV( glm::vec2(cos_alfa, sin_alfa), false ) );

				alfa -= vert_delta_angle;
			}
			verts->AddAttribute( glm::vec3( 0.0, -1.0, 0.0 ) * radius );
			uvs->AddAttribute( /*glm::vec2( 0.0, 0.0 )*/computeClosureUV( glm::vec2(0.0, -1.0), false ) );

			// Degenerated traingles
			verts->AddAttribute( glm::vec3( 0.0, 1.0, 0.0 ) * radius );
			uvs->AddAttribute( glm::vec2( 0.0, 1.0 ) );			//It' doesn't matter
			verts->AddAttribute( glm::vec3( 0.0, 1.0, 0.0 ) * radius );
			uvs->AddAttribute( glm::vec2( 0.0, 1.0 ) );			//It' doesn't matter

			// ===================================================== //
			// The second face
			U_coord = ( hor_angle_clamped ) / float( TWOPI );
			alfa = float( PI ) / float(2) - vert_delta_angle;		// Angle counting from north pole
			cos_hor_angle = radius * cos( hor_angle_clamped + angle_offset );
			sin_hor_angle = radius * sin( hor_angle_clamped + angle_offset );
			for( unsigned int i = 0; i < vertical_stripes; ++i )
			{
				//float V_coord = float( ( PI / 2 + alfa ) / PI );
				float cos_alfa = cos( alfa );
				float sin_alfa = sin( alfa );

				float x = sin_hor_angle * cos_alfa;		// Radius already multiplied
				float y = radius * sin_alfa;
				float z = cos_hor_angle * cos_alfa;		// Radius already multiplied
				verts->AddAttribute( glm::vec3( x, y, z ) );
				uvs->AddAttribute( /*glm::vec2( U_coord, V_coord )*/computeClosureUV( glm::vec2(cos_alfa, sin_alfa), true ) );

				x = 0.0;
				z = 0.0;
				verts->AddAttribute( glm::vec3( x, y, z ) );
				uvs->AddAttribute( /*glm::vec2( 0.0, V_coord )*/computeClosureUV( glm::vec2(0.0, sin_alfa), true ) );

				alfa -= vert_delta_angle;
			}
		}

	};
}






Plugin::Plugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model )
		: DefaultGeometryPluginBase( name, uid, prev, model )
{
	horizontal_stripes = QueryTypedValue< ValueIntPtr >( GetValue( PN::HORIZONTAL_STRIPES ) );
	vertical_stripes = QueryTypedValue< ValueIntPtr >( GetValue( PN::VERTICAL_STRIPES ) );
	radius = QueryTypedValue< ValueFloatPtr >( GetValue( PN::RADIUS ) );
	open_angle = QueryTypedValue< ValueFloatPtr >( GetValue( PN::OPEN_ANGLE ) );
	open_angle_mode = QueryTypedParam< std::shared_ptr< ParamEnum< OpenAngleMode > > >( GetParameter( PN::OPEN_ANGLE_MODE ) );
	mapping_type = QueryTypedParam< std::shared_ptr< ParamEnum< MappingType > > >( GetParameter( PN::MAPPING_TYPE ) );

    m_pluginParamValModel->Update();
    InitGeometry();
}


Plugin::~Plugin(void)
{
}

std::vector<IGeometryGeneratorPtr>    Plugin::GetGenerators()
{
    Generator::horizontal_stripes = horizontal_stripes->GetValue();
	Generator::vertical_stripes = vertical_stripes->GetValue();
	Generator::radius = radius->GetValue();
	Generator::open_angle = open_angle->GetValue();
	Generator::open_angle_mode = open_angle_mode->Evaluate();
	Generator::mapping_type = mapping_type->Evaluate();

    std::vector<IGeometryGeneratorPtr> gens;
	
	int stripes_needed;
	if( Generator::open_angle > 0.0 )
	{
		stripes_needed = Generator::horizontal_stripes;
		float angle_step = float( TWOPI ) / float(stripes_needed);
		float angle_needed = float( TWOPI - TO_RADIANS( Generator::open_angle ) );
		stripes_needed = static_cast<int>( ceil( angle_needed / angle_step ) );
		gens.push_back( IGeometryGeneratorPtr( new Generator::SphereClosureGenerator( stripes_needed ) ) );
	}
	else
		stripes_needed = Generator::horizontal_stripes;

	for( int i = 0; i < stripes_needed; ++i )
	{
		gens.push_back( IGeometryGeneratorPtr( new Generator::SphereGenerator( i ) ) );
	}
    return gens;
}

bool                                Plugin::NeedsTopologyUpdate()
{
	return ParameterChanged( PN::HORIZONTAL_STRIPES )
		|| ParameterChanged( PN::VERTICAL_STRIPES )
		|| ParameterChanged( PN::RADIUS )
		|| ParameterChanged( PN::OPEN_ANGLE )
		|| ParameterChanged( PN::OPEN_ANGLE_MODE )
		|| ParameterChanged( PN::MAPPING_TYPE );
}


}}}