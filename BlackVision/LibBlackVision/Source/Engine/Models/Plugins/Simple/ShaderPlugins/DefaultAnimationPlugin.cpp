#include "stdafx.h"

#include "DefaultAnimationPlugin.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Models/Plugins/Channels/Geometry/HelperVertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/HelperVertexShaderChannel.h"

#include "Engine/Models/Plugins/HelperUVGenerator.h"
#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"

#include "Assets/DefaultAssets.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {

const std::string        DefaultAnimationPlugin::PARAM_ALPHA          = "alpha";
const std::string        DefaultAnimationPlugin::PARAM_FRAME_NUM      = "frameNum";
const std::string        DefaultAnimationPlugin::PARAM_AUTO_PLAY      = "autoPlay";
const std::string        DefaultAnimationPlugin::PARAM_FPS            = "fps";

const std::string        DefaultAnimationPlugin::PARAM_TX_MAT         = "txMat";


// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultAnimationPluginDesc::DefaultAnimationPluginDesc                          ()
    : BasePluginDescriptor( UID(), "animation", "tx" )
{
}

// *******************************
//
IPluginPtr              DefaultAnimationPluginDesc::CreatePlugin              ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultAnimationPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultAnimationPluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper helper( timeEvaluator );

    //Create all models
    auto model  = helper.GetModel();

    helper.SetOrCreatePluginModel();
    helper.AddSimpleParam( BlendHelper::PARAM::BLEND_ENABLE, true, true, true );
	helper.AddEnumParam( BlendHelper::PARAM::BLEND_MODE, BlendHelper::BlendMode::BM_Alpha, true, true );

    helper.AddSimpleParam( DefaultAnimationPlugin::PARAM_AUTO_PLAY, false, true, true );
    helper.AddSimpleParam( DefaultAnimationPlugin::PARAM_FPS, 24.f, true, true );

    helper.SetOrCreatePSModel();
    helper.AddSimpleParam( DefaultAnimationPlugin::PARAM_ALPHA, 1.f, true );
    helper.AddSimpleParam( DefaultAnimationPlugin::PARAM_FRAME_NUM, 0.f, true );    // FIXME: integer parmeters should be used here


    helper.SetOrCreateVSModel();
    helper.AddTransformParam( DefaultAnimationPlugin::PARAM_TX_MAT, true );

    auto param = helper.GetModel()->GetVertexShaderChannelModel()->GetParameter( DefaultAnimationPlugin::PARAM_TX_MAT );
    SetParameterCenterMass( param, 0.0f, glm::vec3( 0.5, 0.5, 0.0 ) );

    return model;
}

// *******************************
//
std::string             DefaultAnimationPluginDesc::UID                       ()
{
    return "DEFAULT_ANIMATION";
}

// *******************************
//
std::string             DefaultAnimationPluginDesc::TextureName               ()
{
    return "Tex0";
}

//FIXME: dodawanie kanalow w ten sposob (przez przypisanie na m_<xxx>channel powoduje bledy, trzeba to jakos poprawic, zeby bylo wiadomo, o co chodzi
//FIXME: teraz zle dodanie wychodzi dopiero po odpaleniu silnika, a to jest oczywisty blad

// ************************************************************************* PLUGIN *************************************************************************

void								DefaultAnimationPlugin::SetPrevPlugin               ( IPluginPtr prev )
{
    BasePlugin::SetPrevPlugin( prev );

    InitVertexAttributesChannel();
        
    HelperPixelShaderChannel::CloneRenderContext( m_psc, prev );
    auto ctx = m_psc->GetRendererContext();
    ctx->cullCtx->enabled = false;

	ctx->alphaCtx->blendEnabled = m_blendEnabled.GetParameter().Evaluate();
	BlendHelper::SetBlendRendererContext( m_psc, m_blendMode.GetParameter() );
}

// *************************************
// 
DefaultAnimationPlugin::DefaultAnimationPlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin( name, uid, prev, model )
    , m_psc( nullptr )
    , m_vsc( nullptr )
    , m_vaChannel( nullptr )
{
    m_psc = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel() );
    m_vsc = DefaultVertexShaderChannel::Create( model->GetVertexShaderChannelModel() );

    SetPrevPlugin( prev );

    m_texturesData = m_psc->GetTexturesDataImpl();
    
    LoadResource( DefaultAssets::Instance().GetDefaultDesc< AnimationAssetDesc >() );

    //Direct param state access (to bypass model querying)
    auto psModel = PixelShaderChannelModel();
    
    m_paramFrameNum = QueryTypedParam< ParamFloatPtr >( psModel->GetParameter( PARAM_FRAME_NUM ) );

    auto pluginModel =  m_pluginParamValModel->GetPluginModel();
    m_paramAutoPlay = QueryTypedParam< ParamBoolPtr >( pluginModel->GetParameter( PARAM_AUTO_PLAY ) );
    m_paramFPS = QueryTypedParam< ParamFloatPtr >( pluginModel->GetParameter( PARAM_FPS ) );

	m_blendEnabled = GetValueParamState< bool >( GetPluginParamValModel()->GetPluginModel().get(), BlendHelper::PARAM::BLEND_ENABLE );
	m_blendMode = GetValueParamState< BlendHelper::BlendMode >( GetPluginParamValModel()->GetPluginModel().get(), BlendHelper::PARAM::BLEND_MODE );

    assert( m_paramFrameNum );
}

