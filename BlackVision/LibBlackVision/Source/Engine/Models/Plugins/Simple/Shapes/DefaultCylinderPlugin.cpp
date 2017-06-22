#include "stdafx.h"

#include "DefaultCylinderPlugin.h"

#include "Memory/MemoryLeaks.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"

DEFINE_ENUM_PARAMETER_CREATOR( bv::model::DefaultCylinder::DefaultPlugin::OpenAngleMode );
DEFINE_ENUM_PARAMETER_CREATOR( bv::model::DefaultCylinder::DefaultPlugin::WeightCenter );
DEFINE_ENUM_PARAMETER_CREATOR( bv::model::DefaultCylinder::DefaultPlugin::MappingType );


namespace bv { namespace model {

typedef ParamEnum< DefaultCylinder::DefaultPlugin::OpenAngleMode > ParamEnumOAM;
typedef ParamEnum< DefaultCylinder::DefaultPlugin::WeightCenter > ParamEnumWC;
typedef ParamEnum< DefaultCylinder::DefaultPlugin::MappingType > ParamEnumMT;



namespace DefaultCylinder {


const std::string PN::TESSELATION = "tesselation";
const std::string PN::HEIGHT = "height";
const std::string PN::INNERRADIUS = "inner radius";
const std::string PN::OUTERRADIUS = "outer radius";
const std::string PN::OPENANGLE = "open angle";
const std::string PN::OPENANGLEMODE = "open angle mode";
const std::string PN::WEIGHTCENTERX = "weight center x";
const std::string PN::WEIGHTCENTERY = "weight center y";
const std::string PN::WEIGHTCENTERZ = "weight center z";
const std::string PN::MAPPINGTYPE = "mapping type";


DefaultCylinderPluginDesc::DefaultCylinderPluginDesc()
            : DefaultGeometryPluginDescBase( UID(), "cylinder" ) { }

DefaultPluginParamValModelPtr   DefaultCylinderPluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper h( timeEvaluator );

    h.SetOrCreateVacModel();
	h.AddSimpleParam( PN::TESSELATION, 20, true, true );
    h.AddSimpleParam( PN::HEIGHT, 1.f, true, true );
    h.AddSimpleParam( PN::OUTERRADIUS, 1.f, true, true );
    h.AddSimpleParam( PN::INNERRADIUS, 0.2f, true, true );
    h.AddSimpleParam( PN::OPENANGLE, 80.f, true, true );
    h.AddParam< IntInterpolator, DefaultPlugin::OpenAngleMode, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumOAM >
        ( PN::OPENANGLEMODE, DefaultPlugin::OpenAngleMode::CW, true, true );
   	h.AddParam< IntInterpolator, DefaultPlugin::WeightCenter, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumWC >
        ( PN::WEIGHTCENTERX, DefaultPlugin::WeightCenter::CENTER, true, true );
	h.AddParam< IntInterpolator, DefaultPlugin::WeightCenter, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumWC >
        ( PN::WEIGHTCENTERY, DefaultPlugin::WeightCenter::MIN, true, true );
	h.AddParam< IntInterpolator, DefaultPlugin::WeightCenter, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumWC >
        ( PN::WEIGHTCENTERZ, DefaultPlugin::WeightCenter::CENTER, true, true );
	h.AddParam< IntInterpolator, DefaultPlugin::MappingType, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumMT >
		( PN::MAPPINGTYPE, DefaultPlugin::MappingType::GOODMAPPING, true, true );

    h.SetOrCreatePSModel();

    return h.GetModel();
}

IPluginPtr                      DefaultCylinderPluginDesc::CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultPlugin >( name, prev, timeEvaluator );
}

std::string                     DefaultCylinderPluginDesc::UID                 ()
{
    return "DEFAULT_CYLINDER";
}


#include "Mathematics/Defines.h"
namespace CylinderGenerator
{
	int tesselation;
    float height, inner_radius, open_angle, outer_radius;
	DefaultPlugin::OpenAngleMode open_angle_mode;
	DefaultPlugin::WeightCenter weight_centerX;
	DefaultPlugin::WeightCenter weight_centerY;
	DefaultPlugin::WeightCenter weight_centerZ;
	DefaultPlugin::MappingType mapping_type;

