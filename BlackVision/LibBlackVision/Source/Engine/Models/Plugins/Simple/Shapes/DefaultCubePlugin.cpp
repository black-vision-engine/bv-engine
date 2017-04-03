#include "stdafx.h"

#include "DefaultCubePlugin.h"

#include "Memory/MemoryLeaks.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"


namespace bv { namespace model {
	
typedef ParamEnum< DefaultCube::Plugin::WeightCenter > ParamEnumWC;
typedef ParamEnum< DefaultCube::Plugin::MappingType > ParamEnumMT;
	

VoidPtr    ParamEnumWC::QueryParamTyped  ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}

template<>
static IParameterPtr        ParametersFactory::CreateTypedParameter< DefaultCube::Plugin::WeightCenter >                 ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return CreateParameterEnum< DefaultCube::Plugin::WeightCenter >( name, timeline );
}

VoidPtr    ParamEnumMT::QueryParamTyped  ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}

template<>
static IParameterPtr        ParametersFactory::CreateTypedParameter< DefaultCube::Plugin::MappingType >                 ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return CreateParameterEnum< DefaultCube::Plugin::MappingType >( name, timeline );
}

	
namespace DefaultCube {

const std::string PN::TESSELATION = "tesselation";
const std::string PN::DIMENSIONS = "dimensions";
const std::string PN::BEVEL = "bevel";
const std::string PN::WEIGHTCENTERX = "weight center x";
const std::string PN::WEIGHTCENTERY = "weight center y";
const std::string PN::WEIGHTCENTERZ = "weight center z";
const std::string PN::MAPPINGTYPE = "mapping type";

PluginDesc::PluginDesc()
    : DefaultGeometryPluginDescBase( UID(), "cube" )
{
}

DefaultPluginParamValModelPtr   PluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper h( timeEvaluator );

    h.SetOrCreateVacModel();
    h.AddSimpleParam( PN::BEVEL, 0.1f, true, true );
    h.AddSimpleParam( PN::DIMENSIONS, glm::vec3( 1, 1, 1 ), true, true );
    h.AddSimpleParam( PN::TESSELATION, 4, true, true );
	h.AddParam< IntInterpolator, Plugin::WeightCenter, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumWC >
        ( DefaultCube::PN::WEIGHTCENTERX, Plugin::WeightCenter::CENTER, true, true );
	h.AddParam< IntInterpolator, Plugin::WeightCenter, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumWC >
        ( DefaultCube::PN::WEIGHTCENTERY, Plugin::WeightCenter::MIN, true, true );
	h.AddParam< IntInterpolator, Plugin::WeightCenter, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumWC >
        ( DefaultCube::PN::WEIGHTCENTERZ, Plugin::WeightCenter::CENTER, true, true );
  	h.AddParam< IntInterpolator, Plugin::MappingType, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumMT >
		( PN::MAPPINGTYPE, Plugin::MappingType::OLDSTYLE, true, true );
    
    h.SetOrCreatePSModel();

    return h.GetModel();
}
    
IPluginPtr                      PluginDesc::CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< Plugin >( name, prev, timeEvaluator );
}

std::string                     PluginDesc::UID                 ()
{
    return "DEFAULT_CUBE";
}

#include "Mathematics/Defines.h"
namespace Generator
{
    int tesselation;
    float bevel;
    glm::vec3 dims;
	glm::vec3 center_translate;
	Plugin::MappingType mapping_type;


    template < typename T >
    int sign( T val )
    {
        return (T(0) < val) - (val < T(0));
    }

	glm::vec3 computeWeightCenter( Plugin::WeightCenter centerX, Plugin::WeightCenter centerY, Plugin::WeightCenter centerZ )
	{
		glm::vec3 centerTranslate = glm::vec3( 0.0f, 0.0f, 0.0f );

		if( centerX == Plugin::WeightCenter::MAX )
			centerTranslate += glm::vec3( -dims.x / 2, 0.0, 0.0 );
		else if( centerX == Plugin::WeightCenter::CENTER )
			centerTranslate += glm::vec3( 0.0, 0.0, 0.0 );
		else if( centerX == Plugin::WeightCenter::MIN )
			centerTranslate += glm::vec3( dims.x / 2, 0.0, 0.0 );
	
		if( centerY == Plugin::WeightCenter::MAX )
			centerTranslate += glm::vec3( 0.0f, -dims.y / 2, 0.0f );
		else if( centerY == Plugin::WeightCenter::CENTER )
			centerTranslate += glm::vec3( 0.0f, 0.0, 0.0f );
		else if( centerY == Plugin::WeightCenter::MIN )
			centerTranslate += glm::vec3( 0.0f, dims.y / 2, 0.0f );

		if( centerZ == Plugin::WeightCenter::MAX )
			centerTranslate += glm::vec3( 0.0, 0.0, -dims.z / 2 );
		else if( centerZ == Plugin::WeightCenter::CENTER )
			centerTranslate += glm::vec3( 0.0, 0.0, 0.0 );
		else if( centerZ == Plugin::WeightCenter::MIN )
			centerTranslate += glm::vec3( 0.0, 0.0, dims.z / 2 );
		
		return centerTranslate;
	}

