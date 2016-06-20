#include "stdafx.h"

#include "DefaultCogWheelPlugin.h"
#include "Mathematics/Defines.h"
#include "Engine/Models/Plugins/HelperIndexedGeometryConverter.h"
#include "Engine/Models/Plugins/HelperSmoothMesh.h"

namespace bv { namespace model {


typedef ParamEnum< DefaultCogWheel::Plugin::WeightCenter > ParamEnumWC;

VoidPtr    ParamEnumWC::QueryParamTyped  ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}

template<>
static IParameterPtr        ParametersFactory::CreateTypedParameter< DefaultCogWheel::Plugin::WeightCenter >                 ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return CreateParameterEnum< DefaultCogWheel::Plugin::WeightCenter >( name, timeline );
}
	
#include "Engine/Models/Plugins/ParamValModel/SimpleParamValEvaluator.inl"

	
namespace DefaultCogWheel {


const std::string PN::INNER_RADIUS = "inner radius";
const std::string PN::OUTER_RADIUS = "outer radius";
const std::string PN::TOOTH_HEIGHT = "tooth height";
const std::string PN::TOOTH_BASE_LENGTH = "tooth base length";
const std::string PN::TOOTH_TOP_LENGTH = "tooth top length";
const std::string PN::TEETH_NUMBER = "teeth number";
const std::string PN::BEVEL = "bevel";
const std::string PN::TESSELATION = "tesselation";
const std::string PN::HEIGHT = "height";
const std::string PN::WEIGHTCENTERX = "weight center x";
const std::string PN::WEIGHTCENTERY = "weight center y";
const std::string PN::WEIGHTCENTERZ = "weight center z";


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
	h.AddSimpleParam( PN::TOOTH_BASE_LENGTH, 0.3f, true, true );
	h.AddSimpleParam( PN::TOOTH_TOP_LENGTH, 0.08f, true, true );
	h.AddSimpleParam( PN::TEETH_NUMBER, 10, true, true );
	h.AddSimpleParam( PN::TESSELATION, 0, true, true );
	h.AddSimpleParam( PN::BEVEL, 0.3f, true, true );
	h.AddSimpleParam( PN::HEIGHT, 0.5f, true, true );
	h.AddParam< IntInterpolator, Plugin::WeightCenter, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumWC >
        ( DefaultCogWheel::PN::WEIGHTCENTERX, Plugin::WeightCenter::CENTER, true, true );
	h.AddParam< IntInterpolator, Plugin::WeightCenter, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumWC >
        ( DefaultCogWheel::PN::WEIGHTCENTERY, Plugin::WeightCenter::MAX, true, true );
	h.AddParam< IntInterpolator, Plugin::WeightCenter, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumWC >
        ( DefaultCogWheel::PN::WEIGHTCENTERZ, Plugin::WeightCenter::CENTER, true, true );

    h.CreatePSModel();

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
	Plugin::WeightCenter weight_centerX;
	Plugin::WeightCenter weight_centerY;
	Plugin::WeightCenter weight_centerZ;

	const unsigned int verticiesPerRing = 12;
	const unsigned int verticiesPerRingTooth = 16;
	unsigned int allVerticies;

	/**@brief Generates CogWheel*/
	class CogWheelGenerator : public IGeometryNormalsUVsGenerator
	{
	private:

		glm::vec3 center_translate;

	public:

		CogWheelGenerator() {}
		~CogWheelGenerator(){}

		void computeWeightCenter( Plugin::WeightCenter centerX, Plugin::WeightCenter centerY, Plugin::WeightCenter centerZ )
		{
			center_translate = glm::vec3( 0.0f, 0.0f, 0.0f );

			if( centerX == Plugin::WeightCenter::MAX )
				center_translate += glm::vec3( -outerRadius, 0.0, 0.0 );
			else if( centerX == Plugin::WeightCenter::CENTER )
				center_translate += glm::vec3( 0.0, 0.0, 0.0 );
			else if( centerX == Plugin::WeightCenter::MIN )
				center_translate += glm::vec3( outerRadius, 0.0, 0.0 );
	
			if( centerY == Plugin::WeightCenter::MAX )
				center_translate += glm::vec3( 0.0f, -height / 2, 0.0f );
			else if( centerY == Plugin::WeightCenter::CENTER )
				center_translate += glm::vec3( 0.0f, 0.0f, 0.0f );
			else if( centerY == Plugin::WeightCenter::MIN )
				center_translate += glm::vec3( 0.0f, height / 2, 0.0f );

			if( centerZ == Plugin::WeightCenter::MAX )
				center_translate += glm::vec3( 0.0, 0.0, -outerRadius );
			else if( centerZ == Plugin::WeightCenter::CENTER )
				center_translate += glm::vec3( 0.0, 0.0, 0.0 );
			else if( centerZ == Plugin::WeightCenter::MIN )
				center_translate += glm::vec3( 0.0, 0.0, outerRadius );
			
		}

