#include "TextureResource.h"

namespace bv
{

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

} // bv