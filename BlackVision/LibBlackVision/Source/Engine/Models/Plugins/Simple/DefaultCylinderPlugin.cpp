#include "DefaultCylinderPlugin.h"


namespace bv { namespace model {

typedef ParamEnum< DefaultCylinder::DefaultPlugin::OpenAngleMode > ParamEnumOAM;

VoidPtr    ParamEnumOAM::QueryParamTyped  ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}

template<>
static IParameterPtr        ParametersFactory::CreateTypedParameter< DefaultCylinder::DefaultPlugin::OpenAngleMode >                 ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return CreateParameterEnum< DefaultCylinder::DefaultPlugin::OpenAngleMode >( name, timeline );
}

#include "Engine/Models/Plugins/ParamValModel/SimpleParamValEvaluator.inl"

namespace DefaultCylinder {


const std::string PN::TESSELATION = "tesselation";
const std::string PN::HEIGHT = "height";
const std::string PN::INNERRADIUS = "inner radius";
const std::string PN::OUTERRADIUS = "outer radius";
const std::string PN::OPENANGLE = "open angle";
const std::string PN::OPENANGLEMODE = "open angle mode";


DefaultCylinderPluginDesc::DefaultCylinderPluginDesc()
            : DefaultGeometryPluginDescBase( UID(), "cylinder" ) { }

DefaultPluginParamValModelPtr   DefaultCylinderPluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper h( timeEvaluator );

    h.CreateVacModel();
	h.AddSimpleParam( PN::TESSELATION, 20, true, true );
    h.AddSimpleParam( PN::HEIGHT, 1.f, true, true );
    h.AddSimpleParam( PN::OUTERRADIUS, 1.f, true, true );
    h.AddSimpleParam( PN::INNERRADIUS, 0.2f, true, true );
    h.AddSimpleParam( PN::OPENANGLE, 80.f, true, true );
    h.AddParam< IntInterpolator, DefaultPlugin::OpenAngleMode, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumOAM >
        ( PN::OPENANGLEMODE, DefaultPlugin::OpenAngleMode::CW, true, true );
    
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

    static void Init( int t, float ir, float oa, float h, float or, DefaultPlugin::OpenAngleMode oam )
    {
		tesselation = t;
        inner_radius = ir; 
        open_angle = oa;
        height = h;
        outer_radius = or;
		open_angle_mode = oam;
    }

	class MainGenerator : public IGeometryAndUVsGenerator
    {
	protected:
		double angle_offset;			// OpenAngleMode needs this

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

		/**Generates verticies of one circuit of the beveled edges.
		R1 and h1 describe position of top verticies of the strip,
		R2 and h2 describe position of bottom verticies of the strip.
		
		@param[inout] direction Because of OpenAngle, we can't generate verticies in continous circles. Thats why we change direction every circle.
		Function gets direction that will use to draw and in the same param returns direction for the next function.
		*/
		void generateCircuit( float R1, float R2, float h1, float h2, Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs, bool& direction )
		{
			int max_loop;
			if( open_angle != 0.0 )
				max_loop = static_cast<int>( ceil( float( ( TWOPI - TO_RADIANS( open_angle ) ) / ( TWOPI / tesselation ) ) ) );
			else
				max_loop = tesselation;

			int i = max_loop;
			if( direction )
				i = 0;

			for( int j = 0; j <= max_loop; j++ )
            {
				double angle1 = computeAngle2Clamped( TWOPI / tesselation, (float)i );
				angle1 += angle_offset;

				double cos_angle1 = cos( angle1 );
				double sin_angle1 = sin( angle1 );

				verts->AddAttribute( glm::vec3( R1 * cos_angle1, h1, R1 * sin_angle1 ) );
				//uvs->AddAttribute( glm::vec2( 0.0, 0.0 ) );		// Temp

				verts->AddAttribute( glm::vec3( R2 * cos_angle1, h2, R2 * sin_angle1 ) );
				//uvs->AddAttribute( glm::vec2( 0.0, 0.0 ) );		// Temp


				if( direction )
					i++;
				else
					i--;
			}

			direction = !direction;
		}
	public:
		virtual Type GetType() { return Type::GEOMETRY_AND_UVS; }
		

