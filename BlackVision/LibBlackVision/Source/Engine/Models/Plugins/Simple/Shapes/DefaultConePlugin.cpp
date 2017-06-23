#include "stdafx.h"

#include "DefaultConePlugin.h"
#include "Mathematics/glm_inc.h"

#include "Memory/MemoryLeaks.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"


namespace bv { namespace model {
    
typedef ParamEnum< DefaultCone::DefaultConePlugin::OpenAngleMode > ParamEnumOAM;
typedef ParamEnum< DefaultCone::DefaultConePlugin::WeightCenter > ParamEnumWC;
typedef ParamEnum< DefaultCone::DefaultConePlugin::MappingType > ParamEnumMT;

VoidPtr    ParamEnumOAM::QueryParamTyped  ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}

template<>
static IParameterPtr        ParametersFactory::CreateTypedParameter< DefaultCone::DefaultConePlugin::OpenAngleMode >                 ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return CreateParameterEnum< DefaultCone::DefaultConePlugin::OpenAngleMode >( name, timeline );
}



VoidPtr    ParamEnumWC::QueryParamTyped  ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}

template<>
static IParameterPtr        ParametersFactory::CreateTypedParameter< DefaultCone::DefaultConePlugin::WeightCenter >                 ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return CreateParameterEnum< DefaultCone::DefaultConePlugin::WeightCenter >( name, timeline );
}

VoidPtr    ParamEnumMT::QueryParamTyped  ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}

template<>
static IParameterPtr        ParametersFactory::CreateTypedParameter< DefaultCone::DefaultConePlugin::MappingType >                 ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return CreateParameterEnum< DefaultCone::DefaultConePlugin::MappingType >( name, timeline );
}



namespace DefaultCone {

const std::string PN::TESSELATION = "tesselation";
const std::string PN::HEIGHT = "height";
const std::string PN::INNERRADIUS = "inner radius";
const std::string PN::OUTERRADIUS = "outer radius";
const std::string PN::INNERHEIGHT = "inner height";
const std::string PN::ROUNDEDTIPHEIGHT = "rounded tip height";
const std::string PN::OPENANGLE = "open angle";
const std::string PN::WEIGHTCENTERX = "weight center x";
const std::string PN::WEIGHTCENTERY = "weight center y";
const std::string PN::WEIGHTCENTERZ = "weight center z";
const std::string PN::OPENANGLEMODE = "open angle mode";
const std::string PN::BEVELTESSELATION = "bevel tesselation";
const std::string PN::MAPPINGTYPE = "mapping type";





DefaultConePluginDesc::DefaultConePluginDesc()
            : DefaultGeometryPluginDescBase( UID(), "cone" ) { }

DefaultPluginParamValModelPtr   DefaultConePluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper h( timeEvaluator );

    h.SetOrCreateVacModel();
    h.AddSimpleParam( PN::TESSELATION, 10, true, true );
    h.AddSimpleParam( PN::HEIGHT, 1.f, true, true );
    h.AddSimpleParam( PN::INNERHEIGHT, 0.6f, true, true );
    h.AddSimpleParam( PN::OUTERRADIUS, 1.f, true, true );
    h.AddSimpleParam( PN::INNERRADIUS, 0.3f, true, true );
    h.AddSimpleParam( PN::ROUNDEDTIPHEIGHT, 0.2f, true, true );
    h.AddSimpleParam( PN::OPENANGLE, 90.f, true, true );
    h.AddSimpleParam( PN::BEVELTESSELATION, 4, true, true );
    h.AddParam< IntInterpolator, DefaultConePlugin::OpenAngleMode, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumOAM >
        ( DefaultCone::PN::OPENANGLEMODE, DefaultConePlugin::OpenAngleMode::CW, true, true );
    h.AddParam< IntInterpolator, DefaultConePlugin::WeightCenter, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumWC >
        ( DefaultCone::PN::WEIGHTCENTERX, DefaultConePlugin::WeightCenter::CENTER, true, true );
    h.AddParam< IntInterpolator, DefaultConePlugin::WeightCenter, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumWC >
        ( DefaultCone::PN::WEIGHTCENTERY, DefaultConePlugin::WeightCenter::MIN, true, true );
    h.AddParam< IntInterpolator, DefaultConePlugin::WeightCenter, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumWC >
        ( DefaultCone::PN::WEIGHTCENTERZ, DefaultConePlugin::WeightCenter::CENTER, true, true );
    h.AddParam< IntInterpolator, DefaultConePlugin::MappingType, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumMT >
        ( PN::MAPPINGTYPE, DefaultConePlugin::MappingType::OLDSTYLE, true, true );

