#include "DefaultCogWheelPlugin.h"
#include "Mathematics/Defines.h"
#include "..\HelperIndexedGeometryConverter.h"
#include "..\HelperSmoothMesh.h"

namespace bv { namespace model { namespace DefaultCogWheel {


const std::string INNER_RADIUS = "inner radius";
const std::string OUTER_RADIUS = "outer radius";
const std::string TOOTH_HEIGHT = "tooth height";
const std::string TOOTH_BASE_LENGTH = "tooth base length";
const std::string TOOTH_TOP_LENGTH = "tooth top length";
const std::string TEETH_NUMBER = "teeth number";
const std::string BEVEL = "bevel";
const std::string TESSELATION = "tesselation";
const std::string HEIGHT = "height";


PluginDesc::PluginDesc()
    : DefaultGeometryPluginDescBase( UID(), "cog wheel" )
{
}


IPluginPtr                      PluginDesc::CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< Plugin >( name, prev, timeEvaluator );
}

std::string PluginDesc::UID()
{
    return "DEFAULT_COGWHEEL";
}


DefaultPluginParamValModelPtr   PluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper h( timeEvaluator );

    h.CreateVacModel();
	h.AddSimpleParam( PN::INNER_RADIUS, 0.3f, true, true );
	h.AddSimpleParam( PN::OUTER_RADIUS, 1.0f, true, true );
	h.AddSimpleParam( PN::TOOTH_HEIGHT, 0.2f, true, true );
	h.AddSimpleParam( PN::TOOTH_BASE_LENGTH, 0.1f, true, true );
	h.AddSimpleParam( PN::TOOTH_TOP_LENGTH, 0.08f, true, true );
	h.AddSimpleParam( PN::TEETH_NUMBER, 10, true, true );
	h.AddSimpleParam( PN::TESSELATION, 0, true, true );
	h.AddSimpleParam( PN::BEVEL, 0.1f, true, true );
	h.AddSimpleParam( PN::HEIGHT, 0.5f, true, true );


    return h.GetModel();
}


namespace Generator
{
	float		height;
	float		innerRadius;
	float		outerRadius;
	float		toothHeight;
	float		toothBaseLength;
	float		toothTopLength;
	int			teethNumber;
	float		bevel;
	int			tesselation;

	const unsigned int verticiesPerRing = 16;

	/**@brief Generates CogWheel*/
	class CogWheelGenerator : public IGeometryAndUVsGenerator
	{
	private:
	public:

		CogWheelGenerator() {}
		~CogWheelGenerator(){}

		Type GetType() { return GEOMETRY_AND_UVS; }
		//GEOMETRY_AND_UVS
		//GEOMETRY_ONLY

