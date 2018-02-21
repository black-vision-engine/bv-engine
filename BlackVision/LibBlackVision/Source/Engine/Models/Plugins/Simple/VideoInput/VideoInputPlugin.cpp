#include "stdafx.h"

#include "VideoInputPlugin.h"

#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Interfaces/IValue.h"

#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/HelperVertexShaderChannel.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"

#include "Assets/DefaultAssets.h"


namespace bv {
namespace model {


const std::string        VideoInputPlugin::PARAMS::ALPHA    = "alpha";
const std::string        VideoInputPlugin::PARAMS::TX_MAT   = "txMat";




// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
VideoInputPluginDesc::VideoInputPluginDesc                          ()
    : BasePluginDescriptor( UID(), "video input", "vidin" )
{
}

// *******************************
//
IPluginPtr              VideoInputPluginDesc::CreatePlugin              ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< VideoInputPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   VideoInputPluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper helper( timeEvaluator );

    //Create all models
    auto model = helper.GetModel();

    helper.SetOrCreatePluginModel();

    helper.SetOrCreatePSModel();
    helper.AddSimpleParam( VideoInputPlugin::PARAMS::ALPHA, 1.f, true );

    helper.SetOrCreateVSModel();
    helper.AddTransformParam( VideoInputPlugin::PARAMS::TX_MAT, true );

    auto param = helper.GetModel()->GetVertexShaderChannelModel()->GetParameter( VideoInputPlugin::PARAMS::TX_MAT );
    SetParameterCenterMass( param, 0.0f, glm::vec3( 0.5, 0.5, 0.0 ) );

    return model;
}

// *******************************
//
std::string             VideoInputPluginDesc::UID                       ()
{
    return "VIDEO_INPUT_PLUGIN";
}

// *******************************
// 
std::string             VideoInputPluginDesc::TextureName               ()
{
    return "VideoInput0";
}



// ************************************************************************* PLUGIN *************************************************************************

// *************************************
// 
bool                    VideoInputPlugin::SetPrevPlugin                 ( IPluginPtr prev )
{
    BasePlugin::SetPrevPlugin( prev );
    HelperPixelShaderChannel::CloneRenderContext( m_psc, prev );

    return true;
}

// *************************************
// 
VideoInputPlugin::VideoInputPlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin( name, uid, prev, model )
    , m_psc( nullptr )
    , m_vsc( nullptr )
{
    m_psc = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel() );
    m_vsc = DefaultVertexShaderChannel::Create( model->GetVertexShaderChannelModel() );

    SetPrevPlugin( prev );

    //LoadResource( DefaultAssets::Instance().GetDefaultDesc< TextureAssetDesc >() );
}

// *************************************
// 
VideoInputPlugin::~VideoInputPlugin         ()
{}

// *************************************
// 
bool							VideoInputPlugin::IsValid     () const
{
    return ( GetPrevPlugin()->IsValid() );
}

// *************************************
// 
bool                            VideoInputPlugin::LoadResource  ( AssetDescConstPtr assetDescr )
{
    // auto txAssetDescr = QueryTypedDesc< TextureAssetDescConstPtr >( assetDescr );

    // // FIXME: dodac tutaj API pozwalajace tez ustawiac parametry dodawanej tekstury (normalny load z dodatkowymi parametrami)
    // if ( txAssetDescr != nullptr )
    // {
        // //FIXME: use some better API to handle resources in general and textures in this specific case
        // auto txDesc = DefaultTextureDescriptor::LoadTexture( txAssetDescr, VideoInputPluginDesc::TextureName() );

        // if( txDesc != nullptr )
        // {
            // txDesc->SetSamplerState( SamplerStateModel::Create( m_pluginParamValModel->GetTimeEvaluator() ) );
            // txDesc->SetSemantic( DataBuffer::Semantic::S_TEXTURE_STATIC );

            // auto txData = m_psc->GetTexturesDataImpl();
            // txData->SetTexture( 0, txDesc );
            // SetAsset( 0, LAsset( txDesc->GetName(), assetDescr, txDesc->GetSamplerState() ) );

            // HelperPixelShaderChannel::SetTexturesDataUpdate( m_psc );

            // m_textureWidth = txAssetDescr->GetOrigTextureDesc()->GetWidth();
            // m_textureHeight = txAssetDescr->GetOrigTextureDesc()->GetHeight();


            // return true;
        // }

    // }
    return false;
}


// *************************************
// 
IPixelShaderChannelPtr              VideoInputPlugin::GetPixelShaderChannel       () const
{
    return m_psc;
}

// *************************************
// 
IVertexShaderChannelConstPtr        VideoInputPlugin::GetVertexShaderChannel      () const
{
    return m_vsc;
}

// *************************************
// 
void                                VideoInputPlugin::Update                      ( TimeType t )
{
    BasePlugin::Update( t );

    HelperVertexShaderChannel::InverseTextureMatrix( m_pluginParamValModel, VideoInputPlugin::PARAMS::TX_MAT );
    HelperPixelShaderChannel::PropagateUpdate( m_psc, GetPrevPlugin() );

    m_vsc->PostUpdate();
    m_psc->PostUpdate();
}




} // model
} // bv


