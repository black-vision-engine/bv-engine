#include "DefaultText3DPlugin.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Types/Values/ValuesFactory.h"

#include "Engine/Models/Plugins/Channels/Geometry/HelperVertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"

#include "Engine/Models/Plugins/Channels/Geometry/VacAABB.h"

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Events/Events.h"
#include "Engine/Events/Interfaces/IEventManager.h"

namespace bv { namespace model {


// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultText3DPluginDesc::DefaultText3DPluginDesc                            ()
    : BasePluginDescriptor( UID(), "text3d", "txt3d" )
{
}

// *******************************
//
IPluginPtr              DefaultText3DPluginDesc::CreatePlugin             ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultText3DPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultText3DPluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    //Create all models
    DefaultPluginParamValModelPtr model  = std::make_shared< DefaultPluginParamValModel >( timeEvaluator );
    DefaultParamValModelPtr psModel      = std::make_shared< DefaultParamValModel >();
    DefaultParamValModelPtr vsModel      = std::make_shared< DefaultParamValModel >();
    DefaultParamValModelPtr plModel      = std::make_shared< DefaultParamValModel >();


    //Create all parameters and evaluators
    SimpleWStringEvaluatorPtr   textEvaluator           = ParamValEvaluatorFactory::CreateSimpleWStringEvaluator( "text", timeEvaluator );
    SimpleFloatEvaluatorPtr     alphaEvaluator          = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "alpha", timeEvaluator );
    SimpleTransformEvaluatorPtr trTxEvaluator           = ParamValEvaluatorFactory::CreateSimpleTransformEvaluator( "txMat", timeEvaluator );
    SimpleFloatEvaluatorPtr     fontSizeEvaluator       = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "fontSize", timeEvaluator );

    SimpleFloatEvaluatorPtr     blurSizeEvaluator       = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "blurSize", timeEvaluator );
	SimpleFloatEvaluatorPtr     outlineSizeEvaluator    = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "outlineSize", timeEvaluator );
	SimpleVec4EvaluatorPtr      outlineColorEvaluator   = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( "outlineColor", timeEvaluator );

    SimpleVec4EvaluatorPtr      rccBeginColorEvaluator  = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( "rcc_beginColor", timeEvaluator );
    SimpleVec4EvaluatorPtr      rccEndColorEvaluator    = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( "rcc_endColor", timeEvaluator );
    SimpleIntEvaluatorPtr       colTextEffectIdEvaluator= ParamValEvaluatorFactory::CreateSimpleIntEvaluator( "colTextEffectId", timeEvaluator );
    SimpleIntEvaluatorPtr       transformTextEffectIdEvaluator= ParamValEvaluatorFactory::CreateSimpleIntEvaluator( "transformTextEffectId", timeEvaluator );

    SimpleFloatEvaluatorPtr     timeValEvaluator       = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "time", timeEvaluator );
    SimpleFloatEvaluatorPtr     transformEffectVal1Evaluator       = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "transformEffectVal1", timeEvaluator );
    SimpleFloatEvaluatorPtr     transformEffectVal2Evaluator       = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "transformEffectVal2", timeEvaluator );

    SimpleFloatEvaluatorPtr     spacingEvaluator        = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "spacing", timeEvaluator );
    SimpleFloatEvaluatorPtr     alignmentEvaluator      = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "alignment", timeEvaluator );
    SimpleFloatEvaluatorPtr     maxTextLenghtEvaluator  = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "maxTextLenght", timeEvaluator );

    SimpleVec2EvaluatorPtr      explosionCenterEvaluator = ParamValEvaluatorFactory::CreateSimpleVec2Evaluator( "explosionCenter", timeEvaluator );

    //Register all parameters and evaloators in models
    vsModel->RegisterAll( trTxEvaluator );
    vsModel->RegisterAll( transformEffectVal1Evaluator );
    vsModel->RegisterAll( transformEffectVal2Evaluator );
	psModel->RegisterAll( outlineColorEvaluator );
    psModel->RegisterAll( alphaEvaluator );

    psModel->RegisterAll( rccBeginColorEvaluator );
    psModel->RegisterAll( rccEndColorEvaluator );
    psModel->RegisterAll( timeValEvaluator );
    psModel->RegisterAll( explosionCenterEvaluator );
    psModel->RegisterAll( colTextEffectIdEvaluator );
    psModel->RegisterAll( transformTextEffectIdEvaluator );

    plModel->RegisterAll( textEvaluator );
    plModel->RegisterAll( blurSizeEvaluator );
	plModel->RegisterAll( outlineSizeEvaluator );
    plModel->RegisterAll( spacingEvaluator );
    plModel->RegisterAll( alignmentEvaluator );
    plModel->RegisterAll( fontSizeEvaluator );
    plModel->RegisterAll( maxTextLenghtEvaluator );

    //Set models structure
    model->SetVertexShaderChannelModel( vsModel );
    model->SetPixelShaderChannelModel( psModel );
    model->SetPluginModel( plModel );

    //Set default values of all parameters
    textEvaluator->Parameter()->SetVal( L"", TimeType( 0.f ) );
    alphaEvaluator->Parameter()->SetVal( 1.f, TimeType( 0.0 ) );
    blurSizeEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.0 ) );
	outlineSizeEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.0 ) );
    spacingEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.0 ) );
    alignmentEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.0 ) );
	outlineColorEvaluator->Parameter()->SetVal( glm::vec4( 0.f, 0.f, 0.f, 0.f ), TimeType( 0.f ) );

    rccBeginColorEvaluator->Parameter()->SetVal( glm::vec4( 1.f, 1.f, 1.f, 1.f ), TimeType( 0.f ) );
    rccEndColorEvaluator->Parameter()->SetVal( glm::vec4( 0.f, 0.f, 0.f, 1.f ), TimeType( 0.f ) );

    rccBeginColorEvaluator->Parameter()->SetVal( glm::vec4( 0.f, 1.f, 0.f, 1.f ), TimeType( 10.f ) );
    rccEndColorEvaluator->Parameter()->SetVal( glm::vec4( 1.f, 0.f, 0.f, 1.f ), TimeType( 10.f ) );

    colTextEffectIdEvaluator->Parameter()->SetVal( 2, TimeType( 0.f ) );
    transformTextEffectIdEvaluator->Parameter()->SetVal( 3, TimeType( 0.f ) );

    explosionCenterEvaluator->Parameter()->SetVal( glm::vec2( 0.0, -0.2 ), TimeType( 0.f ) );

    trTxEvaluator->Parameter()->Transform().InitializeDefaultSRT();
    fontSizeEvaluator->Parameter()->SetVal( 8.f, TimeType( 0.f ) );
    maxTextLenghtEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.f ) );

    transformEffectVal1Evaluator->Parameter()->SetVal( 1.f, TimeType( 0.f ) );

    transformEffectVal1Evaluator->Parameter()->SetVal( 0.1f, TimeType( 10.f ) );

    transformEffectVal2Evaluator->Parameter()->SetVal( 2.f, TimeType( 0.f ) );

    transformEffectVal2Evaluator->Parameter()->SetVal( 5.f, TimeType( 10.f ) );

    return model;
}

