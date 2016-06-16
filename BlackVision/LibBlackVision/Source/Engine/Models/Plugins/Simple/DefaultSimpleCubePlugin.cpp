#include "stdafx.h"

#include "DefaultSimpleCubePlugin.h"
#include "Mathematics/Defines.h"
#include "..\HelperIndexedGeometryConverter.h"
#include "..\HelperSmoothMesh.h"

namespace bv { namespace model { namespace DefaultSimpleCube {


const std::string PN::DIMENSIONS = "dimensions";

PluginDesc::PluginDesc()
    : DefaultGeometryPluginDescBase( UID(), "simple cube" )
{
}


IPluginPtr                      PluginDesc::CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< Plugin >( name, prev, timeEvaluator );
}

std::string PluginDesc::UID()
{
    return "DEFAULT_SIMPLE_CUBE";
}


DefaultPluginParamValModelPtr   PluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper h( timeEvaluator );

    h.CreateVacModel();
    h.AddSimpleParam( PN::DIMENSIONS, glm::vec3( 1, 1, 1 ), true, true );

    h.CreatePSModel();

    return h.GetModel();
}


namespace Generator
{
	glm::vec3		dimmension;

	const unsigned int NORMAL_CUBE = 0;
	const unsigned int SMOOTH_CUBE = 0;

	/**@brief Generates cube built as strips.*/
	class SimpleCubeGenerator : public IGeometryNormalsUVsGenerator
	{
	private:

		unsigned int cube_type;

	public:

		SimpleCubeGenerator() { cube_type = SMOOTH_CUBE;}
		~SimpleCubeGenerator(){}

