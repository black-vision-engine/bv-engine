#include "MipMapAsset.h"

namespace bv
{

// *******************************
//
SingleTextureResourceConstPtr MipMapResource::GetLevel( SizeType level ) const
{
	if( level < GetLevelsNum() )
		return m_mipMaps[ level ];
	else
		return nullptr;
}

// *******************************
//
SizeType MipMapResource::GetLevelsNum() const
{
	return m_mipMaps.size();
}

// *******************************
//
MipMapResourceConstPtr MipMapResource::Create( const std::vector< SingleTextureResourceConstPtr > & mipMaps )
{
	return std::make_shared< MipMapResource >( mipMaps );
}

// *******************************
//
MipMapResource::MipMapResource( const std::vector< SingleTextureResourceConstPtr > & mipMaps )
	: m_mipMaps( mipMaps )
{}

} // bv