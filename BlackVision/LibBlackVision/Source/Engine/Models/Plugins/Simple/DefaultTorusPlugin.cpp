#include "DefaultTorusPlugin.h"

namespace bv { namespace model { namespace DefaultTorus {

const std::string PN::OPENANGLE = "open angle";
const std::string PN::OPENANGLEMODE = "open angle mode";
const std::string PN::RADIUS = "radius";
const std::string PN::RADIUSCROSSSECTION = "radius2";
const std::string PN::TESSELATION = "tesselation";


PluginDesc::PluginDesc()
    : DefaultGeometryPluginDescBase( UID(), "torus" )
{
}

DefaultPluginParamValModelPtr   PluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper h( timeEvaluator );

    h.CreateVacModel();
    h.AddSimpleParam( PN::TESSELATION, 10, true, true );
    h.AddSimpleParam( PN::RADIUS, 1.f, true, true );
    h.AddSimpleParam( PN::RADIUSCROSSSECTION, 0.5f, true, true );
    h.AddSimpleParam( PN::OPENANGLE, 360.f, true, true );
    //h.AddParam( PN::OPENANGLEMODE, ... );

    return h.GetModel();
}

IPluginPtr                      PluginDesc::CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< Plugin >( name, prev, timeEvaluator );
}

std::string                     PluginDesc::UID                 ()
{
    return "DEFAULT_TORUS";
}


bool                                Plugin::NeedsTopologyUpdate()
{
    return ParameterChanged( PN::TESSELATION ) ||
        ParameterChanged( PN::RADIUS ) ||
        ParameterChanged( PN::RADIUSCROSSSECTION ) ||
        ParameterChanged( PN::OPENANGLE ); // || ParameterChanged( PN::OPENANGLEMODE )
}

Plugin::Plugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model )
    : DefaultGeometryPluginBase( name, uid, prev, model )
{
    m_tesselation = QueryTypedValue< ValueIntPtr >( GetValue( PN::TESSELATION ) );
    m_radius = QueryTypedValue< ValueFloatPtr >( GetValue( PN::RADIUS ) );
    m_radiusCrossSection = QueryTypedValue< ValueFloatPtr >( GetValue( PN::RADIUSCROSSSECTION ) );
    m_openAngle = QueryTypedValue< ValueFloatPtr >( GetValue( PN::OPENANGLE ) );

    m_pluginParamValModel->Update();
    InitGeometry();
}

#include "Mathematics/Defines.h"
class Generator : public IGeometryAndUVsGenerator
{
    int tesselation;
    float radius, radius2, openangle;

    glm::vec3 **v;
    int n, m;
public:
    Generator( int t, float r, float r2, float oa )
        : tesselation( t ), radius( r ), radius2( r2), openangle( oa ) { }

    Type GetType() { return Type::GEOMETRY_AND_UVS; }

    void GenerateGeometryAndUVs( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs ) override
    {
    //    Init();
    //    GenerateV();
    //    CopyV( verts, uvs );
    //    Deinit();
        for( int i = 0; i <= tesselation; i++ )
            for( int j = 0; j <= tesselation; j++ )
            {
                double phi = i * 2*PI / tesselation;
                double theta = j * 2*PI / tesselation;

                verts->AddAttribute( glm::vec3( cos( theta )*( radius + radius2*cos( phi ) ), sin(theta) * ( radius + radius2 * cos(phi) ), radius * sin(phi) ) );
                uvs->AddAttribute( glm::vec2( float(i) / tesselation, float(j) / tesselation ) );

                phi = (i+1) * 2*PI / tesselation;
                theta = j * 2*PI / tesselation;

                verts->AddAttribute( glm::vec3( cos( theta )*( radius + radius2*cos( phi ) ), sin(theta) * ( radius + radius2 * cos(phi) ), radius * sin(phi) ) );
                uvs->AddAttribute( glm::vec2( float(i+1) / tesselation, float(j) / tesselation ) );
            }
    }

    //void Init() 
    //{ 
    //    n = tesselation;
    //    m = tesselation;
    //    assert( n >= 0 );
    //    v = new glm::vec3*[ n ];
    //    for( int i = 0; i < n; i++ )
    //        v[ i ] = new glm::vec3[ m ];
    //}

    //void Deinit()
    //{
    //    for( int i = 0; i < n; i++ )
    //        delete[] v[i];
    //    delete[] v;
    //}

    //void CopyV( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs )
    //{
    //    for( int i = 0; i < n-1; i++ )
    //        for( int j = 0; j < m; j++ )
    //        {
    //            verts->AddAttribute( v[ i   ][ j ] );
    //            verts->AddAttribute( v[ i+1 ][ j ] );
    //        }
    //    for( int j = 0; j < m; j++ )
    //    {
    //        verts->AddAttribute( v[ n-1 ][ j ] );
    //        verts->AddAttribute( v[ 0   ][ j ] );
    //    }

    //    for( SizeType v = 0; v < verts->GetNumEntries(); v++ )
    //    {
    //        glm::vec3 vert = verts->GetVertices()[ v ];
    //        uvs->AddAttribute( glm::vec2( 0.5*( vert.x + vert.y + 1.f ),
    //                                        vert.z + 0.5 ) ); // FIXME: scaling
    //    }
    //}

    //void GenerateV()
    //{
    //    for( int i = 0; i < n; i++ )
    //        for( int j = 0; j < m; j++ )
    //        {

    //        }
    //}
};

std::vector<IGeometryGeneratorPtr>  Plugin::GetGenerators()
{
    std::vector< IGeometryGeneratorPtr > gens;
    
    gens.push_back( IGeometryGeneratorPtr( new Generator( 
        m_tesselation->GetValue(),
        m_radius->GetValue(),
        m_radiusCrossSection->GetValue(),
        m_openAngle->GetValue() 
        ) ) );

    return gens;
}


} } }
