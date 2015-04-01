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
    public:
        Type GetType() { return Type::GEOMETRY_AND_UVS; }

        void GenerateGeometryAndUVs( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs ) override
        {
            double w = dims.x/2 - bevel, 
                h = dims.y/2 - bevel, 
                d = dims.z/2 - bevel,
                b = bevel;
// top
            verts->AddAttribute( glm::vec3(  w,  h+b, -d ) );
            verts->AddAttribute( glm::vec3(  w,  h+b,  d ) );
            for( int i = 0; i < tesselation; i++ ) // (-w, h+b) for i = 0
            {
                double angle = i * PI / 2 / ( tesselation - 1 );
                verts->AddAttribute( glm::vec3( -w - b*sin( angle ), h + b*cos(angle), -d ) );
                verts->AddAttribute( glm::vec3( -w - b*sin( angle ), h + b*cos(angle), d ) );
            }
// left
            verts->AddAttribute( glm::vec3( -w-b,  h, -d ) );
            verts->AddAttribute( glm::vec3( -w-b,  h,  d ) );
            for( int i = 0; i < tesselation; i++ ) // (-w-b, -h ) for i = 0
            {
                double angle = i * PI / 2 / ( tesselation - 1 );
                verts->AddAttribute( glm::vec3( -w - b*cos( angle ), -h - b*sin(angle), -d ) );
                verts->AddAttribute( glm::vec3( -w - b*cos( angle ), -h - b*sin(angle), d ) );
            }
// bottom
            verts->AddAttribute( glm::vec3( -w, -h-b, -d ) );
            verts->AddAttribute( glm::vec3( -w, -h-b,  d ) );
            for( int i = 0; i < tesselation; i++ ) // ( w, -h-b ) for i = 0
            {
                double angle = i * PI / 2 / ( tesselation - 1 );
                verts->AddAttribute( glm::vec3( w + b*sin( angle ), -h - b*cos(angle), -d ) );
                verts->AddAttribute( glm::vec3( w + b*sin( angle ), -h - b*cos(angle), d ) );
            }
// right
            verts->AddAttribute( glm::vec3(  w+b, -h, -d ) );
            verts->AddAttribute( glm::vec3(  w+b, -h,  d ) );
            for( int i = 0; i < tesselation; i++ ) // ( w+b, h ) for i = 0
            {
                double angle = i * PI / 2 / ( tesselation - 1 );
                verts->AddAttribute( glm::vec3( w + b*cos( angle ), h + b*sin(angle), -d ) );
                verts->AddAttribute( glm::vec3( w + b*cos( angle ), h + b*sin(angle), d ) );
            }
// and top once again to close
            verts->AddAttribute( glm::vec3(  w,  h, -d ) );
            verts->AddAttribute( glm::vec3(  w,  h,  d ) );
            for( int i = 0; i < tesselation; i++ )
                ;
            
            for( SizeType v = 0; v < verts->GetNumEntries(); v++ )
            {
                glm::vec3 vert = verts->GetVertices()[ v ];
                uvs->AddAttribute( glm::vec2( 0.5*( vert.x + vert.y + 1.f ),
                                                vert.z + 0.5 ) ); // FIXME: scaling
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

    InitGeometry();
}

} } }