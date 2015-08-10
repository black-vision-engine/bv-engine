#include "MipMapAssetDescriptor.h"
#include "MipMapBuilder.h"

#include <sstream>
#include <cassert>
#include <algorithm>

namespace bv
{

const std::string MipMapAssetDesc::uid = "MIP_MAP_ASSET_DESC";

// *******************************
//
SingleTextureAssetDescConstPtr MipMapAssetDesc::GetLevelDesc( SizeType level ) const
{
	if( level < m_mipMapDescs.size() )
		return m_mipMapDescs[ level ];
	return nullptr;
}

// *******************************
//
SizeType MipMapAssetDesc::GetLevelsNum() const
{
	return m_mipMapDescs.size();
}

// *******************************
//
MipMapFilterType MipMapAssetDesc::GetFilter() const
{
	return m_filterType;
}

// *******************************
//
MipMapAssetDescConstPtr	MipMapAssetDesc::Create( const std::vector< SingleTextureAssetDescConstPtr > & mipMaps, MipMapFilterType ft )
{
	return MipMapAssetDescConstPtr( new MipMapAssetDesc( mipMaps, ft ) );
}

// *******************************
//
MipMapAssetDescConstPtr	MipMapAssetDesc::Create( MipMapFilterType ft, const SingleTextureAssetDescConstPtr & origTexture, Int32 maxLevelsNum )
{
	return MipMapAssetDescConstPtr( new MipMapAssetDesc( ft, origTexture, maxLevelsNum ) );
}

// *******************************
//
MipMapAssetDesc::MipMapAssetDesc	( const std::vector< SingleTextureAssetDescConstPtr > & mipMaps, MipMapFilterType ft )
	: m_filterType( ft )
	, m_mipMapDescs( mipMaps )
{}

// *******************************
//
MipMapAssetDesc::MipMapAssetDesc	( MipMapFilterType ft, const SingleTextureAssetDescConstPtr & origTextureDesc, Int32 maxLevelsNum )
	: m_filterType( ft )
{
	GenereateLevelsDescs( origTextureDesc, maxLevelsNum );
}

// *******************************
//
void MipMapAssetDesc::GenereateLevelsDescs( const SingleTextureAssetDescConstPtr & origTexture, Int32 maxLevelsNum )
{
	auto mmSizes = tools::GenerateMipmapsSizes( tools::ImageSize( (unsigned int)origTexture->GetWidth(), (unsigned int)origTexture->GetHeight() ) );

	auto levelsNum = maxLevelsNum < 0 ? mmSizes.size() : std::min( mmSizes.size(), (SizeType)maxLevelsNum );

	for( SizeType i = 0; i < levelsNum; ++i )
	{
		auto key = GenKeyForGeneratedMipMap( origTexture->GetKey(), mmSizes[ i ].width, mmSizes[ i ].height, origTexture->GetFormat(), i, m_filterType );
		m_mipMapDescs.push_back( GeneratedSingleTextureAssetDesc::Create( key, mmSizes[ i ].width, mmSizes[ i ].height, origTexture->GetFormat(), origTexture->IsCacheable() ) );
	}
}

// *******************************
//
VoidConstPtr MipMapAssetDesc::QueryThis() const
{
	return shared_from_this();
}

// *******************************
//
const std::string &	MipMapAssetDesc::GetUID() const
{
	return MipMapAssetDesc::UID();
}

// *******************************
//
bool MipMapAssetDesc::IsCacheable() const
{
	return true;
}

// *******************************
//
const std::string &	MipMapAssetDesc::UID()
{
	return MipMapAssetDesc::uid;
}


// ******************************
//
namespace 
{
	// FIXME: Should be moved to some core module.
	template< typename T > 
	std::string toString( const T & t )
	{
		std::ostringstream ss;
		ss << t;
		return ss.str();
	}

	template<> 
	std::string toString< std::string >( const std::string & t )
	{
		return t;
	}

} // anonymous

// ******************************
//
std::string	MipMapAssetDesc::GenKeyForGeneratedMipMap( const std::string & origPath, SizeType width, SizeType height, TextureFormat format, SizeType mmLevel, MipMapFilterType mmFiletType )
{
	return toString( origPath ) + "W" + toString( width ) + "H" + toString( height ) + "F" + toString( (int)format ) + "ML" + toString( mmLevel ) + "MFT" + toString( (int)mmFiletType );
}

} // bv