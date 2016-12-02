#include "stdafx.h"

#include "DefaultTextPlugin.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Types/Values/ValuesFactory.h"

#include "Engine/Models/Plugins/Channels/Geometry/HelperVertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/DefaultFontDescriptor.h"

#include "Engine/Models/Plugins/Channels/Geometry/VacAABB.h"

#include "Assets/Font/2D/FontAssetDescriptor.h"
#include "Assets/Font/FontLoader.h"
#include "Assets/Font/Text.h"

#include "Application/ApplicationContext.h"

#include "Engine/Events/Events.h"
#include "Engine/Events/Interfaces/IEventManager.h"

#include "Assets/DefaultAssets.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {


extern TextArranger CircleArranger;


const std::string   DefaultTextPlugin::PARAM::TEXT              = "text";
const std::string   DefaultTextPlugin::PARAM::ALIGN_CHARACTER   = "alignCharacter";


// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultTextPluginDesc::DefaultTextPluginDesc                            ()
    : BasePluginDescriptor( UID(), "text", "txt" )
{
}

// *******************************
//
IPluginPtr              DefaultTextPluginDesc::CreatePlugin             ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultTextPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultTextPluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    // Rewrite to ModelHelper.
    ModelHelper h( timeEvaluator );
    h.SetOrCreatePluginModel();

    h.AddSimpleStatedParam( "spacing", 0.0f );
    h.AddSimpleStatedParam( "alignment", 0.0f );
    h.AddSimpleStatedParam( "maxTextLength", 0.0f );
    h.AddSimpleStatedParam( DefaultTextPlugin::PARAM::ALIGN_CHARACTER, (int)L'.' );
    h.AddSimpleStatedParam( DefaultTextPlugin::PARAM::TEXT, std::wstring( L"" ) );


    //Create all models
    DefaultPluginParamValModelPtr model  = h.GetModel();
    DefaultParamValModelPtr psModel      = std::make_shared< DefaultParamValModel >();
    DefaultParamValModelPtr vsModel      = std::make_shared< DefaultParamValModel >();


    //Create all parameters and evaluators
    SimpleFloatEvaluatorPtr     alphaEvaluator          = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "alpha", timeEvaluator );
    SimpleTransformEvaluatorPtr trTxEvaluator           = ParamValEvaluatorFactory::CreateSimpleTransformEvaluator( "txMat", timeEvaluator );
    //SimpleFloatEvaluatorPtr     fontSizeEvaluator       = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "fontSize", timeEvaluator );

    //SimpleFloatEvaluatorPtr     blurSizeEvaluator       = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "blurSize", timeEvaluator );
	//SimpleFloatEvaluatorPtr     outlineSizeEvaluator    = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "outlineSize", timeEvaluator );
	SimpleVec4EvaluatorPtr      outlineColorEvaluator   = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( "outlineColor", timeEvaluator );

    SimpleVec4EvaluatorPtr      rccBeginColorEvaluator  = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( "rcc_beginColor", timeEvaluator );
    SimpleVec4EvaluatorPtr      rccEndColorEvaluator    = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( "rcc_endColor", timeEvaluator );
    SimpleIntEvaluatorPtr       colTextEffectIdEvaluator= ParamValEvaluatorFactory::CreateSimpleIntEvaluator( "colTextEffectId", timeEvaluator );
    SimpleIntEvaluatorPtr       transformTextEffectIdEvaluator= ParamValEvaluatorFactory::CreateSimpleIntEvaluator( "transformTextEffectId", timeEvaluator );

    SimpleFloatEvaluatorPtr     timeValEvaluator       = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "time", timeEvaluator );
    SimpleFloatEvaluatorPtr     transformEffectVal1Evaluator       = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "transformEffectVal1", timeEvaluator );
    SimpleFloatEvaluatorPtr     transformEffectVal2Evaluator       = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "transformEffectVal2", timeEvaluator );

    SimpleVec2EvaluatorPtr      explosionCenterEvaluator = ParamValEvaluatorFactory::CreateSimpleVec2Evaluator( "explosionCenter", timeEvaluator );


    SimpleFloatEvaluatorPtr      animScaleOffsetEvaluator = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "animScaleOffset", timeEvaluator );
    SimpleFloatEvaluatorPtr      animScaleEvaluator     = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "animScale", timeEvaluator );

    SimpleFloatEvaluatorPtr      animAlphaOffsetEvaluator = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "animAlphaOffset", timeEvaluator );
    SimpleFloatEvaluatorPtr      animAlphaEvaluator     = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "animAlpha", timeEvaluator );

    //Register all parameters and evaloators in models
    vsModel->RegisterAll( trTxEvaluator );

    vsModel->RegisterAll( transformEffectVal1Evaluator );
    vsModel->RegisterAll( transformEffectVal2Evaluator );
    vsModel->RegisterAll( animScaleOffsetEvaluator );
    vsModel->RegisterAll( animScaleEvaluator );

    psModel->RegisterAll( outlineColorEvaluator );
    psModel->RegisterAll( alphaEvaluator );
    vsModel->RegisterAll( animAlphaOffsetEvaluator );
    vsModel->RegisterAll( animAlphaEvaluator );

    psModel->RegisterAll( rccBeginColorEvaluator );
    psModel->RegisterAll( rccEndColorEvaluator );
    psModel->RegisterAll( timeValEvaluator );
    psModel->RegisterAll( explosionCenterEvaluator );
    psModel->RegisterAll( colTextEffectIdEvaluator );
    psModel->RegisterAll( transformTextEffectIdEvaluator );


    //Set models structure
    model->SetVertexShaderChannelModel( vsModel );
    model->SetPixelShaderChannelModel( psModel );

    //Set default values of all parameters
    alphaEvaluator->Parameter()->SetVal( 1.f, TimeType( 0.0 ) );
    //blurSizeEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.0 ) );
	//outlineSizeEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.0 ) );
	outlineColorEvaluator->Parameter()->SetVal( glm::vec4( 0.f, 0.f, 0.f, 0.f ), TimeType( 0.f ) );

    rccBeginColorEvaluator->Parameter()->SetVal( glm::vec4( 1.f, 1.f, 1.f, 1.f ), TimeType( 0.f ) );
    rccEndColorEvaluator->Parameter()->SetVal( glm::vec4( 0.f, 0.f, 0.f, 1.f ), TimeType( 0.f ) );

    rccBeginColorEvaluator->Parameter()->SetVal( glm::vec4( 0.f, 1.f, 0.f, 1.f ), TimeType( 10.f ) );
    rccEndColorEvaluator->Parameter()->SetVal( glm::vec4( 1.f, 0.f, 0.f, 1.f ), TimeType( 10.f ) );

    colTextEffectIdEvaluator->Parameter()->SetVal( 0, TimeType( 0.f ) );
    transformTextEffectIdEvaluator->Parameter()->SetVal( 0, TimeType( 0.f ) );

    explosionCenterEvaluator->Parameter()->SetVal( glm::vec2( 0.0, -0.2 ), TimeType( 0.f ) );

    trTxEvaluator->Parameter()->Transform().InitializeDefaultSRT();
    //fontSizeEvaluator->Parameter()->SetVal( 8.f, TimeType( 0.f ) );

    transformEffectVal1Evaluator->Parameter()->SetVal( 1.f, TimeType( 0.f ) );

    transformEffectVal1Evaluator->Parameter()->SetVal( 0.1f, TimeType( 10.f ) );

    transformEffectVal2Evaluator->Parameter()->SetVal( 2.f, TimeType( 0.f ) );

    transformEffectVal2Evaluator->Parameter()->SetVal( 5.f, TimeType( 10.f ) );

    animScaleOffsetEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.f ) );
    animScaleOffsetEvaluator->Parameter()->SetVal( 1.f, TimeType( 15.f ) );

    animScaleOffsetEvaluator->Parameter()->AccessInterpolator().SetWrapPostMethod( WrapMethod::pingPong );

    animScaleEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.f ) );

    animAlphaOffsetEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.f ) );
    animAlphaOffsetEvaluator->Parameter()->SetVal( 1.f, TimeType( 5.f ) );
    animAlphaOffsetEvaluator->Parameter()->AccessInterpolator().SetWrapPostMethod( WrapMethod::pingPong );
    animAlphaEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.f ) );

    return model;
}