// *******************************
//
bool                   DefaultText3DPluginDesc::CanBeAttachedTo   ( IPluginConstPtr plugin ) const
{
    if ( plugin == nullptr )
    {
        return true;
    }

    auto  vac = plugin->GetVertexAttributesChannel();
    if ( vac != nullptr )
    {
        return false;
    }

    return true;
}

// *******************************
//
std::string             DefaultText3DPluginDesc::UID                      ()
{
    return "DEFAULT_TEXT";
}

// *************************************
//
std::wstring            DefaultText3DPlugin::GetText                      () const
{
    return m_textParam->Evaluate();
}


// *******************************
//
std::string             DefaultText3DPluginDesc::TextureName              ()
{
    return "AtlasTex0";
}

// *************************************
// 
void DefaultText3DPlugin::SetPrevPlugin( IPluginPtr prev )
{
    BasePlugin::SetPrevPlugin( prev );

	m_scaleValue =  ValuesFactory::CreateValueMat4( "" );
	m_scaleValue->SetValue( glm::mat4( 1.0 ) );
    ValueMat4PtrVec values;
	values.push_back( m_scaleValue );
	m_transformChannel = DefaultTransformChannelPtr( DefaultTransformChannel::Create( m_prevPlugin, values, false ) ); //<3

	HelperPixelShaderChannel::CloneRenderContext( m_psc, prev );
	auto ctx = m_psc->GetRendererContext();
	ctx->cullCtx->enabled = false;
	ctx->alphaCtx->blendEnabled = true;
	ctx->alphaCtx->srcBlendMode = model::AlphaContext::SrcBlendMode::SBM_ONE;
	ctx->alphaCtx->dstBlendMode = model::AlphaContext::DstBlendMode::DBM_ONE_MINUS_SRC_ALPHA;
}

// *************************************
// 
DefaultText3DPlugin::DefaultText3DPlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin< IPlugin >( name, uid, prev, model )
    , m_psc( nullptr )
    , m_vsc( nullptr )
    , m_vaChannel( nullptr )
{
    m_psc = DefaultPixelShaderChannelPtr( DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel(), nullptr ) );
    m_vsc = DefaultVertexShaderChannelPtr( DefaultVertexShaderChannel::Create( model->GetVertexShaderChannelModel() ) );
	//m_vaChannel = TextHelper::CreateEmptyVACForText();
    
	SetPrevPlugin( prev );

	//FIXME: 'reserve' required texture
	m_psc->GetTexturesDataImpl()->SetTexture( 0, DefaultTextureDescriptor::CreateEmptyTexture2DDesc( DefaultText3DPluginDesc::TextureName(), m_pluginParamValModel->GetTimeEvaluator() ) );

    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &DefaultText3DPlugin::OnSetText ), KeyPressedEvent::Type() );

    m_spacingParam          = QueryTypedParam< ParamFloatPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( "spacing" ) );
    m_alignmentParam        = QueryTypedParam< ParamFloatPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( "alignment" ) );
    m_maxTextLengthParam    = QueryTypedParam< ParamFloatPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( "maxTextLenght" ) );
    m_textParam             = QueryTypedParam< ParamWStringPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( "text" ) );
    
    m_timeParam             = QueryTypedParam< ParamFloatPtr >( GetParameter( "time" ) );
}

