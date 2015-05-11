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
        ( PN::WEIGHTCENTERX, Plugin::WeightCenter::CENTER, true, true );\
	h.AddParam< IntInterpolator, Plugin::WeightCenter, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumWC >
        ( PN::WEIGHTCENTERY, Plugin::WeightCenter::MIN, true, true );
	h.AddParam< IntInterpolator, Plugin::WeightCenter, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumWC >
        ( PN::WEIGHTCENTERZ, Plugin::WeightCenter::CENTER, true, true );

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
protected:
    IParamValModelPtr model;

	int tesselation;
	int tesselation2;
    float r;
    float r2;
    int turns;
	float delta;

	glm::vec3 center_translate;
public:
	Generator( IParamValModelPtr m ) : model( m ) { center_translate = glm::vec3( 0.0, 0.0, 0.0); }

	void setLocalParameters();
    void GenerateGeometryAndUVs( Float3AttributeChannelPtr, Float2AttributeChannelPtr );
	void generateClosure( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs, int outer_loop  );
	void computeWeightCenter( Plugin::WeightCenter centerX, Plugin::WeightCenter centerY, Plugin::WeightCenter centerZ );
};

class ClosureGenerator : public Generator
{
private:
	bool rotated;		///< Indicates which closure should we generate.
public:
	ClosureGenerator( IParamValModelPtr m, bool rot ) : Generator( m ), rotated( rot ) {}

    void GenerateGeometryAndUVs( Float3AttributeChannelPtr, Float2AttributeChannelPtr );
};

std::vector<IGeometryGeneratorPtr>  Plugin::GetGenerators()
{
    std::vector<IGeometryGeneratorPtr> gens;

    gens.push_back( IGeometryGeneratorPtr( new Generator( m_pluginParamValModel->GetVertexAttributesChannelModel() ) ) );
	gens.push_back( IGeometryGeneratorPtr( new ClosureGenerator( m_pluginParamValModel->GetVertexAttributesChannelModel(), false ) ) );
	gens.push_back( IGeometryGeneratorPtr( new ClosureGenerator( m_pluginParamValModel->GetVertexAttributesChannelModel(), true ) ) );

    return gens;
}

#include "Mathematics/Defines.h"

void Generator::setLocalParameters()
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

	// Set fields of class generator
    tesselation = m_tesselation->GetValue();
	tesselation2 = m_tesselation2->GetValue();
    r = m_radius->GetValue();
    r2 = m_radiusCrossSection->GetValue();
    turns = m_turns->GetValue();
	delta = m_delta->GetValue();

	// Must be called after assingments above. It uses generator's variables.
	computeWeightCenter( m_weightCenterX->Evaluate(), m_weightCenterY->Evaluate(), m_weightCenterZ->Evaluate() );
}

void Generator::computeWeightCenter( Plugin::WeightCenter centerX, Plugin::WeightCenter centerY, Plugin::WeightCenter centerZ )
{
	center_translate = glm::vec3( 0.0, 0.0, 0.0 );

	if( centerX == Plugin::WeightCenter::MAX )
		center_translate += glm::vec3( -r - r2, 0.0, 0.0 );
	else if( centerX == Plugin::WeightCenter::CENTER )
		center_translate += glm::vec3( 0.0, 0.0, 0.0 );
	else if( centerX == Plugin::WeightCenter::MIN )
		center_translate += glm::vec3( r + r2, 0.0, 0.0 );
	
	if( centerY == Plugin::WeightCenter::MAX )
		center_translate += glm::vec3( 0.0, -delta - r2, 0.0 );
	else if( centerY == Plugin::WeightCenter::CENTER )
		center_translate += glm::vec3( 0.0, -delta / 2, 0.0 );
	else if( centerY == Plugin::WeightCenter::MIN )
		center_translate += glm::vec3( 0.0, r2, 0.0 );

	if( centerZ == Plugin::WeightCenter::MAX )
		center_translate += glm::vec3( 0.0, 0.0, -r - r2 );
	else if( centerZ == Plugin::WeightCenter::CENTER )
		center_translate += glm::vec3( 0.0, 0.0, 0.0 );
	else if( centerZ == Plugin::WeightCenter::MIN )
		center_translate += glm::vec3( 0.0, 0.0, r + r2 );
}

void Generator::GenerateGeometryAndUVs( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs )
{
	setLocalParameters();

    for( int i = 0; i < tesselation; i++ )
        for( int j = 0; j <= tesselation2; j++ )
        {
            double crossSectionAngle = j *2*PI / tesselation2;

            double h = double(i) / tesselation;
            double turnsAngle = h * turns * PI;

			float x = static_cast<float>( cos( turnsAngle )*( cos( crossSectionAngle )*r2 + r ) );
			float y = static_cast<float>( h * delta + r2 * sin( crossSectionAngle ) );
			float z = static_cast<float>( sin( turnsAngle )*( cos( crossSectionAngle )*r2 + r ) );

			verts->AddAttribute( glm::vec3( x, y, z ) + center_translate );
            uvs->AddAttribute( glm::vec2( double(j) / tesselation, h ) );

            h = double(i+1) / tesselation;
            turnsAngle = h * turns * PI;

			x = static_cast<float>( cos( turnsAngle )*( cos( crossSectionAngle )*r2 + r ) );
			y = static_cast<float>( h * delta + r2 * sin( crossSectionAngle ) );
			z = static_cast<float>( sin( turnsAngle )*( cos( crossSectionAngle )*r2 + r ) );

			verts->AddAttribute( glm::vec3( x, y, z ) + center_translate );
            uvs->AddAttribute( glm::vec2( double(j) / tesselation, h ) );
        }
}

void Generator::generateClosure( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs, int outer_loop )
{
	
    double h = double( outer_loop ) / tesselation;
    double turnsAngle = h * turns * PI;

	for( int j = 0; j <= tesselation2; j++ )
    {
        double crossSectionAngle = j *2*PI / tesselation2;

		float x = static_cast<float>( cos( turnsAngle )*( cos( crossSectionAngle )*r2 + r ) );
		float y = static_cast<float>( h * delta + r2 * sin( crossSectionAngle ) );
		float z = static_cast<float>( sin( turnsAngle )*( cos( crossSectionAngle )*r2 + r ) );

		verts->AddAttribute( glm::vec3( x, y, z ) + center_translate );
		uvs->AddAttribute( glm::vec2( double(j) / tesselation, h ) );

		x = static_cast<float>( cos( turnsAngle ) * r );
		y = static_cast<float>( h * delta );
		z = static_cast<float>( sin( turnsAngle ) * r );

		verts->AddAttribute( glm::vec3( x, y, z ) + center_translate );
		uvs->AddAttribute( glm::vec2( double(j) / tesselation, h ) );
	}
}

void ClosureGenerator::GenerateGeometryAndUVs( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs )
{
	setLocalParameters();

	if( rotated )
		generateClosure( verts, uvs, tesselation );
	else
		generateClosure( verts, uvs, 0 );
}


} } }