	enum CubicMappingPlane
	{
		PLUS_X = 3,
		MINUS_X = 1,
		PLUS_Y = 0,
		MINUS_Y = 2,
		PLUS_Z = 4,
		MINUS_Z = 5
	};

	float choosePlane( glm::vec3 direction, CubicMappingPlane& plane, glm::vec2& remainingValues )
	{
		float max = 0;
		if( abs( direction.x ) > abs( direction.y ) )
		{
			if( abs( direction.x ) > abs( direction.z ) )
			{
				max = abs( direction.x );
				plane = direction.x < 0 ? CubicMappingPlane::MINUS_X : CubicMappingPlane::PLUS_X;
				remainingValues.x = direction.y;
				remainingValues.y = direction.z;
			}
			else
			{
				max = abs( direction.z );
				plane = direction.z < 0 ? CubicMappingPlane::MINUS_Z : CubicMappingPlane::PLUS_Z;
				remainingValues.x = direction.x;
				remainingValues.y = direction.y;
			}
		}
		else
		{
			if( abs( direction.y ) > abs( direction.z ) )
			{
				max = abs( direction.y );
				plane = direction.y < 0 ? CubicMappingPlane::MINUS_Y : CubicMappingPlane::PLUS_Y;
				remainingValues.x = direction.x;
				remainingValues.y = direction.z;
			}
			else
			{
				max = abs( direction.z );
				plane = direction.z < 0 ? CubicMappingPlane::MINUS_Z : CubicMappingPlane::PLUS_Z;
				remainingValues.x = direction.x;
				remainingValues.y = direction.y;
			}
		}
		return max;
	}

	glm::vec2 makeUV( glm::vec2 pre_uv_coords, CubicMappingPlane plane )
	{
		glm::vec2 uv_translate;
		glm::vec2 uv_coords = pre_uv_coords / glm::vec2( 4.0, 3.0 );

		if( plane == CubicMappingPlane::PLUS_X )
			uv_translate = glm::vec2( 0.0, 1.0 / 3.0 );
		else if( plane == CubicMappingPlane::MINUS_X )
			uv_translate = glm::vec2( 1.0 / 2.0, 1.0 / 3.0 );
		else if( plane == CubicMappingPlane::PLUS_Y )
			uv_translate = glm::vec2( 3.0 / 4.0, 1.0 / 3.0 );
		else if( plane == CubicMappingPlane::MINUS_Y )
			uv_translate = glm::vec2( 1.0 / 4.0, 1.0 / 3.0 );
		else if( plane == CubicMappingPlane::PLUS_Z )
			uv_translate = glm::vec2( 1.0 / 2.0, 0.0 );
		else if( plane == CubicMappingPlane::MINUS_Z )
			uv_translate = glm::vec2( 1.0 / 2.0, 2.0 / 3.0 );
		return uv_coords + uv_translate;
	}

	glm::vec2 computeUV( glm::vec3 position )
	{
		CubicMappingPlane plane;
		glm::vec2 uv_coords;
		float max_value = choosePlane( position, plane, uv_coords );

		uv_coords = uv_coords / glm::vec2( 2*max_value, 2*max_value );
		uv_coords += glm::vec2( 0.5, 0.5 );

		return makeUV( uv_coords, plane );
	}


    class SideComp : public IGeometryNormalsUVsGenerator
    {
        double d;
    public:

        void GenerateGeometryNormalsUVs( Float3AttributeChannelPtr verts, Float3AttributeChannelPtr normals, Float2AttributeChannelPtr uvs ) override
        {
			CubicMappingPlane mappingPlane;
			if( d < 0 )
				mappingPlane = CubicMappingPlane::MINUS_Z;
			else
				mappingPlane = CubicMappingPlane::PLUS_Z;

            double w = dims.x/2 - bevel, 
                h = dims.y/2 - bevel;


			if( d > 0 )
			{
                verts->AddAttribute( glm::vec3( -w,  h, d ) + center_translate );   normals->AddAttribute( glm::vec3( 0.0, 0.0, 1.0 ) );
                verts->AddAttribute( glm::vec3( -w, -h, d ) + center_translate );   normals->AddAttribute( glm::vec3( 0.0, 0.0, 1.0 ) );
                verts->AddAttribute( glm::vec3(  w,  h, d ) + center_translate );   normals->AddAttribute( glm::vec3( 0.0, 0.0, 1.0 ) );
                verts->AddAttribute( glm::vec3(  w, -h, d ) + center_translate );   normals->AddAttribute( glm::vec3( 0.0, 0.0, 1.0 ) );
			}
			else
			{
                verts->AddAttribute( glm::vec3(  w,  h, d ) + center_translate );   normals->AddAttribute( glm::vec3( 0.0, 0.0, -1.0 ) );
                verts->AddAttribute( glm::vec3(  w, -h, d ) + center_translate );   normals->AddAttribute( glm::vec3( 0.0, 0.0, -1.0 ) );
                verts->AddAttribute( glm::vec3( -w,  h, d ) + center_translate );   normals->AddAttribute( glm::vec3( 0.0, 0.0, -1.0 ) );
                verts->AddAttribute( glm::vec3( -w, -h, d ) + center_translate );   normals->AddAttribute( glm::vec3( 0.0, 0.0, -1.0 ) );
			}


			float bevelUV1 = bevel / dims.x;
			float bevelUV2 = bevel / dims.y;
			glm::vec2 preUV1 = glm::vec2( 1 - bevelUV2, 1 - bevelUV1 );
			glm::vec2 preUV2 = glm::vec2( bevelUV2, 1 - bevelUV1 );
			glm::vec2 preUV3 = glm::vec2( 1 - bevelUV2, bevelUV1 );
			glm::vec2 preUV4 = glm::vec2( bevelUV2, bevelUV1 );


			uvs->AddAttribute( makeUV( preUV1, mappingPlane ) );
			uvs->AddAttribute( makeUV( preUV2, mappingPlane ) );
			uvs->AddAttribute( makeUV( preUV3, mappingPlane ) );
			uvs->AddAttribute( makeUV( preUV4, mappingPlane ) );
                
            //GeometryGeneratorHelper::GenerateNonWeightedNormalsFromTriangleStrips( verts, normals );
        }

