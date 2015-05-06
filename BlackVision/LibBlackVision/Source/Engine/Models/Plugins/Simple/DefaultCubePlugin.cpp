#include "DefaultCubePlugin.h"

namespace bv { namespace model {
	
typedef ParamEnum< DefaultCube::Plugin::WeightCenter > ParamEnumWC;
	

VoidPtr    ParamEnumWC::QueryParamTyped  ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}

template<>
static IParameterPtr        ParametersFactory::CreateTypedParameter< DefaultCube::Plugin::WeightCenter >                 ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return CreateParameterEnum< DefaultCube::Plugin::WeightCenter >( name, timeline );
}

#include "Engine/Models/Plugins/ParamValModel/SimpleParamValEvaluator.inl"
	
namespace DefaultCube {

const std::string PN::TESSELATION = "tesselation";
const std::string PN::DIMENSIONS = "dimensions";
const std::string PN::BEVEL = "bevel";
const std::string PN::WEIGHTCENTERX = "weight center x";
const std::string PN::WEIGHTCENTERY = "weight center y";
const std::string PN::WEIGHTCENTERZ = "weight center z";


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

	glm::vec3 computeWeightCenter( Plugin::WeightCenter centerX, Plugin::WeightCenter centerY, Plugin::WeightCenter centerZ )
	{
		center_translate = glm::vec3( 0.0f, 0.0f, 0.0f );

		if( centerX == Plugin::WeightCenter::MAX )
			center_translate += glm::vec3( -dims.x / 2, 0.0, 0.0 );
		else if( centerX == Plugin::WeightCenter::CENTER )
			center_translate += glm::vec3( 0.0, 0.0, 0.0 );
		else if( centerX == Plugin::WeightCenter::MIN )
			center_translate += glm::vec3( dims.x / 2, 0.0, 0.0 );
	
		if( centerY == Plugin::WeightCenter::MAX )
			center_translate += glm::vec3( 0.0f, -dims.y / 2, 0.0f );
		else if( centerY == Plugin::WeightCenter::CENTER )
			center_translate += glm::vec3( 0.0f, 0.0, 0.0f );
		else if( centerY == Plugin::WeightCenter::MIN )
			center_translate += glm::vec3( 0.0f, dims.y / 2, 0.0f );

		if( centerZ == Plugin::WeightCenter::MAX )
			center_translate += glm::vec3( 0.0, 0.0, -dims.z / 2 );
		else if( centerZ == Plugin::WeightCenter::CENTER )
			center_translate += glm::vec3( 0.0, 0.0, 0.0 );
		else if( centerZ == Plugin::WeightCenter::MIN )
			center_translate += glm::vec3( 0.0, 0.0, dims.z / 2 );
		
		return center_translate;
	}


    class SideComp : public IGeometryAndUVsGenerator
    {
        double d;
    public:
        Type GetType() { return Type::GEOMETRY_AND_UVS; }

        void GenerateGeometryAndUVs( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs ) override
        {
            double w = dims.x/2 - bevel, 
                h = dims.y/2 - bevel;

            verts->AddAttribute( glm::vec3(  w,  h, d ) + center_translate );
            verts->AddAttribute( glm::vec3(  w, -h, d ) + center_translate );
            verts->AddAttribute( glm::vec3( -w,  h, d ) + center_translate );
            verts->AddAttribute( glm::vec3( -w, -h, d ) + center_translate );

            uvs->AddAttribute( glm::vec2(  1,  1 ) );
            uvs->AddAttribute( glm::vec2(  1, -1 ) );
            uvs->AddAttribute( glm::vec2( -1,  1 ) );
            uvs->AddAttribute( glm::vec2( -1, -1 ) );
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

        void CopyV( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs )
        {
            for( int i = 0; i < n-1; i++ )
                for( int j = 0; j < m; j++ )
                {
					verts->AddAttribute( v[ i   ][ j ] + center_translate );
                    verts->AddAttribute( v[ i+1 ][ j ] + center_translate );
                }
            for( int j = 0; j < m; j++ )
            {
                verts->AddAttribute( v[ n-1 ][ j ] + center_translate );
                verts->AddAttribute( v[ 0   ][ j ] + center_translate );
            }

            for( SizeType v = 0; v < verts->GetNumEntries(); v++ )
            {
                glm::vec3 vert = verts->GetVertices()[ v ];
				vert -= center_translate;
                uvs->AddAttribute( glm::vec2( 0.5*( vert.x + vert.y + 1.f ),
                                                vert.z + 0.5 ) ); // FIXME: scaling
            }
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
		ParameterChanged( PN::WEIGHTCENTERZ );;
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

    m_pluginParamValModel->Update();
    InitGeometry();
}

} } }