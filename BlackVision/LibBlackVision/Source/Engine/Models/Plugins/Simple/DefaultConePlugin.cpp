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
class ConeGenerator : public IGeometryAndUVsGenerator
//class ConeGenerator : public IGeometryOnlyGenerator
{
    int tesselation;
    float height, inner_height, inner_radius, bevel, open_angle;
public:
    ConeGenerator( int t, float ih, float ir, float b, float oa, float h )
        : tesselation( t ), inner_height( ih ), inner_radius( ir ), bevel( b ), open_angle( oa ), height( h ) { }

    virtual Type GetType() { return Type::GEOMETRY_AND_UVS; }
    //virtual Type GetType() { return Type::GEOMETRY_ONLY; }

    virtual void GenerateGeometryAndUVs( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs )
    //virtual void GenerateGeometry( Float3AttributeChannelPtr verts )
    {
// outer component
        for( int i = 0; i < tesselation ; i++ )
        {
            double angle1 = i     * 2 * PI / tesselation;
            double angle2 = (i+1) * 2 * PI / tesselation;

            verts->AddAttribute( glm::vec3( 0, height, 0 ) );
            verts->AddAttribute( glm::vec3( cos( angle1 ), 0, sin( angle1 ) ) );
            verts->AddAttribute( glm::vec3( cos( angle2 ), 0, sin( angle2 ) ) );
            verts->AddAttribute( glm::vec3( 0, height, 0 ) );
        }

        for( SizeType v = 0; v < verts->GetNumEntries(); v++ )
        {
            glm::vec3 vert = verts->GetVertices()[ v ];
            uvs->AddAttribute( glm::vec2( vert.x*0.5 + 0.5,
                                            vert.y*0.5 + 0.5 ) );
        }
    }
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

    m_pluginParamValModel->Update();
    InitGeometry();
}

IGeometryGenerator*                 DefaultConePlugin::GetGenerator()
{
    return new ConeGenerator( m_tesselation->GetValue(),
        m_innerHeight->GetValue(),
        m_innerRadius->GetValue(),
        m_roundedTipHeight->GetValue(),
        m_openAngle->GetValue(),
        m_height->GetValue()
        );
}

bool                                DefaultConePlugin::NeedsTopologyUpdate()
{
    return ParameterChanged( PN::TESSELATION ) ||
        ParameterChanged( PN::INNERHEIGHT ) ||
        ParameterChanged( PN::OUTERRADIUS ) ||
        ParameterChanged( PN::INNERHEIGHT ) ||
        ParameterChanged( PN::ROUNDEDTIPHEIGHT ) ||
        ParameterChanged( PN::OPENANGLE );
}

} } }