		void ConnectVerticiesBetween( unsigned int begin, unsigned int offset, std::vector<INDEX_TYPE>& indicies )
		{
			for( unsigned int j = begin; j < begin + offset - 1; ++j )
			{
				indicies.push_back( static_cast<INDEX_TYPE>( j ) );
				indicies.push_back( static_cast<INDEX_TYPE>( ( j + verticiesPerRingTooth ) % allVerticies ) );
				indicies.push_back( static_cast<INDEX_TYPE>( j + 1 ) );

				indicies.push_back( static_cast<INDEX_TYPE>( ( j + verticiesPerRingTooth ) % allVerticies ) );
				indicies.push_back( static_cast<INDEX_TYPE>( ( j + verticiesPerRingTooth + 1 ) % allVerticies ) );
                indicies.push_back( static_cast<INDEX_TYPE>( j + 1 ) );
			}
		}

		void ConnectSideToothVerticies( unsigned int begin1, unsigned int begin2, unsigned int offset, std::vector<INDEX_TYPE>& indicies, bool inverseOrder )
		{
			for( unsigned int j = 0; j < offset - 1; ++j )
			{
                if( inverseOrder )
                {
			        indicies.push_back( static_cast<INDEX_TYPE>( begin1 + j ) );
                    indicies.push_back( static_cast<INDEX_TYPE>( ( begin2 + j ) ) % allVerticies );
                    indicies.push_back( static_cast<INDEX_TYPE>( begin1 + j + 1 ) );

			        indicies.push_back( static_cast<INDEX_TYPE>( ( begin2 + j ) ) % allVerticies );
			        indicies.push_back( static_cast<INDEX_TYPE>( ( begin2 + j + 1 ) ) % allVerticies );
                    indicies.push_back( static_cast<INDEX_TYPE>( begin1 + j + 1 ) );
                }
                else
                {
			        indicies.push_back( static_cast<INDEX_TYPE>( begin1 + j ) );
                    indicies.push_back( static_cast<INDEX_TYPE>( begin1 + j + 1 ) );
			        indicies.push_back( static_cast<INDEX_TYPE>( ( begin2 + j ) ) % allVerticies );

			        indicies.push_back( static_cast<INDEX_TYPE>( ( begin2 + j ) ) % allVerticies );
			        indicies.push_back( static_cast<INDEX_TYPE>( begin1 + j + 1 ) );
			        indicies.push_back( static_cast<INDEX_TYPE>( ( begin2 + j + 1 ) ) % allVerticies );
                }
			}
		}

		void ConnectTopToothSurface( int index1, int index2, std::vector<INDEX_TYPE>& indicies )
		{
			indicies.push_back( static_cast<INDEX_TYPE>( ( index1 ) % allVerticies ) );
			indicies.push_back( static_cast<INDEX_TYPE>( ( index1 + verticiesPerRingTooth ) % allVerticies ) );
			indicies.push_back( static_cast<INDEX_TYPE>( ( index2 ) % allVerticies ) );

			indicies.push_back( static_cast<INDEX_TYPE>( ( index1 + verticiesPerRingTooth ) % allVerticies ) );
			indicies.push_back( static_cast<INDEX_TYPE>( ( index2 + verticiesPerRingTooth ) % allVerticies ) );
            indicies.push_back( static_cast<INDEX_TYPE>( ( index2 ) % allVerticies ) );
		}

		void ConnectBottomToothSurface( int index1, int index2, std::vector<INDEX_TYPE>& indicies )
		{
			indicies.push_back( static_cast<INDEX_TYPE>( ( index1 ) % allVerticies ) );
			indicies.push_back( static_cast<INDEX_TYPE>( ( index2 ) % allVerticies ) );
            indicies.push_back( static_cast<INDEX_TYPE>( ( index1 + verticiesPerRingTooth ) % allVerticies ) );

			indicies.push_back( static_cast<INDEX_TYPE>( ( index1 + verticiesPerRingTooth ) % allVerticies ) );
            indicies.push_back( static_cast<INDEX_TYPE>( ( index2 ) % allVerticies ) );
			indicies.push_back( static_cast<INDEX_TYPE>( ( index2 + verticiesPerRingTooth ) % allVerticies ) );
		}