        SideComp( double d_ ) : d( d_ ) { }
    };

    class MainComp : public IGeometryNormalsUVsGenerator
    {
        glm::vec3 **v;
        glm::vec3 **norm;
        glm::vec2 **coords;
        int n, m;
    public:

        void GenerateGeometryNormalsUVs( Float3AttributeChannelPtr verts, Float3AttributeChannelPtr normals, Float2AttributeChannelPtr uvs ) override
        {
            Init();
            GenerateV();
            CopyV( verts, uvs, normals );
            Deinit();

            //GeometryGeneratorHelper::GenerateNonWeightedNormalsFromTriangleStrips( verts, normals );
        }

        void Init() 
        {
            // It's a little hackisch. We should implement constraints in paramters.
			if( tesselation < 2 )
				tesselation = 2;

            n = 4 * ( tesselation + 1 );
            m = ( tesselation + 1 ) * 2;
            assert( n >= 0 );

            v = new glm::vec3*[ n ];
            norm = new glm::vec3*[ n ];
            coords = new glm::vec2*[ n + 2 ];

            for( int i = 0; i < n; i++ )
            {
                v[ i ] = new glm::vec3[ m ];
                norm[ i ] = new glm::vec3[ m ];
                coords[ i ] = new glm::vec2[ m + 2 ];       // Check UVs generation strategy to understand n + 2.
            }
        }

        void Deinit()
        {
            for( int i = 0; i < n; i++ )
            {
                delete[] v[ i ];
                delete[] norm[ i ];
                delete[] coords[ i ];
            }
            delete[] v;
            delete[] norm;
            delete[] coords;
        }

		glm::vec2 getUV( float bevel1, float bevel2, bool inverseU, bool inverseV )
		{
			float u = bevel1;
			float v = bevel2;
			if( inverseU )
				u = 1 - u;
			if( inverseV )
				v = 1 - v;
			return glm::vec2(u, v);
		}

		/**@brief Gives UVs in parameter, good orientation in texture space
		depending on face, that we compute currently.
		@param[in] z MINUS_Z lub PLUS_Z.*/
		glm::vec2 uvToZPlaneSpace( glm::vec2 uv, int face, CubicMappingPlane z )
		{
			float u;
			float v;
			z;

			if( face == 0 )
				u = 1 - uv.y, v = uv.x;
			else if( face == 1 )
				u = uv.x, v = uv.y;
			else if( face == 2 )
				u = uv.y, v = 1 - uv.x;
			else if( face == 3 )
				u = 1 - uv.x, v = 1 - uv.y;
			else
				u = 0, v = 0;	// Error

			if( z == CubicMappingPlane::PLUS_Z )
				v = 1 - v;

			return glm::vec2( u, v );
		}

		float compute_scaled_k( float bevelUV, int main_plane_tess, float bevel_step, int k, int j )
		{
			float k_scale = float(main_plane_tess - j) / (float)main_plane_tess;
			return bevelUV - ( bevelUV - bevel_step * k ) * k_scale;
		}

