#include "stdafx.h"

#include "DefaultTexturePlugin.h"

#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine\Interfaces\IValue.h"

#include "Engine/Models/Plugins/Channels/Geometry/VacAABB.h"

#include "Engine/Models/Plugins/Channels/Geometry/HelperVertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/HelperVertexShaderChannel.h"
#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"

#include "Engine/Models/Plugins/HelperUVGenerator.h"

#include "Assets/DefaultAssets.h"


namespace bv { namespace model {


const std::string        DefaultTexturePlugin::PARAM::ALPHA         = "alpha";
const std::string        DefaultTexturePlugin::PARAM::TX_MAT        = "txMat";


// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultTexturePluginDesc::DefaultTexturePluginDesc                          ()
    : BasePluginDescriptor( UID(), "texture", "tx" )
{}

// *******************************
//
DefaultTexturePluginDesc::DefaultTexturePluginDesc( const std::string & uid, const std::string & defaultName, const std::string & abbrv )
	: BasePluginDescriptor( uid, defaultName, abbrv )
{}

// *******************************
//
IPluginPtr              DefaultTexturePluginDesc::CreatePlugin              ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultTexturePlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultTexturePluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper helper( timeEvaluator );

    //Create all models
    auto model  = helper.GetModel();
    DefaultParamValModelPtr vsModel      = std::make_shared< DefaultParamValModel >();

    helper.SetOrCreatePluginModel();
    helper.AddSimpleParam( BlendHelper::PARAM::BLEND_ENABLE, true, true, true );
	helper.AddEnumParam( BlendHelper::PARAM::BLEND_MODE, BlendHelper::BlendMode::BM_Alpha, true, true );

    helper.SetOrCreatePSModel();
    helper.AddSimpleParam( DefaultTexturePlugin::PARAM::ALPHA, 1.f, true );

    helper.SetOrCreateVSModel();
    helper.AddTransformParam( DefaultTexturePlugin::PARAM::TX_MAT, true );

    auto param = helper.GetModel()->GetVertexShaderChannelModel()->GetParameter( DefaultTexturePlugin::PARAM::TX_MAT );
    SetParameterCenterMass( param, 0.0f, glm::vec3( 0.5, 0.5, 0.0 ) );

    return model;
}

// *******************************
//
std::string             DefaultTexturePluginDesc::UID                       ()
{
    return "DEFAULT_TEXTURE";
}

// *******************************
// 
std::string             DefaultTexturePluginDesc::TextureName               ()
{
    return "Tex0";
}



// ************************************************************************* PLUGIN *************************************************************************

// *************************************
// 
bool DefaultTexturePlugin::SetPrevPlugin( IPluginPtr prev )
{
    if( BasePlugin::SetPrevPlugin( prev ) )
    {
        InitVertexAttributesChannel();

        HelperPixelShaderChannel::CloneRenderContext( m_psc, prev );
        auto ctx = m_psc->GetRendererContext();
        ctx->cullCtx->enabled = false;

        ctx->alphaCtx->blendEnabled = m_blendEnabled.GetParameter().Evaluate();
	    BlendHelper::SetBlendRendererContext( m_psc, m_blendMode.GetParameter() );
        //HelperPixelShaderChannel::SetRendererContextUpdate( m_psc );

        return true;
    }
    else
        return false;
}

// *************************************
// 
DefaultTexturePlugin::DefaultTexturePlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin( name, uid, prev, model )
    , m_psc( nullptr )
    , m_vsc( nullptr )
    , m_vaChannel( nullptr )
{
    m_psc = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel() );
    m_vsc = DefaultVertexShaderChannel::Create( model->GetVertexShaderChannelModel() );

	m_blendEnabled = GetValueParamState< bool >( GetPluginParamValModel()->GetPluginModel().get(), BlendHelper::PARAM::BLEND_ENABLE );
	m_blendMode = GetValueParamState< BlendHelper::BlendMode >( GetPluginParamValModel()->GetPluginModel().get(), BlendHelper::PARAM::BLEND_MODE );

    SetPrevPlugin( prev );

    LoadResource( DefaultAssets::Instance().GetDefaultDesc< TextureAssetDesc >() );

    m_blendEnabled = GetValueParamState< bool >( GetPluginParamValModel()->GetPluginModel().get(), BlendHelper::PARAM::BLEND_ENABLE );
}

// *************************************
// 
DefaultTexturePlugin::~DefaultTexturePlugin         ()
{
}

// *************************************
// 
bool							DefaultTexturePlugin::IsValid     () const
{
    return ( m_vaChannel && GetPrevPlugin()->IsValid() );
}

// *************************************
// 
bool                            DefaultTexturePlugin::LoadResource  ( AssetDescConstPtr assetDescr )
{
    auto txAssetDescr = QueryTypedDesc< TextureAssetDescConstPtr >( assetDescr );
    
    // FIXME: dodac tutaj API pozwalajace tez ustawiac parametry dodawanej tekstury (normalny load z dodatkowymi parametrami)
    if ( txAssetDescr != nullptr )
    {
        //FIXME: use some better API to handle resources in general and textures in this specific case
        auto txDesc = DefaultTextureDescriptor::LoadTexture( txAssetDescr, DefaultTexturePluginDesc::TextureName() );

        // If texture doesn't exists, read fallback texture. 
        if( txDesc == nullptr )
        {
            txAssetDescr = DefaultAssets::Instance().GetFallbackDesc< TextureAssetDesc >();
            txDesc = DefaultTextureDescriptor::LoadTexture( txAssetDescr, DefaultTexturePluginDesc::TextureName() );
        }

        if( txDesc != nullptr )
        {
            auto txData = m_psc->GetTexturesDataImpl();
            auto replacedTex = txData->GetTexture( 0 );
            
            SamplerStateModelPtr newSamplerStateModel = replacedTex != nullptr ? replacedTex->GetSamplerState() : SamplerStateModel::Create( m_pluginParamValModel->GetTimeEvaluator() );

            txDesc->SetSamplerState( newSamplerStateModel );
            txDesc->SetSemantic( DataBuffer::Semantic::S_TEXTURE_STATIC );

            txData->SetTexture( 0, txDesc );
            SetAsset( 0, LAsset( txDesc->GetName(), assetDescr, txDesc->GetSamplerState() ) );

            HelperPixelShaderChannel::SetTexturesDataUpdate( m_psc );
            return true;
        }

    }
    return false;
}