		void generateRing( std::vector<glm::vec3>& verticies, double angle, double topToothAngle )
		{
			glm::vec3 newVertex;
			verticies.reserve( verticiesPerRing );
			
			// Part without teeths.
			double sinAngle = sin( angle );
			double cosAngle = cos( angle );

			newVertex = glm::vec3( cosAngle * innerRadius, height - bevel, sinAngle * innerRadius );
			verticies.push_back( newVertex );
			newVertex = glm::vec3( cosAngle * innerRadius, height, sinAngle * innerRadius );
			verticies.push_back( newVertex );
			newVertex = glm::vec3( cosAngle * ( innerRadius + bevel ), height, sinAngle * ( innerRadius + bevel ) );
			verticies.push_back( newVertex );
			newVertex = glm::vec3( cosAngle * ( outerRadius - bevel ), height, sinAngle * ( outerRadius - bevel ) );
			verticies.push_back( newVertex );
			newVertex = glm::vec3( cosAngle * ( outerRadius ), height, sinAngle * ( outerRadius ) );
			verticies.push_back( newVertex );
			newVertex = glm::vec3( cosAngle * ( outerRadius ), height - bevel, sinAngle * ( outerRadius ) );
			verticies.push_back( newVertex );
			newVertex = glm::vec3( cosAngle * ( outerRadius ), -height + bevel, sinAngle * ( outerRadius ) );
			verticies.push_back( newVertex );
			newVertex = glm::vec3( cosAngle * ( outerRadius ), -height, sinAngle * ( outerRadius ) );
			verticies.push_back( newVertex );
			newVertex = glm::vec3( cosAngle * ( outerRadius - bevel ), -height, sinAngle * ( outerRadius - bevel ) );
			verticies.push_back( newVertex );
			newVertex = glm::vec3( cosAngle * ( innerRadius + bevel ), -height, sinAngle * ( innerRadius + bevel ) );
			verticies.push_back( newVertex );
			newVertex = glm::vec3( cosAngle * innerRadius, -height, sinAngle * innerRadius );
			verticies.push_back( newVertex );
			newVertex = glm::vec3( cosAngle * innerRadius, -height + bevel, sinAngle * innerRadius );
			verticies.push_back( newVertex );

			//Tooth
			sinAngle = sin( topToothAngle );
			cosAngle = cos( topToothAngle );
			
			newVertex = glm::vec3( cosAngle * ( outerRadius + toothHeight ), height, sinAngle * ( outerRadius + toothHeight ) );
			verticies.push_back( newVertex );
			newVertex = glm::vec3( cosAngle * ( outerRadius + toothHeight ), height - bevel, sinAngle * ( outerRadius + toothHeight ) );
			verticies.push_back( newVertex );
			newVertex = glm::vec3( cosAngle * ( outerRadius + toothHeight ), -height + bevel, sinAngle * ( outerRadius + toothHeight ) );
			verticies.push_back( newVertex );
			newVertex = glm::vec3( cosAngle * ( outerRadius + toothHeight ), -height, sinAngle * ( outerRadius + toothHeight ) );
			verticies.push_back( newVertex );
		}

		void connectVerticiesIntoTriangles( std::vector<INDEX_TYPE>& indicies, std::vector<glm::vec3>& verticies )
		{
			indicies;
			verticies;
			for( int i = 0; i < 2 * teethNumber; ++i )
			{
				if( i & 0x01 ) // Checks parity
				{

				}
				else
				{
					// We can make whole ring because there's no tooth.
					unsigned int index = i * verticiesPerRing;
					for( unsigned int j = index; j < index + verticiesPerRing - 2; ++j )
					{
						indicies.push_back( static_cast<INDEX_TYPE>( j ) );
						indicies.push_back( static_cast<INDEX_TYPE>( j + 1 ) );
						indicies.push_back( static_cast<INDEX_TYPE>( j + 2 ) );
					}
					// Close the ring.

					indicies.push_back( static_cast<INDEX_TYPE>( index + verticiesPerRing - 2 ) );
					indicies.push_back( static_cast<INDEX_TYPE>( index + verticiesPerRing - 1 ) );
					indicies.push_back( static_cast<INDEX_TYPE>( index ) );

					indicies.push_back( static_cast<INDEX_TYPE>( index + verticiesPerRing - 1 ) );
					indicies.push_back( static_cast<INDEX_TYPE>( index ) );
					indicies.push_back( static_cast<INDEX_TYPE>( index + 1 ) );
				}
			}
		}


