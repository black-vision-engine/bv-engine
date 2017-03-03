#include "stdafx.h"

#include "DefaultTextPlugin.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Types/Values/ValuesFactory.h"

#include "Engine/Models/Plugins/Channels/Geometry/HelperVertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/DefaultFontDescriptor.h"

#include "Engine/Models/Plugins/Channels/Geometry/VacAABB.h"
#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"

#include "Engine/Events/Events.h"
#include "Engine/Events/Interfaces/IEventManager.h"

#include "Assets/Font/2D/FontAssetDescriptor.h"
#include "Assets/Font/FontLoader.h"
#include "Assets/Font/Text.h"

#include "Assets/DefaultAssets.h"

#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {


extern TextArranger CircleArranger;

const std::string   DefaultTextPlugin::PARAM::TEXT              = "text";
const std::string   DefaultTextPlugin::PARAM::MAX_TEXT_LENGTH   = "maxTextLength";
const std::string   DefaultTextPlugin::PARAM::TX_MAT            = "txMat";
const std::string   DefaultTextPlugin::PARAM::RCC_BEGIN_COLOR   = "rcc_beginColor";
const std::string   DefaultTextPlugin::PARAM::RCC_END_COLOR     = "rcc_endColor";
const std::string   DefaultTextPlugin::PARAM::COL_TEXT_EFFECT   = "colTextEffectId";
const std::string   DefaultTextPlugin::PARAM::TX_TEXT_EFFECT    = "transformTextEffectId";
const std::string   DefaultTextPlugin::PARAM::TIME              = "time";
const std::string   DefaultTextPlugin::PARAM::TX_EFFECT_VAL1    = "transformEffectVal1";
const std::string   DefaultTextPlugin::PARAM::TX_EFFECT_VAL2    = "transformEffectVal2";
const std::string   DefaultTextPlugin::PARAM::EXPLOSION_CENTER  = "explosionCenter";
const std::string   DefaultTextPlugin::PARAM::ANIM_SCALE_OFFSET = "animScaleOffset";
const std::string   DefaultTextPlugin::PARAM::ANIM_SCALE        = "animScale";
const std::string   DefaultTextPlugin::PARAM::ANIM_ALPHA_OFFSET = "animAlphaOffset";
const std::string   DefaultTextPlugin::PARAM::ANIM_ALPHA        = "animAlpha";

// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultTextPluginDesc::DefaultTextPluginDesc                            ()
    : TextPluginBaseDesc( UID(), "text", "txt" )
{}

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
    auto model = TextPluginBaseDesc::CreateDefaultModel( timeEvaluator );

    ModelHelper h( timeEvaluator, model );
    h.SetOrCreatePluginModel();

    h.AddSimpleStatedParam( DefaultTextPlugin::PARAM::TEXT, std::wstring( L"" ) );
    h.AddSimpleStatedParam( DefaultTextPlugin::PARAM::MAX_TEXT_LENGTH, 0.0f );

    h.SetOrCreateVSModel();

    h.AddTransformParam( DefaultTextPlugin::PARAM::TX_MAT );

    h.AddSimpleParam( DefaultTextPlugin::PARAM::TX_TEXT_EFFECT, 0, true );
    h.AddSimpleParam( DefaultTextPlugin::PARAM::TX_EFFECT_VAL1, 0.f, true );
    h.AddSimpleParam( DefaultTextPlugin::PARAM::TX_EFFECT_VAL2, 0.f, true );
    h.AddSimpleParam( DefaultTextPlugin::PARAM::ANIM_SCALE_OFFSET, 0.f, true );
    h.AddSimpleParam( DefaultTextPlugin::PARAM::ANIM_SCALE, 0.f, true );
    h.AddSimpleParam( DefaultTextPlugin::PARAM::ANIM_ALPHA_OFFSET, 0.f, true );
    h.AddSimpleParam( DefaultTextPlugin::PARAM::ANIM_ALPHA, 0.f, true );

    h.SetOrCreatePSModel();

    h.AddSimpleParam( DefaultTextPlugin::PARAM::RCC_BEGIN_COLOR, glm::vec4( 0.f, 0.f, 0.f, 0.f ), true );
    h.AddSimpleParam( DefaultTextPlugin::PARAM::RCC_END_COLOR, glm::vec4( 0.f, 0.f, 0.f, 0.f ), true );
    h.AddSimpleParam( DefaultTextPlugin::PARAM::COL_TEXT_EFFECT, 0, true );
    h.AddSimpleParam( DefaultTextPlugin::PARAM::TIME, 0.f, true );
    h.AddSimpleParam( DefaultTextPlugin::PARAM::EXPLOSION_CENTER, glm::vec2( 0.0, -0.2 ), true );

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
    return m_text.GetParameter().Evaluate();
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
    //m_arranger = &CircleArranger; // INFO: Needed for testing arrangers only.
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &DefaultTextPlugin::OnSetText ), KeyPressedEvent::Type() );

	m_text			= GetValueParamState< std::wstring >( GetPluginParamValModel()->GetPluginModel().get(), PARAM::TEXT );
	m_maxTextLength	= GetValueParamState< Float32 >( GetPluginParamValModel()->GetPluginModel().get(), PARAM::MAX_TEXT_LENGTH );

    m_scaleValue =  ValuesFactory::CreateValueMat4( "" );
    m_scaleValue->SetValue( glm::mat4( 1.0 ) );
    
    m_timeParam             = QueryTypedParam< ParamFloatPtr >( GetParameter( PARAM::TIME ) );

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

    SetText( m_text.GetParameter().Evaluate() );

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
bool								DefaultTextPlugin::NeedsSetText					() const
{
	return  m_text.Changed()
		|| m_alignment.Changed()
		|| m_spacing.Changed()
		|| m_alignCharacter.Changed()
		|| m_newLineSize.Changed()
		|| m_shadowEnabled.Changed()
		|| m_outlineEnabled.Changed()
		|| m_glowEnabled.Changed()
		|| m_textBox.Changed()
		|| m_useTextBox.Changed();
}

// *************************************
// 
void                                DefaultTextPlugin::Update                      ( TimeType t )
{
    m_timeParam->SetVal( t, TimeType( 0.0 ) );
    TextPluginBase::Update( t );

    if( NeedsSetText() )
    {
        SetText( m_text.GetParameter().Evaluate() );
    }

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

		auto text = m_text.GetParameter().Evaluate();

        if( c[0] == L'\b' )
        {
            if( !text.empty() )
            {
                text.pop_back();
                SetText( text );
            }
        }
        else
        {
            SetText( text + std::wstring( c ) );
        }
    }
}

// *************************************
//
void DefaultTextPlugin::ScaleToMaxTextLength        ()
{
    m_scaleMat = glm::mat4( 1.0 ); // reset current scale.

    auto maxTextLenght = m_maxTextLength.GetParameter().Evaluate();

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
    m_vaChannel->ClearAll();

	bool useBox = m_useTextBox.GetParameter().Evaluate();

    m_textLength = TextPluginBase::BuildVACForText( newText, true, useBox );

    ScaleToMaxTextLength();

    HelperVertexAttributesChannel::SetTopologyUpdate( m_vaChannel );
}


} // model
} // bv

#include "DefaultTextPluginTests.inl"