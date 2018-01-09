#include "stdafx.h"
#include "TexturePluginBase.h"

#include "Engine/Models/Plugins/Channels/PixelShader/GPUTextureDescriptor.h"
#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTextureDescriptor.h"
#include "Engine/Models/Plugins/Channels/PixelShader/ResourceStateModel.h"

#include "Engine/Models/Plugins/Channels/Geometry/HelperVertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/HelperVertexShaderChannel.h"
#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"

#include "Assets/DefaultAssets.h"




namespace bv {
namespace model
{


// *************************************
// 
TexturePluginBase::TexturePluginBase            ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin( name, uid, prev, model )
    , m_psc( nullptr )
{
    m_psc = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel() );
}


// *************************************
// 
bool                            TexturePluginBase::LoadResource  ( AssetDescConstPtr assetDescr )
{
    auto txAssetDescr = QueryTypedDesc< TextureAssetDescConstPtr >( assetDescr );

    // FIXME: dodac tutaj API pozwalajace tez ustawiac parametry dodawanej tekstury (normalny load z dodatkowymi parametrami)
    if( txAssetDescr != nullptr )
    {
        bool success = true;

        //FIXME: use some better API to handle resources in general and textures in this specific case
        auto txDesc = DefaultTextureDescriptor::LoadTexture( txAssetDescr, GetTextureName() );

        // If texture doesn't exists, read fallback texture. 
        if( txDesc == nullptr )
        {
            txAssetDescr = DefaultAssets::Instance().GetFallbackDesc< TextureAssetDesc >();
            txDesc = DefaultTextureDescriptor::LoadTexture( txAssetDescr, GetTextureName() );

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
            return success;
        }

    }
    return false;
}

// *************************************
// 
IPixelShaderChannelPtr              TexturePluginBase::GetPixelShaderChannel       () const
{
    return m_psc;
}


} // model
} // bv

