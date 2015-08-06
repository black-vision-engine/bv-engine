#include "GeneratedSingleTextureAssetDescriptor.h"

namespace bv
{

const std::string GeneratedSingleTextureAssetDesc::uid = "GENERATED_SINGLE_TEXTURE_ASSET_DESC";

// ***********************
//
const std::string &	GeneratedSingleTextureAssetDesc::GetUID() const
{
	return GeneratedSingleTextureAssetDesc::UID();
}

// ***********************
const std::string &	GeneratedSingleTextureAssetDesc::UID()
{
	return GeneratedSingleTextureAssetDesc::uid;
}

// ***********************
//
VoidConstPtr GeneratedSingleTextureAssetDesc::QueryThis() const
{
	return std::enable_shared_from_this<bv::GeneratedSingleTextureAssetDesc>::shared_from_this();
}

// ***********************
//
std::string	GeneratedSingleTextureAssetDesc::GetKey	() const
{
	return m_key;
}

// ***********************
//
GeneratedSingleTextureAssetDescConstPtr GeneratedSingleTextureAssetDesc::Create( const std::string & key, UInt32 width, UInt32 height, TextureFormat format, bool isCacheable )
{
	return GeneratedSingleTextureAssetDescConstPtr( new GeneratedSingleTextureAssetDesc( key, width, height, format, isCacheable ) );
}

// ***********************
//
GeneratedSingleTextureAssetDesc::GeneratedSingleTextureAssetDesc( const std::string & key, UInt32 width, UInt32 height, TextureFormat format, bool isCacheable )
	: SingleTextureAssetDesc( "", width, height, format, isCacheable )
	, m_key( key )
{}

} // bv

