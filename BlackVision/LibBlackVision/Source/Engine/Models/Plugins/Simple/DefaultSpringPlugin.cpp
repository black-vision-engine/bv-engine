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
    h.AddSimpleParam( PN::TESSELATION, 100, true, true );
    h.AddSimpleParam( PN::RADIUS, 0.5f, true, true );
    h.AddSimpleParam( PN::RADIUSCROSSSECTION, 0.5f, true, true );
    h.AddSimpleParam( PN::DELTA, 0.5f, true, true );
    h.AddSimpleParam( PN::TURNS, 10, true, true );

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

	int tesselation;
    float r;
    float r2;
    int turns;
	float delta;
public:
    Generator( IParamValModelPtr m ) : model( m ) { }

    void GenerateGeometryAndUVs( Float3AttributeChannelPtr, Float2AttributeChannelPtr );
	void generateClosure( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs, int outer_loop  );
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

    tesselation = m_tesselation->GetValue();
    r = m_radius->GetValue();
    r2 = m_radiusCrossSection->GetValue();
    turns = m_turns->GetValue();
	delta = m_delta->GetValue();

	generateClosure( verts, uvs, 0 );

    for( int i = 0; i < tesselation; i++ )
        for( int j = 0; j <= tesselation; j++ )
        {
            double crossSectionAngle = j *2*PI / tesselation;

            double h = double(i) / tesselation;
            double turnsAngle = h * turns * PI;
            verts->AddAttribute( glm::vec3( cos( turnsAngle )*( cos( crossSectionAngle )*r2 + r ), h * delta + r2 * sin( crossSectionAngle ), sin( turnsAngle )*( cos( crossSectionAngle )*r2 + r ) ) );
            uvs->AddAttribute( glm::vec2( double(j) / tesselation, h ) );

            h = double(i+1) / tesselation;
            turnsAngle = h * turns * PI;
            verts->AddAttribute( glm::vec3( cos( turnsAngle )*( cos( crossSectionAngle )*r2 + r ),  h * delta + r2 * sin( crossSectionAngle ), sin( turnsAngle )*( cos( crossSectionAngle )*r2 + r ) ) );
            uvs->AddAttribute( glm::vec2( double(j) / tesselation, h ) );
        }

	generateClosure( verts, uvs, tesselation );
}

void Generator::generateClosure( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs, int outer_loop )
{
	
    double h = double( outer_loop ) / tesselation;
    double turnsAngle = h * turns * PI;

	for( int j = 0; j <= tesselation; j++ )
    {
        double crossSectionAngle = j *2*PI / tesselation;

		verts->AddAttribute( glm::vec3( cos( turnsAngle )*( cos( crossSectionAngle )*r2 + r ), h * delta + r2 * sin( crossSectionAngle ), sin( turnsAngle )*( cos( crossSectionAngle )*r2 + r ) ) );
		uvs->AddAttribute( glm::vec2( double(j) / tesselation, h ) );

		verts->AddAttribute( glm::vec3( cos( turnsAngle ) * r, h * delta, sin( turnsAngle ) * r ) );
		uvs->AddAttribute( glm::vec2( double(j) / tesselation, h ) );
	}
}

} } }