// *************************************
// 
IVertexAttributesChannelConstPtr    DefaultTexturePlugin::GetVertexAttributesChannel  () const
{
    return m_vaChannel;
}

// *************************************
// 
IPixelShaderChannelPtr              DefaultTexturePlugin::GetPixelShaderChannel       () const
{
    return m_psc;
}

// *************************************
// 
IVertexShaderChannelConstPtr        DefaultTexturePlugin::GetVertexShaderChannel      () const
{
    return m_vsc;
}

// *************************************
// 
void                                DefaultTexturePlugin::Update                      ( TimeType t )
{
    BasePlugin::Update( t );

    HelperVertexShaderChannel::InverseTextureMatrix( m_pluginParamValModel, DefaultTexturePlugin::PARAM::TX_MAT );

    BlendHelper::UpdateBlendState( m_psc, m_blendEnabled, m_blendMode );


    HelperVertexAttributesChannel::PropagateAttributesUpdate( m_vaChannel, GetPrevPlugin() );
    if( HelperVertexAttributesChannel::PropagateTopologyUpdate( m_vaChannel, GetPrevPlugin() ) )
    {
        InitVertexAttributesChannel();
    }
    
    HelperPixelShaderChannel::PropagateUpdate( m_psc, GetPrevPlugin() );

    m_vsc->PostUpdate();
    m_psc->PostUpdate();    
}

// *************************************
//
void		DefaultTexturePlugin::InitVertexAttributesChannel		()
{
    if( !( GetPrevPlugin() && GetPrevPlugin()->GetVertexAttributesChannel() ) )
    {
        m_vaChannel = nullptr;
        return;
    }

    auto prevGeomChannel = GetPrevPlugin()->GetVertexAttributesChannel();
    auto prevCC = prevGeomChannel->GetComponents();

    //Only one texture
    VertexAttributesChannelDescriptor vaChannelDesc( * static_cast< const VertexAttributesChannelDescriptor * >( prevGeomChannel->GetDescriptor() ) );
    if( !vaChannelDesc.GetAttrChannelDescriptor( AttributeSemantic::AS_TEXCOORD ) )
    {
        vaChannelDesc.AddAttrChannelDesc( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );
    }
    
    if( !m_vaChannel )
    {		
        m_vaChannel = std::make_shared< VertexAttributesChannel >( prevGeomChannel->GetPrimitiveType(), vaChannelDesc, true, prevGeomChannel->IsTimeInvariant() );
    }
    else
    {
        m_vaChannel->ClearAll();
        m_vaChannel->SetDescriptor( vaChannelDesc );
    }

    auto desc = std::make_shared< AttributeChannelDescriptor >( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );
    for( unsigned int i = 0; i < prevCC.size(); ++i )
    {
        auto connComp = ConnectedComponent::Create();

        auto prevConnComp = std::static_pointer_cast< const model::ConnectedComponent >( prevCC[ i ] );
        auto prevCompChannels = prevConnComp->GetAttributeChannelsPtr();

        for( auto prevCompCh : prevCompChannels )
        {
            connComp->AddAttributeChannel( prevCompCh );
        }

        auto posChannel = prevConnComp->GetAttrChannel( AttributeSemantic::AS_POSITION );
        if( posChannel && !prevConnComp->GetAttrChannel( AttributeSemantic::AS_TEXCOORD ) )
        {
            //FIXME: only one texture - convex hull calculations
            auto uvs = new model::Float2AttributeChannel( desc, DefaultTexturePluginDesc::TextureName(), true );
            auto uvsPtr = Float2AttributeChannelPtr( uvs );
            
            Helper::UVGenerator::GenerateUV( std::static_pointer_cast< Float3AttributeChannel >( posChannel ),
                                             uvsPtr, glm::vec3( 1.0, 0.0, 0.0 ), glm::vec3( 0.0, 1.0, 0.0 ), true );

            connComp->AddAttributeChannel( uvsPtr );
        }

        m_vaChannel->AddConnectedComponent( connComp );
    }
}

// *************************************
// 
mathematics::RectConstPtr					DefaultTexturePlugin::GetAABB						( const glm::mat4 & trans ) const
{
    //auto trParam = GetCurrentParamTransform( this );

    //if( !trParam )
    //	return nullptr;

    //assert( trParam->NumTransforms() <= 1 );

    //if( trParam->NumTransforms() == 1 )
    //{
    //	auto trValue = trParam->Evaluate( 0 );

        auto rect = mathematics::Rect::Create();
        if( AABB( m_vaChannel.get(), trans, rect.get() ) )
            return rect;
        else
            return nullptr;
    //}
    //	
    //return nullptr;
}

} // model
} // bv
