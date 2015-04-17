#include "DefaultConePlugin.h"

namespace bv { namespace model { namespace DefaultCone {

const std::string PN::TESSELATION = "tesselation";
const std::string PN::HEIGHT = "height";
const std::string PN::INNERRADIUS = "inner radius";
const std::string PN::OUTERRADIUS = "outer radius";
const std::string PN::INNERHEIGHT = "inner height";
const std::string PN::ROUNDEDTIPHEIGHT = "rounded tip height";
const std::string PN::OPENANGLE = "open angle";
const std::string PN::WEIGHTCENTER = "weight center";
const std::string PN::OPENANGLEMODE = "open angle mode";


DefaultConePluginDesc::DefaultConePluginDesc()
            : DefaultGeometryPluginDescBase( UID(), "cone" ) { }

DefaultPluginParamValModelPtr   DefaultConePluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper h( timeEvaluator );

    h.CreateVacModel();
    h.AddSimpleParam( PN::TESSELATION, 3, true, true );
    h.AddSimpleParam( PN::HEIGHT, 1.f, true, true );
    h.AddSimpleParam( PN::INNERHEIGHT, 0.f, true, true );
    h.AddSimpleParam( PN::OUTERRADIUS, 1.f, true, true );
    h.AddSimpleParam( PN::INNERRADIUS, 0.f, true, true );
    h.AddSimpleParam( PN::ROUNDEDTIPHEIGHT, 0.f, true, true );
    h.AddSimpleParam( PN::OPENANGLE, 360.f, true, true );
    //h.AddSimpleParam( PN::OPENANGLEMODE
    
    return h.GetModel();
}

IPluginPtr                      DefaultConePluginDesc::CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultConePlugin >( name, prev, timeEvaluator );
}

std::string                     DefaultConePluginDesc::UID                 ()
{
    return "DEFAULT_CONE";
}


#include "Mathematics/Defines.h"
namespace ConeGenerator
{
    int tesselation, bevel_tesselation;
    float height, inner_height, inner_radius, bevel, open_angle, outer_radius;

    static void Init( int t, float ih, float ir, float b, float oa, float h, float or )
    {
        tesselation = t;
        inner_height = ih;
        inner_radius = ir; 
        bevel = b; 
        open_angle = oa;
        height = h;
        outer_radius = or;
    }

    class LateralSurface : public IGeometryAndUVsGenerator
    {
        float height, radius;
    public:
        LateralSurface( float h, float r ) : height( h ), radius( r ) { }

        virtual Type GetType() { return Type::GEOMETRY_AND_UVS; }

		void generateCircuit( float R1, float R2, float h1, float h2, Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs  )
		{
			for( int i = 0; i < tesselation ; i++ )
            {
				double angle1 = i     * 2 * PI / tesselation;
				double angle2 = (i+1) * 2 * PI / tesselation;

				verts->AddAttribute( glm::vec3( R1 * cos( angle1 ), h1, R1 * sin( angle1 ) ) );
				verts->AddAttribute( glm::vec3( R2 * cos( angle1 ), h2, R2 * sin( angle1 ) ) );
				verts->AddAttribute( glm::vec3( R1 * cos( angle2 ), h1, R1 * sin( angle2 ) ) );
				verts->AddAttribute( glm::vec3( R2 * cos( angle2 ), h2, R2 * sin( angle2 ) ) );
			}
		}

		void computeCircleRadiusHeight( glm::vec3& returnValue, glm::vec2 circleCenter, float circleRadius, double angle )
		{
			returnValue.x = circleCenter.x - float( circleRadius * sin( angle ) );
			returnValue.y = circleCenter.y - float( circleRadius * cos( angle ) );
			returnValue.z = 0.0;
		}