	static void Init( int t, float ir, float oa, float h, float or,
		DefaultPlugin::OpenAngleMode oam,
		DefaultPlugin::WeightCenter wcx,
		DefaultPlugin::WeightCenter wcy,
		DefaultPlugin::WeightCenter wcz,
		DefaultPlugin::MappingType mt )
    {
		tesselation = t;
        inner_radius = ir; 
        open_angle = oa;
        height = h;
        outer_radius = or;
		open_angle_mode = oam;
		weight_centerX = wcx;
		weight_centerY = wcy;
		weight_centerZ = wcz;

		mapping_type = mt;
    }

    glm::vec3       OuterNormal( double x, double z )
    {        return glm::vec3( x, 0.0, z );    }

    glm::vec3       InnerNormal( double x, double z )
    {        return glm::vec3( -x, 0.0, -z );    }

    glm::vec3       TopNormal( double, double )
    {        return glm::vec3( 0.0, 1.0, 0.0 );    }

    glm::vec3       BottomNormal( double, double )
    {        return glm::vec3( 0.0, -1.0, 0.0 );    }



	class MainGenerator : public IGeometryNormalsUVsGenerator
    {
	protected:
		double angle_offset;			// OpenAngleMode needs this
		glm::vec3 center_translate;

		void computeAngleOffset()
		{
			if( open_angle_mode == DefaultPlugin::OpenAngleMode::CCW )
				angle_offset = -3*PI / 2;
			else if( open_angle_mode == DefaultPlugin::OpenAngleMode::CW )
				angle_offset = -3*PI / 2 + TO_RADIANS( open_angle );
			else if( open_angle_mode == DefaultPlugin::OpenAngleMode::SYMMETRIC )
				angle_offset =  -3*PI / 2 + TO_RADIANS( open_angle ) / 2;
			else
				assert( false );
		}

		double computeAngle2Clamped( double angle, float stripe_num )
		{
			double ret_value = angle * stripe_num;
			if( open_angle > 0.0 )
			{
				double max_angle = TWOPI - TO_RADIANS( open_angle );
				if( ret_value > max_angle )
					return max_angle;
			}

			return ret_value;
		}

		void computeWeightCenter( DefaultPlugin::WeightCenter centerX, DefaultPlugin::WeightCenter centerY, DefaultPlugin::WeightCenter centerZ )
		{
			center_translate = glm::vec3( 0.0, 0.0, 0.0 );

			if( centerX == DefaultPlugin::WeightCenter::MAX )
				center_translate += glm::vec3( -outer_radius, 0.0, 0.0 );
			else if( centerX == DefaultPlugin::WeightCenter::CENTER )
				center_translate += glm::vec3( 0.0, 0.0, 0.0 );
			else if( centerX == DefaultPlugin::WeightCenter::MIN )
				center_translate += glm::vec3( outer_radius, 0.0, 0.0 );
	
			if( centerY == DefaultPlugin::WeightCenter::MAX )
				center_translate += glm::vec3( 0.0, -height, 0.0 );
			else if( centerY == DefaultPlugin::WeightCenter::CENTER )
				center_translate += glm::vec3( 0.0, -height / 2, 0.0 );
			else if( centerY == DefaultPlugin::WeightCenter::MIN )
				center_translate += glm::vec3( 0.0, 0.0, 0.0 );

			if( centerZ == DefaultPlugin::WeightCenter::MAX )
				center_translate += glm::vec3( 0.0, 0.0, -outer_radius );
			else if( centerZ == DefaultPlugin::WeightCenter::CENTER )
				center_translate += glm::vec3( 0.0, 0.0, 0.0 );
			else if( centerZ == DefaultPlugin::WeightCenter::MIN )
				center_translate += glm::vec3( 0.0, 0.0, outer_radius );
		}

		void computeMaxLoopInitI( int& max_loop, int& i, bool direction )
		{
			if( open_angle != 0.0 )
				max_loop = static_cast<int>( ceil( float( ( TWOPI - TO_RADIANS( open_angle ) ) / ( TWOPI / tesselation ) ) ) );
			else
				max_loop = tesselation;

			i = max_loop;
			if( direction )
				i = 0;
		}