    h.SetOrCreatePSModel();

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
    DefaultConePlugin::OpenAngleMode open_angle_mode;
    DefaultConePlugin::WeightCenter weight_centerX;
    DefaultConePlugin::WeightCenter weight_centerY;
    DefaultConePlugin::WeightCenter weight_centerZ;
    DefaultConePlugin::MappingType mapping_type;

    static void Init( int t, float ih, float ir, float b, float oa, float h, float or, int bt, DefaultConePlugin::OpenAngleMode oam, DefaultConePlugin::WeightCenter wcx, DefaultConePlugin::WeightCenter wcy, DefaultConePlugin::WeightCenter wcz, DefaultConePlugin::MappingType mt )
    {
        tesselation = t;
        inner_height = ih;
        inner_radius = ir; 
        bevel = b; 
        open_angle = oa;
        height = h;
        outer_radius = or;
        bevel_tesselation = bt;
        open_angle_mode = oam;
        weight_centerX = wcx;
        weight_centerY = wcy;
        weight_centerZ = wcz;
        mapping_type = mt;
    }



    class LateralSurface : public IGeometryNormalsUVsGenerator
    {
    protected:
        float height, radius;
        glm::vec3 center_translate;
        double angle_offset;			// OpenAngleMode needs this
    public:
        LateralSurface( float h, float r ) : height( h ), radius( r ) { }

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

        //**This horible function adds two verticies on the end of the circuit, to avoid artefacts.*/
        void genCircuitRepairEnding( float R, float h, Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs, int i, bool direction )
        {
            // There's inversely, because in loop we have incremented it
            if( direction )
                i--;
            else
                i++;

            double angle1 = computeAngle2Clamped( TWOPI / tesselation, (float)i );
            angle1 += angle_offset;

            double cos_angle1 = cos( angle1 );
            double sin_angle1 = sin( angle1 );

            verts->AddAttribute( glm::vec3( R * cos_angle1, h, R * sin_angle1 ) + center_translate );
            verts->AddAttribute( glm::vec3( R * cos_angle1, h, R * sin_angle1 ) + center_translate );
        }

        /**This awfull function adds two additional points. This generates degenerated triangle to jump to the base surface. */
        void repairBaseSurface( float R, float Y, Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs, double angle )
        {
            double angle1 = angle;
            double cos_angle1 = cos( angle1 );
            double sin_angle1 = sin( angle1 );

            verts->AddAttribute( glm::vec3( R * cos_angle1, Y, R * sin_angle1 ) + center_translate );
            verts->AddAttribute( glm::vec3( R * cos_angle1, Y, R * sin_angle1 ) + center_translate );
        }

        /**Generates verticies of one circuit of the beveled edges.
        R1 and h1 describe position of top verticies of the strip,
        R2 and h2 describe position of bottom verticies of the strip.
        
        @param[inout] direction Because of OpenAngle, we can't generate verticies in continous circles. Thats why we change direction every circle.
        Function gets direction that will use to draw and in the same param returns direction for the next function.
        */
        void generateCircuit( float R1, float R2, float h1, float h2, Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs, bool& direction, bool inverse = false)
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

                verts->AddAttribute( glm::vec3( R1 * cos_angle1, h1, R1 * sin_angle1 ) + center_translate );
                verts->AddAttribute( glm::vec3( R2 * cos_angle1, h2, R2 * sin_angle1 ) + center_translate );

