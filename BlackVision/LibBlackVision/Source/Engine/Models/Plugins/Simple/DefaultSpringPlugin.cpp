#include "DefaultSpringPlugin.h"

namespace bv { namespace model { namespace DefaultSpring {

const std::string PN::TESSELATION = "tesselation";
const std::string PN::RADIUS = "radius";
const std::string PN::RADIUSCROSSSECTION = "radius2";
const std::string PN::DELTA = "delta";
const std::string PN::TURNS = "turns";


IPluginPtr                      PluginDesc::CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< Plugin >( name, prev, timeEvaluator );
}

std::string                     PluginDesc::UID                 ()
{
    return "DEFAULT_SPRING";
}

PluginDesc::PluginDesc()
    : DefaultGeometryPluginDescBase( UID(), "spring" )
{
}

DefaultPluginParamValModelPtr   PluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper h( timeEvaluator );

    h.CreateVacModel();
    h.AddSimpleParam( PN::TESSELATION, 10, true, true );
    h.AddSimpleParam( PN::RADIUS, 1.f, true, true );
    h.AddSimpleParam( PN::RADIUSCROSSSECTION, 0.1f, true, true );
    h.AddSimpleParam( PN::DELTA, 0.5f, true, true );
    h.AddSimpleParam( PN::TURNS, 1, true, true );

    return h.GetModel();
}


Plugin::Plugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model )
    : DefaultGeometryPluginBase( name, uid, prev, model )
{

    m_pluginParamValModel->Update();
    InitGeometry();
}

bool                                Plugin::NeedsTopologyUpdate()
{
    return
        ParameterChanged( PN::DELTA ) ||
        ParameterChanged( PN::RADIUS ) ||
        ParameterChanged( PN::RADIUSCROSSSECTION ) ||
        ParameterChanged( PN::TESSELATION ) ||
        ParameterChanged( PN::TURNS );
}

class Generator : public IGeometryAndUVsGenerator
{
    IParamValModelPtr model;
public:
    Generator( IParamValModelPtr m ) : model( m ) { }

    void GenerateGeometryAndUVs( Float3AttributeChannelPtr, Float2AttributeChannelPtr );
};

std::vector<IGeometryGeneratorPtr>  Plugin::GetGenerators()
{
    std::vector<IGeometryGeneratorPtr> gens;

    gens.push_back( IGeometryGeneratorPtr( new Generator( m_pluginParamValModel->GetVertexAttributesChannelModel() ) ) );

    return gens;
}

#include "Mathematics/Defines.h"
void Generator::GenerateGeometryAndUVs( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs )
{
    ValueIntPtr                                 m_tesselation;
    ValueFloatPtr                               m_radius;
    ValueFloatPtr                               m_radiusCrossSection;
    ValueFloatPtr                               m_delta;
    ValueIntPtr                                 m_turns;

    m_tesselation = QueryTypedValue< ValueIntPtr >( model->GetValue( PN::TESSELATION ) );
    m_turns = QueryTypedValue< ValueIntPtr >( model->GetValue( PN::TURNS ) );
    m_radius = QueryTypedValue< ValueFloatPtr >( model->GetValue( PN::RADIUS ) );
    m_radiusCrossSection = QueryTypedValue< ValueFloatPtr >( model->GetValue( PN::RADIUSCROSSSECTION ) );
    m_delta = QueryTypedValue< ValueFloatPtr >( model->GetValue( PN::DELTA ) );

    int t = m_tesselation->GetValue();

    for( int i = 0; i <= t; i++ )
        for( int j = 0; j <= t; j++ )
        {
            double angle = j *2*PI / t;
            
            double h = double(i) / t;
            verts->AddAttribute( glm::vec3( cos( angle ), sin( angle ), h ) );
            uvs->AddAttribute( glm::vec2( double(j) / t, h ) );

            h = double(i+1) / t;
            verts->AddAttribute( glm::vec3( cos( angle ), sin( angle),  h ) );
            uvs->AddAttribute( glm::vec2( double(j) / t, h ) );
        }
}

} } }