#include "MipMapAsset.h"

namespace bv
{

// *******************************
//
SingleTextureAssetConstPtr MipMapAsset::GetLevel( SizeType level ) const
{
	if( level < GetLevelsNum() )
		return m_mipMaps[ level ];
	else
		return nullptr;
}

// *******************************
//
SizeType MipMapAsset::GetLevelsNum() const
{
	return m_mipMaps.size();
}

// *******************************
//
MipMapAssetConstPtr MipMapAsset::Create( const std::vector< SingleTextureAssetConstPtr > & mipMaps )
{
	return MipMapAssetConstPtr( new MipMapAsset( mipMaps ) );
}

// *******************************
//
MipMapAsset::MipMapAsset( const std::vector< SingleTextureAssetConstPtr > & mipMaps )
	: m_mipMaps( mipMaps )
{}

} // bv