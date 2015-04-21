#include "DefaultSpherePlugin.h"
#include "Mathematics/Defines.h"

namespace bv { namespace model { namespace DefaultSphere {


const std::string PN::VERTICAL_STRIPES = "vertical stripes";
const std::string PN::HORIZONTAL_STRIPES = "horizontal stripes";
const std::string PN::RADIUS = "radius";

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


		void GenerateGeometryAndUVs( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs ) override
        {
			assert( stripe_num < horizontal_stripes );
			assert( horizontal_stripes >= 3 );
			assert( vertical_stripes >= 3 );

			// Vertex on north pole ( we need two verticies )
			verts->AddAttribute( glm::vec3( 0.0, 1.0, 0.0 ) * radius );
			verts->AddAttribute( glm::vec3( 0.0, 1.0, 0.0 ) * radius );

			float vert_delta_angle = float( PI ) / float(vertical_stripes);
			float hor_delta_angle = float( 2 * PI ) / float(horizontal_stripes);

			// We compute cos and sin of angles we need (we are processing stripe, so we must
			// take values of theese functions for left and right verticies.
			float cos_hor_angle1 = radius * cos( hor_delta_angle * stripe_num );
			float sin_hor_angle1 = radius * sin( hor_delta_angle * stripe_num );

			float cos_hor_angle2;
			float sin_hor_angle2;

			cos_hor_angle2 = radius * cos( hor_delta_angle * ( stripe_num + 1 ) );
			sin_hor_angle2 = radius * sin( hor_delta_angle * ( stripe_num + 1 ) );

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
			horizontal_angle2 = ( hor_delta_angle * ( stripe_num + 1 ) ) / float( 2 * PI );		//transformed to interval [0.0 , 1.0]

			float horizontal_angle1 = ( hor_delta_angle * stripe_num ) / float( 2 * PI );				//transformed to interval [0.0 , 1.0]
			float vertical_angle = float( PI ) - vert_delta_angle;


			uvs->AddAttribute( glm::clamp( glm::vec2( horizontal_angle1, 1.0 ), glm::vec2( 0.01, 0.01 ), glm::vec2( 0.95, 0.95 ) ) );
			uvs->AddAttribute( glm::clamp( glm::vec2( horizontal_angle2, 1.0 ), glm::vec2( 0.01, 0.01 ), glm::vec2( 0.95, 0.95 ) ) );
			for( unsigned int i = 0; i < vertical_stripes - 1; ++i )
			{
				uvs->AddAttribute(  glm::clamp( glm::vec2( horizontal_angle1, vertical_angle / PI ),  glm::vec2( 0.005, 0.005 ), glm::vec2( 0.995, 0.995 ) ) );
				uvs->AddAttribute(  glm::clamp( glm::vec2( horizontal_angle2, vertical_angle / PI ),  glm::vec2( 0.005, 0.005 ), glm::vec2( 0.995, 0.995 ) ) );

				vertical_angle -= vert_delta_angle;
			}
			uvs->AddAttribute(  glm::clamp( glm::vec2( horizontal_angle1, vertical_angle / PI ),  glm::vec2( 0.01, 0.05 ), glm::vec2( 0.95, 0.95 ) ) );
			vertical_angle -= vert_delta_angle;
			uvs->AddAttribute(  glm::clamp( glm::vec2( horizontal_angle2, vertical_angle / PI ),  glm::vec2( 0.01, 0.05 ), glm::vec2( 0.95, 0.95 ) ) );
		}

	};

}






Plugin::Plugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model )
		: DefaultGeometryPluginBase( name, uid, prev, model )
{
	horizontal_stripes = QueryTypedValue< ValueIntPtr >( GetValue( PN::HORIZONTAL_STRIPES ) );
	vertical_stripes = QueryTypedValue< ValueIntPtr >( GetValue( PN::VERTICAL_STRIPES ) );
	radius = QueryTypedValue< ValueFloatPtr >( GetValue( PN::RADIUS ) );

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

    std::vector<IGeometryGeneratorPtr> gens;
	for( int i = 0; i < horizontal_stripes->GetValue(); ++i )
	{
		gens.push_back( IGeometryGeneratorPtr( new Generator::SphereGenerator( i ) ) );
	}
    return gens;
}

bool                                Plugin::NeedsTopologyUpdate()
{
	return ParameterChanged( PN::HORIZONTAL_STRIPES )
		|| ParameterChanged( PN::VERTICAL_STRIPES )
		|| ParameterChanged( PN::RADIUS );
}


}}}