		/**Generates verticies of one circuit of the beveled edges.
		R1 and h1 describe position of top verticies of the strip,
		R2 and h2 describe position of bottom verticies of the strip.
		
		@param[inout] direction Because of OpenAngle, we can't generate verticies in continous circles. Thats why we change direction every circle.
		Function gets direction that will use to draw and in the same param returns direction for the next function.
		*/
        template< typename glm::vec3 (*NormComputingFun)( double, double ) >
		void generateCircuit( float R1, float R2, float h1, float h2, Float3AttributeChannelPtr verts, Float3AttributeChannelPtr norm, bool& direction )
		{
			int max_loop;
			int i;
			computeMaxLoopInitI( max_loop, i, direction );

			for( int j = 0; j <= max_loop; j++ )
            {
				double angle1 = computeAngle2Clamped( TWOPI / tesselation, (float)i );
				angle1 += angle_offset;

				double cosAngle = cos( angle1 );
				double sinAngle1 = sin( angle1 );

                verts->AddAttribute( glm::vec3( R1 * cosAngle, h1, R1 * sinAngle1 ) + center_translate );   norm->AddAttribute( NormComputingFun( cosAngle, sinAngle1 ) );
				verts->AddAttribute( glm::vec3( R2 * cosAngle, h2, R2 * sinAngle1 ) + center_translate );   norm->AddAttribute( NormComputingFun( cosAngle, sinAngle1 ) );


				if( direction )
					i++;
				else
					i--;
			}

			direction = !direction;
		}

		double computeAngleFromPos( Float3AttributeChannelPtr verts, unsigned int start_index )
		{
			glm::vec2 ref_vector( 0.0, -1.0 );

			glm::vec3 vert = verts->GetVertices()[ start_index ];
			vert -= center_translate;
			glm::vec2 vertex( vert.x, vert.z );

			if( vertex == glm::vec2( 0.0, 0.0 ) )	//It means that the vertex is in circle center. That's no problem. Next vertex will be correct.
				return computeAngleFromPos( verts, start_index + 1 );

			vertex = glm::normalize( vertex );

			double angle = PI + glm::radians( glm::orientedAngle( vertex, ref_vector ) );
			return angle;
		}

		double generateUFromPosition( Float3AttributeChannelPtr verts, unsigned int start_index )
		{
			double angle = computeAngleFromPos( verts, start_index );
			return glm::clamp( angle / TWOPI, 0.0, 1.0 );
		}

		void generateUVCircuit( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs, float topV, float bottomV, unsigned int& startIndex )
		{
			int max_loop;
			int i;
			computeMaxLoopInitI( max_loop, i, false );

			for( int j = 0; j <= max_loop; j++ )
            {
				double U_coord = generateUFromPosition( verts, startIndex );

				if( U_coord > 0.97 )
				//Maybe it should be 0.0 not 1.0 ?
					if( i == j )
						U_coord = 0.0;

				uvs->AddAttribute( glm::vec2( U_coord, topV ) );
				uvs->AddAttribute( glm::vec2( U_coord, bottomV ) );

				++startIndex;
				++startIndex;
			}
		}

		void generateUVCircle( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs, glm::vec2 centerUV, float radiusUV, unsigned int& start_index, bool outer_first )
		{
			int max_loop;
			int i;
			computeMaxLoopInitI( max_loop, i, true );

			for( int j = 0; j <= max_loop; j++ )
            {
				double angle = computeAngleFromPos( verts, start_index );

				float u = static_cast<float>( radiusUV * sin( angle ) );
				float v = static_cast<float>( radiusUV * cos( angle ) );

				if( outer_first )
				{
					uvs->AddAttribute( glm::vec2( u * inner_radius / outer_radius, v * inner_radius / outer_radius ) + centerUV );
					uvs->AddAttribute( glm::vec2( u, v ) + centerUV );
				}
				else
				{
					uvs->AddAttribute( glm::vec2( u, v ) + centerUV );
					uvs->AddAttribute( glm::vec2( u * inner_radius / outer_radius, v * inner_radius / outer_radius ) + centerUV );
				}

				++start_index;
				++start_index;
			}
		}