		void generateLineUV( int face, int k, bool inverse, Float2AttributeChannelPtr uvs )
		{
			int mainPlaneTess = tesselation / 2;
			int remainPlaneTess = tesselation - mainPlaneTess;

			float dim1;
			float dim3;
			if( face == 0 || face == 2 )
				dim1 = dims.y, dim3 = dims.x;
			else
				dim1 = dims.x, dim3 = dims.y;

			float bevelUV1 = bevel / dim1;
			float bevel_step1 = bevelUV1 / mainPlaneTess;

			float bevelUV2 = bevel / dims.z;
			float bevel_step2 = bevelUV2 / remainPlaneTess;

			float bevelUV3 = bevel / dim3;
			float bevel_step3 = bevelUV3 / mainPlaneTess;

			glm::vec2 preUV1;
			glm::vec2 preUV2;

			for( int j = mainPlaneTess; j >= 0; --j )
			{
				float k_step1 = compute_scaled_k( bevelUV3, mainPlaneTess, bevel_step3, k, j );
				float k_step2;
				if( inverse )
					k_step2 = compute_scaled_k( bevelUV3, mainPlaneTess, bevel_step3, k+1, j );
				else
					k_step2 = compute_scaled_k( bevelUV3, mainPlaneTess, bevel_step3, k-1, j );

				preUV1 = getUV( k_step1, bevel_step1 * j, inverse, false );
				preUV2 = getUV( k_step2, bevel_step1 * j, inverse, false );
				preUV1 = uvToZPlaneSpace( preUV1, face, CubicMappingPlane::MINUS_Z );
				preUV2 = uvToZPlaneSpace( preUV2, face, CubicMappingPlane::MINUS_Z );

				uvs->AddAttribute( makeUV( preUV1, CubicMappingPlane::MINUS_Z ) );
				uvs->AddAttribute( makeUV( preUV2, CubicMappingPlane::MINUS_Z ) );
			}

			for( int j = 0; j <= remainPlaneTess; ++j )
			{
				preUV1 = getUV( bevel_step3 * k, bevel_step2 * j, inverse, true );
				preUV2;
				if( inverse )
					preUV2 = getUV( bevel_step3 * (k+1), bevel_step2 * j, inverse, true );
				else
					preUV2 = getUV( bevel_step3 * (k-1), bevel_step2 * j, inverse, true );
				uvs->AddAttribute( makeUV( preUV1, static_cast<CubicMappingPlane>(face) ) );
				uvs->AddAttribute( makeUV( preUV2, static_cast<CubicMappingPlane>(face) ) );
			}


			for( int j = remainPlaneTess; j >= 0; --j )
			{
				preUV1 = getUV( bevel_step3 * k, bevel_step2 * j, inverse, false );
				preUV2;
				if( inverse )
					preUV2 = getUV( bevel_step3 * (k+1), bevel_step2 * j, inverse, false );
				else
					preUV2 = getUV( bevel_step3 * (k-1), bevel_step2 * j, inverse, false );
				uvs->AddAttribute( makeUV( preUV1, static_cast<CubicMappingPlane>(face) ) );
				uvs->AddAttribute( makeUV( preUV2, static_cast<CubicMappingPlane>(face) ) );
			}

			for( int j = 0; j <= mainPlaneTess; ++j )
			{
				float k_step1 = compute_scaled_k( bevelUV3, mainPlaneTess, bevel_step3, k, j );
				float k_step2;
				if( inverse )
					k_step2 = compute_scaled_k( bevelUV3, mainPlaneTess, bevel_step3, k+1, j );
				else
					k_step2 = compute_scaled_k( bevelUV3, mainPlaneTess, bevel_step3, k-1, j );
				preUV1 = getUV( k_step1, bevel_step1 * j, inverse, false );
				preUV2 = getUV( k_step2, bevel_step1 * j, inverse, false );
				preUV1 = uvToZPlaneSpace( preUV1, face, CubicMappingPlane::PLUS_Z );
				preUV2 = uvToZPlaneSpace( preUV2, face, CubicMappingPlane::PLUS_Z );
				preUV1 = makeUV( preUV1, CubicMappingPlane::PLUS_Z );
				preUV2 = makeUV( preUV2, CubicMappingPlane::PLUS_Z );
				uvs->AddAttribute( preUV1 );
				uvs->AddAttribute( preUV2 );
			}

			// Degenerated triangle
			uvs->AddAttribute( preUV2 );
            uvs->AddAttribute( preUV2 );    // This should be uv coord from next line begin
		}

