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
	h.AddSimpleParam<float>( PN::OPEN_ANGLE, 0.0f, true, true );
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
		float retValue = angle * ( stripe_num + 1 );
		if( open_angle > 0.0 )
		{
			float maxAngle = float( TWOPI - TO_RADIANS( open_angle ) );
			if( retValue > maxAngle )
				return maxAngle;
		}

		return retValue;
	}

	/**@brief Generates sphere built as strips.
	
	One instance of class SimpleCubeGenerator generates stripe of a sphere
	from north pole to south pole. Contructor takes in parameter number of
	stripe that it has to process (starting from 0).*/
	class SphereGenerator : public IGeometryNormalsUVsGenerator
	{
	private:
		float stripe_num;			///<Number of stripe 
	public:

		SphereGenerator( int stripe ) { stripe_num = static_cast<float>( stripe );}
		~SphereGenerator(){}

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

		void GenerateGeometryNormalsUVs( Float3AttributeChannelPtr verts, Float3AttributeChannelPtr normals, Float2AttributeChannelPtr uvs ) override
        {
			assert( stripe_num < horizontal_stripes );
			assert( horizontal_stripes >= 3 );
			assert( vertical_stripes >= 3 );

			float angleOffset = computeAngleOffset( open_angle_mode, open_angle );

			// Vertex on north pole ( we need two verticies )
            verts->AddAttribute( glm::vec3( 0.0, 1.0, 0.0 ) * radius ); normals->AddAttribute( glm::vec3( 0.0, 1.0, 0.0 ) );
			verts->AddAttribute( glm::vec3( 0.0, 1.0, 0.0 ) * radius ); normals->AddAttribute( glm::vec3( 0.0, 1.0, 0.0 ) );

			float vertDeltaAngle = float( PI ) / float(vertical_stripes);			// Added each loop
			float horDeltaAngle = float( TWOPI ) / float(horizontal_stripes);		// Added each loop

			// We compute cos and sin of angles we need (we are processing stripe, so we must
			// take values of theese functions for left and right verticies.
			float cosHorAngle1 = cos( horDeltaAngle * stripe_num + angleOffset );
			float sinHorAngle1 = sin( horDeltaAngle * stripe_num + angleOffset );

			float horAngle2Clamped = computeAngle2Clamped( horDeltaAngle, stripe_num );

			float cosHorAngle2 = cos( horAngle2Clamped + angleOffset );
			float sinHorAngle2 = sin( horAngle2Clamped + angleOffset );

			float alfa = float( PI ) / float(2) - vertDeltaAngle;		// Angle counting from north pole

			for( unsigned int i = 0; i < vertical_stripes; ++i )
			{
				float cosAlfa = cos( alfa );
				float sinAlfa = sin( alfa );

				float x = radius * sinHorAngle2 * cosAlfa;
				float y = radius * sinAlfa;
				float z = radius * cosHorAngle2 * cosAlfa;

				verts->AddAttribute( glm::vec3( x, y, z ) );    normals->AddAttribute( glm::vec3( sinHorAngle2 * cosAlfa, sinAlfa, cosHorAngle2 * cosAlfa ) );

                x = radius * sinHorAngle1 * cosAlfa;
                z = radius * cosHorAngle1 * cosAlfa;
				verts->AddAttribute( glm::vec3( x, y, z ) );    normals->AddAttribute( glm::vec3( sinHorAngle1 * cosAlfa, sinAlfa, cosHorAngle1 * cosAlfa ) );

				alfa -= vertDeltaAngle;
			}
			

			float horizontalAngle2;
			horizontalAngle2 = ( horAngle2Clamped + angleOffset ) / float( TWOPI );		//transformed to interval [0.0 , 1.0]

			float horizontalAngle1 = ( horDeltaAngle * stripe_num + angleOffset ) / float( TWOPI );				//transformed to interval [0.0 , 1.0]
			float vertical_angle = float( PI ) - vertDeltaAngle;


            uvs->AddAttribute( /*glm::clamp( glm::vec2( horizontalAngle2, 1.0 ), bottomUV, topUV )*/generateUV( horizontalAngle2, static_cast<float>(PI) ) );
			uvs->AddAttribute( /*glm::clamp( glm::vec2( horizontalAngle1, 1.0 ), bottomUV, topUV )*/generateUV( horizontalAngle1, static_cast<float>(PI) ) );
			for( unsigned int i = 0; i < vertical_stripes - 1; ++i )
			{
                uvs->AddAttribute( /*glm::clamp( glm::vec2( horizontalAngle2, vertical_angle / PI ),  bottomUV, topUV )*/generateUV( horizontalAngle2, vertical_angle ) );
				uvs->AddAttribute( /*glm::clamp( glm::vec2( horizontalAngle1, vertical_angle / PI ),  bottomUV, topUV )*/generateUV( horizontalAngle1, vertical_angle ) );

				vertical_angle -= vertDeltaAngle;
			}

			uvs->AddAttribute( /*glm::clamp( glm::vec2( horizontalAngle2, vertical_angle / PI ),  bottomUV, topUV )*/generateUV( horizontalAngle2, vertical_angle ) );
            vertical_angle -= vertDeltaAngle;
   			uvs->AddAttribute( /*glm::clamp( glm::vec2( horizontalAngle1, vertical_angle / PI ),  bottomUV, topUV )*/generateUV( horizontalAngle1, vertical_angle ) );


            //GeometryGeneratorHelper::GenerateNonWeightedNormalsFromTriangleStrips( verts, normals );        
        }

	};

	class SphereClosureGenerator : public IGeometryNormalsUVsGenerator
	{
	private:
		float number_of_stripes;
	public:

		SphereClosureGenerator( int stripe ) { number_of_stripes = static_cast<float>( stripe );}
		~SphereClosureGenerator(){}

		glm::vec2 computeClosureUV( glm::vec2 normalized_pos, bool invert )
		{
			if( invert )
				normalized_pos.x = -normalized_pos.x;

			glm::vec2 scaleFactor;
			glm::vec2 closureRegion;

			if( mapping_type == Plugin::MappingType::SINGLETEXTURE )
			{
				scaleFactor = glm::vec2( 1.0 / 3.0, 1.0 );
				closureRegion = glm::vec2( 2.0 / 3.0, 0.0 );
			}
			else if( mapping_type == Plugin::MappingType::DOUBLETEXTURE )
			{
				scaleFactor = glm::vec2( 1.0, 1.0 );
				closureRegion = glm::vec2( 0.0, 0.0 );
			}

			return closureRegion + scaleFactor * ( glm::vec2( 0.5, 0.5 ) + normalized_pos * glm::vec2( 0.5, 0.5 ) );
		}

		void GenerateGeometryNormalsUVs( Float3AttributeChannelPtr verts, Float3AttributeChannelPtr normals, Float2AttributeChannelPtr uvs ) override
        {
			float angleOffset = computeAngleOffset( open_angle_mode, open_angle );

			float vertDeltaAngle = float( PI ) / float(vertical_stripes);
			float horDeltaAngle = float( TWOPI ) / float(horizontal_stripes);
			
			float horAngleClamped = computeAngle2Clamped( horDeltaAngle, number_of_stripes );

			float cosHorAngle = radius * cos( angleOffset );
			float sinHorAngle = radius * sin( angleOffset );

			// ===================================================== //
			// The first face
			float U_coord = ( angleOffset ) / float( TWOPI );
			float alfa = float( PI ) / float(2) - vertDeltaAngle;		// Angle counting from north pole

			verts->AddAttribute( glm::vec3( 0.0, 1.0, 0.0 ) * radius );
			uvs->AddAttribute( /*glm::vec2( 0.0, 1.0 )*/computeClosureUV( glm::vec2(0.0, 1.0), false ) );
			for( unsigned int i = 0; i < vertical_stripes; ++i )
			{
				float cosAlfa = cos( alfa );
                float sinAlfa = sin( alfa );

                float x = 0.0f;		// Radius already multiplied
                float y = radius * sinAlfa;
                float z = 0.0f;		// Radius already multiplied

                verts->AddAttribute( glm::vec3( x, y, z ) );
                uvs->AddAttribute( /*glm::vec2( 1.0, V_coord ) */computeClosureUV( glm::vec2(0.0, sinAlfa), false ) );

                x = sinHorAngle * cosAlfa;		// Radius already multiplied
                z = cosHorAngle * cosAlfa;		// Radius already multiplied

                verts->AddAttribute( glm::vec3( x, y, z ) );
                uvs->AddAttribute( /*glm::vec2( U_coord, V_coord )*/computeClosureUV( glm::vec2(cosAlfa, sinAlfa), false ) );

				alfa -= vertDeltaAngle;
			}
			verts->AddAttribute( glm::vec3( 0.0, -1.0, 0.0 ) * radius );
			uvs->AddAttribute( /*glm::vec2( 0.0, 0.0 )*/computeClosureUV( glm::vec2(0.0, -1.0), false ) );

			// Degenerated traingles
			verts->AddAttribute( glm::vec3( 0.0, 1.0, 0.0 ) * radius );
			uvs->AddAttribute( glm::vec2( 0.0, 1.0 ) );			//It' doesn't matter
			verts->AddAttribute( glm::vec3( 0.0, 1.0, 0.0 ) * radius );
			uvs->AddAttribute( computeClosureUV( glm::vec2( 0.0, 1.0 ), true ) );			//It' doesn't matter

			// ===================================================== //
			// The second face
			U_coord = ( horAngleClamped ) / float( TWOPI );
			alfa = float( PI ) / float(2) - vertDeltaAngle;		// Angle counting from north pole
			cosHorAngle = radius * cos( horAngleClamped + angleOffset );
			sinHorAngle = radius * sin( horAngleClamped + angleOffset );
			for( unsigned int i = 0; i < vertical_stripes; ++i )
			{
				//float V_coord = float( ( PI / 2 + alfa ) / PI );
				float cosAlfa = cos( alfa );
				float sinAlfa = sin( alfa );

				float x = 0.0f;		// Radius already multiplied
				float y = radius * sinAlfa;
				float z = 0.0f;		// Radius already multiplied

				verts->AddAttribute( glm::vec3( x, y, z ) );
				uvs->AddAttribute( /*glm::vec2( 0.0, V_coord )*/computeClosureUV( glm::vec2(0.0, sinAlfa), true ) );

				x = sinHorAngle * cosAlfa;		// Radius already multiplied
				z = cosHorAngle * cosAlfa;		// Radius already multiplied

				verts->AddAttribute( glm::vec3( x, y, z ) );
                uvs->AddAttribute( /*glm::vec2( U_coord, V_coord )*/computeClosureUV( glm::vec2(cosAlfa, sinAlfa), true ) );

				alfa -= vertDeltaAngle;
			}

            GeometryGeneratorHelper::GenerateNonWeightedNormalsFromTriangleStrips( verts, normals );
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
        gens.push_back( std::make_shared< Generator::SphereClosureGenerator >( stripes_needed ) );
	}
	else
		stripes_needed = Generator::horizontal_stripes;

	for( int i = 0; i < stripes_needed; ++i )
	{
		gens.push_back( std::make_shared< Generator::SphereGenerator >( i ) );
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