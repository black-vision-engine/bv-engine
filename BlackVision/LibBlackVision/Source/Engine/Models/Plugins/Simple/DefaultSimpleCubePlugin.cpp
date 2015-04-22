#include "DefaultSimpleCubePlugin.h"
#include "Mathematics/Defines.h"

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

    return h.GetModel();
}


namespace Generator
{
	glm::vec3		dimmension;

	/**@brief Generates cube built as strips.*/
	class SimpleCubeGenerator : public IGeometryAndUVsGenerator
	{
	private:
	public:

		SimpleCubeGenerator() {}
		~SimpleCubeGenerator(){}

		Type GetType() { return Type::GEOMETRY_AND_UVS; }

		void GenerateGeometryAndUVs( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs ) override
        {
			float x = dimmension.x / 2;
			float y = dimmension.y / 2;
			float z = dimmension.z / 2;


			// Generates top strip of cube
			verts->AddAttribute( glm::vec3( -x, -y, z ) );
			verts->AddAttribute( glm::vec3( x, -y, z ) );
			verts->AddAttribute( glm::vec3( -x , y, z ) );
			verts->AddAttribute( glm::vec3( x, y, z ) );
			verts->AddAttribute( glm::vec3( -x, y, -z ) );
			verts->AddAttribute( glm::vec3( x, y, -z ) );
			verts->AddAttribute( glm::vec3( -x, -y, -z ) );
			verts->AddAttribute( glm::vec3( x, -y, -z ) );

			// Generates bottom strip of cube
			verts->AddAttribute( glm::vec3( -x, y, z ) );
			verts->AddAttribute( glm::vec3( -x, y, -z ) );
			verts->AddAttribute( glm::vec3( -x, -y, z ) );
			verts->AddAttribute( glm::vec3( -x, -y, -z ) );
			verts->AddAttribute( glm::vec3( x, -y, z ) );
			verts->AddAttribute( glm::vec3( x, -y, -z ) );
			verts->AddAttribute( glm::vec3( x, y, z ) );
			verts->AddAttribute( glm::vec3( x, y, -z ) );

			// Generates UVs coordinates for top strip
			float u = 0.0f;
			for( int i = 0; i < 4; ++i )
			{
				for( int j = 0; j < 2; ++j )
				{
					if( j )
						uvs->AddAttribute( glm::vec2( u, 0.0 ) );
					else
						uvs->AddAttribute( glm::vec2( u, 0.5 ) );
				}
				u += 1.0f/3.0f;
			}

			// Generates UVs coordinates for bottom strip
			u = 0.0;
			for( int i = 0; i < 4; ++i )
			{
				for( int j = 0; j < 2; ++j )
				{
					if( j )
						uvs->AddAttribute( glm::vec2( u, 0.5 ) );
					else
						uvs->AddAttribute( glm::vec2( u, 1.0 ) );
				}
				u += 1.0f/3.0f;
			}
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
    gens.push_back( IGeometryGeneratorPtr( new Generator::SimpleCubeGenerator() ) );
    return gens;
}

bool                                Plugin::NeedsTopologyUpdate()
{
    return ParameterChanged( PN::DIMENSIONS );
}


}}}