		void generateBigLine( int face, Float2AttributeChannelPtr uvs )
		{
			int mainPlaneTess = tesselation / 2;
			int remainPlaneTess = tesselation - mainPlaneTess;

			float dim1;
			float dim3;
			if( face == 0 || face == 2 )
				dim1 = dims.y, dim3 = dims.x;
			else
				dim1 = dims.x, dim3 = dims.y;

			float bevelUV1 = bevel / dim1;
			float bevelStep1 = bevelUV1 / mainPlaneTess;

			float bevelUV2 = bevel / dims.z;
			float bevelStep2 = bevelUV2 / remainPlaneTess;

			float bevelUV3 = bevel / dim3;
			//float bevelStep3 = bevelUV3 / mainPlaneTess;

			glm::vec2 preUV1;
			glm::vec2 preUV2;

			for( int j = mainPlaneTess; j >= 0; --j )
			{
				preUV1 = getUV( bevelUV3, bevelStep1 * j, true, false );
				preUV2 = getUV( bevelUV3, bevelStep1 * j, false, false );
				preUV1 = uvToZPlaneSpace( preUV1, face, CubicMappingPlane::MINUS_Z );
				preUV2 = uvToZPlaneSpace( preUV2, face, CubicMappingPlane::MINUS_Z );
				uvs->AddAttribute( makeUV( preUV1, CubicMappingPlane::MINUS_Z ) );
				uvs->AddAttribute( makeUV( preUV2, CubicMappingPlane::MINUS_Z ) );
			}

			for( int j = 0; j <= remainPlaneTess; ++j )
			{
				preUV1 = getUV( bevelUV3, bevelStep2 * j, true, true );
				preUV2 = getUV( bevelUV3, bevelStep2 * j, false, true );
				uvs->AddAttribute( makeUV( preUV1, static_cast<CubicMappingPlane>(face) ) );
				uvs->AddAttribute( makeUV( preUV2, static_cast<CubicMappingPlane>(face) ) );
			}


			for( int j = remainPlaneTess; j >= 0; --j )
			{
				preUV1 = getUV( bevelUV3, bevelStep2 * j, true, false );
				preUV2 = getUV( bevelUV3, bevelStep2 * j, false, false );
				uvs->AddAttribute( makeUV( preUV1, static_cast<CubicMappingPlane>(face) ) );
				uvs->AddAttribute( makeUV( preUV2, static_cast<CubicMappingPlane>(face) ) );
			}
			for( int j = 0; j <= mainPlaneTess; ++j )
			{
				preUV1 = getUV( bevelUV3, bevelStep1 * j, true, false );
				preUV2 = getUV( bevelUV3, bevelStep1 * j, false, false );
				preUV1 = uvToZPlaneSpace( preUV1, face, CubicMappingPlane::PLUS_Z );
				preUV2 = uvToZPlaneSpace( preUV2, face, CubicMappingPlane::PLUS_Z );
				preUV1 = makeUV( preUV1, CubicMappingPlane::PLUS_Z );
				preUV2 = makeUV( preUV2, CubicMappingPlane::PLUS_Z );
				uvs->AddAttribute( preUV1 );
				uvs->AddAttribute( preUV2 );
			}

			// Degenerated triangle
			uvs->AddAttribute( preUV2 );
            uvs->AddAttribute( preUV2 );    // This should be uv coord from next line begin
		}
		
        void        GenerateBevelLineUV      ( int i, int face, int k, bool inverse )
        {
            int mainPlaneTess = tesselation / 2;
            int remainPlaneTess = tesselation - mainPlaneTess;

            float dim1;
            float dim3;
            if( face == 0 || face == 2 )
                dim1 = dims.y, dim3 = dims.x;
            else
                dim1 = dims.x, dim3 = dims.y;

            float bevelUV1 = bevel / dim1;
            float bevel_step1 = bevelUV1 / mainPlaneTess;

            float bevelUV2 = bevel / dims.z;
            float bevel_step2 = bevelUV2 / remainPlaneTess;

            float bevelUV3 = bevel / dim3;
            float bevel_step3 = bevelUV3 / mainPlaneTess;

            glm::vec2 preUV1;
            glm::vec2 preUV2;

            int base = 0;
            for( int j = mainPlaneTess; j >= 0; --j )
            {
                float k_step1 = compute_scaled_k( bevelUV3, mainPlaneTess, bevel_step3, k, j );
                float k_step2;
                if( inverse )
                    k_step2 = compute_scaled_k( bevelUV3, mainPlaneTess, bevel_step3, k + 1, j );
                else
                    k_step2 = compute_scaled_k( bevelUV3, mainPlaneTess, bevel_step3, k - 1, j );

                preUV1 = getUV( k_step1, bevel_step1 * j, inverse, false );
                preUV2 = getUV( k_step2, bevel_step1 * j, inverse, false );
                preUV1 = uvToZPlaneSpace( preUV1, face, CubicMappingPlane::MINUS_Z );
                preUV2 = uvToZPlaneSpace( preUV2, face, CubicMappingPlane::MINUS_Z );

                coords[ i ][ base + j ] = makeUV( preUV1, CubicMappingPlane::MINUS_Z );
                //coords[ i ][ j + 1 ] = makeUV( preUV2, CubicMappingPlane::MINUS_Z );

                //uvs->AddAttribute( makeUV( preUV1, CubicMappingPlane::MINUS_Z ) );
                //uvs->AddAttribute( makeUV( preUV2, CubicMappingPlane::MINUS_Z ) );
            }

            base += mainPlaneTess + 1;
            for( int j = 0; j <= remainPlaneTess; ++j )
            {
                preUV1 = getUV( bevel_step3 * k, bevel_step2 * j, inverse, true );
                preUV2;
                if( inverse )
                    preUV2 = getUV( bevel_step3 * ( k + 1 ), bevel_step2 * j, inverse, true );
                else
                    preUV2 = getUV( bevel_step3 * ( k - 1 ), bevel_step2 * j, inverse, true );

                coords[ i ][ base + j ] = makeUV( preUV1, static_cast< CubicMappingPlane >( face ) );
                //coords[ i ][ j + 1 ] = makeUV( preUV2, static_cast< CubicMappingPlane >( face ) );
            }


            base += remainPlaneTess + 1;
            for( int j = remainPlaneTess; j >= 0; --j )
            {
                preUV1 = getUV( bevel_step3 * k, bevel_step2 * j, inverse, false );
                preUV2;
                if( inverse )
                    preUV2 = getUV( bevel_step3 * ( k + 1 ), bevel_step2 * j, inverse, false );
                else
                    preUV2 = getUV( bevel_step3 * ( k - 1 ), bevel_step2 * j, inverse, false );

                coords[ i ][ base + j ] = makeUV( preUV1, static_cast< CubicMappingPlane >( face ) );
                //coords[ i ][ j + 1 ] = makeUV( preUV2, static_cast< CubicMappingPlane >( face ) );
            }

            base += remainPlaneTess + 1;
            for( int j = 0; j <= mainPlaneTess; ++j )
            {
                float k_step1 = compute_scaled_k( bevelUV3, mainPlaneTess, bevel_step3, k, j );
                float k_step2;
                if( inverse )
                    k_step2 = compute_scaled_k( bevelUV3, mainPlaneTess, bevel_step3, k + 1, j );
                else
                    k_step2 = compute_scaled_k( bevelUV3, mainPlaneTess, bevel_step3, k - 1, j );
                preUV1 = getUV( k_step1, bevel_step1 * j, inverse, false );
                preUV2 = getUV( k_step2, bevel_step1 * j, inverse, false );
                preUV1 = uvToZPlaneSpace( preUV1, face, CubicMappingPlane::PLUS_Z );
                preUV2 = uvToZPlaneSpace( preUV2, face, CubicMappingPlane::PLUS_Z );
                preUV1 = makeUV( preUV1, CubicMappingPlane::PLUS_Z );
                preUV2 = makeUV( preUV2, CubicMappingPlane::PLUS_Z );

                coords[ i ][ base + j ] = preUV1;
                //coords[ i ][ j + 1 ] = preUV2;
            }

        }

