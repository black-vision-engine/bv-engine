#include "stdafx.h"

#include "DefaultCubePlugin.h"

#include "Memory/MemoryLeaks.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"


DEFINE_ENUM_PARAMETER_CREATOR( bv::model::DefaultCubePlugin::WeightCenter );
DEFINE_ENUM_PARAMETER_CREATOR( bv::model::DefaultCubePlugin::MappingType );

namespace bv { namespace model {
	


typedef ParamEnum< DefaultCubePlugin::WeightCenter > ParamEnumWC;
typedef ParamEnum< DefaultCubePlugin::MappingType > ParamEnumMT;



const std::string       DefaultCubePlugin::PARAM::TESSELATION = "tesselation";
const std::string       DefaultCubePlugin::PARAM::DIMENSIONS = "dimensions";
const std::string       DefaultCubePlugin::PARAM::BEVEL = "bevel";
const std::string       DefaultCubePlugin::PARAM::WEIGHTCENTERX = "weight center x";
const std::string       DefaultCubePlugin::PARAM::WEIGHTCENTERY = "weight center y";
const std::string       DefaultCubePlugin::PARAM::WEIGHTCENTERZ = "weight center z";
const std::string       DefaultCubePlugin::PARAM::MAPPINGTYPE = "mapping type";
const std::string       DefaultCubePlugin::PARAM::SMOOTH_BEVEL = "smooth bevel";


// ***********************
//
DefaultCubePluginDesc::DefaultCubePluginDesc()
    : DefaultGeometryPluginDescBase( UID(), "cube" )
{}

// ***********************
//
DefaultPluginParamValModelPtr   DefaultCubePluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper h( timeEvaluator );

    h.SetOrCreateVacModel();
    h.AddSimpleParam( DefaultCubePlugin::PARAM::BEVEL, 0.1f, true, true );
    h.AddSimpleParam( DefaultCubePlugin::PARAM::DIMENSIONS, glm::vec3( 1, 1, 1 ), true, true );
    h.AddSimpleParam( DefaultCubePlugin::PARAM::TESSELATION, 4, true, true );
    h.AddSimpleParam( DefaultCubePlugin::PARAM::SMOOTH_BEVEL, true, true, true );
	h.AddParam< IntInterpolator, DefaultCubePlugin::WeightCenter, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumWC >
        ( DefaultCubePlugin::PARAM::WEIGHTCENTERX, DefaultCubePlugin::WeightCenter::CENTER, true, true );
	h.AddParam< IntInterpolator, DefaultCubePlugin::WeightCenter, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumWC >
        ( DefaultCubePlugin::PARAM::WEIGHTCENTERY, DefaultCubePlugin::WeightCenter::MIN, true, true );
	h.AddParam< IntInterpolator, DefaultCubePlugin::WeightCenter, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumWC >
        ( DefaultCubePlugin::PARAM::WEIGHTCENTERZ, DefaultCubePlugin::WeightCenter::CENTER, true, true );
  	h.AddParam< IntInterpolator, DefaultCubePlugin::MappingType, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumMT >
		( DefaultCubePlugin::PARAM::MAPPINGTYPE, DefaultCubePlugin::MappingType::OLDSTYLE, true, true );
    
    h.SetOrCreatePSModel();

    return h.GetModel();
}
    
// ***********************
//
IPluginPtr                      DefaultCubePluginDesc::CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultCubePlugin >( name, prev, timeEvaluator );
}

// ***********************
//
std::string                     DefaultCubePluginDesc::UID                 ()
{
    return "DEFAULT_CUBE";
}

#include "Mathematics/Defines.h"
namespace Generator
{
    int                     tesselation;
    float                   bevel;
    glm::vec3               dims;
    glm::vec3               centerTranslate;
    bool                    smooth;

    DefaultCubePlugin::MappingType     mappingType;


    // ***********************
    //
    template < typename T >
    int sign( T val )
    {
        return ( T( 0 ) < val ) - ( val < T( 0 ) );
    }