// *************************************
// 
DefaultText3DPlugin::~DefaultText3DPlugin         ()
{
}

// *************************************
// 
bool                            DefaultText3DPlugin::LoadResource  ( AssetDescConstPtr assetDescr )
{
    assert( false );  // TODO: Implement
    return false;
}

// *************************************
// 
IVertexAttributesChannelConstPtr    DefaultText3DPlugin::GetVertexAttributesChannel  () const
{
    return m_vaChannel;
}

// *************************************
// 
IPixelShaderChannelConstPtr         DefaultText3DPlugin::GetPixelShaderChannel       () const
{
    return m_psc;
}

// *************************************
// 
IVertexShaderChannelConstPtr        DefaultText3DPlugin::GetVertexShaderChannel      () const
{
    return m_vsc;
}


// *************************************
// 
mathematics::RectConstPtr			DefaultText3DPlugin::GetAABB						( const glm::mat4 & trans ) const
{
	auto rect = mathematics::Rect::Create();
	if( AABB( m_vaChannel.get(), trans, rect.get() ) )
		return rect;
	else
		return nullptr;
}

// *************************************
// 
void                                DefaultText3DPlugin::Update                      ( TimeType t )
{
    { t; } // FIXME: suppress unused warning
    assert( false );  // TODO: Implement
}

namespace {
// *************************************
// FIXME: implement int parameters and bool parameters
template< typename EnumClassType >
inline EnumClassType EvaluateAsInt( ParamFloatPtr param )
{
    int val = int( param->Evaluate() );

    return EnumClassType( val );
}

} //anonymous

// *************************************
//
void DefaultText3DPlugin::OnSetText                   ( IEventPtr evt )
{
    if( evt->GetEventType() == KeyPressedEvent::m_sEventType)
    {
        KeyPressedEventPtr evtTyped = std::static_pointer_cast<KeyPressedEvent>( evt );
        wchar_t c[2] = {evtTyped->GetChar() , '\0'};

        if( c[0] == L'\b' )
        {
            if( !m_textParam->Evaluate().empty() )
            {
                auto text = m_textParam->Evaluate();
                text.pop_back();
                SetText( text );
            }
        }
        else
		{
			SetText( m_textParam->Evaluate() + std::wstring( c ) );
		}
    }
}

namespace 
{

// *************************************
//
glm::mat4 BuildScaleMatrix( const glm::vec3 & center, const glm::vec3 & scale )
{
	{ center; }
    return  glm::translate( glm::mat4( 1.f ), center ) *
            glm::scale( glm::mat4( 1.f ), scale ) *
            glm::translate( glm::mat4( 1.f ), -center );
}

// *************************************
//
void TransformPosChannel( VertexAttributesChannelPtr vaChannel, const glm::mat4 & trans )
{
    auto components = vaChannel->GetComponents();

    for( auto cc : components )
    {
        auto vertsNum = cc->GetNumVertices();

        auto arttChannels = cc->GetAttributeChannels();

        if( vertsNum > 0 && arttChannels.size() > 0 )
        {
            auto attrChannelDesc = arttChannels[ 0 ]->GetDescriptor();
            { attrChannelDesc; } // FIXME: suppress unused warning
            assert( attrChannelDesc->GetType() == AttributeType::AT_FLOAT3 );
            assert( attrChannelDesc->GetSemantic() == AttributeSemantic::AS_POSITION );

            auto f3AttrChannel = std::static_pointer_cast< Float3AttributeChannel >( arttChannels[ 0 ] );
            auto & verts = f3AttrChannel->GetVertices();

            for( auto& v : verts )
            {
                glm::vec4 tmp( v.x, v.y, v.z, 1.f ); 
                tmp = trans * tmp;
                v.x = tmp.x;
                v.y = tmp.y;
                v.z = tmp.z;
            }
        }

        
    }
}

} // anonymous

// *************************************
//
void DefaultText3DPlugin::ScaleToMaxTextLength		()
{
    assert( false );  // TODO: Implement
}

// *************************************
//
void DefaultText3DPlugin::SetText                     ( const std::wstring & newText )
{
    assert( false );  // TODO: Implement

    m_textParam->SetVal( newText, TimeType( 0.f ) );

    m_vaChannel->ClearAll();

	HelperVertexAttributesChannel::SetTopologyUpdate( m_vaChannel );
}

// *************************************
//
bool DefaultText3DPlugin::SetText( IPluginPtr textPlugin, const std::wstring& text )
{
    if( textPlugin->GetTypeUid() == DefaultText3DPluginDesc::UID() )
    {
        std::static_pointer_cast< DefaultText3DPlugin >( textPlugin )->SetText( text );
        return true;
    }
    else
        return false;
}

} // model
} // bv