        void        GenerateFaceUVs     ( int face )
        {
            // Tesselated bevel consists of two regions. First regions maps UVs from main face, that we got in
            // function parameter. Second region takes UVs from face following this face.
            // We map half of tesselated lines into our main face. Remaining tesselation goes to next face.
            int mainFaceTess = tesselation / 2;
            int followingFaceTess = tesselation - mainFaceTess;

            // Each face is built of tesselation + 1 lines.
            int lineIdx = ( tesselation + 1 ) * face;

            // Cube is generated as big rectangular plane in the middle and small connected lines on bevel part.
            // This function generates Uvs for this main plane.
            GenerateMainFaceUVs( face );

            // First lineIdx in face was used for main face.
            lineIdx++;

            for( int k = mainFaceTess; k > 0; --k )
                GenerateBevelLineUV( followingFaceTess + lineIdx + mainFaceTess - k, face, k, false );
            for( int k = 0; k < followingFaceTess; ++k )
                GenerateBevelLineUV( lineIdx + k, ( face + 1 ) % 4, k, true );
        }

        void        GenerateMainFaceUVs( int face )
        {
            int mainPlaneTess = tesselation / 2;
            int remainPlaneTess = tesselation - mainPlaneTess;

            // Each face is built of tesselation + 1 lines.
            int lineIdx = ( tesselation + 1 ) * face;

            float dim1;
            float dim3;
            if( face == 0 || face == 2 )
                dim1 = dims.y, dim3 = dims.x;
            else
                dim1 = dims.x, dim3 = dims.y;

            float bevelUV1 = bevel / dim1;
            float bevelStep1 = bevelUV1 / mainPlaneTess;

            float bevelUV2 = bevel / dims.z;
            float bevelStep2 = bevelUV2 / remainPlaneTess;

            float bevelUV3 = bevel / dim3;
            //float bevelStep3 = bevelUV3 / mainPlaneTess;

            glm::vec2 preUV1;
            glm::vec2 preUV2;

            int base = 0;
            for( int j = mainPlaneTess; j >= 0; --j )
            {
                preUV1 = getUV( bevelUV3, bevelStep1 * j, true, false );
                preUV2 = getUV( bevelUV3, bevelStep1 * j, false, false );
                preUV1 = uvToZPlaneSpace( preUV1, face, CubicMappingPlane::MINUS_Z );
                preUV2 = uvToZPlaneSpace( preUV2, face, CubicMappingPlane::MINUS_Z );

                coords[ lineIdx ][ base + j ] = makeUV( preUV1, CubicMappingPlane::MINUS_Z );
            }


            base += mainPlaneTess + 1;
            for( int j = 0; j <= remainPlaneTess; ++j )
            {
                preUV1 = getUV( bevelUV3, bevelStep2 * j, true, true );
                preUV2 = getUV( bevelUV3, bevelStep2 * j, false, true );

                coords[ lineIdx ][ base + j ] = makeUV( preUV1, static_cast<CubicMappingPlane>( face ) );
            }

            base += remainPlaneTess + 1;
            for( int j = remainPlaneTess; j >= 0; --j )
            {
                preUV1 = getUV( bevelUV3, bevelStep2 * j, true, false );
                preUV2 = getUV( bevelUV3, bevelStep2 * j, false, false );

                coords[ lineIdx ][ base + j ] = makeUV( preUV1, static_cast<CubicMappingPlane>( face ) );
            }

            base += remainPlaneTess + 1;
            for( int j = 0; j <= mainPlaneTess; ++j )
            {
                preUV1 = getUV( bevelUV3, bevelStep1 * j, true, false );
                preUV2 = getUV( bevelUV3, bevelStep1 * j, false, false );
                preUV1 = uvToZPlaneSpace( preUV1, face, CubicMappingPlane::PLUS_Z );
                preUV2 = uvToZPlaneSpace( preUV2, face, CubicMappingPlane::PLUS_Z );
                preUV1 = makeUV( preUV1, CubicMappingPlane::PLUS_Z );
                preUV2 = makeUV( preUV2, CubicMappingPlane::PLUS_Z );

                coords[ lineIdx ][ base + j ] = preUV1;
            }
        }