    // ***********************
    //
    glm::vec3 computeWeightCenter( DefaultCubePlugin::WeightCenter centerX, DefaultCubePlugin::WeightCenter centerY, DefaultCubePlugin::WeightCenter centerZ )
    {
        glm::vec3 centerTranslate = glm::vec3( 0.0f, 0.0f, 0.0f );

        if( centerX == DefaultCubePlugin::WeightCenter::MAX )
            centerTranslate += glm::vec3( -dims.x / 2, 0.0, 0.0 );
        else if( centerX == DefaultCubePlugin::WeightCenter::CENTER )
            centerTranslate += glm::vec3( 0.0, 0.0, 0.0 );
        else if( centerX == DefaultCubePlugin::WeightCenter::MIN )
            centerTranslate += glm::vec3( dims.x / 2, 0.0, 0.0 );

        if( centerY == DefaultCubePlugin::WeightCenter::MAX )
            centerTranslate += glm::vec3( 0.0f, -dims.y / 2, 0.0f );
        else if( centerY == DefaultCubePlugin::WeightCenter::CENTER )
            centerTranslate += glm::vec3( 0.0f, 0.0, 0.0f );
        else if( centerY == DefaultCubePlugin::WeightCenter::MIN )
            centerTranslate += glm::vec3( 0.0f, dims.y / 2, 0.0f );

        if( centerZ == DefaultCubePlugin::WeightCenter::MAX )
            centerTranslate += glm::vec3( 0.0, 0.0, -dims.z / 2 );
        else if( centerZ == DefaultCubePlugin::WeightCenter::CENTER )
            centerTranslate += glm::vec3( 0.0, 0.0, 0.0 );
        else if( centerZ == DefaultCubePlugin::WeightCenter::MIN )
            centerTranslate += glm::vec3( 0.0, 0.0, dims.z / 2 );

        return centerTranslate;
    }

    // ***********************
    //
    enum CubicMappingPlane
    {
        PLUS_X = 3,
        MINUS_X = 1,
        PLUS_Y = 0,
        MINUS_Y = 2,
        PLUS_Z = 4,
        MINUS_Z = 5
    };

    // ***********************
    //
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

    // ***********************
    //
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

    // ***********************
    //
    glm::vec2 computeUV( glm::vec3 position )
    {
        CubicMappingPlane plane;
        glm::vec2 uv_coords;
        float max_value = choosePlane( position, plane, uv_coords );

        uv_coords = uv_coords / glm::vec2( 2 * max_value, 2 * max_value );
        uv_coords += glm::vec2( 0.5, 0.5 );

        return makeUV( uv_coords, plane );
    }


    class MainComp : public IGeometryNormalsUVsGenerator
    {
        glm::vec3 **v;
        glm::vec3 **norm;
        glm::vec2 **coords;
        int n, m;
    public:

        // ***********************
        //
        void GenerateGeometryNormalsUVs( Float3AttributeChannelPtr verts, Float3AttributeChannelPtr normals, Float2AttributeChannelPtr uvs ) override
        {
            Init();
            Generate();
            CopyV( verts, uvs, normals );
            Deinit();

            double depth = Generator::dims.z / 2;

            GenerateSideFace( depth, verts, normals, uvs );
            GenerateSideFace( -depth, verts, normals, uvs );
        }

