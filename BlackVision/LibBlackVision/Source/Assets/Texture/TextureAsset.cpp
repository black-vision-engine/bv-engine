#include "stdafx.h"

#include "TextureAsset.h"

namespace bv
{

const std::string TextureAsset::uid = "TEXTURE_ASSET";

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
bool TextureAsset::HasMipMaps() const
{
	return GetMipMaps() && GetMipMaps()->GetLevelsNum() > 0;
}

// *******************************
// 
TextureAssetConstPtr	TextureAsset::Create( const SingleTextureAssetConstPtr & originalTexture, const MipMapAssetConstPtr & mipMaps )
{
	return TextureAssetConstPtr( new TextureAsset( originalTexture, mipMaps ) );
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