		void GenerateGeometryNormalsUVs( Float3AttributeChannelPtr verts, Float3AttributeChannelPtr normals, Float2AttributeChannelPtr uvs ) override
        {
			float x = dimmension.x / 2;
			float y = dimmension.y / 2;
			float z = dimmension.z / 2;

			if( cube_type == NORMAL_CUBE )
			{
				// Generates top strip of cube
				verts->AddAttribute( glm::vec3( -x, -y, z ) );
				verts->AddAttribute( glm::vec3( x, -y, z ) );
				verts->AddAttribute( glm::vec3( -x , y, z ) );
				verts->AddAttribute( glm::vec3( x, y, z ) );

				verts->AddAttribute( glm::vec3( x, y, z ) );
				verts->AddAttribute( glm::vec3( -x , y, z ) );
				
                verts->AddAttribute( glm::vec3( -x , y, z ) );
                verts->AddAttribute( glm::vec3( x, y, z ) );
				verts->AddAttribute( glm::vec3( -x, y, -z ) );
				verts->AddAttribute( glm::vec3( x, y, -z ) );
                
				verts->AddAttribute( glm::vec3( x, y, -z ) );
				verts->AddAttribute( glm::vec3( -x, y, -z ) );

				verts->AddAttribute( glm::vec3( -x, y, -z ) );
				verts->AddAttribute( glm::vec3( x, y, -z ) );
				verts->AddAttribute( glm::vec3( -x, -y, -z ) );
				verts->AddAttribute( glm::vec3( x, -y, -z ) );

				verts->AddAttribute( glm::vec3( x, -y, -z ) );
				verts->AddAttribute( glm::vec3( -x, y, z ) );
				
                // Generates bottom strip of cube
				verts->AddAttribute( glm::vec3( -x, y, z ) );
				verts->AddAttribute( glm::vec3( -x, y, -z ) );
				verts->AddAttribute( glm::vec3( -x, -y, z ) );
				verts->AddAttribute( glm::vec3( -x, -y, -z ) );
				
				verts->AddAttribute( glm::vec3( -x, -y, -z ) );
				verts->AddAttribute( glm::vec3( -x, -y, z ) );
                
				verts->AddAttribute( glm::vec3( -x, -y, z ) );
				verts->AddAttribute( glm::vec3( -x, -y, -z ) );
                verts->AddAttribute( glm::vec3( x, -y, z ) );
				verts->AddAttribute( glm::vec3( x, -y, -z ) );
                
				verts->AddAttribute( glm::vec3( x, -y, -z ) );
                verts->AddAttribute( glm::vec3( x, -y, z ) );

                verts->AddAttribute( glm::vec3( x, -y, z ) );
				verts->AddAttribute( glm::vec3( x, -y, -z ) );
				verts->AddAttribute( glm::vec3( x, y, z ) );
				verts->AddAttribute( glm::vec3( x, y, -z ) );

				verts->AddAttribute( glm::vec3( x, y, -z ) );
				verts->AddAttribute( glm::vec3( x, y, -z ) );
                

				// Generates UVs coordinates for top strip
                float u = 0.0f;
				for( int i = 0; i < 3; ++i )
				{
                    uvs->AddAttribute( glm::vec2( u, 0.0 ) );
                    uvs->AddAttribute( glm::vec2( u, 0.5 ) );

                    u += 1.0f/3.0f;
                    uvs->AddAttribute( glm::vec2( u, 0.0 ) );
                    uvs->AddAttribute( glm::vec2( u, 0.5 ) );

                    uvs->AddAttribute( glm::vec2( 0.0, 0.0 ) );
                    uvs->AddAttribute( glm::vec2( 0.0, 0.0 ) );
				}
                
				// Generates UVs coordinates for bottom strip
				u = 0.0;
				for( int i = 0; i < 3; ++i )
				{
                    uvs->AddAttribute( glm::vec2( u, 0.5 ) );
                    uvs->AddAttribute( glm::vec2( u, 1.0 ) );

                    u += 1.0f/3.0f;
                    uvs->AddAttribute( glm::vec2( u, 0.5 ) );
                    uvs->AddAttribute( glm::vec2( u, 1.0 ) );

                    uvs->AddAttribute( glm::vec2( 0.0, 0.0 ) );
                    uvs->AddAttribute( glm::vec2( 0.0, 0.0 ) );
				}

			}
			else if( cube_type == SMOOTH_CUBE )
			{
				HelperSmoothMesh smoother;
				IndexedGeometryConverter converter;
				IndexedGeometry cube;

				std::vector<glm::vec3>& verticies = cube.GetVerticies();
				std::vector<INDEX_TYPE>& indicies = cube.GetIndicies();

				verticies.reserve( 8 );
				indicies.reserve( 36 );

				//verticies.push_back( glm::vec3( -x, -y, z ) );
				//verticies.push_back( glm::vec3( x, -y, z ) );
				//verticies.push_back( glm::vec3( x, y, z ) );
				//verticies.push_back( glm::vec3( -x, y, z ) );
				//verticies.push_back( glm::vec3( -x, -y, -z ) );
				//verticies.push_back( glm::vec3( x, -y, -z ) );
				//verticies.push_back( glm::vec3( x, y, -z ) );
				//verticies.push_back( glm::vec3( -x, y, -z ) );

				verticies.push_back( glm::vec3( -x, y, -z ) );
				verticies.push_back( glm::vec3( x, y, -z ) );
				verticies.push_back( glm::vec3( -x, -y, -z ) );
				verticies.push_back( glm::vec3( x, -y, -z ) );
				verticies.push_back( glm::vec3( -x, y, z ) );
				verticies.push_back( glm::vec3( x, y, z ) );
				verticies.push_back( glm::vec3( -x, -y, z ) );
				verticies.push_back( glm::vec3( x, -y, z ) );

				verticies.push_back( glm::vec3( -x / 2, y / 2, -z ) );
				verticies.push_back( glm::vec3( x / 2, y / 2, -z ) );
				verticies.push_back( glm::vec3( -x / 2, -y / 2, -z ) );
				verticies.push_back( glm::vec3( x / 2, -y / 2, -z ) );
				verticies.push_back( glm::vec3( -x / 2, y / 2, z ) );
				verticies.push_back( glm::vec3( x / 2, y / 2, z ) );
				verticies.push_back( glm::vec3( -x / 2, -y / 2, z) );
				verticies.push_back( glm::vec3( x / 2, -y / 2, z) );

				//INDEX_TYPE indicesData[] = { 
				//	0, 1, 2, 2, 3, 0, 
				//	3, 2, 6, 6, 7, 3, 
				//	7, 6, 5, 5, 4, 7, 
				//	4, 0, 3, 3, 7, 4, 
				//	0, 1, 5, 5, 4, 0,
				//	1, 5, 6, 6, 2, 1 
				//};

				// normal cube
				INDEX_TYPE indicesData[] =
				{
					0, 1, 2,    // side 1
					2, 1, 3,
					4, 0, 6,    // side 2
					6, 0, 2,
					7, 5, 6,    // side 3
					6, 5, 4,
					3, 1, 7,    // side 4
					7, 1, 5,
					4, 5, 0,    // side 5
					0, 5, 1,
					3, 7, 2,    // side 6
					2, 7, 6,
				};


#define in( x ) (x + 8)

				//Cube with hole
				//INDEX_TYPE indicesData[] =
				//{
				//	4, 0, 6,    // side 2
				//	6, 0, 2,
				//	3, 1, 7,    // side 4
				//	7, 1, 5,
				//	4, 5, 0,    // side 5
				//	0, 5, 1,
				//	3, 7, 2,    // side 6
				//	2, 7, 6,

				//	in(4), in(0), in(6),    // side 2
				//	in(6), in(0), in(2),
				//	in(3), in(1), in(7),    // side 4
				//	in(7), in(1), in(5),
				//	in(4), in(5), in(0),    // side 5
				//	in(0), in(5), in(1),
				//	in(3), in(7), in(2),    // side 6
				//	in(2), in(7), in(6),

				//	0, in(0), 1,
				//	in(0), 1, in(1),
				//	1, in(1), 3,
				//	in(1), 3, in(3),
				//	3, in(3), 2,
				//	in(3), 2, in(2),
				//	2, in(2), 0,
				//	in(2), 0, in(0),

				//	4, in(4), 5,
				//	in(4), 5, in(5),
				//	5, in(5), 7,
				//	in(5), 7, in(7),
				//	7, in(7), 6,
				//	in(7), 6, in(6),
				//	6, in(6), 4,
				//	in(6), 4, in(4),
				//};
#undef in
				

				for( auto i : indicesData )
					indicies.push_back( i );

				std::vector<INDEX_TYPE> sharpEdges;

				//sharpEdges.push_back( 0 );
				//sharpEdges.push_back( 1 );
				//sharpEdges.push_back( 1 );
				//sharpEdges.push_back( 2 );
				//sharpEdges.push_back( 2 );
				//sharpEdges.push_back( 3 );
				//sharpEdges.push_back( 3 );
				//sharpEdges.push_back( 0 );

				sharpEdges.push_back( 3 );
				sharpEdges.push_back( 1 );
				sharpEdges.push_back( 3 );
				sharpEdges.push_back( 2 );
				sharpEdges.push_back( 3 );
				sharpEdges.push_back( 7 );
				sharpEdges.push_back( 1 );
				sharpEdges.push_back( 2 );
				sharpEdges.push_back( 2 );
				sharpEdges.push_back( 7 );
				sharpEdges.push_back( 7 );
				sharpEdges.push_back( 1 );

				const unsigned tesselation = 3;

				IndexedGeometry resultMesh = smoother.smooth( cube, sharpEdges, tesselation );
				converter.MakeStrip( resultMesh, verts );

				for( SizeType v = 0; v < verts->GetNumEntries(); v++ )
				{
					glm::vec3 vert = verts->GetVertices()[ v ];
					uvs->AddAttribute( glm::vec2( 0.5*( vert.x + vert.y + 1.f ),
													vert.z + 0.5 ) ); // FIXME: scaling
				}
			}

            //GeometryGeneratorHelper::GenerateNonWeightedNormalsFromTriangleStrips( verts, normals );

            glm::vec3 normalsVec[] = {
                glm::vec3( 0, 0, 1 ),
                glm::vec3( 0, 1, 0 ),
                glm::vec3( 0, 0, -1 ),
                glm::vec3( -1, 0, 0 ),
                glm::vec3( 0, -1, 0 ),
                glm::vec3( 1, 0, 0 ),
            };

            for( auto norm : normalsVec )
            {
                normals->AddAttribute( norm );
                normals->AddAttribute( norm );
                normals->AddAttribute( norm );
                normals->AddAttribute( norm );

                normals->AddAttribute( glm::vec3( 0, 0, 0 ) );
                normals->AddAttribute( glm::vec3( 0, 0, 0 ) );
            }
		}

