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
const std::string PN::BEVELTESSELATION = "bevel tesselation";


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
    h.AddSimpleParam( PN::ROUNDEDTIPHEIGHT, 0.2f, true, true );
    h.AddSimpleParam( PN::OPENANGLE, 360.f, true, true );
	h.AddSimpleParam( PN::BEVELTESSELATION, 4, true, true );
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

    static void Init( int t, float ih, float ir, float b, float oa, float h, float or, int bt )
    {
        tesselation = t;
        inner_height = ih;
        inner_radius = ir; 
        bevel = b; 
        open_angle = oa;
        height = h;
        outer_radius = or;
		bevel_tesselation = bt;
    }

    class LateralSurface : public IGeometryAndUVsGenerator
    {
        float height, radius;
    public:
        LateralSurface( float h, float r ) : height( h ), radius( r ) { }

        virtual Type GetType() { return Type::GEOMETRY_AND_UVS; }

		/**Generates verticies of one circuit of the beveled edges.
		R1 and h1 describe position of top verticies of the strip,
		R2 and h2 describe position of bottom verticies of the strip.*/
		void generateCircuit( float R1, float R2, float h1, float h2, Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs  )
		{
			bool test = false;
			if( test )
				return;

			for( int i = 0; i < tesselation ; i++ )
            {
				double angle1 = i     * 2 * PI / tesselation;
				double angle2 = (i+1) * 2 * PI / tesselation;

				double cos_angle1 = cos( angle1 );
				double sin_angle1 = sin( angle1 );
				double cos_angle2 = cos( angle2 );
				double sin_angle2 = sin( angle2 );

				verts->AddAttribute( glm::vec3( R1 * cos_angle1, h1, R1 * sin_angle1 ) );
				//uvs->AddAttribute( glm::vec2( 0.0, 0.0 ) );		// Temp

				verts->AddAttribute( glm::vec3( R2 * cos_angle1, h2, R2 * sin_angle1 ) );
				//uvs->AddAttribute( glm::vec2( 0.0, 0.0 ) );		// Temp

				verts->AddAttribute( glm::vec3( R1 * cos_angle2, h1, R1 * sin_angle2 ) );
				//uvs->AddAttribute( glm::vec2( 0.0, 0.0 ) );		// Temp

				verts->AddAttribute( glm::vec3( R2 * cos_angle2, h2, R2 * sin_angle2 ) );
				//uvs->AddAttribute( glm::vec2( 0.0, 0.0 ) );		// Temp
			}
		}

		/**Computes radius and height of circle, relative to center of the cone, which forms beveled area.
		
		@param[in] angle Angle from bottom of the circle to the point we are thinking of.*/
		void computeCircleRadiusHeight( glm::vec2& returnValue, glm::vec2 circleCenter, float circleRadius, double angle, bool inverse = false )
		{
			if( inverse )
				returnValue.x = abs( circleCenter.x ) - float( circleRadius * sin( angle ) );
			else
				returnValue.x = abs( circleCenter.x ) + float( circleRadius * sin( angle ) );
			
			returnValue.y = circleCenter.y - float( circleRadius * cos( angle ) );
		}

		/**
		@param[in] circle_center Center of the circle that forms bevel.
		@param[in] angle_between_edges Angle between edges, that will be beveled.
		@param[out] verts Object, that will hold produced verticies.
		@param[out] uvs Object, that will hold produced UV coordinates.
		@param[in] inverse_angle Set true if you want to draw inner beveled edge.
		@return We have to shorten the edge of a cone. Thats why we return height of a bevel and radius on top.*/
		glm::vec2 generateBeveledEdge( glm::vec2 circle_center, double angle_between_edges, Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs, bool inverse_angle = false )
		{
			float circle_radius = circle_center.y;						// Its true only for bottom edges of the cone.
			double sum_of_angles = PI - angle_between_edges;			// Circle is tangent to edges, so we have to angles 90 degrees + alfa + sum_of_angles that we are looking for.
			double delta_angle = sum_of_angles / bevel_tesselation;		// Used to increment angle


			for( int tess = 0; tess < bevel_tesselation; ++tess)
			{
				double angle1;
				double angle2;
				if( inverse_angle )
				{
					angle2 = ( tess + 1 ) * delta_angle;	// Top verticies go as first
					angle1 = tess * delta_angle;
				}
				else
				{
					angle1 = ( tess + 1 ) * delta_angle;	// Top verticies go as first
					angle2 = tess * delta_angle;
				}

				glm::vec2 radius_height1;
				glm::vec2 radius_height2;

				computeCircleRadiusHeight( radius_height1, circle_center, circle_radius, angle1, inverse_angle );
				computeCircleRadiusHeight( radius_height2, circle_center, circle_radius, angle2, inverse_angle );

				generateCircuit( radius_height1.x, radius_height2.x, radius_height1.y, radius_height2.y, verts, uvs );

			}


			glm::vec2 return_value;
			computeCircleRadiusHeight( return_value, circle_center, circle_radius, sum_of_angles, inverse_angle );
			return return_value;
		}

        virtual void GenerateGeometryAndUVs( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs )
        {
			// Add bevel to cone (outer bevel)
			double angle_between_edges = atan2( height, outer_radius );

			glm::vec2 circleCenter;
			circleCenter.x = outer_radius - bevel;
			circleCenter.y = float( bevel * tan( angle_between_edges / 2 ) );

			glm::vec2 correction = generateBeveledEdge( circleCenter, angle_between_edges, verts, uvs );
			float correct_radius = correction.x;
			float correct_y = correction.y;

			// Add Lateral surface
			generateCircuit( 0.0f, correct_radius, height, correct_y, verts, uvs );

			// Base surface
			generateCircuit( outer_radius - bevel, inner_radius + bevel, 0.0f, 0.0f, verts, uvs );

			// Add bevel to cone (inner bevel)
			angle_between_edges = atan2( inner_height, inner_radius );
			angle_between_edges = ( PI - angle_between_edges );

			circleCenter.x = inner_radius + bevel;
			circleCenter.y = float( bevel * tan( angle_between_edges / 2 ) );

			correction = generateBeveledEdge( circleCenter, angle_between_edges, verts, uvs, true );

			correct_radius = correction.x;
			correct_y = correction.y;

			generateCircuit( correct_radius, 0.0f, correct_y, inner_height, verts, uvs );

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
	m_bevelTesselation = QueryTypedValue< ValueIntPtr >( GetValue( PN::BEVELTESSELATION ) );
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
        m_outerRadius->GetValue(),
		m_bevelTesselation->GetValue()
        );

    std::vector<IGeometryGeneratorPtr> gens;
    gens.push_back( IGeometryGeneratorPtr( new ConeGenerator::LateralSurface( ConeGenerator::height, ConeGenerator::outer_radius ) ) );
    //gens.push_back( IGeometryGeneratorPtr( new ConeGenerator::BaseSurface() ) );
    //gens.push_back( IGeometryGeneratorPtr( new ConeGenerator::LateralSurface( ConeGenerator::inner_height, ConeGenerator::inner_radius ) ) );
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
        ParameterChanged( PN::INNERRADIUS ) ||
		ParameterChanged( PN::BEVELTESSELATION );
}

} } }
