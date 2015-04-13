#include "DefaultCubePlugin.h"

namespace bv { namespace model { namespace DefaultCube {

const std::string PN::TESSELATION = "tesselation";
const std::string PN::DIMENSIONS = "dimensions";
const std::string PN::BEVEL = "bevel";


PluginDesc::PluginDesc()
    : DefaultGeometryPluginDescBase( UID(), "cube" )
{
}

DefaultPluginParamValModelPtr   PluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper h( timeEvaluator );

    h.CreateVacModel();
    h.AddSimpleParam( PN::BEVEL, 0.f, true, true );
    h.AddSimpleParam( PN::DIMENSIONS, glm::vec3( 1, 1, 1 ), true, true );
    h.AddSimpleParam( PN::TESSELATION, 0, true, true );

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
            n = 4*(tesselation+1);
            m = 2; //(tesselation+1) * 2;
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
            for( int i = 0; i < n; i++ )
                for( int j = 0; j < m; j++ )
                    verts->AddAttribute( v[ i ][ j ] );
            for( int j = 0; j < m; j++ )
                verts->AddAttribute( v[ 0 ][ j ] );

            for( SizeType v = 0; v < verts->GetNumEntries(); v++ )
            {
                glm::vec3 vert = verts->GetVertices()[ v ];
                uvs->AddAttribute( glm::vec2( 0.5*( vert.x + vert.y + 1.f ),
                                                vert.z + 0.5 ) ); // FIXME: scaling
            }
        }

        void GenerateLine( int i, double x, double y, double a )
        {
            double d = dims.z/2 - bevel;
            double b = bevel;
            v[ i ][ 0 ] = glm::vec3( x - b*sin( a ), y + b*cos( a ), -d );
            v[ i ][ 1 ] = glm::vec3( x - b*sin( a ), y + b*cos( a ),  d );
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

    std::vector<IGeometryGeneratorPtr> gens;
    gens.push_back( IGeometryGeneratorPtr( new Generator::MainComp() ) );
    return gens;
}

bool                                Plugin::NeedsTopologyUpdate()
{
    return ParameterChanged( PN::BEVEL ) || 
        ParameterChanged( PN::DIMENSIONS ) ||
        ParameterChanged( PN::TESSELATION );
}

Plugin::Plugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model )
    : DefaultGeometryPluginBase( name, uid, prev, model )
{
    m_bevel = QueryTypedValue< ValueFloatPtr >( GetValue( PN::BEVEL ) );
    m_dimensions = QueryTypedValue< ValueVec3Ptr >( GetValue( PN::DIMENSIONS ) );
    m_tesselation = QueryTypedValue< ValueIntPtr >( GetValue( PN::TESSELATION ) );

    m_pluginParamValModel->Update();
    InitGeometry();
}

} } }