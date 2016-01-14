#include "DefaultRectPlugin.h"


namespace bv { namespace model { 


typedef ParamEnum< DefaultRect::Plugin::WeightCenter > ParamEnumWC;

// *******************************
//
VoidPtr                 ParamEnumWC::QueryParamTyped            ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}

// *******************************
//
template<>
static IParameterPtr    ParametersFactory::CreateTypedParameter< DefaultRect::Plugin::WeightCenter >                 ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return CreateParameterEnum< DefaultRect::Plugin::WeightCenter >( name, timeline );
}

#include "Engine/Models/Plugins/ParamValModel/SimpleParamValEvaluator.inl"

namespace DefaultRect {


const std::string PN::WIDTH = "width";
const std::string PN::HEIGHT = "height";
const std::string PN::WEIGHTCENTERX = "weight center x";
const std::string PN::WEIGHTCENTERY = "weight center y";

// *******************************
//
                                    PluginDesc::PluginDesc          ()
    : DefaultGeometryPluginDescBase( UID(), "rectangle" )
{
}

// *******************************
//
DefaultPluginParamValModelPtr       PluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper h( timeEvaluator );
    
    h.CreateVacModel();

    h.AddSimpleStatedParam( PN::WIDTH, 1.f );
    h.AddSimpleStatedParam( PN::HEIGHT, 1.f );

	h.AddParam< IntInterpolator, Plugin::WeightCenter, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumWC >
        ( DefaultRect::PN::WEIGHTCENTERX, Plugin::WeightCenter::CENTER, true, true );
	h.AddParam< IntInterpolator, Plugin::WeightCenter, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumWC >
        ( DefaultRect::PN::WEIGHTCENTERY, Plugin::WeightCenter::CENTER, true, true );

    return h.GetModel();
}

// *******************************
//
IPluginPtr                          PluginDesc::CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< Plugin >( name, prev, timeEvaluator );
}

// *******************************
//
std::string                         PluginDesc::UID                 ()
{
    return "DEFAULT_RECTANGLE";
}

// *******************************
//
bool                                Plugin::NeedsTopologyUpdate     ()
{
    return ParameterChanged( PN::WIDTH ) || 
        ParameterChanged( PN::HEIGHT ) ||
        ParameterChanged( PN::WEIGHTCENTERX )	||
		ParameterChanged( PN::WEIGHTCENTERY );
}

// *******************************
//
                                    Generator::Generator            ( float w, float h, Plugin::WeightCenter wcx, Plugin::WeightCenter wcy )
    : m_width( w )
    , m_height( h )
    , m_centerTranslate( GetWeightCenter( wcx, wcy ) )
{
}

// *******************************
//
IGeometryGenerator::Type            Generator::GetType              () 
{ 
    return IGeometryGenerator::Type::GEOMETRY_ONLY; 
}

// *******************************
//
glm::vec2                           Generator::GetWeightCenter      ( Plugin::WeightCenter centerX, Plugin::WeightCenter centerY )
{
	glm::vec2 centerTranslate;

    if( centerX == Plugin::WeightCenter::MIN )
    {
		centerTranslate += glm::vec2( -m_width / 2.f, 0.f );
    }
	else if( centerX == Plugin::WeightCenter::MAX )
    {
		centerTranslate += glm::vec2( m_width / 2.f, 0.f );
    }

    if( centerY == Plugin::WeightCenter::MIN )
    {
		centerTranslate += glm::vec2( 0.f, -m_height / 2.f );
    }
	else if( centerY == Plugin::WeightCenter::MAX )
    {
		centerTranslate += glm::vec2( 0.f, m_height / 2.f );
    }

	return centerTranslate;
}

// *******************************
//
void                                Generator::GenerateGeometry     ( Float3AttributeChannelPtr verts ) 
{
    float w = m_width / 2.f;
    float h = m_height / 2.f;

    verts->AddAttribute( glm::vec3( -w + m_centerTranslate.x, -h + m_centerTranslate.y, 0.f ) );
    verts->AddAttribute( glm::vec3(  w + m_centerTranslate.x, -h + m_centerTranslate.y, 0.f ) );
    verts->AddAttribute( glm::vec3( -w + m_centerTranslate.x,  h + m_centerTranslate.y, 0.f ) );
    verts->AddAttribute( glm::vec3(  w + m_centerTranslate.x,  h + m_centerTranslate.y, 0.f ) );
}

// *******************************
//
std::vector<IGeometryGeneratorPtr>    Plugin::GetGenerators()
{
    return std::vector< IGeometryGeneratorPtr >( 1, std::make_shared< Generator >
        ( m_width->GetValue(), m_height->GetValue(), m_weightCenterX->Evaluate(), m_weightCenterY->Evaluate() ) );
}

// *******************************
//
Plugin::Plugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model )
    : DefaultGeometryPluginBase( name, uid, prev, model )
{
    m_width = QueryTypedValue< ValueFloatPtr >( GetValue( PN::WIDTH ) );
    m_height = QueryTypedValue< ValueFloatPtr >( GetValue( PN::HEIGHT ) );

	m_weightCenterX = QueryTypedParam< std::shared_ptr< ParamEnum< WeightCenter > > >( GetParameter( PN::WEIGHTCENTERX ) );
	m_weightCenterY = QueryTypedParam< std::shared_ptr< ParamEnum< WeightCenter > > >( GetParameter( PN::WEIGHTCENTERY ) );

    m_pluginParamValModel->Update();
    InitGeometry();
}

} // DefaultRect
} // model
} // bv
