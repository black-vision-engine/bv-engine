#include "DefaultGeospherePlugin.h"
#include "..\..\Dep\Common\glm\glm\gtx\vector_angle.hpp"
#include "Mathematics/Defines.h"

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
    h.AddSimpleParam<int>( PN::TESSELLATION, 6, true, true );
	h.AddSimpleParam<float>( PN::RADIUS, 2.0f, true, true );

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
			glm::vec2 uv[3];
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

			faces_table[0].vertex[0] = glm::vec3( 1, 1, -1 );		faces_table[0].uv[0] = glm::vec2( 0.0, 0.0 );
			faces_table[0].vertex[1] = glm::vec3( 1, -1, 1 );		faces_table[0].uv[1] = glm::vec2( 1.0 / 5.0, 1.0 );
			faces_table[0].vertex[2] = glm:: vec3( -1, 1, 1 );		faces_table[0].uv[2] = glm::vec2( 2.0 / 5.0, 0.0 );

			faces_table[1].vertex[0] = glm::vec3(1, -1, 1);			faces_table[1].uv[0] = glm::vec2();
			faces_table[1].vertex[1] = glm::vec3(-1, 1, 1);			faces_table[1].uv[1] = glm::vec2();
			faces_table[1].vertex[2] = glm:: vec3(-1, -1, -1);		faces_table[1].uv[2] = glm::vec2();

			faces_table[2].vertex[0] = glm::vec3(-1, 1, 1);			faces_table[2].uv[0] = glm::vec2();
			faces_table[2].vertex[1] = glm::vec3(-1, -1, -1);		faces_table[2].uv[1] = glm::vec2();
			faces_table[2].vertex[2] = glm:: vec3(1, 1, -1);		faces_table[2].uv[2] = glm::vec2();

			faces_table[3].vertex[0] = glm::vec3(-1, -1, -1);		faces_table[3].uv[0] = glm::vec2();
			faces_table[3].vertex[1] = glm::vec3(1, 1, -1);			faces_table[3].uv[1] = glm::vec2();
			faces_table[3].vertex[2] = glm:: vec3(1, -1, 1);		faces_table[3].uv[2] = glm::vec2();
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
			glm::vec3 edge = faces_table[face].vertex[1]  - faces_table[face].vertex[0];
			edge *= 1/  float( pow( 2, tesselletion ) );
			return edge;
		}

		glm::vec3 getRightLeftVector( int face )
		{
			glm::vec3 edge = faces_table[face].vertex[2] - faces_table[face].vertex[1];
			edge *= 1/  float( pow( 2, tesselletion ) );
			return edge;
		}

		glm::vec3 computeLength( glm::vec3 vector )
		{
			return glm::normalize( vector ) * radius;
			//return vector;
		}

		//glm::vec2 interpolateBetweenUV( int row, int column, glm::vec2& uv[3] )
		//{

		//}

		glm::vec2 computeUV( int face, int row, int column, glm::vec3 vertex )
		{
			{ vertex; }
			{ row; }
			{ column; }
			{ face; }
			return glm::vec2( 0.0, 0.0 );//interpolateBetweenUV();
		}


#define FACES 4
		void GenerateGeometryAndUVs( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs ) override
        {
			initFacesTable();	// Needed for functions get[...]Vector

			glm::vec3 top_down = getTopDownVector( 0 );
			glm::vec3 right_left;

			glm::vec3 current_vertex = faces_table[0].vertex[0];

			current_vertex += float( tile_num ) * top_down;
			verts->AddAttribute( computeLength( current_vertex ) );
			uvs->AddAttribute( computeUV( 0, 0, (int)tile_num, current_vertex ) );

			current_vertex += top_down;
			verts->AddAttribute( computeLength( current_vertex ) );
			uvs->AddAttribute( computeUV( 0, 1, (int)tile_num + 1, current_vertex ) );

			for( int i = 0; i < FACES; ++i )
			{// We have 4 faces
				top_down = getTopDownVector( i );
				right_left = getRightLeftVector( i );
				
				int max_loop = innerLoopMax( i );
				for( int j = 0; j < max_loop; ++j )
				{// We have already two first verticies.
					// Step left
					current_vertex += right_left;
					verts->AddAttribute( computeLength( current_vertex ) );
					uvs->AddAttribute( computeUV( i, j+1, (int)tile_num, current_vertex ) );

					current_vertex += top_down;
					verts->AddAttribute( computeLength( current_vertex ) );
					uvs->AddAttribute( computeUV( i, j+2, (int)tile_num + 1, current_vertex ) );
				}

				current_vertex += right_left;
				verts->AddAttribute( computeLength( current_vertex ) );
				uvs->AddAttribute( computeUV( i, (int)pow( 2, tesselletion ) + 1, (int)tile_num, current_vertex ) );
			}

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

	int loop_max = static_cast<int>( pow( 2, tesselletion->GetValue() ) );
	for( int i = 0; i < loop_max; ++i )
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