        virtual void GenerateGeometryAndUVs( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs )
        {
            for( int i = 0; i < tesselation ; i++ )
            {
                double angle1 = i     * 2 * PI / tesselation;
                double angle2 = (i+1) * 2 * PI / tesselation;

                verts->AddAttribute( glm::vec3( 0, height, 0 ) );
                verts->AddAttribute( glm::vec3( radius * cos( angle1 ), 0, radius * sin( angle1 ) ) );
                verts->AddAttribute( glm::vec3( radius * cos( angle2 ), 0, radius * sin( angle2 ) ) );
                verts->AddAttribute( glm::vec3( 0, height, 0 ) );
            }

			///todo:
			glm::vec2 circle_center;
			float circle_radius = 0.0;
			double angle1 = 0.0;
			double angle2 = 0.0;


			for( int tess = 0; tess < bevel_tesselation; ++tess)
			{
				glm::vec3 radius_height1;
				glm::vec3 radius_height2;
				computeCircleRadiusHeight( radius_height1, circle_center, circle_radius, angle1 );
				computeCircleRadiusHeight( radius_height2, circle_center, circle_radius, angle2 );

				for( int i = 0; i < tesselation ; i++ )
				{
					double angle1 = i     * 2 * PI / tesselation;
					double angle2 = (i+1) * 2 * PI / tesselation;\
					double cos_angle1 = cos( angle1 );
					double sin_angle1 = sin( angle1 );
					double cos_angle2 = cos( angle2 );
					double sin_angle2 = sin( angle2 );

					verts->AddAttribute( glm::vec3( radius_height1.x * cos_angle1, radius_height1.y, radius * sin_angle1 ) );
					verts->AddAttribute( glm::vec3( radius_height2.x * cos_angle1, radius_height2.y, radius * sin_angle1 ) );
					verts->AddAttribute( glm::vec3( radius_height1.x * cos_angle2, radius_height1.y, radius * sin_angle2 ) );
					verts->AddAttribute( glm::vec3( radius_height2.x * cos_angle2, radius_height2.y, radius * sin_angle2 ) );
				}
			}


            for( SizeType v = 0; v < verts->GetNumEntries(); v++ )
            {
                glm::vec3 vert = verts->GetVertices()[ v ];
                uvs->AddAttribute( glm::vec2( vert.x*0.5 + 0.5,
                                                vert.y*0.5 + 0.5 ) ); // FIXME: scaling
            }
        }
    };

    class BaseSurface : public IGeometryAndUVsGenerator
    {
    public:
        virtual Type GetType() { return Type::GEOMETRY_AND_UVS; }

        virtual void GenerateGeometryAndUVs( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs )
        {
            for( int i = 0; i <= tesselation; i++ )
            {
                double angle   = i     * 2 * PI / tesselation;
                auto vec       = glm::vec3( cos( angle ), 0, sin( angle ) );

                verts->AddAttribute( vec * inner_radius );
                verts->AddAttribute( vec * outer_radius );
            }

            for( SizeType v = 0; v < verts->GetNumEntries(); v++ )
            {
                glm::vec3 vert = verts->GetVertices()[ v ];
                uvs->AddAttribute( glm::vec2( vert.x*0.5 + 0.5,
                                                vert.z*0.5 + 0.5 ) ); // FIXME: scaling
            }
        }
    };
};


DefaultConePlugin::DefaultConePlugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model )
    : DefaultGeometryPluginBase( name, uid, prev, model ) 
{ 
    m_tesselation = QueryTypedValue< ValueIntPtr >( GetValue( PN::TESSELATION ) );
    m_innerHeight = QueryTypedValue< ValueFloatPtr >( GetValue( PN::INNERHEIGHT ) );
    m_innerRadius = QueryTypedValue< ValueFloatPtr >( GetValue( PN::INNERRADIUS ) );
    m_roundedTipHeight = QueryTypedValue< ValueFloatPtr >( GetValue( PN::ROUNDEDTIPHEIGHT ) );
    m_openAngle = QueryTypedValue< ValueFloatPtr >( GetValue( PN::OPENANGLE ) );
    m_height = QueryTypedValue< ValueFloatPtr >( GetValue( PN::HEIGHT ) );
    m_outerRadius = QueryTypedValue< ValueFloatPtr >( GetValue( PN::OUTERRADIUS ) );

    m_pluginParamValModel->Update();
    InitGeometry();
}

std::vector<IGeometryGeneratorPtr>    DefaultConePlugin::GetGenerators()
{
    ConeGenerator::Init( 
        m_tesselation->GetValue(),
        m_innerHeight->GetValue(),
        m_innerRadius->GetValue(),
        m_roundedTipHeight->GetValue(),
        m_openAngle->GetValue(),
        m_height->GetValue(),
        m_outerRadius->GetValue()
        );

    std::vector<IGeometryGeneratorPtr> gens;
    gens.push_back( IGeometryGeneratorPtr( new ConeGenerator::LateralSurface( ConeGenerator::height, ConeGenerator::outer_radius ) ) );
    gens.push_back( IGeometryGeneratorPtr( new ConeGenerator::BaseSurface() ) );
    gens.push_back( IGeometryGeneratorPtr( new ConeGenerator::LateralSurface( ConeGenerator::inner_height, ConeGenerator::inner_radius ) ) );
    return gens;
}

bool                                DefaultConePlugin::NeedsTopologyUpdate()
{
    return ParameterChanged( PN::TESSELATION ) ||
        ParameterChanged( PN::INNERHEIGHT ) ||
        ParameterChanged( PN::OUTERRADIUS ) ||
        ParameterChanged( PN::INNERHEIGHT ) ||
        ParameterChanged( PN::ROUNDEDTIPHEIGHT ) ||
        ParameterChanged( PN::OPENANGLE ) ||
        ParameterChanged( PN::INNERRADIUS );
}

} } }
