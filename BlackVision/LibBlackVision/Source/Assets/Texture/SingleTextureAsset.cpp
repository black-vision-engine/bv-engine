#include "SingleTextureAsset.h"

namespace bv
{

// ******************************
//
const std::string SingleTextureAsset::uid = "SIMPLE_TEXTURE_ASSET";

// ******************************
//
const std::string &	SingleTextureAsset::GetKey() const
{
	return m_key;
}

// ******************************
//
UInt32 SingleTextureAsset::GetWidth() const
{
	return m_width;
}

// ******************************
//
UInt32 SingleTextureAsset::GetHeight() const
{
	return m_height;
}

// ******************************
//
TextureFormat SingleTextureAsset::GetFormat() const
{
	return m_format;
}

// ******************************
//
MemoryChunkConstPtr	SingleTextureAsset::GetData() const
{
	return m_memory;
}

// ******************************
//
SingleTextureAssetConstPtr SingleTextureAsset::Create( const MemoryChunkConstPtr & memory, const std::string & key, UInt32 width, UInt32 height, TextureFormat format, bool cacheOnHardDrive )
{
	return SingleTextureAssetConstPtr( new SingleTextureAsset( memory, key, width, height, format, cacheOnHardDrive ) );
}

// ******************************
//
SingleTextureAsset::SingleTextureAsset( const MemoryChunkConstPtr & memory, const std::string & key, UInt32 width, UInt32 height, TextureFormat format, bool cacheOnHardDrive )
	: m_memory( memory )
	, m_key( key )
	, m_width( width )
	, m_height( height )
	, m_format( format )
	, m_cacheOnHardDrive( cacheOnHardDrive )
{}

// *******************************
// 
bool SingleTextureAsset::GetCacheOnHardDrive () const
{
	return m_cacheOnHardDrive;
}

// *******************************
// 
const std::string &	SingleTextureAsset::GetUID() const
{
	return SingleTextureAsset::uid;
}

// *******************************
// 
const std::string &	SingleTextureAsset::UID()
{
	return SingleTextureAsset::uid;
}

// *******************************
// 
VoidConstPtr SingleTextureAsset::QueryThis() const
{
	return shared_from_this();
}

} //bv