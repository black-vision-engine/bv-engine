#include "stdafx.h"

#include "DefaultText3DPlugin.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Types/Values/ValuesFactory.h"

#include "Engine/Models/Plugins/Channels/Geometry/HelperVertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"

#include "Engine/Models/Plugins/Channels/Geometry/VacAABB.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Events/Events.h"
#include "Engine/Events/Interfaces/IEventManager.h"

#include "Assets/DefaultAssets.h"
#include "Application/ApplicationContext.h"

#include "Assets/Font/TextHelper.h"
#include "Text3DUtils.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {


typedef ParamEnum< TextAlignmentType > ParamEnumTAT;

// ***********************
//
VoidPtr    ParamEnumTAT::QueryParamTyped  ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}

// ***********************
//
template<>
static IParameterPtr        ParametersFactory::CreateTypedParameter< ParamEnumTAT >                 ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return CreateParameterEnum< ParamEnumTAT >( name, timeline );
}




const std::string        DefaultText3DPlugin::PARAMS::TEXT              = "text";
const std::string        DefaultText3DPlugin::PARAMS::FONT_SIZE         = "fontSize";
const std::string        DefaultText3DPlugin::PARAMS::NEW_LINE_SIZE     = "newLineSize";
const std::string        DefaultText3DPlugin::PARAMS::SPACING           = "spacing";
const std::string        DefaultText3DPlugin::PARAMS::ALIGNEMENT        = "alignment";
const std::string        DefaultText3DPlugin::PARAMS::USE_KERNING       = "useKerning";
const std::string        DefaultText3DPlugin::PARAMS::ALIGN_CHARACTER   = "alignCharacter";
const std::string        DefaultText3DPlugin::PARAMS::TEXT_BOX          = "textBox";
const std::string        DefaultText3DPlugin::PARAMS::USE_TEXT_BOX      = "useTextBox";


// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultText3DPluginDesc::DefaultText3DPluginDesc                            ()
    : BasePluginDescriptor( UID(), "text3d", "" )
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
    ModelHelper h( timeEvaluator );
    h.SetOrCreateVacModel();

    h.AddSimpleStatedParam( DefaultText3DPlugin::PARAMS::TEXT, std::wstring( L"" ) );
    h.AddSimpleStatedParam( DefaultText3DPlugin::PARAMS::SPACING, 0.0f );
    h.AddSimpleStatedParam( DefaultText3DPlugin::PARAMS::FONT_SIZE, 8.0f );
    h.AddSimpleStatedParam( DefaultText3DPlugin::PARAMS::NEW_LINE_SIZE, 1.0f );
    h.AddSimpleStatedParam( DefaultText3DPlugin::PARAMS::USE_KERNING, true );
    h.AddSimpleStatedParam( DefaultText3DPlugin::PARAMS::ALIGN_CHARACTER, (int)L'.' );
    h.AddEnumParam( DefaultText3DPlugin::PARAMS::ALIGNEMENT, TextAlignmentType::Left, true, true );
    h.AddSimpleStatedParam( DefaultText3DPlugin::PARAMS::TEXT_BOX, glm::vec2( 1.0f, 1.0f ) );
    h.AddSimpleStatedParam( DefaultText3DPlugin::PARAMS::USE_TEXT_BOX, false );

    h.SetOrCreatePSModel();
    h.SetOrCreateVSModel();

    return h.GetModel();
}

// *******************************
//
std::string             DefaultText3DPluginDesc::UID                      ()
{
    return "DEFAULT_TEXT3D";
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

	HelperPixelShaderChannel::CloneRenderContext( m_psc, prev );
	auto ctx = m_psc->GetRendererContext();
	ctx->cullCtx->enabled = false;
	ctx->alphaCtx->blendEnabled = true;
	ctx->alphaCtx->srcRGBBlendMode = model::AlphaContext::SrcBlendMode::SBM_ONE;
	ctx->alphaCtx->dstRGBBlendMode = model::AlphaContext::DstBlendMode::DBM_ONE_MINUS_SRC_ALPHA;
}

// *************************************
// 
DefaultText3DPlugin::DefaultText3DPlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin( name, uid, prev, model )
    , m_psc( nullptr )
    , m_vsc( nullptr )
    , m_vaChannel( nullptr )
{
    m_psc = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel() );
    m_vsc = DefaultVertexShaderChannel::Create( model->GetVertexShaderChannelModel() );
    m_vaChannel = Text3DUtils::CreateEmptyVACForText3D();
    
	SetPrevPlugin( prev );

    auto modelVAC = GetPluginParamValModel()->GetVertexAttributesChannelModel();

    m_fontSize              = QueryTypedValue< ValueFloatPtr >( modelVAC->GetValue( PARAMS::FONT_SIZE ) );
    m_spacingValue          = QueryTypedValue< ValueFloatPtr >( modelVAC->GetValue( PARAMS::SPACING ) );
    m_newLineSize           = QueryTypedValue< ValueFloatPtr >( modelVAC->GetValue( PARAMS::NEW_LINE_SIZE ) );
    m_textParam             = QueryTypedParam< ParamWStringPtr >( modelVAC->GetParameter( PARAMS::TEXT ) );
    m_useKerningValue       = QueryTypedValue< ValueBoolPtr >( modelVAC->GetValue( PARAMS::USE_KERNING ) );
    m_alignmentParam        = QueryTypedParam< ParamEnumTATPtr >( modelVAC->GetParameter( PARAMS::ALIGNEMENT ) );
    m_alignCharacter        = QueryTypedValue< ValueIntPtr >( modelVAC->GetValue( PARAMS::ALIGN_CHARACTER ) );

    LoadResource( DefaultAssets::Instance().GetDefaultDesc< FontAssetDesc >() );
}