		void CloseRing( unsigned int begin, std::vector<INDEX_TYPE>& indicies )
		{
			indicies.push_back( static_cast<INDEX_TYPE>( ( begin + verticiesPerRing - 1 ) % allVerticies ) );
			indicies.push_back( static_cast<INDEX_TYPE>( ( begin + verticiesPerRingTooth + verticiesPerRing - 1 ) % allVerticies ) );
			indicies.push_back( static_cast<INDEX_TYPE>( begin ) );

			indicies.push_back( static_cast<INDEX_TYPE>( ( begin + verticiesPerRingTooth + verticiesPerRing - 1 ) % allVerticies ) );
			indicies.push_back( static_cast<INDEX_TYPE>( ( begin + verticiesPerRingTooth ) % allVerticies ) );
            indicies.push_back( static_cast<INDEX_TYPE>( begin ) );
		}

		void ConnectToothRingVerticies( int i, std::vector<INDEX_TYPE>& indicies )
		{
			unsigned int index = i * verticiesPerRingTooth;
			const int verticiesWithoutTooth = 5;

			ConnectVerticiesBetween( index, verticiesWithoutTooth, indicies );

		// We must add tooth
            // Left side
			ConnectSideToothVerticies( index + verticiesWithoutTooth - 1, index + verticiesPerRing, verticiesWithoutTooth - 1, indicies, true );
            // Right side
			ConnectSideToothVerticies( index + verticiesPerRingTooth + verticiesWithoutTooth - 1, index + verticiesPerRingTooth + verticiesPerRing, verticiesWithoutTooth - 1, indicies, false );
            // Front side
            ConnectVerticiesBetween( index + verticiesPerRing, verticiesWithoutTooth - 1, indicies );
			//ConnectSideToothVerticies( index + verticiesPerRing, index + verticiesPerRingTooth + verticiesPerRing, verticiesWithoutTooth - 1, indicies, true );
			
            // Top and bottom surface of the tooth.
			ConnectTopToothSurface( index + verticiesWithoutTooth - 1, index + verticiesPerRing, indicies );
			ConnectBottomToothSurface( index + 7, index + verticiesPerRingTooth - 1, indicies );

        // Close rest of tooth
			ConnectVerticiesBetween( index + 7, verticiesWithoutTooth, indicies );
			CloseRing( index, indicies );
		}

		void GenerateRing( std::vector<glm::vec3>& verticies, double angle, double uniformAngle, double topToothAngle )
		{
			glm::vec3 newVertex;
			verticies.reserve( verticiesPerRing );
			
			// Part without teeths.
			double sinAngle = sin( angle );
			double cosAngle = cos( angle );
			double cosUniformAngle = cos( uniformAngle );
			double sinUniformAngle = sin( uniformAngle );
			

			newVertex = glm::vec3( cosUniformAngle * innerRadius, height - bevel, sinUniformAngle * innerRadius );
			verticies.push_back( newVertex + center_translate );
			newVertex = glm::vec3( cosUniformAngle * innerRadius, height, sinUniformAngle * innerRadius );
			verticies.push_back( newVertex + center_translate );
			newVertex = glm::vec3( cosUniformAngle * ( innerRadius + bevel ), height, sinUniformAngle * ( innerRadius + bevel ) );
			verticies.push_back( newVertex + center_translate );
			newVertex = glm::vec3( cosAngle * ( outerRadius - bevel ), height, sinAngle * ( outerRadius - bevel ) );
			verticies.push_back( newVertex + center_translate );
			newVertex = glm::vec3( cosAngle * ( outerRadius ), height, sinAngle * ( outerRadius ) );
			verticies.push_back( newVertex + center_translate );
			newVertex = glm::vec3( cosAngle * ( outerRadius ), height - bevel, sinAngle * ( outerRadius ) );
			verticies.push_back( newVertex + center_translate );
			newVertex = glm::vec3( cosAngle * ( outerRadius ), -height + bevel, sinAngle * ( outerRadius ) );
			verticies.push_back( newVertex + center_translate );
			newVertex = glm::vec3( cosAngle * ( outerRadius ), -height, sinAngle * ( outerRadius ) );
			verticies.push_back( newVertex + center_translate );
			newVertex = glm::vec3( cosAngle * ( outerRadius - bevel ), -height, sinAngle * ( outerRadius - bevel ) );
			verticies.push_back( newVertex + center_translate );
			newVertex = glm::vec3( cosUniformAngle * ( innerRadius + bevel ), -height, sinUniformAngle * ( innerRadius + bevel ) );
			verticies.push_back( newVertex + center_translate );
			newVertex = glm::vec3( cosUniformAngle * innerRadius, -height, sinUniformAngle * innerRadius );
			verticies.push_back( newVertex + center_translate );
			newVertex = glm::vec3( cosUniformAngle * innerRadius, -height + bevel, sinUniformAngle * innerRadius );
			verticies.push_back( newVertex + center_translate );

			//Tooth
			sinAngle = sin( topToothAngle );
			cosAngle = cos( topToothAngle );
			
			newVertex = glm::vec3( cosAngle * ( outerRadius + toothHeight ), height, sinAngle * ( outerRadius + toothHeight ) );
			verticies.push_back( newVertex + center_translate );
			newVertex = glm::vec3( cosAngle * ( outerRadius + toothHeight ), height - bevel, sinAngle * ( outerRadius + toothHeight ) );
			verticies.push_back( newVertex + center_translate );
			newVertex = glm::vec3( cosAngle * ( outerRadius + toothHeight ), -height + bevel, sinAngle * ( outerRadius + toothHeight ) );
			verticies.push_back( newVertex + center_translate );
			newVertex = glm::vec3( cosAngle * ( outerRadius + toothHeight ), -height, sinAngle * ( outerRadius + toothHeight ) );
			verticies.push_back( newVertex + center_translate );
		}