                if( direction )
                    i++;
                else
                    i--;
            }

            if( inverse )
                genCircuitRepairEnding( R2, h2, verts, uvs, i, direction );		/// horrible, but there's no other way to do this
            else
                genCircuitRepairEnding( R1, h1, verts, uvs, i, direction );		/// horrible, but there's no other way to do this

            direction = !direction;
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
        @param[in] inverse_angle Set true if you want to draw inner beveled edge.*/
        void generateBeveledEdge( glm::vec2 circle_center, double angle_between_edges, Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs, bool& direction, bool inverse_angle = false )
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

                generateCircuit( radius_height1.x, radius_height2.x, radius_height1.y, radius_height2.y, verts, uvs, direction, inverse_angle );

            }

        }

        glm::vec2 computeCorrection( glm::vec2 circle_center, double angle_between_edges, bool inverse = false )
        {
            double sum_of_angles = PI - angle_between_edges;			// Circle is tangent to edges, so we have to angles 90 degrees + alfa + sum_of_angles that we are looking for.
            
            glm::vec2 return_value;
            computeCircleRadiusHeight( return_value, circle_center, circle_center.y, sum_of_angles, inverse );
            return return_value;
        }

        void computeWeightCenter( DefaultConePlugin::WeightCenter centerX, DefaultConePlugin::WeightCenter centerY, DefaultConePlugin::WeightCenter centerZ )
        {
            center_translate = glm::vec3( 0.0f, 0.0f, 0.0f );

            if( centerX == DefaultConePlugin::WeightCenter::MAX )
                center_translate += glm::vec3( -outer_radius, 0.0, 0.0 );
            else if( centerX == DefaultConePlugin::WeightCenter::CENTER )
                center_translate += glm::vec3( 0.0, 0.0, 0.0 );
            else if( centerX == DefaultConePlugin::WeightCenter::MIN )
                center_translate += glm::vec3( outer_radius, 0.0, 0.0 );
    
            if( centerY == DefaultConePlugin::WeightCenter::MAX )
                center_translate += glm::vec3( 0.0f, -height, 0.0f );
            else if( centerY == DefaultConePlugin::WeightCenter::CENTER )
                center_translate += glm::vec3( 0.0f, -height / 2, 0.0f );
            else if( centerY == DefaultConePlugin::WeightCenter::MIN )
                center_translate += glm::vec3( 0.0f, 0.0f, 0.0f );

            if( centerZ == DefaultConePlugin::WeightCenter::MAX )
                center_translate += glm::vec3( 0.0, 0.0, -outer_radius );
            else if( centerZ == DefaultConePlugin::WeightCenter::CENTER )
                center_translate += glm::vec3( 0.0, 0.0, 0.0 );
            else if( centerZ == DefaultConePlugin::WeightCenter::MIN )
                center_translate += glm::vec3( 0.0, 0.0, outer_radius );
            
        }

        void computeAngleOffset()
        {
            if( open_angle_mode == DefaultConePlugin::OpenAngleMode::CCW )
                angle_offset = -3*PI / 2;
            else if( open_angle_mode == DefaultConePlugin::OpenAngleMode::CW )
                angle_offset = -3*PI / 2 + TO_RADIANS( open_angle );
            else if( open_angle_mode == DefaultConePlugin::OpenAngleMode::SYMMETRIC )
                angle_offset =  -3*PI / 2 + TO_RADIANS( open_angle ) / 2;
            else
                assert( false );
        }


        glm::vec2 compute_direction2D( Float3AttributeChannelPtr verts, unsigned int verts_index )
        {
            glm::vec3 vert = verts->GetVertices()[ verts_index ];
            vert -= center_translate;		// We need position before weightCenter was applied.

            glm::vec2 direction( vert.x, vert.z );
            if( direction == glm::vec2( 0.0, 0.0 ) )	//If vector is null, we cane take next element. It should be correct.
                return compute_direction2D( verts, verts_index + 1 );

            return glm::normalize( direction );
        }

        /**
        @param[in] centerUV Circle center on texture.
        @param[in] UVradius1 Radius of a cricle 1 on texture.
        @param[in] UVradius2 Radius of a cricle 2 on texture.
        @param[in] angle Angle between vertex (casted on plane XY) and direction [0.0,0.0,1.0).
        @param[inout] verts_index Index of first vertex in verts table that we begin with. Function returns index
        after last that was used.
        */
        void generateUVCircuit( glm::vec2 centerUV, double UVradius1, double UVradius2, Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs, unsigned int& verts_index )
        {
            glm::vec2 reference_direction( 0.0, 1.0 );
            double angle = 0.0;
            int max_loop;
            if( open_angle != 0.0 )
                max_loop = static_cast<int>( ceil( float( ( TWOPI - TO_RADIANS( open_angle ) ) / ( TWOPI / tesselation ) ) ) );
            else
                max_loop = tesselation;

            for( int j = 0; j <= max_loop; j++ )
            {
                glm::vec2 direction = compute_direction2D( verts, verts_index );
                angle = PI + glm::radians( glm::orientedAngle( direction, reference_direction ) );

                uvs->AddAttribute( centerUV - glm::vec2( UVradius1 * sin( angle ), UVradius1 * cos( angle ) ) );
                uvs->AddAttribute( centerUV - glm::vec2( UVradius2 * sin( angle ), UVradius2 * cos( angle ) ) );

                ++verts_index;
                ++verts_index;
            }

            // We add two uvs (function generateCircuit adds always two verticies at the end)
            uvs->AddAttribute( centerUV - glm::vec2( UVradius1 * sin( angle ), UVradius1 * cos( angle ) ) );
            uvs->AddAttribute( centerUV - glm::vec2( UVradius2 * sin( angle ), UVradius2 * cos( angle ) ) );
            ++verts_index;
            ++verts_index;
        }

        void generateGOODMAPPING( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs )
        {
            glm::vec2 circle_center( 0.25, 0.75 );
            const double circle_radiusUV = 0.25;
            unsigned int verts_index = 0;
            unsigned int bevel_count1 = bevel_tesselation / 2;
            unsigned int bevel_count2 = bevel_tesselation - bevel_count1;
            double bevel_radiusUV1 = circle_radiusUV * ( bevel / (sqrt( height * height + outer_radius * outer_radius) ) );
            double bevel_radiusUV2 = circle_radiusUV * ( bevel / outer_radius );
            double surface_radius1 = circle_radiusUV - bevel_radiusUV1;		// UV radius for lateral surface
            double surface_radius2 = circle_radiusUV - bevel_radiusUV2;		// UV radius for base surface

            generateUVCircuit( circle_center, 0.0f, surface_radius1, verts, uvs, verts_index );

            // Adding two repair surface verticies
            uvs->AddAttribute( glm::vec2( sin(computeAngle2Clamped(TWOPI, 1) + angle_offset), cos( computeAngle2Clamped(TWOPI, 1) + angle_offset) ) );	++verts_index;
            uvs->AddAttribute( glm::vec2( sin(computeAngle2Clamped(TWOPI, 1) + angle_offset), cos( computeAngle2Clamped(TWOPI, 1) + angle_offset) ) );	++verts_index;

            // UVs of beveled region
            if( bevel != 0.0 )
            {
                circle_center = glm::vec2( 0.25, 0.25 );
                double bevel_radius_step = bevel_radiusUV2 / (double)bevel_count2;
                
                for( unsigned int i = 0; i < bevel_count2; ++i )
                    generateUVCircuit(	circle_center,
                                        (i+1)*bevel_radius_step + surface_radius2,
                                        i*bevel_radius_step + surface_radius2,
                                        verts, uvs, verts_index );

                circle_center = glm::vec2( 0.25, 0.75 );
                bevel_radius_step = bevel_radiusUV1 / (double)bevel_count1;

                for( unsigned int i = bevel_count1; i > 0; --i )
                    generateUVCircuit(	circle_center,
                                        (i-1)*bevel_radius_step + surface_radius1,
                                        i*bevel_radius_step + surface_radius1,
                                        verts, uvs, verts_index );
            }

        // Base surface of the cone.
            circle_center = glm::vec2( 0.25, 0.25 );
            bevel_radiusUV1 = circle_radiusUV * ( outer_radius - bevel ) / outer_radius;
            if( inner_radius > 0.0 && inner_height > 0.0 )
                bevel_radiusUV2 = circle_radiusUV * ( inner_radius + bevel ) / outer_radius;
            else	// Since there will be no inner surface, we generate UVs to the circle center.
                bevel_radiusUV2 = 0.0;

            // Adding two repair surface verticies
            uvs->AddAttribute( glm::vec2( sin(angle_offset), cos( angle_offset) ) );	++verts_index;
            uvs->AddAttribute( glm::vec2( sin(angle_offset), cos( angle_offset) ) );	++verts_index;
            // Generating UVs for surface
            generateUVCircuit( circle_center, bevel_radiusUV2, bevel_radiusUV1, verts, uvs, verts_index );


            surface_radius1 = circle_radiusUV * inner_radius / outer_radius;
            bevel_radiusUV1 = circle_radiusUV * ( bevel / outer_radius );
            bevel_radiusUV2 = circle_radiusUV * ( bevel / (sqrt( inner_height * inner_height + inner_radius * inner_radius) ) );
            surface_radius2 = circle_radiusUV - bevel_radiusUV2;
            // UVs of beveled region
            if( bevel != 0.0 && inner_radius > 0.0 && inner_height > 0.0 )
            {
                circle_center = glm::vec2( 0.25, 0.25 );
                double bevel_radius_step = bevel_radiusUV1 / (double)bevel_count1;

                for( unsigned int i = bevel_count1; i > 0; --i )
                    generateUVCircuit(	circle_center,
                                        i*bevel_radius_step + surface_radius1,
                                        (i-1)*bevel_radius_step + surface_radius1,
                                        verts, uvs, verts_index );

                circle_center = glm::vec2( 0.75, 0.75 );
                bevel_radius_step = bevel_radiusUV2 / (double)bevel_count2;

                for( unsigned int i = bevel_count2; i > 0; --i )
                    generateUVCircuit(	circle_center,
                                        i*bevel_radius_step + surface_radius2,
                                        (i-1)*bevel_radius_step + surface_radius2,
                                        verts, uvs, verts_index );
            }
        // Inner cone
            circle_center = glm::vec2( 0.75, 0.75 );
            bevel_radiusUV1 = surface_radius2;
            bevel_radiusUV2 = 0.0;

            if( inner_radius > 0.0 && inner_height > 0.0 )
                generateUVCircuit( circle_center, bevel_radiusUV1, bevel_radiusUV2, verts, uvs, verts_index );
        }

        void GenerateUV( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs )
        {
            if( mapping_type == DefaultConePlugin::MappingType::GOODMAPPING )
                generateGOODMAPPING( verts, uvs );
            else if( mapping_type == DefaultConePlugin::MappingType::OLDSTYLE )
            {
                for( SizeType v = 0; v < verts->GetNumEntries(); v++ )
                {
                    glm::vec3 vert = verts->GetVertices()[ v ];
                    vert -= center_translate;
                    uvs->AddAttribute( glm::vec2( vert.x*0.5 + 0.5,
                                                    vert.y*0.5 + 0.5 ) ); // FIXME: scaling
                }
            }
            else
                assert( false );
        }

        virtual void GenerateGeometryNormalsUVs( Float3AttributeChannelPtr verts, Float3AttributeChannelPtr normals, Float2AttributeChannelPtr uvs )
        {
            computeWeightCenter( weight_centerX, weight_centerY, weight_centerZ );
            computeAngleOffset();
            bool gen_direction = true;

            // Prepare data for bevel. ( We need this to generate first latteral surface too, thats why it happens in this place.
            double angle_between_edges = atan2( height, outer_radius );

            glm::vec2 circleCenter;
            circleCenter.x = outer_radius - bevel;
            circleCenter.y = float( bevel * tan( angle_between_edges / 2 ) );

            glm::vec2 correction = computeCorrection( circleCenter, angle_between_edges );	// Data needed for lateral surface
            float correct_radius = correction.x;
            float correct_y = correction.y;

            // Add lateral surface
            generateCircuit( 0.0f, correct_radius, height, correct_y, verts, uvs, gen_direction );
            repairBaseSurface( outer_radius - bevel, 0.0, verts, uvs, computeAngle2Clamped(TWOPI, 1) + angle_offset );

            // Add bevel to cone (outer bevel)
            if( bevel != 0.0 )
                generateBeveledEdge( circleCenter, angle_between_edges, verts, uvs, gen_direction );

            // Base surface
            if( gen_direction )	// We add verticies to base surface depending on from which side we generate it.
                repairBaseSurface( inner_radius + bevel, 0.0, verts, uvs, angle_offset );
            else
                repairBaseSurface( inner_radius + bevel, 0.0, verts, uvs, computeAngle2Clamped(TWOPI, 1) + angle_offset );

            if( inner_radius > 0.0 && inner_height > 0.0 )	// If there's no inner cone, Base surface must close the solid.
                generateCircuit( inner_radius + bevel, outer_radius - bevel, 0.0f, 0.0f, verts, uvs, gen_direction );
            else
                generateCircuit( 0.0f, outer_radius - bevel, 0.0f, 0.0f, verts, uvs, gen_direction );

            // Add bevel to cone (inner bevel)
            angle_between_edges = atan2( inner_height, inner_radius );
            angle_between_edges = ( PI - angle_between_edges );

            circleCenter.x = inner_radius + bevel;
            circleCenter.y = float( bevel * tan( angle_between_edges / 2 ) );

            if( bevel != 0.0 && inner_radius > 0.0 && inner_height > 0.0  )
                generateBeveledEdge( circleCenter, angle_between_edges, verts, uvs, gen_direction, true );

            correction = computeCorrection( circleCenter, angle_between_edges, true );
            correct_radius = correction.x;
            correct_y = correction.y;

            if( inner_radius > 0.0 && inner_height > 0.0 )
                generateCircuit( correct_radius, 0.0f, correct_y, inner_height, verts, uvs, gen_direction );

            GenerateUV( verts, uvs );

            GeometryGeneratorHelper::GenerateNonWeightedNormalsFromTriangleStrips( verts, normals );
        }
    };


    class ConeClosure : public LateralSurface
    {
    private:
    public:
        ConeClosure( float h, float r ) : LateralSurface( h, r ) { }

        void generateHalfClosure( glm::vec2 circleCenter1, glm::vec2 circleCenter2, Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs, double rotation )
        {
            bool rotate = true;
            if( rotation == angle_offset )
                rotate = false;

            double angle_between_edges1 = atan2( height, outer_radius );

            glm::vec2 correction1 = computeCorrection( circleCenter1, angle_between_edges1 );
            float correct_radius1 = correction1.x;
            float correct_y1 = correction1.y;

            double angle_between_edges2 = atan2( inner_height, inner_radius );
            angle_between_edges2 = ( PI - angle_between_edges2 );

            glm::vec2 correction2 = computeCorrection( circleCenter2, angle_between_edges2, true );
            float correct_radius2 = correction2.x;
            float correct_y2 = correction2.y;

            //// In some situations inner_height is lower then beveled part of cone
            float in_height = glm::max( inner_height, correct_y1 );

            // Triangle from top to inner height
            verts->AddAttribute( glm::vec3( 0.0, height, 0.0 ) + center_translate );
            verts->AddAttribute( glm::vec3( outer_radius * ( ( height - in_height ) / height) * cos( rotation ), in_height, outer_radius * ( ( height - in_height ) / height) * sin( rotation ) ) + center_translate );
            verts->AddAttribute( glm::vec3( 0.0, in_height, 0.0 ) + center_translate );
            setUV( 0.0, height, rotate, uvs );
            setUV( outer_radius * ( ( height - in_height ) / height), in_height, rotate, uvs );
            setUV( 0.0, in_height, rotate, uvs );

            // Quad from inner height to bevel
            verts->AddAttribute( glm::vec3( correct_radius1 * cos( rotation ), correct_y1, correct_radius1 * sin( rotation ) ) + center_translate );
            verts->AddAttribute( glm::vec3( correct_radius2 * cos( rotation ), correct_y2, correct_radius2 * sin( rotation ) ) + center_translate );
            setUV( correct_radius1, correct_y1, rotate, uvs );
            setUV( correct_radius2, correct_y2, rotate, uvs );

            // From top of bevel to bottom of it

            //===================//
            float circle_radius1 = circleCenter1.y;						// Its true only for bottom edges of the cone.
            float circle_radius2 = circleCenter2.y;						// Its true only for bottom edges of the cone.

            glm::vec2 radius_height1;
            glm::vec2 radius_height2;

            double sum_of_angles1 = PI - angle_between_edges1;			// Circle is tangent to edges, so we have to angles 90 degrees + alfa + sum_of_angles that we are looking for.
            double delta_angle1 = sum_of_angles1 / bevel_tesselation;		// Used to increment angle

            double sum_of_angles2 = PI - angle_between_edges2;			// Circle is tangent to edges, so we have to angles 90 degrees + alfa + sum_of_angles that we are looking for.
            double delta_angle2 = sum_of_angles2 / bevel_tesselation;		// Used to increment angle

            for( int tess = 1; tess <= bevel_tesselation; ++tess)
            {
                double angle1;
                double angle2;
                angle1 = sum_of_angles1 - tess * delta_angle1;
                angle2 = sum_of_angles2 - tess * delta_angle2;
                
                computeCircleRadiusHeight( radius_height1, circleCenter1, circle_radius1, angle1 );
                computeCircleRadiusHeight( radius_height2, circleCenter2, circle_radius2, angle2, true );

                verts->AddAttribute( glm::vec3( radius_height1.x * cos( rotation ), radius_height1.y, radius_height1.x * sin( rotation ) ) + center_translate );
                setUV( radius_height1.x, radius_height1.y, rotate, uvs );
                verts->AddAttribute( glm::vec3( radius_height2.x * cos( rotation ), radius_height2.y, radius_height2.x * sin( rotation ) ) + center_translate );
                setUV( radius_height2.x, radius_height2.y, rotate, uvs );
            }

        }

        /**Sets uv coord for vertex. Function does something only if MappingType::GOODMAPPING is set.*/
        void setUV( float R, float H, bool rotated, Float2AttributeChannelPtr uvs )
        {
            if( mapping_type == DefaultConePlugin::MappingType::GOODMAPPING )
            {
                glm::vec2 referenceUV( 3.0 / 4.0, 0.0 );
                const double maxU = 1.0 / 4.0;
                const double maxV = 1.0 / 2.0;
                glm::vec2 uv( maxU * R / outer_radius, maxV * H / height );

                if( !rotated )	// Left or right surface
                    uv.x = -uv.x;

                uvs->AddAttribute( referenceUV + uv );
            }
        }

        void computeOLDSTYLEmapping( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs )
        {
            if( mapping_type == DefaultConePlugin::MappingType::OLDSTYLE )
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
            computeWeightCenter( weight_centerX, weight_centerY, weight_centerZ );
            computeAngleOffset();

            // Prepare data for bevel. ( We need this to generate first latteral surface too, thats why it happens in this place.
            double angle_between_edges = atan2( height, outer_radius );

            glm::vec2 circleCenter1;
            circleCenter1.x = outer_radius - bevel;
            circleCenter1.y = float( bevel * tan( angle_between_edges / 2 ) );
            
            // Add bevel to cone (inner bevel)
            angle_between_edges = atan2( inner_height, inner_radius );
            angle_between_edges = ( PI - angle_between_edges );

            glm::vec2 circleCenter2;
            circleCenter2.x = inner_radius + bevel;
            circleCenter2.y = float( bevel * tan( angle_between_edges / 2 ) );
            if( inner_height == 0.0 )
            {
                circleCenter2.x = 0.0;
                circleCenter2.y = 0.0;
            }

            generateHalfClosure( circleCenter1, circleCenter2, verts, uvs, angle_offset );

            double angle = computeAngle2Clamped( TWOPI, 1 );
            angle += angle_offset;

            glm::vec3 degenerate( 0.0, height, 0.0 );
            degenerate += center_translate;
            verts->AddAttribute( degenerate );			setUV( 0.0, height, false, uvs );
            verts->AddAttribute( degenerate );			setUV( 0.0, height, false, uvs );

            generateHalfClosure( circleCenter1, circleCenter2, verts, uvs, angle );

            computeOLDSTYLEmapping( verts, uvs );	// Computed only if mapping_type::OLDSTYLE is set.
        
            GeometryGeneratorHelper::GenerateNonWeightedNormalsFromTriangleStrips( verts, normals );
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
    m_openAngleMode = QueryTypedParam< std::shared_ptr< ParamEnum< OpenAngleMode > > >( GetParameter( PN::OPENANGLEMODE ) );
    m_weightCenterX = QueryTypedParam< std::shared_ptr< ParamEnum< WeightCenter > > >( GetParameter( PN::WEIGHTCENTERX ) );
    m_weightCenterY = QueryTypedParam< std::shared_ptr< ParamEnum< WeightCenter > > >( GetParameter( PN::WEIGHTCENTERY ) );
    m_weightCenterZ = QueryTypedParam< std::shared_ptr< ParamEnum< WeightCenter > > >( GetParameter( PN::WEIGHTCENTERZ ) );
    m_mappingType = QueryTypedParam< std::shared_ptr< ParamEnum< MappingType > > >( GetParameter( PN::MAPPINGTYPE ) );

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
        m_bevelTesselation->GetValue(),
        m_openAngleMode->Evaluate(),
        m_weightCenterX->Evaluate(),
        m_weightCenterY->Evaluate(),
        m_weightCenterZ->Evaluate(),
        m_mappingType->Evaluate()
        );

    std::vector< IGeometryGeneratorPtr > gens;
    gens.push_back( std::make_shared< ConeGenerator::LateralSurface >( ConeGenerator::height, ConeGenerator::outer_radius ) );
    if( m_openAngle->GetValue() > 0.0 )
    {
        gens.push_back( std::make_shared< ConeGenerator::ConeClosure >( ConeGenerator::height, ConeGenerator::outer_radius ) );
    }
    return gens;
}

bool                                DefaultConePlugin::NeedsTopologyUpdate()
{
    return ParameterChanged( PN::TESSELATION ) ||
        ParameterChanged( PN::HEIGHT ) ||
        ParameterChanged( PN::INNERHEIGHT ) ||
        ParameterChanged( PN::OUTERRADIUS ) ||
        ParameterChanged( PN::ROUNDEDTIPHEIGHT ) ||
        ParameterChanged( PN::OPENANGLE ) ||
        ParameterChanged( PN::INNERRADIUS ) ||
        ParameterChanged( PN::BEVELTESSELATION )||
        ParameterChanged( PN::OPENANGLEMODE ) ||
        ParameterChanged( PN::WEIGHTCENTERX ) ||
        ParameterChanged( PN::WEIGHTCENTERY ) ||
        ParameterChanged( PN::WEIGHTCENTERZ );
}

} } }