// *******************************
//
std::string             DefaultTextPluginDesc::UID                      ()
{
    return "DEFAULT_TEXT";
}

//
std::wstring            DefaultTextPlugin::GetText                      () const
{
    return m_textParam->Evaluate();
}

// *******************************
//
std::string             DefaultTextPluginDesc::TextureName              ()
{
    return "AtlasTex0";
}

// *************************************
// 
DefaultTextPlugin::DefaultTextPlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : TextPluginBase( name, uid, prev, model )
    , m_textLength( 0.f )
    , m_arranger( nullptr )
{
    //m_arranger = &CircleArranger;
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &DefaultTextPlugin::OnSetText ), KeyPressedEvent::Type() );

    m_spacingParam          = QueryTypedParam< ParamFloatPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( "spacing" ) );
    m_alignmentParam        = QueryTypedParam< ParamFloatPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( "alignment" ) );
    m_maxTextLengthParam    = QueryTypedParam< ParamFloatPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( "maxTextLength" ) );
    m_textParam             = QueryTypedParam< ParamWStringPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( PARAM::TEXT ) );
    m_alignCharacter        = QueryTypedValue< ValueIntPtr >( GetPluginParamValModel()->GetPluginModel()->GetValue( PARAM::ALIGN_CHARACTER ) );

    m_scaleValue =  ValuesFactory::CreateValueMat4( "" );
    m_scaleValue->SetValue( glm::mat4( 1.0 ) );
    
    m_timeParam             = QueryTypedParam< ParamFloatPtr >( GetParameter( "time" ) );

    LoadResource( DefaultAssets::Instance().GetDefaultDesc< FontAssetDesc >() );
}

