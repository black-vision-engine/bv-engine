#include "stdafx.h"

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

#include "Assets/DefaultAssets.h"
#include "Application/ApplicationContext.h"

#include "Assets/Font/TextHelper.h"
#include "Text3DUtils.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {


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
    h.CreateVacModel();

    h.AddSimpleParam( "text", std::wstring( L"" ), true, true );
    h.AddSimpleParam( "spacing", 0.0f, true, true );
    h.AddSimpleParam( "alignment", 0.0f, true, true );
    h.AddSimpleParam( "maxTextLenght", 0.0f, true, true );
    h.AddSimpleParam( "fontSize", 8.0f, true, true );

    h.CreatePSModel();
    h.CreateVSModel();

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

	m_scaleValue =  ValuesFactory::CreateValueMat4( "" );
	m_scaleValue->SetValue( glm::mat4( 1.0 ) );
	m_transformChannel = DefaultTransformChannelPtr( DefaultTransformChannel::Create( m_prevPlugin, m_scaleValue, false ) ); //<3

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
    : BasePlugin< IPlugin >( name, uid, prev, model )
    , m_psc( nullptr )
    , m_vsc( nullptr )
    , m_vaChannel( nullptr )
{
    m_psc = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel() );
    m_vsc = DefaultVertexShaderChannel::Create( model->GetVertexShaderChannelModel() );
    m_vaChannel = Text3DUtils::CreateEmptyVACForText3D();
    
	SetPrevPlugin( prev );

    m_fontSize              = QueryTypedValue< ValueFloatPtr >( GetPluginParamValModel()->GetVertexAttributesChannelModel()->GetValue( "fontSize" ) );
    m_spacingParam          = QueryTypedParam< ParamFloatPtr >( GetPluginParamValModel()->GetVertexAttributesChannelModel()->GetParameter( "spacing" ) );
    m_alignmentParam        = QueryTypedParam< ParamFloatPtr >( GetPluginParamValModel()->GetVertexAttributesChannelModel()->GetParameter( "alignment" ) );
    m_maxTextLengthParam    = QueryTypedParam< ParamFloatPtr >( GetPluginParamValModel()->GetVertexAttributesChannelModel()->GetParameter( "maxTextLenght" ) );
    m_textParam             = QueryTypedParam< ParamWStringPtr >( GetPluginParamValModel()->GetVertexAttributesChannelModel()->GetParameter( "text" ) );

    LoadResource( DefaultAssets::Instance().GetDefaultDesc< FontAssetDesc >() );
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
    auto fontAssetDesc = QueryTypedDesc< FontAssetDescConstPtr >( assetDescr );

    if ( fontAssetDesc != nullptr )
    {
        auto fontResource = LoadTypedAsset<FontAsset>( fontAssetDesc );
        if( fontResource == nullptr )
        {
            return false;
        }

        m_text = fontResource->GetText();

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
    layout.BlurSize = 0;
    layout.OutlineSize = 0;
    layout.Spacing = m_spacingParam->Evaluate();
    layout.Tat = TextAlignmentType::Center;
    layout.TextAsset = m_text;
    layout.UseKerning = false;
    layout.ViewWidth = ApplicationContext::Instance().GetWidth();
    layout.ViewHeight = ApplicationContext::Instance().GetHeight();

    auto connectedComponents = Text3DUtils::CreateText( m_textParam->Evaluate(), m_text, layout );

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

    if( ParameterChanged( "text" ) || 
        ParameterChanged( "alignment" ) ||
        ParameterChanged( "spacing" ) ||
        ParameterChanged( "fontSize" ) )
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
