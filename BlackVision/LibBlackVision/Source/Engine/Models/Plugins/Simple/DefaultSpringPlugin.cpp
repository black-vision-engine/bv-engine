#include "DefaultSpringPlugin.h"

namespace bv { namespace model {
	
typedef ParamEnum< DefaultSpring::Plugin::WeightCenter > ParamEnumWC;


VoidPtr    ParamEnumWC::QueryParamTyped  ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}

template<>
static IParameterPtr        ParametersFactory::CreateTypedParameter< DefaultSpring::Plugin::WeightCenter >                 ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return CreateParameterEnum< DefaultSpring::Plugin::WeightCenter >( name, timeline );
}


#include "Engine/Models/Plugins/ParamValModel/SimpleParamValEvaluator.inl"
	
namespace DefaultSpring {

const std::string PN::TESSELATION = "tesselation";
const std::string PN::TESSELATION2 = "tesselation2";
const std::string PN::RADIUS = "radius";
const std::string PN::RADIUSCROSSSECTION = "radius2";
const std::string PN::DELTA = "delta";
const std::string PN::TURNS = "turns";
const std::string PN::WEIGHTCENTERX = "weight center x";
const std::string PN::WEIGHTCENTERY = "weight center y";
const std::string PN::WEIGHTCENTERZ = "weight center z";




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
	h.AddSimpleParam( PN::TESSELATION2, 12, true, true );
    h.AddSimpleParam( PN::RADIUS, 0.5f, true, true );
    h.AddSimpleParam( PN::RADIUSCROSSSECTION, 0.2f, true, true );
    h.AddSimpleParam( PN::DELTA, 6.0f, true, true );
    h.AddSimpleParam( PN::TURNS, 10, true, true );
	h.AddParam< IntInterpolator, Plugin::WeightCenter, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumWC >
        ( PN::WEIGHTCENTERX, Plugin::WeightCenter::MIN, true, true );\
	h.AddParam< IntInterpolator, Plugin::WeightCenter, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumWC >
        ( PN::WEIGHTCENTERY, Plugin::WeightCenter::MIN, true, true );
	h.AddParam< IntInterpolator, Plugin::WeightCenter, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumWC >
        ( PN::WEIGHTCENTERZ, Plugin::WeightCenter::MIN, true, true );

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
		ParameterChanged( PN::TESSELATION2 ) ||
        ParameterChanged( PN::TURNS ) ||
		ParameterChanged( PN::WEIGHTCENTERX ) ||
		ParameterChanged( PN::WEIGHTCENTERY ) ||
		ParameterChanged( PN::WEIGHTCENTERZ );
}


class Generator : public IGeometryAndUVsGenerator
{
    IParamValModelPtr model;

	int tesselation;
	int tesselation2;
    float r;
    float r2;
    int turns;
	float delta;

	glm::vec3 center_translate;
public:
    Generator( IParamValModelPtr m ) : model( m ) { }

    void GenerateGeometryAndUVs( Float3AttributeChannelPtr, Float2AttributeChannelPtr );
	void generateClosure( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs, int outer_loop  );
	glm::vec3 computeWeightCenter( Plugin::WeightCenter centerX, Plugin::WeightCenter centerY, Plugin::WeightCenter centerZ );
};

std::vector<IGeometryGeneratorPtr>  Plugin::GetGenerators()
{
    std::vector<IGeometryGeneratorPtr> gens;

    gens.push_back( IGeometryGeneratorPtr( new Generator( m_pluginParamValModel->GetVertexAttributesChannelModel() ) ) );

    return gens;
}

#include "Mathematics/Defines.h"


