#include "MipMapAssetDescriptor.h"
#include "MipMapBuilder.h"

namespace bv
{

const std::string MipMapAssetDesc::uid = "MIP_MAP_ASSET_DESC";

// ***********************
//
void                MipMapAssetDesc::Serialize       ( SerializeObject & /*sob*/ ) const
{
    assert( !"implement me" );
}

// ***********************
//
ISerializablePtr     MipMapAssetDesc::Create          ( DeserializeObject & /*dob*/ )
{
    assert( !"implement me" );
    return nullptr;
}

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
	return std::make_shared< MipMapAssetDesc >( mipMaps, ft );
}

// *******************************
//
MipMapAssetDescConstPtr	MipMapAssetDesc::Create( MipMapFilterType ft, const SingleTextureAssetDescConstPtr & origTexture )
{
	return std::make_shared< MipMapAssetDesc >( ft, origTexture );
}

// *******************************
//
MipMapAssetDesc::MipMapAssetDesc	( const std::vector< SingleTextureAssetDescConstPtr > & mipMaps, MipMapFilterType ft )
	: m_filterType( ft )
	, m_mipMapDescs( mipMaps )
{}

// *******************************
//
MipMapAssetDesc::MipMapAssetDesc	( MipMapFilterType ft, const SingleTextureAssetDescConstPtr & origTextureDesc )
	: m_filterType( ft )
{
	GenereateLevelsDescs( origTextureDesc );
}

// *******************************
//
void MipMapAssetDesc::GenereateLevelsDescs( const SingleTextureAssetDescConstPtr & origTexture )
{
	auto mmSizes = tools::GenerateMipmapsSizes( tools::ImageSize( (unsigned int)origTexture->GetWidth(), (unsigned int)origTexture->GetHeight() ) );

	for( auto mms : mmSizes )
		m_mipMapDescs.push_back( SingleTextureAssetDesc::Create( origTexture->GetImagePath(), mms.width, mms.height, origTexture->GetFormat(), origTexture->IsCacheable() ) );
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

} // bv