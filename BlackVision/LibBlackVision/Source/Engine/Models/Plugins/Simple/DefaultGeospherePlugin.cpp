#include "DefaultGeospherePlugin.h"

namespace bv { namespace model { namespace DefaultGeosphere {

const std::string PN::TESSELLATION = "tessellation";
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
    return "DEFAULT_GEOSPHERE";
}


DefaultPluginParamValModelPtr   PluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper h( timeEvaluator );

    h.CreateVacModel();
    h.AddSimpleParam<int>( PN::TESSELLATION, 10, true, true );
	h.AddSimpleParam<float>( PN::RADIUS, 1.0f, true, true );

    return h.GetModel();
}

//====================================================//
// Generator
//====================================================//
namespace Generator
{
	unsigned int		tesselletion;
	float				radius;

	/**@brief Generates geosphere based on tetrahedron.

	Generates geosphere based on tetrahedron.
	Number of generators depends on tessellations count. pow( 2, tesselletion )*/
	class GeosphereGenerator : public IGeometryAndUVsGenerator
	{
		struct face
		{
			glm::vec3 vertex[3];
		};

	private:
		float tile_num;
		struct face faces_table[4];
	public:

		GeosphereGenerator( int tile ) { tile_num = static_cast<float>( tile );}
		~GeosphereGenerator(){}

		Type GetType() { return Type::GEOMETRY_AND_UVS; }

		void initFacesTable()
		{
			// We take combinations of theese coordinates
			// ( +-1, 0, -1/sqrt(2) )
			// ( 0, +-1, 1/sqrt(2) )

			faces_table[0].vertex[0] = glm::vec3( 1, 1, -1 );
			faces_table[0].vertex[1] = glm::vec3( 1, -1, 1 );
			faces_table[0].vertex[2] = glm:: vec3( -1, 1, 1 );

			faces_table[1].vertex[0] = glm::vec3(1, -1, 1);
			faces_table[1].vertex[1] = glm::vec3(-1, 1, 1);
			faces_table[1].vertex[2] = glm:: vec3(-1, -1, -1);

			faces_table[2].vertex[0] = glm::vec3(-1, 1, 1);
			faces_table[2].vertex[1] = glm::vec3(1, -1, 1);
			faces_table[2].vertex[2] = glm:: vec3(1, 1, -1);

			faces_table[3].vertex[0] = glm::vec3();
			faces_table[3].vertex[1] = glm::vec3();
			faces_table[3].vertex[2] = glm:: vec3();
		}

		int innerLoopMax( int face )
		{
			if( face % 2 )
				// Odd faces
				return (int)tile_num;
			else
				// Even faces
				return (int)(pow( 2, tesselletion ) - tile_num - 1);
		}

		glm::vec3 getTopDownVector( int face )
		{
			glm::vec3 edge = faces_table[face].vertex[0] - faces_table[face].vertex[1];
			edge *= 1/  float( pow( 2, tesselletion ) );
			return edge;
		}

		glm::vec3 getRightLeftVector( int face )
		{
			glm::vec3 edge = faces_table[face].vertex[1] - faces_table[face].vertex[2];
			edge *= 1/  float( pow( 2, tesselletion ) );
			return edge;
		}


		void GenerateGeometryAndUVs( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs ) override
        {
			initFacesTable();	// Needed for functions get[...]Vector

			glm::vec3 top_down = getTopDownVector( 0 );
			glm::vec3 right_left;

			glm::vec3 current_vertex = faces_table[0].vertex[0];
			verts->AddAttribute( glm::normalize( current_vertex ) * radius );
			current_vertex += top_down;
			verts->AddAttribute( glm::normalize( current_vertex ) * radius );

			for( int i = 0; i < 4; ++i )
			{// We have 4 faces
				top_down = getTopDownVector( i );
				right_left = getRightLeftVector( i );
				
				int max_loop = innerLoopMax( i );
				for( int j = 0; j < max_loop; ++j )
				{// We have already two first verticies.
					current_vertex += right_left;
					verts->AddAttribute( glm::normalize( current_vertex ) * radius );
					current_vertex += top_down;
					verts->AddAttribute( glm::normalize( current_vertex ) * radius );
				}

				current_vertex += right_left;
				verts->AddAttribute( current_vertex );
			}

			// UVs coordinates
		}

	};
}


Plugin::Plugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model )
		: DefaultGeometryPluginBase( name, uid, prev, model )
{
	tesselletion = QueryTypedValue< ValueIntPtr >( GetValue( PN::TESSELLATION ) );
	radius = QueryTypedValue< ValueFloatPtr >( GetValue( PN::RADIUS ) );

    m_pluginParamValModel->Update();
    InitGeometry();
}

Plugin::~Plugin(void)
{
}



std::vector<IGeometryGeneratorPtr>    Plugin::GetGenerators()
{
	Generator::tesselletion = tesselletion->GetValue();
	Generator::radius = radius->GetValue();

    std::vector<IGeometryGeneratorPtr> gens;
	for( int i = 0; i < tesselletion->GetValue(); ++i )
	{
		gens.push_back( IGeometryGeneratorPtr( new Generator::GeosphereGenerator( i ) ) );
	}
    return gens;
}

bool                                Plugin::NeedsTopologyUpdate()
{
	return ParameterChanged( PN::TESSELLATION )
		|| ParameterChanged( PN::RADIUS );
}


}}}
