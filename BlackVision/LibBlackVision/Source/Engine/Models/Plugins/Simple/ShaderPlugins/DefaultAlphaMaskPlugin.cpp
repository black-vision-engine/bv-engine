#include "stdafx.h"

#include "DefaultAlphaMaskPlugin.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Models/Plugins/Channels/Geometry/HelperVertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"
#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"

#include "Application/ApplicationContext.h"

#include "Assets/DefaultAssets.h"



#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {

namespace {
    const float pixelsPerUnitUVSpace = 540.f; //FIXME: ultra hacking
}

// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultAlphaMaskPluginDesc::DefaultAlphaMaskPluginDesc                      ()
    : BasePluginDescriptor( UID(), "alpha_mask", "am" )
{
}

// *******************************
//
IPluginPtr              DefaultAlphaMaskPluginDesc::CreatePlugin            ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultAlphaMaskPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultAlphaMaskPluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    //Create all models
    DefaultPluginParamValModelPtr model  = std::make_shared< DefaultPluginParamValModel >( timeEvaluator );
    DefaultParamValModelPtr psModel      = std::make_shared< DefaultParamValModel >();
    DefaultParamValModelPtr vsModel      = std::make_shared< DefaultParamValModel >();

    //Create all parameters and evaluators
    SimpleTransformEvaluatorPtr trTxEvaluator    = ParamValEvaluatorFactory::CreateSimpleTransformEvaluator( "txAlphaMat", timeEvaluator );

    //Register all parameters and evaloators in models
    vsModel->RegisterAll( trTxEvaluator );

    //Set models structure
    model->SetVertexShaderChannelModel( vsModel );
    model->SetPixelShaderChannelModel( psModel );

    //Set default values of all parameters
    trTxEvaluator->Parameter()->Transform().InitializeDefaultSRT();

    return model;
}

// *******************************
//
std::string             DefaultAlphaMaskPluginDesc::UID                     ()
{
    return "DEFAULT_ALPHA_MASK";
}

// *******************************
//
std::string             DefaultAlphaMaskPluginDesc::TextureName             ()
{
    return "AlphaTex0";
}

//FIXME: dodawanie kanalow w ten sposob (przez przypisanie na m_<xxx>channel powoduje bledy, trzeba to jakos poprawic, zeby bylo wiadomo, o co chodzi
//FIXME: teraz zle dodanie wychodzi dopiero po odpaleniu silnika, a to jest oczywisty blad

// ************************************************************************* PLUGIN *************************************************************************

// *************************************
// 
bool								DefaultAlphaMaskPlugin::SetPrevPlugin               ( IPluginPtr prev )
{
    if( BasePlugin::SetPrevPlugin( prev ) )
    {
        InitVertexAttributesChannel();

        HelperPixelShaderChannel::CloneRenderContext( m_psc, prev );
        auto ctx = m_psc->GetRendererContext();
        ctx->cullCtx->enabled = false;
        ctx->alphaCtx->blendEnabled = true;
        return true;
    }
    else
        return false;
    
}

// *************************************
// 
DefaultAlphaMaskPlugin::DefaultAlphaMaskPlugin  ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin( name, uid, prev, model )
    , m_psc( nullptr )
    , m_vsc( nullptr )
    , m_vaChannel( nullptr )
    , m_textureWidth( 0 )
    , m_textureHeight( 0 )
{
    m_psc = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel() );
    m_vsc = DefaultVertexShaderChannel::Create( model->GetVertexShaderChannelModel() );
    
    SetPrevPlugin( prev );

    LoadResource( DefaultAssets::Instance().GetDefaultDesc< TextureAssetDesc >() );
}

// *************************************
// 
DefaultAlphaMaskPlugin::~DefaultAlphaMaskPlugin         ()
{
}

// *************************************
// 
bool						DefaultAlphaMaskPlugin::IsValid     () const
{
    return ( m_vaChannel && GetPrevPlugin()->IsValid() );
}

// *************************************
// 
bool                        DefaultAlphaMaskPlugin::LoadResource  ( AssetDescConstPtr assetDescr )
{
    auto txAssetDescr = QueryTypedDesc< TextureAssetDescConstPtr >( assetDescr );

    // FIXME: dodac tutaj API pozwalajace tez ustawiac parametry dodawanej tekstury (normalny load z dodatkowymi parametrami)
    if ( txAssetDescr != nullptr )
    {
        bool success = true;

        //FIXME: use some better API to handle resources in general and textures in this specific case
        auto txDesc = DefaultTextureDescriptor::LoadTexture( txAssetDescr, DefaultAlphaMaskPluginDesc::TextureName() );
        
        // If texture doesn't exists, read fallback texture. 
        if( txDesc == nullptr )
        {
            txAssetDescr = DefaultAssets::Instance().GetFallbackDesc< TextureAssetDesc >();
            txDesc = DefaultTextureDescriptor::LoadTexture( txAssetDescr, DefaultAlphaMaskPluginDesc::TextureName() );

            success = false;
        }

        if( txDesc != nullptr )
        {
            auto txData = m_psc->GetTexturesDataImpl();
            auto replacedTex = txData->GetTexture( 0 );

            SamplerStateModelPtr newSamplerStateModel = replacedTex != nullptr ? replacedTex->GetSamplerState() : SamplerStateModel::Create( m_pluginParamValModel->GetTimeEvaluator() );

            txDesc->SetSamplerState( newSamplerStateModel );
            txDesc->SetSemantic( DataBuffer::Semantic::S_TEXTURE_STATIC );
            
            txData->SetTexture( 0, txDesc );
            SetAsset( 0, LAsset( txDesc->GetName(), txAssetDescr, txDesc->GetSamplerState() ) );
            
            HelperPixelShaderChannel::SetTexturesDataUpdate( m_psc );

            m_textureWidth = txDesc->GetWidth();
            m_textureHeight = txDesc->GetHeight();

            RecalculateUVChannel();

            return success;
        }
    }

    return false;
}

