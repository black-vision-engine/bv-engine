#include "stdafx.h"
#include "DefaultEnvironmentMapPlugin.h"

#include "Assets/DefaultAssets.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"

namespace bv { namespace model {


const std::string        DefaultEnvironmentMapPlugin::PARAM::Reflectivity   = "reflectivity";

// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultEnvironmentMapPluginDesc::DefaultEnvironmentMapPluginDesc                ()
    : BasePluginDescriptor( UID(), "environmental map", "env" )
{}

// *******************************
//
IPluginPtr              DefaultEnvironmentMapPluginDesc::CreatePlugin                ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultEnvironmentMapPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultEnvironmentMapPluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper helper( timeEvaluator );

    //Create all models
    auto model  = helper.GetModel();

    helper.CreatePluginModel();

    helper.CreatePSModel();
    helper.AddSimpleParam( DefaultEnvironmentMapPlugin::PARAM::Reflectivity, 0.5f, true );

    return helper.GetModel();
}

// *******************************
//
std::string             DefaultEnvironmentMapPluginDesc::UID                    ()
{
    return "DEFAULT_ENVIRONMENTAL_MAP";
}

// *******************************
// 
std::string             DefaultEnvironmentMapPluginDesc::TextureName            ()
{
    //return "Tex0";
    return "EnvMap0";
}

// ************************************************************************* PLUGIN ************************************************************************* 


// ***********************
//
DefaultEnvironmentMapPlugin::DefaultEnvironmentMapPlugin( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
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
DefaultEnvironmentMapPlugin::~DefaultEnvironmentMapPlugin()
{}

// *************************************
// 
void DefaultEnvironmentMapPlugin::SetPrevPlugin( IPluginPtr prev )
{
    BasePlugin::SetPrevPlugin( prev );
	HelperPixelShaderChannel::CloneRenderContext( m_pixelShaderChannel, prev );
}

// *************************************
//
IPixelShaderChannelPtr              DefaultEnvironmentMapPlugin::GetPixelShaderChannel       () const
{
    return m_pixelShaderChannel;    
}

// *************************************
//
void                                DefaultEnvironmentMapPlugin::Update                      ( TimeType t )
{
	BasePlugin::Update( t );
    HelperPixelShaderChannel::PropagateUpdate( m_pixelShaderChannel, m_prevPlugin );
    m_pixelShaderChannel->PostUpdate();
}


// *************************************
// 
bool                            DefaultEnvironmentMapPlugin::LoadResource  ( AssetDescConstPtr assetDescr )
{
    auto txAssetDescr = QueryTypedDesc< TextureAssetDescConstPtr >( assetDescr );
    
    if ( txAssetDescr != nullptr )
    {
        auto txDesc = DefaultTextureDescriptor::LoadTexture( txAssetDescr, DefaultEnvironmentMapPluginDesc::TextureName() );

        if( txDesc != nullptr )
        {
            txDesc->SetSamplerState( SamplerStateModel::Create( m_pluginParamValModel->GetTimeEvaluator() ) );
            txDesc->SetSemantic( DataBuffer::Semantic::S_TEXTURE_STATIC );
            
            auto txData = m_pixelShaderChannel->GetTexturesDataImpl();
            txData->SetTexture( 0, txDesc );
            SetAsset( 0, LAsset( txDesc->GetName(), assetDescr, txDesc->GetSamplerState() ) );

            HelperPixelShaderChannel::SetTexturesDataUpdate( m_pixelShaderChannel );

            return true;
        }

    }
    return false;
}

} // model
} // bv

