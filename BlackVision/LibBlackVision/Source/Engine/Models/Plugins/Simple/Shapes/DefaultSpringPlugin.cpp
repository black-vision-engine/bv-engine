#include "stdafx.h"

#include "DefaultSpringPlugin.h"

#include "Memory/MemoryLeaks.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"


namespace bv { namespace model {
	
typedef ParamEnum< DefaultSpring::Plugin::WeightCenter > ParamEnumWC;
typedef ParamEnum< DefaultSpring::Plugin::MappingType > ParamEnumMT;

VoidPtr    ParamEnumWC::QueryParamTyped  ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}

template<>
static IParameterPtr        ParametersFactory::CreateTypedParameter< DefaultSpring::Plugin::WeightCenter >                 ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return CreateParameterEnum< DefaultSpring::Plugin::WeightCenter >( name, timeline );
}


VoidPtr    ParamEnumMT::QueryParamTyped  ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}

template<>
static IParameterPtr        ParametersFactory::CreateTypedParameter< DefaultSpring::Plugin::MappingType >                 ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return CreateParameterEnum< DefaultSpring::Plugin::MappingType >( name, timeline );
}


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
const std::string PN::MAPPINGTYPE = "mapping type";



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

    h.SetOrCreateVacModel();
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
	h.AddParam< IntInterpolator, Plugin::MappingType, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumMT >
		( PN::MAPPINGTYPE, Plugin::MappingType::DOUBLETEXTURE, true, true );

    h.SetOrCreatePSModel();

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
		ParameterChanged( PN::WEIGHTCENTERZ ) ||
		ParameterChanged( PN::MAPPINGTYPE );
}


class Generator : public IGeometryNormalsUVsGenerator
{
protected:
    IParamValModelPtr model;

	int tesselation;
	int tesselation2;
    float r;
    float r2;
    int turns;
	float delta;
	Plugin::MappingType mapping_type;

	glm::vec3 center_translate;
public:
	Generator( IParamValModelPtr m ) : model( m ) { center_translate = glm::vec3( 0.0, 0.0, 0.0); }

	void setLocalParameters();
    void GenerateGeometryNormalsUVs( Float3AttributeChannelPtr, Float3AttributeChannelPtr, Float2AttributeChannelPtr );
	void computeWeightCenter( Plugin::WeightCenter centerX, Plugin::WeightCenter centerY, Plugin::WeightCenter centerZ );
};

class ClosureGenerator : public Generator
{
private:
	bool rotated;		///< Indicates which closure should we generate.
public:
	ClosureGenerator( IParamValModelPtr m, bool rot ) : Generator( m ), rotated( rot ) {}

    void            GenerateGeometryNormalsUVs      ( Float3AttributeChannelPtr, Float3AttributeChannelPtr, Float2AttributeChannelPtr );
	void            GenerateClosure                 ( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs, int outerLoop, bool inverseOrdering );
	glm::vec2       computeUV                       ( int loop_counter, double h, bool center  );
};

