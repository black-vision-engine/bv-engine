#include "TextureResource.h"

namespace bv
{

const std::string TextureResource::uid = "TEXTURE_RESOURCE";

// *******************************
// 
SingleTextureResourceConstPtr TextureResource::GetOriginal() const
{
	return m_originalTexture;
}

// *******************************
// 
MipMapResourceConstPtr TextureResource::GetMipMaps() const
{
	return m_mipMaps;
}

// *******************************
// 
TextureResourceConstPtr	TextureResource::Create( const SingleTextureResourceConstPtr & originalTexture, const MipMapResourceConstPtr & mipMaps )
{
	return std::make_shared< TextureResource >( originalTexture, mipMaps );
}

// *******************************
// 
TextureResource::TextureResource( const SingleTextureResourceConstPtr & originalTexture, const MipMapResourceConstPtr & mipMaps )
	: m_originalTexture( originalTexture )
	, m_mipMaps( mipMaps )
{}

// *******************************
// 
const std::string &	TextureResource::GetUID() const
{
	return TextureResource::uid;
}

// *******************************
// 
const std::string &	TextureResource::UID()
{
	return TextureResource::uid;
}

// *******************************
// 
VoidConstPtr TextureResource::QueryThis() const
{
	return shared_from_this();
}

} // bv