glm::vec3 Generator::computeWeightCenter( Plugin::WeightCenter centerX, Plugin::WeightCenter centerY, Plugin::WeightCenter centerZ )
{
	glm::vec3 centerTranslate( 0.0, 0.0, 0.0 );

	if( centerX == Plugin::WeightCenter::MAX )
		centerTranslate += glm::vec3( r + r2, 0.0, 0.0 );
	else if( centerX == Plugin::WeightCenter::CENTER )
		centerTranslate += glm::vec3( 0.0, 0.0, 0.0 );
	else if( centerX == Plugin::WeightCenter::MIN )
		centerTranslate += glm::vec3( -r - r2, 0.0, 0.0 );
	
	if( centerY == Plugin::WeightCenter::MAX )
		center_translate += glm::vec3( 0.0, 0.0, 0.0 ); //@todo

	if( centerZ == Plugin::WeightCenter::MAX )
		center_translate += glm::vec3( 0.0, 0.0, 0.0 ); //@todo

	return centerTranslate;
}

void Generator::GenerateGeometryAndUVs( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs )
{
    ValueIntPtr                                 m_tesselation;
	ValueIntPtr                                 m_tesselation2;
    ValueFloatPtr                               m_radius;
    ValueFloatPtr                               m_radiusCrossSection;
    ValueFloatPtr                               m_delta;
    ValueIntPtr                                 m_turns;
	
	std::shared_ptr< ParamEnum< Plugin::WeightCenter >	>		m_weightCenterX;
	std::shared_ptr< ParamEnum< Plugin::WeightCenter >	>		m_weightCenterY;
	std::shared_ptr< ParamEnum< Plugin::WeightCenter >	>		m_weightCenterZ;

    m_tesselation = QueryTypedValue< ValueIntPtr >( model->GetValue( PN::TESSELATION ) );
	m_tesselation2 = QueryTypedValue< ValueIntPtr >( model->GetValue( PN::TESSELATION2 ) );
    m_turns = QueryTypedValue< ValueIntPtr >( model->GetValue( PN::TURNS ) );
    m_radius = QueryTypedValue< ValueFloatPtr >( model->GetValue( PN::RADIUS ) );
    m_radiusCrossSection = QueryTypedValue< ValueFloatPtr >( model->GetValue( PN::RADIUSCROSSSECTION ) );
    m_delta = QueryTypedValue< ValueFloatPtr >( model->GetValue( PN::DELTA ) );

	m_weightCenterX = QueryTypedParam< std::shared_ptr< ParamEnum< Plugin::WeightCenter > > >( model->GetParameter( PN::WEIGHTCENTERX ) );
	m_weightCenterY = QueryTypedParam< std::shared_ptr< ParamEnum< Plugin::WeightCenter > > >( model->GetParameter( PN::WEIGHTCENTERY ) );
	m_weightCenterZ = QueryTypedParam< std::shared_ptr< ParamEnum< Plugin::WeightCenter > > >( model->GetParameter( PN::WEIGHTCENTERZ ) );

    tesselation = m_tesselation->GetValue();
	tesselation2 = m_tesselation2->GetValue();
    r = m_radius->GetValue();
    r2 = m_radiusCrossSection->GetValue();
    turns = m_turns->GetValue();
	delta = m_delta->GetValue();

	generateClosure( verts, uvs, 0 );

    for( int i = 0; i < tesselation; i++ )
        for( int j = 0; j <= tesselation2; j++ )
        {
            double crossSectionAngle = j *2*PI / tesselation2;

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

	for( int j = 0; j <= tesselation2; j++ )
    {
        double crossSectionAngle = j *2*PI / tesselation2;

		verts->AddAttribute( glm::vec3( cos( turnsAngle )*( cos( crossSectionAngle )*r2 + r ), h * delta + r2 * sin( crossSectionAngle ), sin( turnsAngle )*( cos( crossSectionAngle )*r2 + r ) ) );
		uvs->AddAttribute( glm::vec2( double(j) / tesselation, h ) );

		verts->AddAttribute( glm::vec3( cos( turnsAngle ) * r, h * delta, sin( turnsAngle ) * r ) );
		uvs->AddAttribute( glm::vec2( double(j) / tesselation, h ) );
	}
}

} } }