		void GenerateGeometryAndUVs( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs ) override
        {
			HelperSmoothMesh smoother;
			IndexedGeometryConverter converter;
			IndexedGeometry cogWheel;
			std::vector<glm::vec3>& verticies = cogWheel.getVerticies();
			std::vector<INDEX_TYPE>& indicies = cogWheel.getIndicies();

			double anglePerTooth = toothBaseLength / outerRadius;
			double angleBetweenTeeth = ( TWOPI * outerRadius - toothBaseLength * teethNumber ) / ( teethNumber * outerRadius );
			double anglePerTopTooth = toothTopLength / ( outerRadius + toothHeight );

			for( int i = 0; i < teethNumber; ++i )
			{
				double angle1 = ( anglePerTooth + angleBetweenTeeth ) * (float)i;
				double angle2 = angle1 + anglePerTooth;
				double toothCenterAngle = ( angle1 + angle2 ) / 2;

				generateRing( verticies, angle1, toothCenterAngle + anglePerTopTooth / 2 );
				generateRing( verticies, angle2, toothCenterAngle - anglePerTopTooth / 2 );
			}

			connectVerticiesIntoTriangles( indicies, verticies );

			//Smooth cog wheel
			std::vector<INDEX_TYPE> sharpEdges;
			
			IndexedGeometry resultMesh = smoother.smooth( cogWheel, sharpEdges, tesselation );
			converter.makeStrip( resultMesh, verts );

			for( SizeType v = 0; v < verts->GetNumEntries(); v++ )
			{
				glm::vec3 vert = verts->GetVertices()[ v ];
				//vert -= center_translate;
				uvs->AddAttribute( glm::vec2( vert.x*0.5 + 0.5,
												vert.y*0.5 + 0.5 ) ); // FIXME: scaling
			}
		}
	};
}



Plugin::Plugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model )
	: DefaultGeometryPluginBase( name, uid, prev, model )
{
	innerRadius = QueryTypedValue< ValueFloatPtr >( GetValue( PN::INNER_RADIUS ) );
	outerRadius = QueryTypedValue< ValueFloatPtr >( GetValue( PN::OUTER_RADIUS ) );
	toothHeight = QueryTypedValue< ValueFloatPtr >( GetValue( PN::TOOTH_HEIGHT ) );
	toothBaseLength = QueryTypedValue< ValueFloatPtr >( GetValue( PN::TOOTH_BASE_LENGTH ) );
	toothTopLength = QueryTypedValue< ValueFloatPtr >( GetValue( PN::TOOTH_TOP_LENGTH ) );
	bevel = QueryTypedValue< ValueFloatPtr >( GetValue( PN::BEVEL ) );
	tesselation = QueryTypedValue< ValueIntPtr >( GetValue( PN::TESSELATION ) );
	teethNumber = QueryTypedValue< ValueIntPtr >( GetValue( PN::TEETH_NUMBER ) );
	height = QueryTypedValue< ValueFloatPtr >( GetValue( PN::HEIGHT ) );


    m_pluginParamValModel->Update();
    InitGeometry();
}


Plugin::~Plugin(void)
{
}

std::vector<IGeometryGeneratorPtr>    Plugin::GetGenerators()
{
	Generator::height = height->GetValue();
	Generator::bevel = bevel->GetValue();
	Generator::innerRadius = innerRadius->GetValue();
	Generator::outerRadius = outerRadius->GetValue();
	Generator::teethNumber = teethNumber->GetValue();
	Generator::tesselation = tesselation->GetValue();
	Generator::toothBaseLength = toothBaseLength->GetValue();
	Generator::toothHeight = toothHeight->GetValue();
	Generator::toothTopLength = toothHeight->GetValue();


    std::vector<IGeometryGeneratorPtr> gens;
    gens.push_back( IGeometryGeneratorPtr( new Generator::CogWheelGenerator() ) );
    return gens;
}

bool                                Plugin::NeedsTopologyUpdate()
{
	return ParameterChanged( PN::OUTER_RADIUS )
		|| ParameterChanged( PN::INNER_RADIUS )
		|| ParameterChanged( PN::BEVEL )
		|| ParameterChanged( PN::TEETH_NUMBER )
		|| ParameterChanged( PN::TESSELATION )
		|| ParameterChanged( PN::TOOTH_BASE_LENGTH )
		|| ParameterChanged( PN::TOOTH_HEIGHT )
		|| ParameterChanged( PN::TOOTH_TOP_LENGTH )
		|| ParameterChanged( PN::HEIGHT );
}


}}}