std::vector<IGeometryGeneratorPtr>  Plugin::GetGenerators()
{
    std::vector<IGeometryGeneratorPtr> gens;

    gens.push_back( std::make_shared< Generator >( m_pluginParamValModel->GetVertexAttributesChannelModel() ) );
	gens.push_back( std::make_shared< ClosureGenerator >( m_pluginParamValModel->GetVertexAttributesChannelModel(), false ) );
	gens.push_back( std::make_shared< ClosureGenerator >( m_pluginParamValModel->GetVertexAttributesChannelModel(), true ) );

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
	std::shared_ptr< ParamEnum< Plugin::MappingType >	>		m_mappingType;;

    m_tesselation = QueryTypedValue< ValueIntPtr >( model->GetValue( PN::TESSELATION ) );
	m_tesselation2 = QueryTypedValue< ValueIntPtr >( model->GetValue( PN::TESSELATION2 ) );
    m_turns = QueryTypedValue< ValueIntPtr >( model->GetValue( PN::TURNS ) );
    m_radius = QueryTypedValue< ValueFloatPtr >( model->GetValue( PN::RADIUS ) );
    m_radiusCrossSection = QueryTypedValue< ValueFloatPtr >( model->GetValue( PN::RADIUSCROSSSECTION ) );
    m_delta = QueryTypedValue< ValueFloatPtr >( model->GetValue( PN::DELTA ) );

	m_weightCenterX = QueryTypedParam< std::shared_ptr< ParamEnum< Plugin::WeightCenter > > >( model->GetParameter( PN::WEIGHTCENTERX ) );
	m_weightCenterY = QueryTypedParam< std::shared_ptr< ParamEnum< Plugin::WeightCenter > > >( model->GetParameter( PN::WEIGHTCENTERY ) );
	m_weightCenterZ = QueryTypedParam< std::shared_ptr< ParamEnum< Plugin::WeightCenter > > >( model->GetParameter( PN::WEIGHTCENTERZ ) );

	m_mappingType = QueryTypedParam< std::shared_ptr< ParamEnum< Plugin::MappingType > > >( model->GetParameter( PN::MAPPINGTYPE ) );

	// Set fields of class generator
    tesselation = m_tesselation->GetValue();
	tesselation2 = m_tesselation2->GetValue();
    r = m_radius->GetValue();
    r2 = m_radiusCrossSection->GetValue();
    turns = m_turns->GetValue();
	delta = m_delta->GetValue();
	mapping_type = m_mappingType->Evaluate();

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

void Generator::GenerateGeometryNormalsUVs( Float3AttributeChannelPtr verts, Float3AttributeChannelPtr normals, Float2AttributeChannelPtr uvs )
{
	setLocalParameters();

    for( int i = 0; i < tesselation; i++ )
    {
        for( int j = 0; j <= tesselation2; j++ )
        {
            glm::vec3 point1;
            glm::vec3 point2;
            glm::vec2 uv1;
            glm::vec2 uv2;

            double crossSectionAngle = j * 2 * PI / tesselation2;

            double h = double( i ) / tesselation;
            double turnsAngle = h * turns * PI;

            point1.x = static_cast< float >( cos( turnsAngle ) * ( cos( crossSectionAngle ) * r2 + r ) );
            point1.y = static_cast< float >( h * delta + r2 * sin( crossSectionAngle ) );
            point1.z = static_cast< float >( sin( turnsAngle ) * ( cos( crossSectionAngle ) * r2 + r ) );

            uv1 = glm::vec2( double( j ) / tesselation, h );

            h = double( i + 1 ) / tesselation;
            turnsAngle = h * turns * PI;

            point2.x = static_cast< float >( cos( turnsAngle ) * ( cos( crossSectionAngle ) * r2 + r ) );
            point2.y = static_cast< float >( h * delta + r2 * sin( crossSectionAngle ) );
            point2.z = static_cast< float >( sin( turnsAngle ) * ( cos( crossSectionAngle ) * r2 + r ) );

            uv2 = glm::vec2( double( j ) / tesselation, h );

            verts->AddAttribute( point2 + center_translate );
            uvs->AddAttribute( uv2 );

            verts->AddAttribute( point1 + center_translate );
            uvs->AddAttribute( uv1 );
        }
    }

    GeometryGeneratorHelper::GenerateNonWeightedNormalsFromTriangleStrips( verts, normals );
}

void ClosureGenerator::GenerateClosure( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs, int outerLoop, bool inverseOrdering )
{

    double h = double( outerLoop ) / tesselation;
    double turnsAngle = h * turns * PI;

    for( int j = 0; j <= tesselation2; j++ )
    {
        glm::vec3 point1;
        glm::vec3 point2;
        glm::vec2 uv1;
        glm::vec2 uv2;

        double crossSectionAngle = j * 2 * PI / tesselation2;

        point1.x = static_cast< float >( cos( turnsAngle ) * ( cos( crossSectionAngle ) * r2 + r ) );
        point1.y = static_cast< float >( h * delta + r2 * sin( crossSectionAngle ) );
        point1.z = static_cast< float >( sin( turnsAngle ) * ( cos( crossSectionAngle ) * r2 + r ) );

        uv1 = computeUV( j, h, false );

        point2.x = static_cast< float >( cos( turnsAngle ) * r );
        point2.y = static_cast< float >( h * delta );
        point2.z = static_cast< float >( sin( turnsAngle ) * r );

        uv2 = computeUV( j, h, true );

        if( inverseOrdering )
        {
            verts->AddAttribute( point1 + center_translate );
            uvs->AddAttribute( uv1 );

            verts->AddAttribute( point2 + center_translate );
            uvs->AddAttribute( uv2 );
        }
        else
        {
            verts->AddAttribute( point2 + center_translate );
            uvs->AddAttribute( uv2 );

            verts->AddAttribute( point1 + center_translate );
            uvs->AddAttribute( uv1 );
        }
    }
}

void ClosureGenerator::GenerateGeometryNormalsUVs( Float3AttributeChannelPtr verts, Float3AttributeChannelPtr normals, Float2AttributeChannelPtr uvs )
{
	setLocalParameters();

	if( rotated )
		GenerateClosure( verts, uvs, tesselation, false );
	else
		GenerateClosure( verts, uvs, 0, true );

    GeometryGeneratorHelper::GenerateNonWeightedNormalsFromTriangleStrips( verts, normals );
}

/**@brief Generates UV depending on mapping type.

@param[in] loop_counter Number of loops made by generating loop.
@param[in] h The same h as in generateClosure function.
@param[in] center Center of the cirlce.
*/
glm::vec2 ClosureGenerator::computeUV( int loop_counter, double h, bool center )
{
	if( mapping_type == Plugin::MappingType::OLDSTYLE )
		return glm::vec2( (double)loop_counter / tesselation2, h );
	else if( mapping_type == Plugin::MappingType::DOUBLETEXTURE )
	{
		double angle = TWOPI * (double)loop_counter / tesselation2;
		glm::vec2 translate;
		glm::vec2 scale( 0.25, 0.5 );

		if( rotated )
			translate = glm::vec2( 0.25, 0.5 );
		else
			translate = glm::vec2( 0.75, 0.5 );

		if( center )
			return translate;

		glm::vec2 result( cos( angle ), sin( angle ) );
		result = translate + scale * result;

		return result;
	}
	else
		assert(false);

	 return glm::vec2( 0.0, 0.0 );
}


} } }