// *************************************
// 
DefaultTextPlugin::~DefaultTextPlugin         ()
{
}


// *************************************
// 
bool                            DefaultTextPlugin::LoadResource  ( AssetDescConstPtr assetDescr )
{
    auto success = TextPluginBase::LoadResource( assetDescr,  DefaultTextPluginDesc::TextureName() );

    SetText( m_textParam->Evaluate() );

    return success;
}

// *************************************
// 
mathematics::RectConstPtr       DefaultTextPlugin::GetAABB      ( const glm::mat4 & trans ) const
{
    auto rect = mathematics::Rect::Create();
    if( AABB( m_vaChannel.get(), trans, rect.get() ) )
    {
        return rect;
    }
    else
    {
        return nullptr;
    }

}

// *************************************
// 
void                                DefaultTextPlugin::Update                      ( TimeType t )
{
    m_timeParam->SetVal( t, TimeType( 0.0 ) );
    BasePlugin::Update( t );

    if( ParameterChanged( PARAM::TEXT ) ||
        ParameterChanged( "alignment" ) ||
        ParameterChanged( "spacing" ) ||
        ParameterChanged( PARAM::ALIGN_CHARACTER ) )
    {
        SetText( m_textParam->Evaluate() );
    }

    m_scaleMat = glm::mat4( 1.0 );

    ScaleToMaxTextLength();

    m_scaleValue->SetValue( m_scaleMat );

    //assumption that text plugin provides vertices, so no need for backward topology propagation
    HelperVertexAttributesChannel::PropagateAttributesUpdate( m_vaChannel, m_prevPlugin );
    HelperPixelShaderChannel::PropagateUpdate( m_psc, m_prevPlugin );

    m_vsc->PostUpdate();
    m_psc->PostUpdate();
}

// *************************************
//
void DefaultTextPlugin::OnSetText                   ( IEventPtr evt )
{
    if( evt->GetEventType() == KeyPressedEvent::Type())
    {
        KeyPressedEventPtr evtTyped = std::static_pointer_cast<KeyPressedEvent>( evt );
        wchar_t c[2] = { evtTyped->GetChar() , '\0' };

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

// *************************************
//
void DefaultTextPlugin::ScaleToMaxTextLength		()
{
    auto maxTextLenght = m_maxTextLengthParam->Evaluate();

    if( maxTextLenght > 0.f && m_textLength > 0.f && m_textLength > maxTextLenght )
    {
        m_scaleMat = glm::scale( glm::mat4( 1.f ), glm::vec3( maxTextLenght / m_textLength, 1.f, 1.f ) );
        m_scaleValue->SetValue( m_scaleMat );
    }
}

// *************************************
//
void DefaultTextPlugin::SetText                     ( const std::wstring & newText )
{
    m_currentText = newText;
    m_currentAligment = m_alignmentParam->Evaluate();
    m_currentSpacing = m_spacingParam->Evaluate();

    m_vaChannel->ClearAll();

    auto alignType		=  TextAlignmentType( (int)m_currentAligment );

    auto viewWidth  = ApplicationContext::Instance().GetWidth();
    auto viewHeight = ApplicationContext::Instance().GetHeight();
    m_textLength = TextHelper::BuildVACForText( m_vaChannel.get(), m_atlas, m_currentText, m_blurSize, m_currentSpacing, alignType, (wchar_t)m_alignCharacter->GetValue(), m_outlineSize, viewWidth, viewHeight, m_arranger, true );

    ScaleToMaxTextLength();

    HelperVertexAttributesChannel::SetTopologyUpdate( m_vaChannel );
}


} // model
} // bv