        // ***********************
        //
        void Init()
        {
            // It's a little hackisch. We should implement constraints in paramters.
            if( tesselation < 1 )
            {
                // If tesselation is below 1, we have simple cube without bevel.
                tesselation = 0;
                bevel = 0.0f;
            }

            n = 4 * ( tesselation + 1 ) + 1;
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

        // ***********************
        //
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

        // ***********************
        //
        glm::vec2 getUV( float bevel1, float bevel2, bool inverseU, bool inverseV )
        {
            float u = bevel1;
            float v = bevel2;
            if( inverseU )
                u = 1 - u;
            if( inverseV )
                v = 1 - v;
            return glm::vec2( u, v );
        }

        /**@brief Gives UVs in parameter, good orientation in texture space
        depending on face, that we compute currently.
        @param[in] z MINUS_Z lub PLUS_Z.*/
        glm::vec2       uvToZPlaneSpace( glm::vec2 uv, int face, CubicMappingPlane z )
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

        // ***********************
        //
        float       ComputeScaledU( float bevelUV, int mainPlaneTess, float bevelStep, int k, int j )
        {
            float kScale = float( mainPlaneTess - j ) / ( float )mainPlaneTess;
            return bevelUV - ( bevelUV - bevelStep * k ) * kScale;
        }

        // ***********************
        //
        void        GenerateBevelLineUV      ( int i, int face, int k, bool isMainPlane )
        {
            bool inverseU = !isMainPlane;

            int mainPlaneTess = tesselation / 2;
            int remainPlaneTess = tesselation - mainPlaneTess;
            int uvTess = isMainPlane ? mainPlaneTess : remainPlaneTess;

            float dim1;
            float dim3;
            if( face == 0 || face == 2 )
                dim1 = dims.y, dim3 = dims.x;
            else
                dim1 = dims.x, dim3 = dims.y;

            float bevelUV1 = bevel / dim1;
            float bevelStep1 = bevelUV1 / ( mainPlaneTess ? mainPlaneTess : 1 );        // Prevent from division by 0.

            float bevelUV2 = bevel / dims.z;
            float bevelStep2 = bevelUV2 / ( remainPlaneTess ? remainPlaneTess : 1 );    // Prevent from division by 0.

            float bevelUV3 = bevel / dim3;
            float bevelStep3 = bevelUV3 / ( uvTess ? uvTess : 1 );      // Prevent from division by 0.

            glm::vec2 preUV1;

            int base = 0;
            for( int j = mainPlaneTess; j >= 0; --j )
            {
                float k_step1 = ComputeScaledU( bevelUV3, mainPlaneTess, bevelStep3, k, j );

                preUV1 = getUV( k_step1, bevelStep1 * j, inverseU, false );
                preUV1 = uvToZPlaneSpace( preUV1, face, CubicMappingPlane::MINUS_Z );

                coords[ i ][ base + mainPlaneTess - j ] = makeUV( preUV1, CubicMappingPlane::MINUS_Z );
            }

            base += mainPlaneTess + 1;
            for( int j = 0; j <= remainPlaneTess; ++j )
            {
                preUV1 = getUV( bevelStep3 * k, bevelStep2 * j, inverseU, true );
                coords[ i ][ base + j ] = makeUV( preUV1, static_cast< CubicMappingPlane >( face ) );
            }


            base += remainPlaneTess + 1;
            for( int j = remainPlaneTess; j >= 0; --j )
            {
                preUV1 = getUV( bevelStep3 * k, bevelStep2 * j, inverseU, false );
                coords[ i ][ base + remainPlaneTess - j ] = makeUV( preUV1, static_cast< CubicMappingPlane >( face ) );
            }

            base += remainPlaneTess + 1;
            for( int j = 0; j <= mainPlaneTess; ++j )
            {
                float k_step1 = ComputeScaledU( bevelUV3, mainPlaneTess, bevelStep3, k, j );

                preUV1 = getUV( k_step1, bevelStep1 * j, inverseU, false );
                preUV1 = uvToZPlaneSpace( preUV1, face, CubicMappingPlane::PLUS_Z );
                preUV1 = makeUV( preUV1, CubicMappingPlane::PLUS_Z );

                coords[ i ][ base + j ] = preUV1;
            }

        }

        // ***********************
        //
        void        GenerateMainFaceUVs( int face )
        {
            int mainPlaneTess = tesselation / 2;
            int remainPlaneTess = tesselation - mainPlaneTess;

            // Each face is built of tesselation + 1 lines.
            int lineIdx = ( tesselation + 1 ) * face + remainPlaneTess;

            float dim1;
            float dim3;
            if( face == 0 || face == 2 )
                dim1 = dims.y, dim3 = dims.x;
            else
                dim1 = dims.x, dim3 = dims.y;

            float bevelUV1 = bevel / dim1;
            float bevelStep1 = bevelUV1 / ( mainPlaneTess ? mainPlaneTess : 1 );        // Prevent from division by 0.

            float bevelUV2 = bevel / dims.z;
            float bevelStep2 = bevelUV2 / ( remainPlaneTess ? remainPlaneTess : 1 );    // Prevent from division by 0.

            float bevelUV3 = bevel / dim3;
            //float bevelStep3 = bevelUV3 / mainPlaneTess;

            glm::vec2 preUV1;

            int base = 0;
            for( int j = mainPlaneTess; j >= 0; --j )
            {
                preUV1 = getUV( bevelUV3, bevelStep1 * j, true, false );
                preUV1 = uvToZPlaneSpace( preUV1, face, CubicMappingPlane::MINUS_Z );

                coords[ lineIdx ][ base + mainPlaneTess - j ] = makeUV( preUV1, CubicMappingPlane::MINUS_Z );
            }

            base += mainPlaneTess + 1;
            for( int j = 0; j <= remainPlaneTess; ++j )
            {
                preUV1 = getUV( bevelUV3, bevelStep2 * j, true, true );
                coords[ lineIdx ][ base + j ] = makeUV( preUV1, static_cast< CubicMappingPlane >( face ) );
            }

            base += remainPlaneTess + 1;
            for( int j = remainPlaneTess; j >= 0; --j )
            {
                preUV1 = getUV( bevelUV3, bevelStep2 * j, true, false );
                coords[ lineIdx ][ base + remainPlaneTess - j ] = makeUV( preUV1, static_cast< CubicMappingPlane >( face ) );
            }

            base += remainPlaneTess + 1;
            for( int j = 0; j <= mainPlaneTess; ++j )
            {
                preUV1 = getUV( bevelUV3, bevelStep1 * j, true, false );
                preUV1 = uvToZPlaneSpace( preUV1, face, CubicMappingPlane::PLUS_Z );

                coords[ lineIdx ][ base + j ] = makeUV( preUV1, CubicMappingPlane::PLUS_Z );
            }
        }

        // ***********************
        //
        void        GenerateFaceUVs     ( int face )
        {
            // Tesselated bevel consists of two regions. First regions maps UVs from main face, that we got in
            // function parameter. Second region takes UVs from face following this face.
            // We map half of tesselated lines into our main face. Remaining tesselation goes to next face.
            int mainFaceTess = tesselation / 2;
            int followingFaceTess = tesselation - mainFaceTess;

            // Each face is built of tesselation + 1 lines.
            int lineIdx = ( tesselation + 1 ) * face;

            for( int k = 0; k < followingFaceTess; ++k )
                GenerateBevelLineUV( lineIdx + k, face, k, false );  // face + 3 = face - 1

            // Cube is generated as big rectangular plane in the middle and small connected lines on bevel part.
            // This function generates Uvs for this main plane.
            GenerateMainFaceUVs( face );

            // First lineIdx in face was used for main face.
            lineIdx += followingFaceTess + 1;

            for( int k = mainFaceTess; k > 0; --k )
                GenerateBevelLineUV( lineIdx + mainFaceTess - k, face, k, true );
        }

        // ***********************
        //
        void        CopyV       ( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs, Float3AttributeChannelPtr normals )
        {
            if( smooth )
                CopySmooth( verts, uvs, normals );
            else
                CopyFlat( verts, uvs, normals );
        }

        // ***********************
        //
        void        CopySmooth      ( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs, Float3AttributeChannelPtr normals )
        {
            int mainFaceTess = tesselation / 2;
            int followingFaceTess = tesselation - mainFaceTess;

            for( int i = 0; i < n - 1; i++ )
            {
                int uvsIdx = 0;

                for( int j = 0; j < m - 1; j++ )
                {
                    verts->AddAttribute( v[ i ][ j ] + centerTranslate );          normals->AddAttribute( norm[ i ][ j ] );            uvs->AddAttribute( coords[ i ][ uvsIdx ] );
                    verts->AddAttribute( v[ i + 1 ][ j ] + centerTranslate );      normals->AddAttribute( norm[ i + 1 ][ j ] );        uvs->AddAttribute( coords[ i + 1 ][ uvsIdx ] );
                    verts->AddAttribute( v[ i ][ j + 1 ] + centerTranslate );      normals->AddAttribute( norm[ i ][ j + 1 ] );        uvs->AddAttribute( coords[ i ][ uvsIdx + 1 ] );

                    verts->AddAttribute( v[ i ][ j + 1 ] + centerTranslate );      normals->AddAttribute( norm[ i ][ j + 1 ] );        uvs->AddAttribute( coords[ i ][ uvsIdx + 1 ] );
                    verts->AddAttribute( v[ i + 1 ][ j ] + centerTranslate );      normals->AddAttribute( norm[ i + 1 ][ j ] );        uvs->AddAttribute( coords[ i + 1 ][ uvsIdx ] );
                    verts->AddAttribute( v[ i + 1 ][ j + 1 ] + centerTranslate );  normals->AddAttribute( norm[ i + 1 ][ j + 1 ] );    uvs->AddAttribute( coords[ i + 1 ][ uvsIdx + 1 ] );


                    if( j == mainFaceTess || j == tesselation + 1 + followingFaceTess )
                    {
                        uvsIdx++;

                        verts->AddAttribute( v[ i ][ j ] + centerTranslate );          normals->AddAttribute( norm[ i ][ j ] );            uvs->AddAttribute( coords[ i ][ uvsIdx ] );
                        verts->AddAttribute( v[ i + 1 ][ j ] + centerTranslate );      normals->AddAttribute( norm[ i + 1 ][ j ] );        uvs->AddAttribute( coords[ i + 1 ][ uvsIdx ] );
                        verts->AddAttribute( v[ i ][ j + 1 ] + centerTranslate );      normals->AddAttribute( norm[ i ][ j + 1 ] );        uvs->AddAttribute( coords[ i ][ uvsIdx + 1 ] );

                        verts->AddAttribute( v[ i ][ j + 1 ] + centerTranslate );      normals->AddAttribute( norm[ i ][ j + 1 ] );        uvs->AddAttribute( coords[ i ][ uvsIdx + 1 ] );
                        verts->AddAttribute( v[ i + 1 ][ j ] + centerTranslate );      normals->AddAttribute( norm[ i + 1 ][ j ] );        uvs->AddAttribute( coords[ i + 1 ][ uvsIdx ] );
                        verts->AddAttribute( v[ i + 1 ][ j + 1 ] + centerTranslate );  normals->AddAttribute( norm[ i + 1 ][ j + 1 ] );    uvs->AddAttribute( coords[ i + 1 ][ uvsIdx + 1 ] );
                    }

                    uvsIdx++;
                }
            }
        }

        // ***********************
        //
        void        CopyFlat    ( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs, Float3AttributeChannelPtr normals )
        {
            int mainFaceTess = tesselation / 2;
            int followingFaceTess = tesselation - mainFaceTess;

            for( int i = 0; i < n - 1; i++ )
            {
                int uvsIdx = 0;

                for( int j = 0; j < m - 1; j++ )
                {
                    glm::vec3 normal = ComputeFlatNormal( v[ i ][ j ], v[ i + 1 ][ j ], v[ i ][ j + 1 ], v[ i + 1 ][ j + 1 ] );

                    verts->AddAttribute( v[ i ][ j ] + centerTranslate );          normals->AddAttribute( normal );        uvs->AddAttribute( coords[ i ][ uvsIdx ] );
                    verts->AddAttribute( v[ i + 1 ][ j ] + centerTranslate );      normals->AddAttribute( normal );        uvs->AddAttribute( coords[ i + 1 ][ uvsIdx ] );
                    verts->AddAttribute( v[ i ][ j + 1 ] + centerTranslate );      normals->AddAttribute( normal );        uvs->AddAttribute( coords[ i ][ uvsIdx + 1 ] );

                    verts->AddAttribute( v[ i ][ j + 1 ] + centerTranslate );      normals->AddAttribute( normal );        uvs->AddAttribute( coords[ i ][ uvsIdx + 1 ] );
                    verts->AddAttribute( v[ i + 1 ][ j ] + centerTranslate );      normals->AddAttribute( normal );        uvs->AddAttribute( coords[ i + 1 ][ uvsIdx ] );
                    verts->AddAttribute( v[ i + 1 ][ j + 1 ] + centerTranslate );  normals->AddAttribute( normal );        uvs->AddAttribute( coords[ i + 1 ][ uvsIdx + 1 ] );


                    if( j == mainFaceTess || j == tesselation + 1 + followingFaceTess )
                    {
                        uvsIdx++;

                        normal = ComputeFlatNormal( v[ i ][ j ], v[ i + 1 ][ j ], v[ i ][ j + 1 ], v[ i + 1 ][ j + 1 ] );

                        verts->AddAttribute( v[ i ][ j ] + centerTranslate );          normals->AddAttribute( normal );        uvs->AddAttribute( coords[ i ][ uvsIdx ] );
                        verts->AddAttribute( v[ i + 1 ][ j ] + centerTranslate );      normals->AddAttribute( normal );        uvs->AddAttribute( coords[ i + 1 ][ uvsIdx ] );
                        verts->AddAttribute( v[ i ][ j + 1 ] + centerTranslate );      normals->AddAttribute( normal );        uvs->AddAttribute( coords[ i ][ uvsIdx + 1 ] );

                        verts->AddAttribute( v[ i ][ j + 1 ] + centerTranslate );      normals->AddAttribute( normal );        uvs->AddAttribute( coords[ i ][ uvsIdx + 1 ] );
                        verts->AddAttribute( v[ i + 1 ][ j ] + centerTranslate );      normals->AddAttribute( normal );        uvs->AddAttribute( coords[ i + 1 ][ uvsIdx ] );
                        verts->AddAttribute( v[ i + 1 ][ j + 1 ] + centerTranslate );  normals->AddAttribute( normal );        uvs->AddAttribute( coords[ i + 1 ][ uvsIdx + 1 ] );
                    }

                    uvsIdx++;
                }
            }
        }

        // ***********************
        //
        glm::vec3   ComputeFlatNormal       ( glm::vec3 & vert1, glm::vec3 & vert2, glm::vec3 & vert3, glm::vec3 & vert4 )
        {
            glm::vec3 triangleVector1 = vert4 - vert3;
            glm::vec3 triangleVector2 = vert1 - vert3;

            if( triangleVector1 == glm::vec3( 0.0f, 0.0f, 0.0f ) )
            {
                triangleVector1 = vert2 - vert1;
                triangleVector2 = triangleVector2;
            }

            return glm::normalize( glm::cross( triangleVector2, triangleVector1 ) );
        }

        // ***********************
        //
        void        GenerateLine( int i, double x, double y, double a )
        {
            double d = dims.z / 2 - bevel;
            double b = bevel;

            int angleTesselation = tesselation > 0 ? tesselation : 1;       // Avoid division by zero.

            double sinA = sin( a );
            double cosA = cos( a );

            for( int j = 0; j <= tesselation; j++ )
            {
                double angle2 = j * PI / 2 / angleTesselation;

                double sinAngle2 = sin( angle2 );
                double cosAngle2 = cos( angle2 );

                v[ i ][ j ] = glm::vec3( x - b * sinA * sinAngle2, y + b * cosA * sinAngle2, -d - b * cosAngle2 );
                norm[ i ][ j ] = glm::normalize( glm::vec3( -sinA * sinAngle2, cosA * sinAngle2, -cosAngle2 ) );
            }

            for( int j = 0; j <= tesselation; j++ )
            {
                double angle2 = j * PI / 2 / angleTesselation + PI / 2;

                double sinAngle2 = sin( angle2 );
                double cosAngle2 = cos( angle2 );

                v[ i ][ tesselation + 1 + j ] = glm::vec3( x - b * sinA * sinAngle2, y + b * cosA * sinAngle2, d - b * cosAngle2 );
                norm[ i ][ tesselation + 1 + j ] = glm::normalize( glm::vec3( -sinA * sinAngle2, cosA * sinAngle2, -cosAngle2 ) );
            }
        }

        // ***********************
        //
        void        Generate()
        {
            int mainFaceTess = tesselation / 2;

            double w = dims.x / 2 - bevel,
                h = dims.y / 2 - bevel;

            int t = tesselation;
            int angleTesselation = tesselation > 0 ? tesselation : 1;       // Avoid division by zero.

            // Note: We generate beveled lines around the cube. We must place first line in such way, that
            // UV mapping won't break. First and last vertex must be doubled, because texture begins and ends there.
            // In the past we generated main plane first and then following bevel. This process was repeated for each of 4 cube faces.
            // Now bevel generation for first face is broken down into two parts: bevel before and bevel after main plane.
            // This way we can apply UVs from different parts of texture without making complicated changes to traingulation code.

            int lineIdxOffset = 0;
            for( int i = mainFaceTess; i < tesselation; i++ )
            {
                double angle = i * PI / 2 / angleTesselation + 3 * PI / 2;
                GenerateLine( lineIdxOffset, w, h, angle );

                lineIdxOffset++;
            }

            // top
            GenerateLine( lineIdxOffset, w, h, 0. );
            for( int i = 0; i < tesselation; i++ )
            {
                double angle = i * PI / 2 / angleTesselation;
                GenerateLine( lineIdxOffset + 1 + i, -w, h, angle );
            }
            // left
            GenerateLine( lineIdxOffset + t + 1, -w, h, PI / 2 );
            for( int i = 0; i < tesselation; i++ )
            {
                double angle = i * PI / 2 / angleTesselation + PI / 2;
                GenerateLine( lineIdxOffset + t + 2 + i, -w, -h, angle );
            }
            // bottom
            GenerateLine( lineIdxOffset + 2 * ( t + 1 ), -w, -h, PI );
            for( int i = 0; i < tesselation; i++ )
            {
                double angle = i * PI / 2 / angleTesselation + PI;
                GenerateLine( lineIdxOffset + 2 * ( t + 1 ) + 1 + i, w, -h, angle );
            }
            // right
            GenerateLine( lineIdxOffset + 3 * ( t + 1 ), w, -h, 3 * PI / 2 );
            for( int i = 0; i < mainFaceTess + 1; i++ )
            {
                double angle = i * PI / 2 / angleTesselation + 3 * PI / 2;
                GenerateLine( lineIdxOffset + 3 * ( t + 1 ) + 1 + i, w, h, angle );
            }

            // UV mapping

            for( int face = 0; face < 4; ++face )
                GenerateFaceUVs( face );

            // Note: Normally last line should be connected to first line, but UVs are different so we separated them.
            // Here we generate UVs for additional line added at the end.
            GenerateBevelLineUV( n - 1, 3, 0, true );

        }

        // ***********************
        //
        void        GenerateSideFace        ( double d, Float3AttributeChannelPtr verts, Float3AttributeChannelPtr normals, Float2AttributeChannelPtr uvs )
        {
            CubicMappingPlane mappingPlane;
            if( d < 0 )
                mappingPlane = CubicMappingPlane::MINUS_Z;
            else
                mappingPlane = CubicMappingPlane::PLUS_Z;

            double w = dims.x / 2 - bevel,
                h = dims.y / 2 - bevel;


            if( d > 0 )
            {
                verts->AddAttribute( glm::vec3( -w, h, d ) + centerTranslate );   normals->AddAttribute( glm::vec3( 0.0, 0.0, 1.0 ) );
                verts->AddAttribute( glm::vec3( -w, -h, d ) + centerTranslate );   normals->AddAttribute( glm::vec3( 0.0, 0.0, 1.0 ) );
                verts->AddAttribute( glm::vec3( w, h, d ) + centerTranslate );   normals->AddAttribute( glm::vec3( 0.0, 0.0, 1.0 ) );

                verts->AddAttribute( glm::vec3( w, h, d ) + centerTranslate );   normals->AddAttribute( glm::vec3( 0.0, 0.0, 1.0 ) );
                verts->AddAttribute( glm::vec3( -w, -h, d ) + centerTranslate );   normals->AddAttribute( glm::vec3( 0.0, 0.0, 1.0 ) );
                verts->AddAttribute( glm::vec3( w, -h, d ) + centerTranslate );   normals->AddAttribute( glm::vec3( 0.0, 0.0, 1.0 ) );
            }
            else
            {
                verts->AddAttribute( glm::vec3( w, h, d ) + centerTranslate );   normals->AddAttribute( glm::vec3( 0.0, 0.0, -1.0 ) );
                verts->AddAttribute( glm::vec3( w, -h, d ) + centerTranslate );   normals->AddAttribute( glm::vec3( 0.0, 0.0, -1.0 ) );
                verts->AddAttribute( glm::vec3( -w, h, d ) + centerTranslate );   normals->AddAttribute( glm::vec3( 0.0, 0.0, -1.0 ) );

                verts->AddAttribute( glm::vec3( -w, h, d ) + centerTranslate );   normals->AddAttribute( glm::vec3( 0.0, 0.0, -1.0 ) );
                verts->AddAttribute( glm::vec3( w, -h, d ) + centerTranslate );   normals->AddAttribute( glm::vec3( 0.0, 0.0, -1.0 ) );
                verts->AddAttribute( glm::vec3( -w, -h, d ) + centerTranslate );   normals->AddAttribute( glm::vec3( 0.0, 0.0, -1.0 ) );
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

            uvs->AddAttribute( makeUV( preUV3, mappingPlane ) );
            uvs->AddAttribute( makeUV( preUV2, mappingPlane ) );
            uvs->AddAttribute( makeUV( preUV4, mappingPlane ) );
        }
    };
}

// ***********************
//
std::vector<IGeometryGeneratorPtr>    DefaultCubePlugin::GetGenerators()
{
    Generator::smooth = m_smooth->GetValue();
    Generator::bevel = m_bevel->GetValue();
    Generator::dims = m_dimensions->GetValue();
    Generator::tesselation = m_tesselation->GetValue();
	Generator::centerTranslate = Generator::computeWeightCenter(
																	m_weightCenterX->Evaluate(),
																	m_weightCenterY->Evaluate(),
																	m_weightCenterZ->Evaluate() );
	Generator::mappingType = m_mappingType->Evaluate();

    std::vector<IGeometryGeneratorPtr> gens;
    gens.push_back( std::make_shared< Generator::MainComp >() );
    return gens;
}

// ***********************
//
bool                                DefaultCubePlugin::NeedsTopologyUpdate()
{
    return ParameterChanged( PARAM::BEVEL ) || 
        ParameterChanged( PARAM::DIMENSIONS ) ||
        ParameterChanged( PARAM::TESSELATION )	||
		ParameterChanged( PARAM::WEIGHTCENTERX ) ||
		ParameterChanged( PARAM::WEIGHTCENTERY ) ||
		ParameterChanged( PARAM::WEIGHTCENTERZ ) ||
		ParameterChanged( PARAM::MAPPINGTYPE ) ||
        ParameterChanged( PARAM::SMOOTH_BEVEL );
}

// ***********************
//
DefaultCubePlugin::DefaultCubePlugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model )
    : DefaultGeometryPluginBase( name, uid, prev, model )
{
    m_smooth = QueryTypedValue< ValueBoolPtr >( GetValue( PARAM::SMOOTH_BEVEL ) );
    m_bevel = QueryTypedValue< ValueFloatPtr >( GetValue( PARAM::BEVEL ) );
    m_dimensions = QueryTypedValue< ValueVec3Ptr >( GetValue( PARAM::DIMENSIONS ) );
    m_tesselation = QueryTypedValue< ValueIntPtr >( GetValue( PARAM::TESSELATION ) );

	m_weightCenterX = QueryTypedParam< std::shared_ptr< ParamEnum< WeightCenter > > >( GetParameter( PARAM::WEIGHTCENTERX ) );
	m_weightCenterY = QueryTypedParam< std::shared_ptr< ParamEnum< WeightCenter > > >( GetParameter( PARAM::WEIGHTCENTERY ) );
	m_weightCenterZ = QueryTypedParam< std::shared_ptr< ParamEnum< WeightCenter > > >( GetParameter( PARAM::WEIGHTCENTERZ ) );

	m_mappingType = QueryTypedParam< std::shared_ptr< ParamEnum< MappingType > > >( GetParameter( PARAM::MAPPINGTYPE ) );

    m_pluginParamValModel->Update();
    InitGeometry( PrimitiveType::PT_TRIANGLES );
}

} }