// *************************************
// 
DefaultText3DPlugin::~DefaultText3DPlugin         ()
{}

// *************************************
// 
bool                            DefaultText3DPlugin::LoadResource  ( AssetDescConstPtr assetDescr )
{
    auto fontAssetDesc = QueryTypedDesc< FontAssetDescConstPtr >( assetDescr );

    if ( fontAssetDesc != nullptr )
    {
        // Translate font descriptor to descriptor for 3D asset.
        auto font3DDesc = FontAsset3DDesc::Create( fontAssetDesc, fontAssetDesc->GetFontSize() );

        auto fontResource = LoadTypedAsset< FontAsset3D >( font3DDesc );
        if( fontResource == nullptr )
        {
            return false;
        }

        m_fontAsset = fontResource;

        SetAsset( 0, LAsset( "Font3D", assetDescr, nullptr ) );

        RebuildText();

        return true;
    }    
    else
    {
        return false;
    }
}

// ***********************
//
void                                DefaultText3DPlugin::RebuildText                 ()
{
    Text3DUtils::TextLayout layout;
    layout.Arranger = nullptr;
    layout.Size = m_fontSize->GetValue();
    layout.NewLineSize = m_newLineSize->GetValue();
    layout.Spacing = m_spacingValue->GetValue();
    layout.Tat = m_alignmentParam->Evaluate();
    layout.AlignChar = static_cast< wchar_t >( m_alignCharacter->GetValue() );
    layout.FontAsset = m_fontAsset;
    layout.UseKerning = m_useKerningValue->GetValue();
    layout.ViewWidth = ApplicationContext::Instance().GetWidth();
    layout.ViewHeight = ApplicationContext::Instance().GetHeight();
    layout.Box = QueryTypedValue< ValueVec2Ptr >( GetValue( PARAMS::TEXT_BOX ) )->GetValue();
    layout.UseBox = QueryTypedValue< ValueBoolPtr >( GetValue( PARAMS::USE_TEXT_BOX ) )->GetValue();

    auto connectedComponents = Text3DUtils::CreateText( m_textParam->Evaluate(), m_fontAsset, layout );

    m_vaChannel->ClearAll();
    for( auto & component : connectedComponents )
    {
        m_vaChannel->AddConnectedComponent( component );
    }

    HelperVertexAttributesChannel::SetTopologyUpdate( m_vaChannel );
}


// *************************************
// 
void                                DefaultText3DPlugin::Update                      ( TimeType t )
{
    BasePlugin::Update( t );

    if( ParameterChanged( PARAMS::TEXT ) || 
        ParameterChanged( PARAMS::ALIGNEMENT ) ||
        ParameterChanged( PARAMS::SPACING ) ||
        ParameterChanged( PARAMS::FONT_SIZE ) ||
        ParameterChanged( PARAMS::NEW_LINE_SIZE ) ||
        ParameterChanged( PARAMS::USE_KERNING ) ||
        ParameterChanged( PARAMS::ALIGN_CHARACTER ) ||
        ParameterChanged( PARAMS::TEXT_BOX ) ||
        ParameterChanged( PARAMS::USE_TEXT_BOX ) )
    {
        RebuildText();
    }

    //assumption that text plugin provides vertices, so no need for backward topology propagation
    //HelperVertexAttributesChannel::PropagateAttributesUpdate( m_vaChannel, m_prevPlugin );
    HelperPixelShaderChannel::PropagateUpdate( m_psc, m_prevPlugin );

    m_vsc->PostUpdate();
    m_psc->PostUpdate();
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


// *************************************
// 
IVertexAttributesChannelConstPtr    DefaultText3DPlugin::GetVertexAttributesChannel  () const
{
    return m_vaChannel;
}

// *************************************
// 
IPixelShaderChannelPtr              DefaultText3DPlugin::GetPixelShaderChannel       () const
{
    return m_psc;
}

// *************************************
// 
IVertexShaderChannelConstPtr        DefaultText3DPlugin::GetVertexShaderChannel      () const
{
    return m_vsc;
}


} // model
} // bv