// *************************************
// 
IVertexAttributesChannelConstPtr    DefaultAlphaMaskPlugin::GetVertexAttributesChannel  () const
{
    return m_vaChannel;
}

// *************************************
// 
IPixelShaderChannelPtr         DefaultAlphaMaskPlugin::GetPixelShaderChannel       () const
{
    return m_psc;
}

// *************************************
// 
IVertexShaderChannelConstPtr        DefaultAlphaMaskPlugin::GetVertexShaderChannel      () const
{
    return m_vsc;
}

// *************************************
// 
void                                DefaultAlphaMaskPlugin::Update                      ( TimeType t )
{
    BasePlugin::Update( t );

    if( HelperVertexAttributesChannel::PropagateAttributesUpdate( m_vaChannel, GetPrevPlugin() ) )
    {
        RecalculateUVChannel();
    }

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
void		DefaultAlphaMaskPlugin::InitVertexAttributesChannel			()
{
    if( !( GetPrevPlugin() && GetPrevPlugin()->GetVertexAttributesChannel() ) )
    {
        m_vaChannel = nullptr;
        return;
    }

    auto prevGeomChannel = GetPrevPlugin()->GetVertexAttributesChannel();
    auto prevCC = prevGeomChannel->GetComponents();

    //add alpha mask texture desc
    //FIXME: is it possible that CC is empty?
    VertexAttributesChannelDescriptor vaChannelDesc( * static_cast< const VertexAttributesChannelDescriptor * >( prevGeomChannel->GetDescriptor() ) );
    vaChannelDesc.AddAttrChannelDesc( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );

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
        auto prevCompChannels = std::static_pointer_cast< const model::ConnectedComponent >( prevCC[ i ] )->GetAttributeChannelsPtr();
        
        for( auto prevCompCh : prevCompChannels )
        {
            connComp->AddAttributeChannel( prevCompCh );
        }

        //add alpha mask uv channel
        connComp->AddAttributeChannel( std::make_shared< Float2AttributeChannel >( desc, DefaultAlphaMaskPluginDesc::TextureName(), true ) );

        m_vaChannel->AddConnectedComponent( connComp );
    }

    RecalculateUVChannel();
}

// *************************************
//
void     DefaultAlphaMaskPlugin::RecalculateUVChannel         ()
{
    if( !m_vaChannel )
        return;

    //FIXME: only one texture - convex hull calculations
    float minX = 100000.0f, minY = 100000.0f;
    float maxX = 0.0f, maxY = 0.0f;

    float pixelsPerUnitUVSpace = ( float )( std::min( ApplicationContext::Instance().GetWidth(), ApplicationContext::Instance().GetHeight() ) / 2 );

    auto cc = m_vaChannel->GetComponents();
    for( unsigned int i = 0; i < cc.size(); ++i )
    {
        auto prevComp = std::static_pointer_cast< const model::ConnectedComponent >( cc[ i ] );
        auto posChannel = prevComp->GetAttrChannel( AttributeSemantic::AS_POSITION );
        if( posChannel )
        {
            auto pos = std::static_pointer_cast< Float3AttributeChannel >( posChannel )->GetVertices();
            for( unsigned int j = 0; j < posChannel->GetNumEntries(); ++j )
            {
                minX = std::min( minX, pos[ j ].x );
                minY = std::min( minY, pos[ j ].y );
                maxX = std::max( maxX, pos[ j ].x );
                maxY = std::max( maxY, pos[ j ].y );
            }
        }
    }

    for( unsigned int i = 0; i < cc.size(); ++i )
    {
        auto prevComp = std::static_pointer_cast< const model::ConnectedComponent >( cc[ i ] );
        auto posChannel = prevComp->GetAttrChannel( AttributeSemantic::AS_POSITION );
        auto uvChannel = prevComp->GetAttrChannel( AttributeSemantic::AS_TEXCOORD, -1 );
        
        if( posChannel && uvChannel )
        {
            auto pos = std::static_pointer_cast< Float3AttributeChannel >( posChannel );
            auto uvs = std::static_pointer_cast< Float2AttributeChannel >( uvChannel );

            auto & uvVerts = uvs->GetVertices();
            if( uvVerts.size() < posChannel->GetNumEntries() )
            {
                uvVerts.resize( posChannel->GetNumEntries() );
            }

            auto & posVerts = pos->GetVertices();
            for( unsigned int j = 0; j < posChannel->GetNumEntries(); ++j )
            {
                uvVerts[ j ] = glm::vec2( pixelsPerUnitUVSpace * ( posVerts[ j ].x - minX ) / m_textureWidth, 
                    pixelsPerUnitUVSpace * ( posVerts[ j ].y - minY ) / m_textureHeight );
            }
        }
    }
}

// *************************************
// 
SizeType									DefaultAlphaMaskPlugin::GetAlphaTextureWidth    () const
{
    return m_textureWidth;
}

// *************************************
//
SizeType                                     DefaultAlphaMaskPlugin::GetAlphaTextureHeight   () const
{
    return m_textureHeight;
}

} // model
} // bv