		void GenerateGeometryNormals ( Float3AttributeChannelPtr verts, Float3AttributeChannelPtr normals ) 
		{
			//assert( true );

			float x = dimmension.x / 2;
			float y = dimmension.y / 2;
			float z = dimmension.z / 2;

			HelperSmoothMesh smoother;
			IndexedGeometryConverter converter;
			IndexedGeometry cube;

			auto verticies = cube.GetVerticies();
			auto indicies = cube.GetIndicies();

			verticies.reserve( 8 );
			indicies.reserve( 36 );

			verticies.push_back( glm::vec3( -x, -y, z ) );
			verticies.push_back( glm::vec3( x, -y, z ) );
			verticies.push_back( glm::vec3( x, y, z ) );
			verticies.push_back( glm::vec3( -x, y, z ) );
			verticies.push_back( glm::vec3( -x, y, -z ) );
			verticies.push_back( glm::vec3( x, -y, -z ) );
			verticies.push_back( glm::vec3( x, y, -z ) );
			verticies.push_back( glm::vec3( -x, y, -z ) );


			unsigned short indicesData[] = { 
				0, 1, 2, 2, 3, 0, 
				3, 2, 6, 6, 7, 3, 
				7, 6, 5, 5, 4, 7, 
				4, 0, 3, 3, 7, 4, 
				0, 1, 5, 5, 4, 0,
				1, 5, 6, 6, 2, 1 
			};

			for( int i = 0; i < 36; ++i )
				indicies.push_back( indicesData[i] );

			std::vector<INDEX_TYPE> sharpEdges;
			const unsigned tesselation = 1;

			IndexedGeometry resultMesh = smoother.smooth( cube, sharpEdges, tesselation );
			converter.MakeStrip( resultMesh, verts );

            //GeometryGeneratorHelper::GenerateNonWeightedNormalsFromTriangleStrips( verts, normals );
		}
	};
}




Plugin::Plugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model )
	: DefaultGeometryPluginBase( name, uid, prev, model )
{
    dimensions = QueryTypedValue< ValueVec3Ptr >( GetValue( PN::DIMENSIONS ) );

    m_pluginParamValModel->Update();
    InitGeometry();
}


Plugin::~Plugin(void)
{
}

std::vector<IGeometryGeneratorPtr>    Plugin::GetGenerators()
{
    Generator::dimmension = dimensions->GetValue();

    std::vector<IGeometryGeneratorPtr> gens;
    gens.push_back( std::make_shared< Generator::SimpleCubeGenerator >() );
    return gens;
}

bool                                Plugin::NeedsTopologyUpdate()
{
    return ParameterChanged( PN::DIMENSIONS );
}


}}}
