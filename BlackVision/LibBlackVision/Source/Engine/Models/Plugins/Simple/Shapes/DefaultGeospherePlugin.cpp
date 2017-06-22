#include "stdafx.h"

#include "DefaultGeospherePlugin.h"

#include "Mathematics/Defines.h"

#include "Memory/MemoryLeaks.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"


namespace bv { namespace model { namespace DefaultGeosphere {

const std::string PN::TESSELATION = "tesselation";
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

    h.SetOrCreateVacModel();
    h.AddSimpleParam<int>( PN::TESSELATION, 6, true, true );
	h.AddSimpleParam<float>( PN::RADIUS, 2.0f, true, true );

    h.SetOrCreatePSModel();

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
	class GeosphereGenerator : public IGeometryNormalsUVsGenerator
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

		void initFacesTable()
		{
			// We take combinations of theese coordinates
			// ( +-1, 0, -1/sqrt(2) )
			// ( 0, +-1, 1/sqrt(2) )

			glm::vec3 pointA( 1, 1, -1 );
			glm::vec3 pointB( 1, -1, 1 );
			glm::vec3 pointC( -1, 1, 1 );
			glm::vec3 pointD(-1, -1, -1);
			//glm::vec2 pointA1UV( 1.0, 1.0 );
			//glm::vec2 pointA2UV( 1.0, 0.0 );
			//glm::vec2 pointBUV( 1.0, 1.0 / 2.0 );
			//glm::vec2 pointC1UV( 1.0, 0.0 );
			//glm::vec2 pointC2UV( 0.0, 0.0 );
			//glm::vec2 pointDUV( 0.0, 1.0 / 2.0 );

			glm::vec2 pointA1UV( 0.0, 0.0 );
			glm::vec2 pointA2UV( 1.0, 0.0 );
			glm::vec2 pointBUV( 1.0 / 2.0, 0.0 );
			glm::vec2 pointC1UV( 0.0, 1.0 );
			glm::vec2 pointC2UV( 1.0, 1.0 );
			glm::vec2 pointDUV( 1.0 / 2.0, 1.0 );

			faces_table[0].vertex[0] = pointA;		faces_table[0].uv[0] = pointA1UV;
			faces_table[0].vertex[1] = pointB;		faces_table[0].uv[1] = pointBUV;
			faces_table[0].vertex[2] = pointC;		faces_table[0].uv[2] = pointC1UV;

			faces_table[1].vertex[0] = pointC;		faces_table[1].uv[0] = pointC1UV;
			faces_table[1].vertex[1] = pointD;		faces_table[1].uv[1] = pointDUV;
			faces_table[1].vertex[2] = pointB;		faces_table[1].uv[2] = pointBUV;
			
			faces_table[2].vertex[0] = pointB;		faces_table[2].uv[0] = pointBUV;
			faces_table[2].vertex[1] = pointA;		faces_table[2].uv[1] = pointA2UV;
			faces_table[2].vertex[2] = pointD;		faces_table[2].uv[2] = pointDUV;

			faces_table[3].vertex[0] = pointD;		faces_table[3].uv[0] = pointDUV;
			faces_table[3].vertex[1] = pointC;		faces_table[3].uv[1] = pointC2UV;
			faces_table[3].vertex[2] = pointA;		faces_table[3].uv[2] = pointA2UV;

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
			glm::vec3 edge = faces_table[face].vertex[2]  - faces_table[face].vertex[0];
			edge *= 1/  float( pow( 2, tesselletion ) );
			return edge;
		}

		glm::vec3 getRightLeftVector( int face )
		{
			glm::vec3 edge = faces_table[face].vertex[1] - faces_table[face].vertex[2];
			edge *= 1/  float( pow( 2, tesselletion ) );
			return edge;
		}

		glm::vec3 computeLength( glm::vec3 vector )
		{
			return glm::normalize( vector ) * radius;
			//return vector;
		}


		glm::vec2 computeUV( int face, glm::vec3 vertex )
		{
			glm::mat3 vert_matrix( faces_table[face].vertex[0], faces_table[face].vertex[1], faces_table[face].vertex[2] );
			glm::vec3 barycentric_coords;

			vert_matrix = glm::inverse( vert_matrix );
			barycentric_coords = vert_matrix * vertex;

			glm::mat3x2 UVs( faces_table[face].uv[0], faces_table[face].uv[1], faces_table[face].uv[2] );
			glm::vec2 UV = UVs * barycentric_coords;
			UV = glm::clamp( UV, glm::vec2( 0.005, 0.005 ), glm::vec2( 0.995, 0.995 ) );

			return UV;
		}


#define FACES 4
		void GenerateGeometryNormalsUVs( Float3AttributeChannelPtr verts, Float3AttributeChannelPtr normals, Float2AttributeChannelPtr uvs ) override
        {
			initFacesTable();	// Needed for functions get[...]Vector

			glm::vec3 top_down = getTopDownVector( 0 );
			glm::vec3 right_left;

			glm::vec3 current_vertex = faces_table[0].vertex[0];

			current_vertex += float( tile_num ) * top_down;
			verts->AddAttribute( computeLength( current_vertex ) );
			uvs->AddAttribute( computeUV( 0, current_vertex ) );

			current_vertex += top_down;
			verts->AddAttribute( computeLength( current_vertex ) );
			uvs->AddAttribute( computeUV( 0, current_vertex ) );

			for( int face = 0; face < FACES; ++face )
			{// We have 4 faces
				top_down = getTopDownVector( face );
				right_left = getRightLeftVector( face );
				
				int max_loop = innerLoopMax( face );
				for( int j = 0; j < max_loop; ++j )
				{// We have already two first verticies.
					// Step left
					current_vertex += right_left;
					verts->AddAttribute( computeLength( current_vertex ) );
					uvs->AddAttribute( computeUV( face, current_vertex ) );
					// Step down
					current_vertex += top_down;
					verts->AddAttribute( computeLength( current_vertex ) );
					uvs->AddAttribute( computeUV( face, current_vertex ) );
				}

				current_vertex += right_left;
				verts->AddAttribute( computeLength( current_vertex ) );
				uvs->AddAttribute( computeUV( face, current_vertex ) );
			}

            GeometryGeneratorHelper::GenerateNonWeightedNormalsFromTriangleStrips( verts, normals );
		}

	};
}


Plugin::Plugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model )
		: DefaultGeometryPluginBase( name, uid, prev, model )
{
	tesselletion = QueryTypedValue< ValueIntPtr >( GetValue( PN::TESSELATION ) );
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
        gens.push_back( std::make_shared< Generator::GeosphereGenerator >( i ) );
	}
    return gens;
}

bool                                Plugin::NeedsTopologyUpdate()
{
	return ParameterChanged( PN::TESSELATION )
		|| ParameterChanged( PN::RADIUS );
}


}}}
