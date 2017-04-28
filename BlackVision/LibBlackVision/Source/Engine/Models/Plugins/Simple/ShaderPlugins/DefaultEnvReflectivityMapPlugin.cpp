#include "stdafx.h"
#include "DefaultEnvReflectivityMapPlugin.h"

#include "Assets/DefaultAssets.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"
#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {


    // ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultEnvReflectivityMapPluginDesc::DefaultEnvReflectivityMapPluginDesc                ()
    : BasePluginDescriptor( UID(), "env reflectivity map", "refl" )
{}

// *******************************
//
IPluginPtr              DefaultEnvReflectivityMapPluginDesc::CreatePlugin                ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultEnvReflectivityMapPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultEnvReflectivityMapPluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper helper( timeEvaluator );

    //Create all models
    auto model  = helper.GetModel();

    helper.SetOrCreatePluginModel();
    helper.SetOrCreatePSModel();
    
    DefaultParamValModelPtr vsModel             = std::make_shared< DefaultParamValModel >();
    SimpleTransformEvaluatorPtr trTxEvaluator   = ParamValEvaluatorFactory::CreateSimpleTransformEvaluator( "reflMat", timeEvaluator );

    //Register all parameters and evaloators in models
    vsModel->RegisterAll( trTxEvaluator );

    //Set models structure
    model->SetVertexShaderChannelModel( vsModel );

    //Set default values of all parameters
    trTxEvaluator->Parameter()->Transform().InitializeDefaultSRT();
    trTxEvaluator->Parameter()->Transform().SetCenter( glm::vec3( 0.5, 0.5, 0.0 ), 0.0f );

    return helper.GetModel();
}

// *******************************
//
std::string             DefaultEnvReflectivityMapPluginDesc::UID                    ()
{
    return "DEFAULT_ENV_REFLECTIVITY_MAP";
}

// *******************************
// 
std::string             DefaultEnvReflectivityMapPluginDesc::TextureName            ()
{
    return "ReflectivityMap0";
}

// ************************************************************************* PLUGIN ************************************************************************* 


DefaultEnvReflectivityMapPlugin::DefaultEnvReflectivityMapPlugin( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model)
    : BasePlugin( name, uid, prev, std::static_pointer_cast< IPluginParamValModel >( model ) )
    , m_pixelShaderChannel( nullptr )
    , m_paramValModel( model )
{
    m_pixelShaderChannel = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel() );
	SetPrevPlugin( prev );

    LoadResource( DefaultAssets::Instance().GetDefaultDesc< TextureAssetDesc >() );
}


DefaultEnvReflectivityMapPlugin::~DefaultEnvReflectivityMapPlugin()
{}


// *************************************
// 
void DefaultEnvReflectivityMapPlugin::SetPrevPlugin( IPluginPtr prev )
{
    BasePlugin::SetPrevPlugin( prev );
	HelperPixelShaderChannel::CloneRenderContext( m_pixelShaderChannel, prev );
}

// *************************************
//
IPixelShaderChannelPtr              DefaultEnvReflectivityMapPlugin::GetPixelShaderChannel       () const
{
    return m_pixelShaderChannel;    
}

// *************************************
//
void                                DefaultEnvReflectivityMapPlugin::Update                      ( TimeType t )
{
	BasePlugin::Update( t );
    HelperPixelShaderChannel::PropagateUpdate( m_pixelShaderChannel, GetPrevPlugin() );
    m_pixelShaderChannel->PostUpdate();
}


// *************************************
// 
bool                            DefaultEnvReflectivityMapPlugin::LoadResource  ( AssetDescConstPtr assetDescr )
{
    auto txAssetDescr = QueryTypedDesc< TextureAssetDescConstPtr >( assetDescr );
    
    if ( txAssetDescr != nullptr )
    {
        auto txDesc = DefaultTextureDescriptor::LoadTexture( txAssetDescr, DefaultEnvReflectivityMapPluginDesc::TextureName() );

        if( txDesc != nullptr )
        {
            auto txData = m_pixelShaderChannel->GetTexturesDataImpl();
            auto replacedTex = txData->GetTexture( 0 );

            SamplerStateModelPtr newSamplerStateModel = replacedTex != nullptr ? replacedTex->GetSamplerState() : SamplerStateModel::Create( m_pluginParamValModel->GetTimeEvaluator() );

            txDesc->SetSamplerState( newSamplerStateModel );
            txDesc->SetSemantic( DataBuffer::Semantic::S_TEXTURE_STATIC );
            
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