		void generatePartUV( int face, Float2AttributeChannelPtr uvs )
		{
			int mainPlaneTess = tesselation / 2;
			int remainPlaneTess = tesselation - mainPlaneTess;

			// Big plane and things around
			generateBigLine( face, uvs );

			for( int k = mainPlaneTess; k > 0; --k )
				generateLineUV( face, k, false, uvs );
			for( int k = 0; k < remainPlaneTess; ++k )
				generateLineUV( ( face + 1 ) % 4, k, true, uvs );
			
		}


        void CopyV( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs, Float3AttributeChannelPtr normals )
        {
			for( int face = 0; face < 4; ++face )
                GenerateFaceUVs( face );

            int mainFaceTess = tesselation / 2;
            int followingFaceTess = tesselation - mainFaceTess;


            for( int i = 0; i < n - 1; i++ )
            {
                int uvsIdx = 0;

                for( int j = 0; j < m; j++ )
                {
                    verts->AddAttribute( v[ i ][ j ] + center_translate );      normals->AddAttribute( norm[ i ][ j ] );            uvs->AddAttribute( coords[ i ][ uvsIdx ] );
                    verts->AddAttribute( v[ i + 1 ][ j ] + center_translate );  normals->AddAttribute( norm[ i + 1 ][ j ] );        uvs->AddAttribute( coords[ i + 1 ][ uvsIdx ] );

                    if( j == mainFaceTess || j == tesselation + 1 + followingFaceTess )
                    {
                        uvsIdx++;

                        verts->AddAttribute( v[ i ][ j ] + center_translate );      normals->AddAttribute( norm[ i ][ j ] );        uvs->AddAttribute( coords[ i ][ uvsIdx ] );
                        verts->AddAttribute( v[ i + 1 ][ j ] + center_translate );  normals->AddAttribute( norm[ i + 1 ][ j ] );    uvs->AddAttribute( coords[ i + 1 ][ uvsIdx ] );
                    }

                    uvsIdx++;
                }
                // Degenerated triangle
                verts->AddAttribute( v[ i + 1 ][ m - 1 ] + center_translate );    normals->AddAttribute( norm[ i + 1 ][ m - 1 ] );  uvs->AddAttribute( coords[ i + 1 ][ m - 1 ] );
                verts->AddAttribute( v[ i + 1 ][ 0 ] + center_translate );        normals->AddAttribute( norm[ i + 1 ][ 0 ] );      uvs->AddAttribute( coords[ i + 1 ][ 0 ] );
            }

            int uvsIdx = 0;
            for( int j = 0; j < m; j++ )
            {
                verts->AddAttribute( v[ n - 1 ][ j ] + center_translate );      normals->AddAttribute( norm[ n - 1 ][ j ] );        uvs->AddAttribute( coords[ n - 1 ][ uvsIdx ] );
                verts->AddAttribute( v[ 0 ][ j ] + center_translate );          normals->AddAttribute( norm[ 0 ][ j ] );            uvs->AddAttribute( coords[ 0 ][ uvsIdx ] );

                if( j == mainFaceTess || j == tesselation + 1 + followingFaceTess )
                {
                    uvsIdx++;

                    verts->AddAttribute( v[ n - 1 ][ j ] + center_translate );  normals->AddAttribute( norm[ n - 1 ][ j ] );        uvs->AddAttribute( coords[ n - 1 ][ uvsIdx ] );
                    verts->AddAttribute( v[ 0 ][ j ] + center_translate );      normals->AddAttribute( norm[ 0 ][ j ] );            uvs->AddAttribute( coords[ 0 ][ uvsIdx ] );
                }

                uvsIdx++;
            }

            // Degenerated triangle
            verts->AddAttribute( v[ 0 ][ 0 ] + center_translate );          normals->AddAttribute( norm[ 0 ][ 0 ] );            uvs->AddAttribute( coords[ 0 ][ 0 ] );
            verts->AddAttribute( v[ 0 ][ 0 ] + center_translate );          normals->AddAttribute( norm[ 0 ][ 0 ] );            uvs->AddAttribute( coords[ 0 ][ 0 ] );

        }

