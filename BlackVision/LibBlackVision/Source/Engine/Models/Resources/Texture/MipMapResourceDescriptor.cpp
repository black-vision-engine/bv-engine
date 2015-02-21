#include "MipMapResourceDescriptor.h"
#include "Tools/MipMapBuilder/Source/MipMapBuilder.h"

namespace bv
{

const std::string MipMapResourceDesc::uid = "MIP_MAP_RESOURCE_DESC";

// *******************************
//
SingleTextureResourceDescConstPtr MipMapResourceDesc::GetLevelDesc( SizeType level ) const
{
	if( level < m_mipMapDescs.size() )
		return m_mipMapDescs[ level ];
	return nullptr;
}

// *******************************
//
SizeType MipMapResourceDesc::GetLevelsNum() const
{
	return m_mipMapDescs.size();
}

// *******************************
//
MipMapFilterType MipMapResourceDesc::GetFilter() const
{
	return m_filterType;
}

// *******************************
//
MipMapResourceDescConstPtr	MipMapResourceDesc::Create( const std::vector< SingleTextureResourceDescConstPtr > & mipMaps )
{
	return std::make_shared< MipMapResourceDesc >( mipMaps );
}

// *******************************
//
MipMapResourceDescConstPtr	MipMapResourceDesc::Create( MipMapFilterType ft, const SingleTextureResourceDescConstPtr & origTexture )
{
	return std::make_shared< MipMapResourceDesc >( ft, origTexture );
}

// *******************************
//
MipMapResourceDesc::MipMapResourceDesc	( const std::vector< SingleTextureResourceDescConstPtr > & mipMaps )
	: m_filterType()
	, m_mipMapDescs( mipMaps	 )
{}

// *******************************
//
MipMapResourceDesc::MipMapResourceDesc	( MipMapFilterType ft, const SingleTextureResourceDescConstPtr & origTextureDesc )
	: m_filterType( ft )
{
	GenereateLevelsDescs( origTextureDesc );
}

// *******************************
//
void MipMapResourceDesc::GenereateLevelsDescs( const SingleTextureResourceDescConstPtr & origTexture )
{
	auto mmSizes = tools::GenerateMipmapsSizes( tools::ImageSize( (unsigned int)origTexture->GetWidth(), (unsigned int)origTexture->GetHeight() ) );

	for( auto mms : mmSizes )
		m_mipMapDescs.push_back( SingleTextureResourceDesc::Create( origTexture->GetImagePath(), mms.width, mms.height, origTexture->GetFormat() ) );
}

// *******************************
//
VoidConstPtr MipMapResourceDesc::QueryThis() const
{
	return shared_from_this();
}

// *******************************
//
const std::string &	MipMapResourceDesc::GetUID() const
{
	return MipMapResourceDesc::UID();
}

// *******************************
//
bool MipMapResourceDesc::IsCacheable() const
{
	return true;
}

// *******************************
//
const std::string &	MipMapResourceDesc::UID()
{
	return MipMapResourceDesc::uid;
}

} // bv