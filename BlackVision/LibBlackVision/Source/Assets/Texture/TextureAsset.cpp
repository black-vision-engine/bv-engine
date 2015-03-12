#include "TextureAsset.h"

namespace bv
{

const std::string TextureAsset::uid = "TEXTURE_RESOURCE";

// *******************************
// 
SingleTextureAssetConstPtr TextureAsset::GetOriginal() const
{
	return m_originalTexture;
}

// *******************************
// 
MipMapAssetConstPtr TextureAsset::GetMipMaps() const
{
	return m_mipMaps;
}

// *******************************
// 
TextureAssetConstPtr	TextureAsset::Create( const SingleTextureAssetConstPtr & originalTexture, const MipMapAssetConstPtr & mipMaps )
{
	return std::make_shared< TextureAsset >( originalTexture, mipMaps );
}

// *******************************
// 
TextureAsset::TextureAsset( const SingleTextureAssetConstPtr & originalTexture, const MipMapAssetConstPtr & mipMaps )
	: m_originalTexture( originalTexture )
	, m_mipMaps( mipMaps )
{}

// *******************************
// 
const std::string &	TextureAsset::GetUID() const
{
	return TextureAsset::uid;
}

// *******************************
// 
const std::string &	TextureAsset::UID()
{
	return TextureAsset::uid;
}

// *******************************
// 
VoidConstPtr TextureAsset::QueryThis() const
{
	return shared_from_this();
}

} // bv