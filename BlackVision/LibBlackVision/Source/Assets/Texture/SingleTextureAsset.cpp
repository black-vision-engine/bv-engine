#include "SingleTextureAsset.h"

namespace bv
{

// ******************************
//
const std::string SingleTextureResource::uid = "SIMPLE_TEXTURE_RESOURCE";

// ******************************
//
const std::string &	SingleTextureResource::GetKey() const
{
	return m_key;
}

// ******************************
//
UInt32 SingleTextureResource::GetWidth() const
{
	return m_width;
}

// ******************************
//
UInt32 SingleTextureResource::GetHeight() const
{
	return m_height;
}

// ******************************
//
TextureFormat SingleTextureResource::GetFormat() const
{
	return m_format;
}

// ******************************
//
MemoryChunkConstPtr	SingleTextureResource::GetData() const
{
	return m_memory;
}

// ******************************
//
SingleTextureResourceConstPtr SingleTextureResource::Create( const MemoryChunkConstPtr & memory, const std::string & key, UInt32 width, UInt32 height, TextureFormat format )
{
	return std::make_shared< SingleTextureResource >( memory, key, width, height, format );
}

// ******************************
//
SingleTextureResource::SingleTextureResource( const MemoryChunkConstPtr & memory, const std::string & key, UInt32 width, UInt32 height, TextureFormat format )
	: m_memory( memory )
	, m_key( key )
	, m_width( width )
	, m_height( height )
	, m_format( format )
{}

// *******************************
// 
const std::string &	SingleTextureResource::GetUID() const
{
	return SingleTextureResource::uid;
}

// *******************************
// 
const std::string &	SingleTextureResource::UID()
{
	return SingleTextureResource::uid;
}

// *******************************
// 
VoidConstPtr SingleTextureResource::QueryThis() const
{
	return shared_from_this();
}

} //bv