// *************************************
// 
DefaultAnimationPlugin::~DefaultAnimationPlugin         ()
{}

// *************************************
// 
bool							DefaultAnimationPlugin::IsValid     () const
{
    return ( m_vaChannel && m_prevPlugin->IsValid() );
}

// *************************************
// 
bool                            DefaultAnimationPlugin::LoadResource  ( AssetDescConstPtr assetDescr )
{
    auto animAssetDescr = QueryTypedDesc< AnimationAssetDescConstPtr >( assetDescr );

    // FIXME: dodac tutaj API pozwalajace tez ustawiac parametry dodawanej tekstury (normalny load z dodatkowymi parametrami)
    if ( animAssetDescr != nullptr )
    {
        //FIXME: use some better API to handle resources in general and textures in this specific case
        auto animDesc = DefaultAnimationDescriptor::LoadAnimation( animAssetDescr, DefaultAnimationPluginDesc::TextureName() );

        if( animDesc != nullptr )
        {
            auto txData = m_psc->GetTexturesDataImpl();
            auto replacedAnim = txData->GetAnimation( 0 );

            SamplerStateModelPtr newSamplerStateModel = replacedAnim != nullptr ? replacedAnim->GetSamplerState() : SamplerStateModel::Create( m_pluginParamValModel->GetTimeEvaluator() );

            animDesc->SetSamplerState( newSamplerStateModel );
            animDesc->SetSamplerState( SamplerStateModel::Create( m_pluginParamValModel->GetTimeEvaluator() ) );
            
            txData->SetAnimation( 0, animDesc );
            SetAsset( 0, LAsset( animDesc->GetName(), animAssetDescr, animDesc->GetSamplerState() ) );

            HelperPixelShaderChannel::SetTexturesDataUpdate( m_psc );

            m_texturesNum = animDesc->NumTextures();
    
            return true;
        }
    }

    return false;
}

// *************************************
// 
IVertexAttributesChannelConstPtr    DefaultAnimationPlugin::GetVertexAttributesChannel  () const
{
    return m_vaChannel;
}

// *************************************
// 
IPixelShaderChannelPtr         DefaultAnimationPlugin::GetPixelShaderChannel       () const
{
    return m_psc;
}

// *************************************
// 
IVertexShaderChannelConstPtr        DefaultAnimationPlugin::GetVertexShaderChannel      () const
{
    return m_vsc;
}

// *************************************
// 
void                                DefaultAnimationPlugin::Update                      ( TimeType t )
{
    BasePlugin::Update( t );

    HelperVertexShaderChannel::InverseTextureMatrix( m_pluginParamValModel, DefaultAnimationPlugin::PARAM_TX_MAT );

    FrameUpdate();

	BlendHelper::UpdateBlendState( m_psc, m_blendEnabled, m_blendMode );

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
void		DefaultAnimationPlugin::InitVertexAttributesChannel		()
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
            auto uvs = new model::Float2AttributeChannel( desc, DefaultAnimationPluginDesc::TextureName(), true );
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
void		DefaultAnimationPlugin::FrameUpdate		                    ()
{
    if( m_paramAutoPlay->Evaluate() && ( ParameterChanged( PARAM_AUTO_PLAY ) || ParameterChanged( PARAM_FPS ) ) )
    {
        m_paramFrameNum->AccessInterpolator().RemoveAllKeys();
        SetParameter( m_paramFrameNum, 0.f, 0.f );
        SetParameter( m_paramFrameNum, ( Float32 )m_texturesNum / m_paramFPS->Evaluate(), ( Float32 )m_texturesNum );
    }

    auto frameNum = ( UInt32 )m_paramFrameNum->Evaluate(); // TODO: A to chyba juz nie potrzebne bo Update na modelu zrobiony
    m_texturesData->SetAnimationFrame( 0, frameNum ); // TODO: A to chyba juz nie potrzebne bo Update na modelu zrobiony
}

} // model
} // bv