		virtual void GenerateGeometryAndUVs( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs )
        {
			computeAngleOffset();
			bool gen_direction = true;		// Generation direction clockwise or counter clockwise

			// Top of cylinder
			generateCircuit( inner_radius, outer_radius, height, height, verts, uvs, gen_direction );

			// Lateral surface
			generateCircuit( outer_radius, outer_radius, height, 0.0f, verts, uvs, gen_direction );

			// Bottom of cylinder
			generateCircuit( outer_radius, inner_radius, 0.0f, 0.0f, verts, uvs, gen_direction );

			if( open_angle > 0.0 )
				generateCircuit( inner_radius, inner_radius, 0.0f, height, verts, uvs, gen_direction );

            for( SizeType v = 0; v < verts->GetNumEntries(); v++ )
            {
                glm::vec3 vert = verts->GetVertices()[ v ];
				//vert -= center_translate;
                uvs->AddAttribute( glm::vec2( vert.x*0.5 + 0.5,
                                                vert.y*0.5 + 0.5 ) ); // FIXME: scaling
            }
		}
	};

	class ClosureGenerator : public MainGenerator
    {
	private:
		bool rotated;
	public:
		ClosureGenerator( bool isRotated ) : rotated( isRotated ){}

		virtual Type GetType() { return Type::GEOMETRY_AND_UVS; }

		virtual void GenerateGeometryAndUVs( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs )
        {
			computeAngleOffset();
			double angle = 0.0;

			if( rotated )	// If surface is rotated we must compute rotation angle
				angle = computeAngle2Clamped( TWOPI, 1 );
			angle += angle_offset;


			double cos_angle = cos( angle );
			double sin_angle = sin( angle );

			verts->AddAttribute( glm::vec3( outer_radius * cos_angle, height, outer_radius * sin_angle ) );
			verts->AddAttribute( glm::vec3( inner_radius * cos_angle, height, inner_radius * sin_angle ) );
			verts->AddAttribute( glm::vec3( outer_radius * cos_angle, 0.0f, outer_radius * sin_angle ) );
			verts->AddAttribute( glm::vec3( inner_radius * cos_angle, 0.0f, inner_radius * sin_angle ) );

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



DefaultPlugin::DefaultPlugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model )
    : DefaultGeometryPluginBase( name, uid, prev, model ) 
{ 
	m_tesselation = QueryTypedValue< ValueIntPtr >( GetValue( PN::TESSELATION ) );
    m_innerRadius = QueryTypedValue< ValueFloatPtr >( GetValue( PN::INNERRADIUS ) );
    m_openAngle = QueryTypedValue< ValueFloatPtr >( GetValue( PN::OPENANGLE ) );
    m_height = QueryTypedValue< ValueFloatPtr >( GetValue( PN::HEIGHT ) );
    m_outerRadius = QueryTypedValue< ValueFloatPtr >( GetValue( PN::OUTERRADIUS ) );
	m_openAngleMode = QueryTypedParam< std::shared_ptr< ParamEnum< OpenAngleMode > > >( GetParameter( PN::OPENANGLEMODE ) );

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
		m_openAngleMode->Evaluate() );

    std::vector<IGeometryGeneratorPtr> gens;
    gens.push_back( IGeometryGeneratorPtr( new CylinderGenerator::MainGenerator() ) );
	if( m_openAngle->GetValue() > 0.0 )
	{
		gens.push_back( IGeometryGeneratorPtr( new CylinderGenerator::ClosureGenerator(true) ) );
		gens.push_back( IGeometryGeneratorPtr( new CylinderGenerator::ClosureGenerator(false) ) );
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
		ParameterChanged( PN::OPENANGLEMODE );
}


} //DefaultCylinder
} // model
} // bv