        void GenerateLine( int i, double x, double y, double a )
        {
            double d = dims.z/2 - bevel;
            double b = bevel;

            double sinA = sin( a );
            double cosA = cos( a );
            
            for( int j = 0; j <= tesselation; j++ )
            {
                double angle2 = j * PI/2 / tesselation;

                double sinAngle2 = sin( angle2 );
                double cosAngle2 = cos( angle2 );
                
                v[ i ][ j ] = glm::vec3( x - b * sinA * sinAngle2, y + b * cosA * sinAngle2, -d - b * cosAngle2 );
                norm[ i ][ j ] = glm::normalize( glm::vec3( -sinA * sinAngle2, cosA * sinAngle2, -cosAngle2 ) );
            }
            for( int j = 0; j <= tesselation; j++ )
            {
                double angle2 = j * PI/2 / tesselation + PI/2;

                double sinAngle2 = sin( angle2 );
                double cosAngle2 = cos( angle2 );
                
                v[ i ][ tesselation + 1 + j ] = glm::vec3( x - b * sinA * sinAngle2, y + b * cosA * sinAngle2,  d - b * cosAngle2 );
                norm[ i ][ tesselation + 1 + j ] = glm::normalize( glm::vec3( -sinA * sinAngle2, cosA * sinAngle2, -cosAngle2 ) );
            }
        }

        void GenerateV()
        {
            double w = dims.x/2 - bevel, 
                h = dims.y/2 - bevel;
            int t = tesselation;

// top
            GenerateLine( 0, w, h, 0. );
            for( int i = 0; i < tesselation; i++ )
            {
                double angle = i * PI / 2 / ( tesselation - 1 );
                GenerateLine( 1 + i, -w,  h, angle );
            }
// left
            GenerateLine( t+1, -w,  h, PI/2 );
            for( int i = 0; i < tesselation; i++ )
            {
                double angle = i * PI / 2 / ( tesselation - 1 ) + PI/2;
                GenerateLine( t+2 + i, -w, -h, angle );
            }
// bottom
            GenerateLine( 2*( t + 1 ), -w, -h, PI );
            for( int i = 0; i < tesselation; i++ )
            {
                double angle = i * PI / 2 / ( tesselation - 1 ) + PI;
                GenerateLine( 2*( t + 1 ) + 1 + i, w, -h, angle );
            }
// right
            GenerateLine( 3*( t + 1 ), w, -h, 3*PI/2 );
            for( int i = 0; i < tesselation; i++ )
            {
                double angle = i * PI / 2 / ( tesselation - 1 ) + 3*PI/2;
                GenerateLine( 3*( t + 1 ) + 1 + i, w, h, angle );
            }
        }
    };
}

std::vector<IGeometryGeneratorPtr>    Plugin::GetGenerators()
{
    Generator::bevel = m_bevel->GetValue();
    Generator::dims = m_dimensions->GetValue();
    Generator::tesselation = m_tesselation->GetValue();
	Generator::center_translate = Generator::computeWeightCenter(
																	m_weightCenterX->Evaluate(),
																	m_weightCenterY->Evaluate(),
																	m_weightCenterZ->Evaluate() );
	Generator::mapping_type = m_mappingType->Evaluate();
    
    double depth = Generator::dims.z/2;
    
    std::vector<IGeometryGeneratorPtr> gens;
    gens.push_back( std::make_shared< Generator::MainComp >() );
    gens.push_back( std::make_shared< Generator::SideComp >( depth ) );
    gens.push_back( std::make_shared< Generator::SideComp >( -depth ) );
    return gens;
}

bool                                Plugin::NeedsTopologyUpdate()
{
    return ParameterChanged( PN::BEVEL ) || 
        ParameterChanged( PN::DIMENSIONS ) ||
        ParameterChanged( PN::TESSELATION )	||
		ParameterChanged( PN::WEIGHTCENTERX ) ||
		ParameterChanged( PN::WEIGHTCENTERY ) ||
		ParameterChanged( PN::WEIGHTCENTERZ ) ||
		ParameterChanged( PN::MAPPINGTYPE );
}

Plugin::Plugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model )
    : DefaultGeometryPluginBase( name, uid, prev, model )
{
    m_bevel = QueryTypedValue< ValueFloatPtr >( GetValue( PN::BEVEL ) );
    m_dimensions = QueryTypedValue< ValueVec3Ptr >( GetValue( PN::DIMENSIONS ) );
    m_tesselation = QueryTypedValue< ValueIntPtr >( GetValue( PN::TESSELATION ) );

	m_weightCenterX = QueryTypedParam< std::shared_ptr< ParamEnum< WeightCenter > > >( GetParameter( PN::WEIGHTCENTERX ) );
	m_weightCenterY = QueryTypedParam< std::shared_ptr< ParamEnum< WeightCenter > > >( GetParameter( PN::WEIGHTCENTERY ) );
	m_weightCenterZ = QueryTypedParam< std::shared_ptr< ParamEnum< WeightCenter > > >( GetParameter( PN::WEIGHTCENTERZ ) );

	m_mappingType = QueryTypedParam< std::shared_ptr< ParamEnum< MappingType > > >( GetParameter( PN::MAPPINGTYPE ) );

    m_pluginParamValModel->Update();
    InitGeometry();
}

} } }