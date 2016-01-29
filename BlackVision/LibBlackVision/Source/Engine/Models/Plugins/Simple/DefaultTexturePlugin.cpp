#include "stdafx.h"

#include "DefaultTexturePlugin.h"

#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine\Interfaces\IValue.h"

#include "Engine/Models/Plugins/Channels/Geometry/VacAABB.h"

#include "Engine/Models/Plugins/Channels/Geometry/HelperVertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"

#include "Engine/Models/Plugins/HelperUVGenerator.h"

#include "Assets/DefaultAssets.h"


namespace bv { namespace model {


const std::string        DefaultTexturePlugin::BLEND_ENABLE = "blend enable";


// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultTexturePluginDesc::DefaultTexturePluginDesc                          ()
    : BasePluginDescriptor( UID(), "texture", "tx" )
{
}

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
    helper.CreateVacModel();

    //Create all models
    //DefaultPluginParamValModelPtr model  = std::make_shared< DefaultPluginParamValModel >( timeEvaluator );
    DefaultPluginParamValModelPtr model  = helper.GetModel();
    DefaultParamValModelPtr psModel      = std::make_shared< DefaultParamValModel >();
    DefaultParamValModelPtr vsModel      = std::make_shared< DefaultParamValModel >();

    //Create all parameters and evaluators
    SimpleFloatEvaluatorPtr     alphaEvaluator   = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "alpha", timeEvaluator );
    SimpleTransformEvaluatorPtr trTxEvaluator    = ParamValEvaluatorFactory::CreateSimpleTransformEvaluator( "txMat", timeEvaluator );
    
    helper.AddSimpleParam( DefaultTexturePlugin::BLEND_ENABLE, true, true, true );

    //Register all parameters and evaloators in models
    vsModel->RegisterAll( trTxEvaluator );
    psModel->RegisterAll( alphaEvaluator );

    //Set models structure
    model->SetVertexShaderChannelModel( vsModel );
    model->SetPixelShaderChannelModel( psModel );

    //Set default values of all parameters
    alphaEvaluator->Parameter()->SetVal( 1.f, TimeType( 0.0 ) );
    trTxEvaluator->Parameter()->Transform().InitializeDefaultSRT();

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

//FIXME: dodawanie kanalow w ten sposob (przez przypisanie na m_<xxx>channel powoduje bledy, trzeba to jakos poprawic, zeby bylo wiadomo, o co chodzi
//FIXME: teraz zle dodanie wychodzi dopiero po odpaleniu silnika, a to jest oczywisty blad

// ************************************************************************* PLUGIN *************************************************************************

// *************************************
// 
void DefaultTexturePlugin::SetPrevPlugin( IPluginPtr prev )
{
    BasePlugin::SetPrevPlugin( prev );

    InitVertexAttributesChannel();

    HelperPixelShaderChannel::CloneRenderContext( m_psc, prev );
    auto ctx = m_psc->GetRendererContext();
    ctx->cullCtx->enabled = false;
    
    ctx->alphaCtx->blendEnabled = true;
    ctx->alphaCtx->srcRGBBlendMode = model::AlphaContext::SrcBlendMode::SBM_SRC_ALPHA;
    ctx->alphaCtx->dstRGBBlendMode = model::AlphaContext::DstBlendMode::DBM_ONE_MINUS_SRC_ALPHA;
    //HelperPixelShaderChannel::SetRendererContextUpdate( m_psc );
}

// *************************************
// 
DefaultTexturePlugin::DefaultTexturePlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin< IPlugin >( name, uid, prev, model )
    , m_psc( nullptr )
    , m_vsc( nullptr )
    , m_vaChannel( nullptr )
{
    m_psc = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel() );
    m_vsc = DefaultVertexShaderChannel::Create( model->GetVertexShaderChannelModel() );

    SetPrevPlugin( prev );

    LoadResource( DefaultAssets::Instance().GetDefaultDesc< TextureAssetDesc >() );
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
    return ( m_vaChannel && m_prevPlugin->IsValid() );
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

        if( txDesc != nullptr )
        {
            txDesc->SetSamplerState( SamplerStateModel::Create( m_pluginParamValModel->GetTimeEvaluator() ) );
            txDesc->SetSemantic( DataBuffer::Semantic::S_TEXTURE_STATIC );
            
            auto txData = m_psc->GetTexturesDataImpl();
            txData->SetTexture( 0, txDesc );
            SetAsset( 0, LAsset( txDesc->GetName(), assetDescr, txDesc->GetSamplerState() ) );

            HelperPixelShaderChannel::SetTexturesDataUpdate( m_psc );

            m_textureWidth = txAssetDescr->GetOrigTextureDesc()->GetWidth();
            m_textureHeight = txAssetDescr->GetOrigTextureDesc()->GetHeight();


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
    
    if( ParameterChanged( BLEND_ENABLE ) )
    {
        auto ctx = m_psc->GetRendererContext();
        ctx->alphaCtx->blendEnabled = std::static_pointer_cast<ParamBool>( GetParameter( BLEND_ENABLE ) )->Evaluate();
        
        auto contextUpdateId = m_psc->GetRendererContextUpdateID();
        m_psc->SetRendererContextUpdateID( contextUpdateId + 1 );
    }

    HelperVertexAttributesChannel::PropagateAttributesUpdate( m_vaChannel, m_prevPlugin );
    if( HelperVertexAttributesChannel::PropagateTopologyUpdate( m_vaChannel, m_prevPlugin ) )
    {
        InitVertexAttributesChannel();
    }
    
    HelperPixelShaderChannel::PropagateUpdate( m_psc, m_prevPlugin );

    m_vsc->PostUpdate();
    m_psc->PostUpdate();    
}

// *************************************
//
void		DefaultTexturePlugin::InitVertexAttributesChannel		()
{
    if( !( m_prevPlugin && m_prevPlugin->GetVertexAttributesChannel() ) )
    {
        m_vaChannel = nullptr;
        return;
    }

    auto prevGeomChannel = m_prevPlugin->GetVertexAttributesChannel();
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

    auto desc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );
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
            
            Helper::UVGenerator::generateUV( reinterpret_cast< const glm::vec3 * >( posChannel->GetData() ), posChannel->GetNumEntries(),
                                            uvsPtr, glm::vec3( 1.0, 0.0, 0.0 ), glm::vec3( 0.0, 1.0, 0.0 ), true );

            connComp->AddAttributeChannel( uvsPtr );
        }

        m_vaChannel->AddConnectedComponent( connComp );
    }
    
    assert( prevGeomChannel->GetComponents().size() > 0 );
}

// *************************************
// 
SizeType									DefaultTexturePlugin::GetTextureWidth             () const
{
    return m_textureWidth;
}

// *************************************
//
SizeType									DefaultTexturePlugin::GetTextureHeight            () const
{
    return m_textureHeight;
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