		void generateUVs( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs )
		{
			//bool direction = true;		// It should have the same value at the beginning, as gen_direction in GenerateGeometryAndUVs function

			if( mapping_type == DefaultPlugin::MappingType::GOODMAPPING )
			{
				unsigned int start_index = 0;

				generateUVCircle( verts, uvs, glm::vec2( 1.0 / 6.0, 5.0 / 6.0 ), static_cast<float>( 1.0 / 6.0 ), start_index, true );
				generateUVCircuit( verts, uvs, static_cast<float>( 0.0 ), static_cast<float>( 1.0 / 3.0 ), start_index );
				generateUVCircle( verts, uvs, glm::vec2( 1.0 / 2.0, 5.0 / 6.0 ), float( 1.0 / 6.0 ), start_index, false );
				if( inner_radius > 0.0 )
					generateUVCircuit( verts, uvs, static_cast<float>( 2.0 / 3.0 ), static_cast<float>( 1.0 / 3.0 ), start_index );
			}
			else
			{
				for( SizeType v = 0; v < verts->GetNumEntries(); v++ )
				{
					glm::vec3 vert = verts->GetVertices()[ v ];
					vert -= center_translate;
					uvs->AddAttribute( glm::vec2( vert.x*0.5 + 0.5,
													vert.y*0.5 + 0.5 ) ); // FIXME: scaling
				}
			}
		}
	public:

		virtual void GenerateGeometryNormalsUVs( Float3AttributeChannelPtr verts, Float3AttributeChannelPtr normals, Float2AttributeChannelPtr uvs ) override
        {
			if( outer_radius == 0.0 )
				return;		//Nothing to do.

			computeAngleOffset();
			computeWeightCenter( weight_centerX, weight_centerY, weight_centerZ );
			bool gen_direction = false;		// Generation direction clockwise or counter clockwise

			// Top of cylinder
            generateCircuit< TopNormal >( inner_radius, outer_radius, height, height, verts, normals, gen_direction );

			// Lateral surface
            generateCircuit< OuterNormal >( outer_radius, outer_radius, 0.0f, height, verts, normals, gen_direction );

			// Bottom of cylinder
            generateCircuit< BottomNormal >( outer_radius, inner_radius, 0.0f, 0.0f, verts, normals, gen_direction );

			if( inner_radius > 0.0 )
                generateCircuit< InnerNormal >( inner_radius, inner_radius, height, 0.0f, verts, normals, gen_direction );

			generateUVs( verts, uvs );
            
            //GeometryGeneratorHelper::GenerateNonWeightedNormalsFromTriangleStrips( verts, normals );
		}
	};

	class ClosureGenerator : public MainGenerator
    {
	private:
		bool rotated;
	public:
		ClosureGenerator( bool isRotated ) : rotated( isRotated ){}

		void generateUVs( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs )
		{
			if( mapping_type == DefaultPlugin::MappingType::GOODMAPPING )
			{
				// @fixme Nothing done here.
				double minU;
				double maxU;
				double minV = 1.0 / 3.0;
				double maxV = 1.0;

				if( rotated )	//Rotated closure part
				{
					maxU = 1.0;
					minU = 5.0 / 6.0 + ( 1.0 / 6.0 ) * ( inner_radius / outer_radius );
				}
				else
				{
					maxU = 5.0 / 6.0;
					minU = 2.0 / 3.0 + ( 1.0 / 6.0 ) * ( inner_radius / outer_radius );
				}

				uvs->AddAttribute( glm::vec2( maxU, maxV ) );
				uvs->AddAttribute( glm::vec2( minU, maxV ) );
				uvs->AddAttribute( glm::vec2( maxU, minV ) );
				uvs->AddAttribute( glm::vec2( minU, minV ) );
			}
			else
			{
				for( SizeType v = 0; v < verts->GetNumEntries(); v++ )
				{
					glm::vec3 vert = verts->GetVertices()[ v ];
					vert -= center_translate;
					uvs->AddAttribute( glm::vec2( vert.x*0.5 + 0.5,
													vert.y*0.5 + 0.5 ) ); // FIXME: scaling
				}
			}
		}

