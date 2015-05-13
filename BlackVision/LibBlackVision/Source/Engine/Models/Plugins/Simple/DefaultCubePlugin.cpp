#include "DefaultCubePlugin.h"

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

#include "Engine/Models/Plugins/ParamValModel/SimpleParamValEvaluator.inl"
	
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

    h.CreateVacModel();
    h.AddSimpleParam( PN::BEVEL, 0.1f, true, true );
    h.AddSimpleParam( PN::DIMENSIONS, glm::vec3( 1, 1, 1 ), true, true );
    h.AddSimpleParam( PN::TESSELATION, 2, true, true );
	h.AddParam< IntInterpolator, Plugin::WeightCenter, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumWC >
        ( DefaultCube::PN::WEIGHTCENTERX, Plugin::WeightCenter::CENTER, true, true );
	h.AddParam< IntInterpolator, Plugin::WeightCenter, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumWC >
        ( DefaultCube::PN::WEIGHTCENTERY, Plugin::WeightCenter::MIN, true, true );
	h.AddParam< IntInterpolator, Plugin::WeightCenter, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumWC >
        ( DefaultCube::PN::WEIGHTCENTERZ, Plugin::WeightCenter::CENTER, true, true );
  	h.AddParam< IntInterpolator, Plugin::MappingType, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumMT >
		( PN::MAPPINGTYPE, Plugin::MappingType::OLDSTYLE, true, true );

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
		PLUS_Z,
		MINUS_Z
	};

	float choosePlane( glm::vec3 direction, CubicMappingPlane& plane, glm::vec2& remaining_values )
	{
		float max = 0;
		if( abs( direction.x ) > abs( direction.y ) )
		{
			if( abs( direction.x ) > abs( direction.z ) )
			{
				max = abs( direction.x );
				plane = direction.x < 0 ? CubicMappingPlane::MINUS_X : CubicMappingPlane::PLUS_X;
				remaining_values.x = direction.y;
				remaining_values.y = direction.z;
			}
			else
			{
				max = abs( direction.z );
				plane = direction.z < 0 ? CubicMappingPlane::MINUS_Z : CubicMappingPlane::PLUS_Z;
				remaining_values.x = direction.x;
				remaining_values.y = direction.y;
			}
		}
		else
		{
			if( abs( direction.y ) > abs( direction.z ) )
			{
				max = abs( direction.y );
				plane = direction.y < 0 ? CubicMappingPlane::MINUS_Y : CubicMappingPlane::PLUS_Y;
				remaining_values.x = direction.x;
				remaining_values.y = direction.z;
			}
			else
			{
				max = abs( direction.z );
				plane = direction.z < 0 ? CubicMappingPlane::MINUS_Z : CubicMappingPlane::PLUS_Z;
				remaining_values.x = direction.x;
				remaining_values.y = direction.y;
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
			uv_translate = glm::vec2( 1.0 / 2.0, 1.0 / 3.0 );
		else if( plane == CubicMappingPlane::PLUS_Z )
			uv_translate = glm::vec2( 1.0 / 2.0, 2.0 / 3.0 );
		else if( plane == CubicMappingPlane::MINUS_Z )
			uv_translate = glm::vec2( 1.0 / 2.0, 0.0 );
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


    class SideComp : public IGeometryAndUVsGenerator
    {
        double d;
    public:
        Type GetType() { return Type::GEOMETRY_AND_UVS; }

        void GenerateGeometryAndUVs( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs ) override
        {
			CubicMappingPlane mapping_plane;
			if( d < 0 )
				mapping_plane = CubicMappingPlane::MINUS_Z;
			else
				mapping_plane = CubicMappingPlane::PLUS_Z;

            double w = dims.x/2 - bevel, 
                h = dims.y/2 - bevel;

            verts->AddAttribute( glm::vec3(  w,  h, d ) + center_translate );
            verts->AddAttribute( glm::vec3(  w, -h, d ) + center_translate );
            verts->AddAttribute( glm::vec3( -w,  h, d ) + center_translate );
            verts->AddAttribute( glm::vec3( -w, -h, d ) + center_translate );

			float bevelUV1 = bevel / dims.x;
			float bevelUV2 = bevel / dims.y;
			glm::vec2 pre_uv1 = glm::vec2( bevelUV1, bevelUV2 );
			glm::vec2 pre_uv2 = glm::vec2( bevelUV1, 1 - bevelUV2 );
			glm::vec2 pre_uv3 = glm::vec2( 1 - bevelUV1, bevelUV2 );
			glm::vec2 pre_uv4 = glm::vec2( 1 - bevelUV1, 1 - bevelUV2 );

			uvs->AddAttribute( makeUV( pre_uv1, mapping_plane ) );
			uvs->AddAttribute( makeUV( pre_uv2, mapping_plane ) );
			uvs->AddAttribute( makeUV( pre_uv3, mapping_plane ) );
			uvs->AddAttribute( makeUV( pre_uv4, mapping_plane ) );
        }

        SideComp( double d_ ) : d( d_ ) { }
    };

    class MainComp : public IGeometryAndUVsGenerator
    {
        glm::vec3 **v;
        int n, m;
    public:
        Type GetType() { return Type::GEOMETRY_AND_UVS; }

        void GenerateGeometryAndUVs( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs ) override
        {
            Init();
            GenerateV();
            CopyV( verts, uvs );
            Deinit();
        }

        void Init() 
        {
			if( tesselation < 2 )
				tesselation = 2;		// It's a little hackisch.

            n = 4*(tesselation+1);
            m = (tesselation+1) * 2;
            assert( n >= 0 );
            v = new glm::vec3*[ n ];
            for( int i = 0; i < n; i++ )
                v[ i ] = new glm::vec3[ m ];
        }

        void Deinit()
        {
            for( int i = 0; i < n; i++ )
                delete[] v[i];
            delete[] v;
        }

		glm::vec2 getUV( float bevel1, float bevel2, bool inverse1, bool inverse2 )
		{
			float u = bevel1;
			float v = bevel2;
			if( inverse1 )
				u = 1 - u;
			if( inverse2 )
				v = 1 - v;
			return glm::vec2(u, v);
		}

		glm::vec2 prepare_for_z_surface( glm::vec2 uv, int face )
		{
			if( face == 1 )
				return glm::vec2( -uv.y, uv.x );
			else if( face == 3 )
				return glm::vec2( -uv.x, -uv.y );
			else if( face == 4 )
				return glm::vec2( uv.y, -uv.x );
			return uv;		// If face == 0, everythig is ok
		}

		void generateLineUV( int face, int k, bool inverse, Float2AttributeChannelPtr uvs )
		{
			int main_plane_tess = tesselation / 2;
			int remain_plane_tess = tesselation - main_plane_tess;

			float bevel_step1;
			if( face == 0 || face == 2 )
				bevel_step1 = bevel / ( main_plane_tess * dims.y );
			else
				bevel_step1 = bevel / ( main_plane_tess * dims.x );

			float bevelUV2 = bevel / dims.z;
			float bevel_step2 = bevelUV2 / remain_plane_tess;



			for( int j = 0; j <= main_plane_tess; ++j )
			{
				// @fixme
				glm::vec2 pre_uv1 = getUV( bevel_step1 * k, bevel_step2 * j, inverse, false );
				glm::vec2 pre_uv2 = getUV( bevel_step1 * (k+1), bevel_step2 * j, inverse, false );
				pre_uv1 = prepare_for_z_surface( pre_uv1, face );
				pre_uv2 = prepare_for_z_surface( pre_uv2, face );
				uvs->AddAttribute( makeUV( pre_uv1, CubicMappingPlane::PLUS_Z ) );
				uvs->AddAttribute( makeUV( pre_uv2, CubicMappingPlane::PLUS_Z ) );
			}

			for( int j = 0; j <= remain_plane_tess; ++j )
			{
				glm::vec2 pre_uv1 = getUV( bevel_step1 * k, bevel_step2 * j, inverse, true );
				glm::vec2 pre_uv2;
				if( inverse )
					pre_uv2 = getUV( bevel_step1 * (k+1), bevel_step2 * j, inverse, true );
				else
					pre_uv2 = getUV( bevel_step1 * (k-1), bevel_step2 * j, inverse, true );
				uvs->AddAttribute( makeUV( pre_uv1, static_cast<CubicMappingPlane>(face) ) );
				uvs->AddAttribute( makeUV( pre_uv2, static_cast<CubicMappingPlane>(face) ) );
			}


			for( int j = remain_plane_tess; j >= 0; --j )
			{
				glm::vec2 pre_uv1 = getUV( bevel_step1 * k, bevel_step2 * j, inverse, false );
				glm::vec2 pre_uv2;
				if( inverse )
					pre_uv2 = getUV( bevel_step1 * (k+1), bevel_step2 * j, inverse, false );
				else
					pre_uv2 = getUV( bevel_step1 * (k-1), bevel_step2 * j, inverse, false );
				uvs->AddAttribute( makeUV( pre_uv1, static_cast<CubicMappingPlane>(face) ) );
				uvs->AddAttribute( makeUV( pre_uv2, static_cast<CubicMappingPlane>(face) ) );
			}
			for( int j = main_plane_tess; j >= 0; --j )
			{
				// @fixme
				glm::vec2 pre_uv1 = getUV( bevel_step1 * k, bevel_step2 * j, inverse, true );
				glm::vec2 pre_uv2 = getUV( bevel_step1 * (k+1), bevel_step2 * j, inverse, true );
				pre_uv1 = prepare_for_z_surface( pre_uv1, face );
				pre_uv2 = prepare_for_z_surface( pre_uv2, face );
				uvs->AddAttribute( makeUV( pre_uv1, CubicMappingPlane::MINUS_Z ) );
				uvs->AddAttribute( makeUV( pre_uv2, CubicMappingPlane::MINUS_Z ) );
			}
		}

		void generateBigLine( int face, Float2AttributeChannelPtr uvs )
		{
			int main_plane_tess = tesselation / 2;
			int remain_plane_tess = tesselation - main_plane_tess;

			float dim;
			if( face == 0 || face == 2 )
				dim = dims.y;
			else
				dim = dims.x;

			float bevelUV1 = bevel / dim;
			float bevel_step1 = bevelUV1 / main_plane_tess;

			float bevelUV2 = bevel / dims.z;
			float bevel_step2 = bevelUV2 / remain_plane_tess;


			for( int j = main_plane_tess; j >= 0; --j )
			{
				// @fixme
				glm::vec2 pre_uv1 = getUV( bevelUV1, bevel_step1 * j, false, false );
				glm::vec2 pre_uv2 = getUV( bevelUV1, bevel_step1 * j, true, false );
				pre_uv1 = prepare_for_z_surface( pre_uv1, face );
				pre_uv2 = prepare_for_z_surface( pre_uv2, face );
				uvs->AddAttribute( makeUV( pre_uv1, CubicMappingPlane::PLUS_Z ) );
				uvs->AddAttribute( makeUV( pre_uv2, CubicMappingPlane::PLUS_Z ) );
			}

			for( int j = 0; j <= remain_plane_tess; ++j )
			{
				glm::vec2 pre_uv1 = getUV( bevelUV1, bevel_step2 * j, true, true );
				glm::vec2 pre_uv2 = getUV( bevelUV1, bevel_step2 * j, false, true );
				uvs->AddAttribute( makeUV( pre_uv1, static_cast<CubicMappingPlane>(face) ) );
				uvs->AddAttribute( makeUV( pre_uv2, static_cast<CubicMappingPlane>(face) ) );
			}


			for( int j = remain_plane_tess; j >= 0; --j )
			{
				glm::vec2 pre_uv1 = getUV( bevelUV1, bevel_step2 * j, true, false );
				glm::vec2 pre_uv2 = getUV( bevelUV1, bevel_step2 * j, false, false );
				uvs->AddAttribute( makeUV( pre_uv1, static_cast<CubicMappingPlane>(face) ) );
				uvs->AddAttribute( makeUV( pre_uv2, static_cast<CubicMappingPlane>(face) ) );
			}
			for( int j = 0; j <= main_plane_tess; ++j )
			{
				// @fixme
				glm::vec2 pre_uv1 = getUV( bevelUV1, bevel_step1 * j, false, false );
				glm::vec2 pre_uv2 = getUV( bevelUV1, bevel_step1 * j, true, false );
				pre_uv1 = prepare_for_z_surface( pre_uv1, face );
				pre_uv2 = prepare_for_z_surface( pre_uv2, face );
				uvs->AddAttribute( makeUV( pre_uv1, CubicMappingPlane::MINUS_Z ) );
				uvs->AddAttribute( makeUV( pre_uv2, CubicMappingPlane::MINUS_Z ) );
			}
		}
		

		void generatePartUV( int face, Float2AttributeChannelPtr uvs )
		{
			int main_plane_tess = tesselation / 2;
			int remain_plane_tess = tesselation - main_plane_tess;

			// Big plane and things around
			generateBigLine( face, uvs );

			for( int k = main_plane_tess; k > 0; --k )
				generateLineUV( face, k, false, uvs );
			for( int k = 0; k < remain_plane_tess; ++k )
				generateLineUV( ( face + 1 ) % 4, k, true, uvs );
			
		}


        void CopyV( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs )
        {
			//for( int i = 0; i < n-1; i++ )
   //             for( int j = 0; j < m; j++ )
			//	{
			//		uvs->AddAttribute( computeUV( v[ i   ][ j ] ) );
			//		uvs->AddAttribute( computeUV( v[ i+1 ][ j ] ) );
			//	}
   //         for( int j = 0; j < m; j++ )
   //         {
			//	uvs->AddAttribute( computeUV( v[  n-1 ][ j ] ) );
			//	uvs->AddAttribute( computeUV( v[ 0 ][ j ] ) );
			//}

			for( int face = 0; face < 4; ++face )
				generatePartUV( face, uvs );

			int main_plane_tess = tesselation / 2;
			int remain_plane_tess = tesselation - main_plane_tess;


            for( int i = 0; i < n-1; i++ )
                for( int j = 0; j < m; j++ )
                {
					verts->AddAttribute( v[ i   ][ j ] + center_translate );
                    verts->AddAttribute( v[ i+1 ][ j ] + center_translate );

					if( j == main_plane_tess || j == tesselation + 1 + remain_plane_tess )
					{
						verts->AddAttribute( v[ i   ][ j ] + center_translate );
						verts->AddAttribute( v[ i+1 ][ j ] + center_translate );
					}
                }
            for( int j = 0; j < m; j++ )
            {
                verts->AddAttribute( v[ n-1 ][ j ] + center_translate );
                verts->AddAttribute( v[ 0   ][ j ] + center_translate );

				if( j == main_plane_tess || j == tesselation + 1 + remain_plane_tess )
				{
					verts->AddAttribute( v[ n-1   ][ j ] + center_translate );
					verts->AddAttribute( v[ 0 ][ j ] + center_translate );
				}
            }

    //        for( SizeType v = 0; v < verts->GetNumEntries(); v++ )
    //        {
    //            glm::vec3 vert = verts->GetVertices()[ v ];
				//vert -= center_translate;
    //            uvs->AddAttribute( glm::vec2( 0.5*( vert.x + vert.y + 1.f ),
    //                                            vert.z + 0.5 ) ); // FIXME: scaling
    //        }
        }

        void GenerateLine( int i, double x, double y, double a )
        {
            double d = dims.z/2 - bevel;
            double b = bevel;
            
            for( int j = 0; j <= tesselation; j++ )
            {
                double angle2 = j * PI/2 / tesselation;
                v[ i ][ j ] = glm::vec3( x - b*sin( a )*sin( angle2 ), y + b*cos( a )*sin( angle2 ), -d - b*cos( angle2 ) );
            }
            for( int j = 0; j <= tesselation; j++ )
            {
                double angle2 = j * PI/2 / tesselation + PI/2;
                v[ i ][ tesselation+1 + j ] = glm::vec3( x - b*sin( a )*sin( angle2 ), y + b*cos( a )*sin( angle2 ),  d - b*cos( angle2 ) );
            }
        }

        void GenerateV()
        {
            double w = dims.x/2 - bevel, 
                h = dims.y/2 - bevel;
            int t = tesselation;

// top
            GenerateLine( 0, w, h, 0. );
            for( int i = 0; i < tesselation; i++ ) // (-w, h+b) for i = 0
            {
                double angle = i * PI / 2 / ( tesselation - 1 );
                GenerateLine( 1 + i,                        -w,  h, angle );
            }
// left
            GenerateLine( t+1, -w,  h, PI/2 );
            for( int i = 0; i < tesselation; i++ ) // (-w-b, -h ) for i = 0
            {
                double angle = i * PI / 2 / ( tesselation - 1 ) + PI/2;
                GenerateLine( t+2 + i,                      -w, -h, angle );
            }
// bottom
            GenerateLine( 2*( t + 1 ), -w, -h, PI );
            for( int i = 0; i < tesselation; i++ ) // ( w, -h-b ) for i = 0
            {
                double angle = i * PI / 2 / ( tesselation - 1 ) + PI;
                GenerateLine( 2*( t + 1 )+1 + i,            w,  -h, angle );
            }
// right
            GenerateLine( 3*( t + 1 ), w, -h, 3*PI/2 );
            for( int i = 0; i < tesselation; i++ ) // ( w+b, h ) for i = 0
            {
                double angle = i * PI / 2 / ( tesselation - 1 ) + 3*PI/2;
                GenerateLine( 3*( t + 1 )+1 + i,            w,   h, angle );
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
    gens.push_back( IGeometryGeneratorPtr( new Generator::MainComp() ) );
    gens.push_back( IGeometryGeneratorPtr( new Generator::SideComp( depth ) ) );
    gens.push_back( IGeometryGeneratorPtr( new Generator::SideComp( -depth ) ) );
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