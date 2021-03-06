#include "stdafx.h"
#include "DefaultEnvironmentTexturePlugin.h"

#include "Assets/DefaultAssets.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"



#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {


const std::string        DefaultEnvironmentTexturePlugin::PARAM::Reflectivity   = "reflectivity";
const std::string        DefaultEnvironmentTexturePlugin::PARAM::TxMat			= "envMat";
const std::string        DefaultEnvironmentTexturePlugin::PARAM::EnvMixMode     = "envMixMode";



// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultEnvironmentTexturePluginDesc::DefaultEnvironmentTexturePluginDesc                ()
    : BasePluginDescriptor( UID(), "environmental tex", "env" )
{}

// *******************************
//
IPluginPtr              DefaultEnvironmentTexturePluginDesc::CreatePlugin                ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultEnvironmentTexturePlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultEnvironmentTexturePluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper helper( timeEvaluator );

    //Create all models
    auto model  = helper.GetModel();

    helper.SetOrCreatePluginModel();

    helper.SetOrCreatePSModel();
    helper.AddSimpleParam( DefaultEnvironmentTexturePlugin::PARAM::Reflectivity, 0.5f, true );
	helper.AddTransformParam( DefaultEnvironmentTexturePlugin::PARAM::TxMat );
    helper.AddSimpleParam( DefaultEnvironmentTexturePlugin::PARAM::EnvMixMode, ( int )DefaultEnvironmentTexturePlugin::MixMode::Blend, true );

    return helper.GetModel();
}

// *******************************
//
std::string             DefaultEnvironmentTexturePluginDesc::UID                    ()
{
    return "DEFAULT_ENVIRONMENTAL_TEXTURE";
}

// *******************************
// 
std::string             DefaultEnvironmentTexturePluginDesc::TextureName            ()
{
    return "EnvMap0";
}

// ************************************************************************* PLUGIN ************************************************************************* 


// ***********************
//
DefaultEnvironmentTexturePlugin::DefaultEnvironmentTexturePlugin( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin( name, uid, prev, std::static_pointer_cast< IPluginParamValModel >( model ) )
    , m_pixelShaderChannel( nullptr )
    , m_paramValModel( model )
{
    m_pixelShaderChannel = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel() );
	SetPrevPlugin( prev );

    LoadResource( DefaultAssets::Instance().GetDefaultDesc< TextureAssetDesc >() );
}

// ***********************
//
DefaultEnvironmentTexturePlugin::~DefaultEnvironmentTexturePlugin()
{}

// *************************************
// 
bool DefaultEnvironmentTexturePlugin::SetPrevPlugin( IPluginPtr prev )
{
    if( BasePlugin::SetPrevPlugin( prev ) )
    {
        HelperPixelShaderChannel::CloneRenderContext( m_pixelShaderChannel, prev );
        return true;
    }
    else 
        return false;
}

// *************************************
//
IPixelShaderChannelPtr              DefaultEnvironmentTexturePlugin::GetPixelShaderChannel       () const
{
    return m_pixelShaderChannel;    
}

// *************************************
//
void                                DefaultEnvironmentTexturePlugin::Update                      ( TimeType t )
{
	BasePlugin::Update( t );
    HelperPixelShaderChannel::PropagateUpdate( m_pixelShaderChannel, GetPrevPlugin() );
    m_pixelShaderChannel->PostUpdate();
}


// *************************************
// 
bool                            DefaultEnvironmentTexturePlugin::LoadResource  ( AssetDescConstPtr assetDescr )
{
    auto txAssetDescr = QueryTypedDesc< TextureAssetDescConstPtr >( assetDescr );
    
    if ( txAssetDescr != nullptr )
    {
        bool success = true;

        auto txDesc = DefaultTextureDescriptor::LoadTexture( txAssetDescr, DefaultEnvironmentTexturePluginDesc::TextureName() );

        // If texture doesn't exists, read fallback texture. 
        if( txDesc == nullptr )
        {
            txAssetDescr = DefaultAssets::Instance().GetFallbackDesc< TextureAssetDesc >();
            txDesc = DefaultTextureDescriptor::LoadTexture( txAssetDescr, DefaultEnvironmentTexturePluginDesc::TextureName() );

            success = false;
        }

        if( txDesc != nullptr )
        {
            auto txData = m_pixelShaderChannel->GetTexturesDataImpl();
            auto replacedTex = txData->GetTexture( 0 );

            SamplerStateModelPtr newSamplerStateModel = replacedTex != nullptr ? replacedTex->GetSamplerState() : SamplerStateModel::Create( m_pluginParamValModel->GetTimeEvaluator() );

            txDesc->SetSamplerState( newSamplerStateModel );
            txDesc->SetSemantic( DataBuffer::Semantic::S_TEXTURE_STATIC );
            
            txData->SetTexture( 0, txDesc );
            SetAsset( 0, LAsset( txDesc->GetName(), txAssetDescr, txDesc->GetSamplerState() ) );

            HelperPixelShaderChannel::SetTexturesDataUpdate( m_pixelShaderChannel );

            return success;
        }

    }
    return false;
}

} // model
} // bv