		void ConnectVerticiesIntoTriangles( std::vector<INDEX_TYPE>& indicies )
		{
			for( int i = 0; i < 2 * teethNumber; ++i )
			{
				if( i & 0x01 ) // Checks parity
				{
					// We can make whole ring because there's no tooth.
					unsigned int index = i * verticiesPerRingTooth;
					ConnectVerticiesBetween( index, verticiesPerRing, indicies );
				
					// Close the ring.
					CloseRing( index, indicies );
				}
				else
					ConnectToothRingVerticies( i, indicies );
			}
		}


		void GenerateGeometryNormalsUVs( Float3AttributeChannelPtr verts, Float3AttributeChannelPtr normals, Float2AttributeChannelPtr uvs ) override
        {
			computeWeightCenter( weight_centerX, weight_centerY, weight_centerZ );

			HelperSmoothMesh smoother;
			IndexedGeometryConverter converter;
			IndexedGeometry cogWheel;
			std::vector<glm::vec3>& verticies = cogWheel.GetVerticies();
			std::vector<INDEX_TYPE>& indicies = cogWheel.GetIndicies();

			allVerticies = 2 * teethNumber * verticiesPerRingTooth;

			double anglePerTooth = toothBaseLength / outerRadius;
			double angleBetweenTeeth = ( TWOPI * outerRadius - toothBaseLength * teethNumber ) / ( teethNumber * outerRadius );
			double anglePerTopTooth = toothTopLength / ( outerRadius + toothHeight );

			for( int i = 0; i < teethNumber; ++i )
			{
				double angle1 = ( anglePerTooth + angleBetweenTeeth ) * (float)i;
				double angle2 = angle1 + anglePerTooth;
				double uniformAngle1 = double( i << 1 ) * PI / teethNumber;
				double uniformAngle2 = double( (i << 1) + 1) * PI / teethNumber;
				double toothCenterAngle = ( angle1 + angle2 ) / 2;

				GenerateRing( verticies, angle1, uniformAngle1, toothCenterAngle - anglePerTopTooth / 2 );
				GenerateRing( verticies, angle2, uniformAngle2, toothCenterAngle + anglePerTopTooth / 2 );
			}

			ConnectVerticiesIntoTriangles( indicies );

			//Smooth cog wheel
			std::vector<INDEX_TYPE> sharpEdges;
			
			IndexedGeometry resultMesh = smoother.smooth( cogWheel, sharpEdges, tesselation );
            converter.MakeTriangles( resultMesh, verts );

            GeometryGeneratorHelper::GenerateNonWeightedNormalsFromTriangles( resultMesh, normals );


			for( SizeType v = 0; v < verts->GetNumEntries(); v++ )
			{
				glm::vec3 vert = verts->GetVertices()[ v ];
				vert -= center_translate;
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
	m_weightCenterX = QueryTypedParam< std::shared_ptr< ParamEnum< WeightCenter > > >( GetParameter( PN::WEIGHTCENTERX ) );
	m_weightCenterY = QueryTypedParam< std::shared_ptr< ParamEnum< WeightCenter > > >( GetParameter( PN::WEIGHTCENTERY ) );
	m_weightCenterZ = QueryTypedParam< std::shared_ptr< ParamEnum< WeightCenter > > >( GetParameter( PN::WEIGHTCENTERZ ) );


    m_pluginParamValModel->Update();
    InitGeometry( PrimitiveType::PT_TRIANGLES );
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
	Generator::weight_centerX = m_weightCenterX->Evaluate();
	Generator::weight_centerY = m_weightCenterY->Evaluate();
	Generator::weight_centerZ = m_weightCenterZ->Evaluate();


    std::vector< IGeometryGeneratorPtr > gens;
    gens.push_back( std::make_shared< Generator::CogWheelGenerator >() );
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
		|| ParameterChanged( PN::HEIGHT )
		|| ParameterChanged( PN::WEIGHTCENTERX )
		|| ParameterChanged( PN::WEIGHTCENTERY )
		|| ParameterChanged( PN::WEIGHTCENTERZ );
}


}}}