		virtual void GenerateGeometryNormalsUVs( Float3AttributeChannelPtr verts, Float3AttributeChannelPtr normals, Float2AttributeChannelPtr uvs )
        {
			computeAngleOffset();
			computeWeightCenter( weight_centerX, weight_centerY, weight_centerZ );
			double angle = 0.0;

			if( rotated )	// If surface is rotated we must compute rotation angle
				angle = computeAngle2Clamped( TWOPI, 1 );
			angle += angle_offset;


			double cos_angle = cos( angle );
			double sin_angle = sin( angle );

            if( rotated )
            {
		        verts->AddAttribute( glm::vec3( outer_radius * cos_angle, height, outer_radius * sin_angle ) + center_translate );
		        verts->AddAttribute( glm::vec3( inner_radius * cos_angle, height, inner_radius * sin_angle ) + center_translate );
		        verts->AddAttribute( glm::vec3( outer_radius * cos_angle, 0.0f, outer_radius * sin_angle ) + center_translate );
		        verts->AddAttribute( glm::vec3( inner_radius * cos_angle, 0.0f, inner_radius * sin_angle ) + center_translate );
            }
            else
            {
                verts->AddAttribute( glm::vec3( outer_radius * cos_angle, 0.0f, outer_radius * sin_angle ) + center_translate );
		        verts->AddAttribute( glm::vec3( inner_radius * cos_angle, 0.0f, inner_radius * sin_angle ) + center_translate );
		        verts->AddAttribute( glm::vec3( outer_radius * cos_angle, height, outer_radius * sin_angle ) + center_translate );
		        verts->AddAttribute( glm::vec3( inner_radius * cos_angle, height, inner_radius * sin_angle ) + center_translate );
            }

			generateUVs( verts, uvs );
            
            GeometryGeneratorHelper::GenerateNonWeightedNormalsFromTriangleStrips( verts, normals );
		}
	};
}



DefaultPlugin::DefaultPlugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model )
    : DefaultGeometryPluginBase( name, uid, prev, model ) 
{ 
	m_tesselation = QueryTypedValue< ValueIntPtr >( GetValue( PN::TESSELATION ) );
    m_innerRadius = QueryTypedValue< ValueFloatPtr >( GetValue( PN::INNERRADIUS ) );
    m_openAngle = QueryTypedValue< ValueFloatPtr >( GetValue( PN::OPENANGLE ) );
    m_height = QueryTypedValue< ValueFloatPtr >( GetValue( PN::HEIGHT ) );
    m_outerRadius = QueryTypedValue< ValueFloatPtr >( GetValue( PN::OUTERRADIUS ) );
	m_openAngleMode = QueryTypedParam< std::shared_ptr< ParamEnum< OpenAngleMode > > >( GetParameter( PN::OPENANGLEMODE ) );

	m_weightCenterX = QueryTypedParam< std::shared_ptr< ParamEnum< WeightCenter > > >( GetParameter( PN::WEIGHTCENTERX ) );
	m_weightCenterY = QueryTypedParam< std::shared_ptr< ParamEnum< WeightCenter > > >( GetParameter( PN::WEIGHTCENTERY ) );
	m_weightCenterZ = QueryTypedParam< std::shared_ptr< ParamEnum< WeightCenter > > >( GetParameter( PN::WEIGHTCENTERZ ) );

	m_mappingType = QueryTypedParam< std::shared_ptr< ParamEnum< MappingType > > >( GetParameter( PN::MAPPINGTYPE ) );

    m_pluginParamValModel->Update();
    InitGeometry();
}

std::vector<IGeometryGeneratorPtr>    DefaultPlugin::GetGenerators()
{
	CylinderGenerator::Init( 
		m_tesselation->GetValue(),
		m_innerRadius->GetValue(),
		m_openAngle->GetValue(),
		m_height->GetValue(),
		m_outerRadius->GetValue(),
		m_openAngleMode->Evaluate(),
		m_weightCenterX->Evaluate(),
		m_weightCenterY->Evaluate(),
		m_weightCenterZ->Evaluate(),
		m_mappingType->Evaluate());

    std::vector<IGeometryGeneratorPtr> gens;
    gens.push_back( std::make_shared< CylinderGenerator::MainGenerator >() );
	if( m_openAngle->GetValue() > 0.0 )
	{
		gens.push_back( std::make_shared< CylinderGenerator::ClosureGenerator >( true ) );
		gens.push_back( std::make_shared< CylinderGenerator::ClosureGenerator >( false ) );
	}

    return gens;
}

bool                                DefaultPlugin::NeedsTopologyUpdate()
{
    return ParameterChanged( PN::HEIGHT ) ||
		ParameterChanged( PN::TESSELATION ) ||
        ParameterChanged( PN::OUTERRADIUS ) ||
        ParameterChanged( PN::OPENANGLE ) ||
        ParameterChanged( PN::INNERRADIUS ) ||
		ParameterChanged( PN::OPENANGLEMODE ) ||
		ParameterChanged( PN::WEIGHTCENTERX ) ||
		ParameterChanged( PN::WEIGHTCENTERY ) ||
		ParameterChanged( PN::WEIGHTCENTERZ ) ||
		ParameterChanged( PN::MAPPINGTYPE );
}


} //DefaultCylinder
} // model
} // bv
