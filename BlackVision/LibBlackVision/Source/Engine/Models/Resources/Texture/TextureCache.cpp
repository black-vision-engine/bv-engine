#include "TextureCache.h"

#include <sstream>
#include <cassert>

namespace bv
{

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
bool TextureCache::Add( const std::string & key, const TextureResourceConstPtr & textureRes )
{
	if( Find( key ) )
		return false;
	else
	{
		Update( key, textureRes );
		return true;
	}
}

// ******************************
//
bool TextureCache::Add( const TextureResourceDescConstPtr & textureDesc, const TextureResourceConstPtr & textureRes )
{
	return Add( GenKeyForTextureResource( textureDesc ), textureRes );
}


// ******************************
//
void TextureCache::Update( const std::string & key, const TextureResourceConstPtr & textureRes )
{
	m_textures[ key ] = textureRes;
}

// ******************************
//
bool TextureCache::Exists	( const std::string & key ) const
{
	return Find( key ) != nullptr;
}

// ******************************
//
bool TextureCache::Exists( const TextureResourceDescConstPtr & textureDesc ) const
{
	return Exists( GenKeyForTextureResource( textureDesc ) );
}

// ******************************
//
TextureResourceConstPtr	TextureCache::Get( const std::string & key ) const
{
	return Find( key );
}

// ******************************
//
TextureResourceConstPtr	TextureCache::Get( const TextureResourceDescConstPtr & textureDesc ) const
{
	return Find( GenKeyForTextureResource( textureDesc ) );
}

// ******************************
//
TextureCache & TextureCache::GetInstance()
{
	static auto innstance = TextureCache();
	return innstance;
}

// ******************************
//
std::string TextureCache::GenKeyForSingleTexture( const SingleTextureResourceDescConstPtr & sTRDesc )
{
	return toString( sTRDesc->GetImagePath() ) + toString( sTRDesc->GetWidth() ) + toString( sTRDesc->GetHeight() ) + toString( (int)sTRDesc->GetFormat() );
}

// ******************************
//
std::string TextureCache::GenKeyForTextureResource( const TextureResourceDescConstPtr & tRDesc )
{
	switch( tRDesc->GetLoadingType() )
	{
		case TextureResourceLoadingType::LOAD_ONLY_ORIGINAL_TEXTURE:
			return GenKeyForSingleTexture( tRDesc->GetOrigTextureDesc() );
		case TextureResourceLoadingType::LOAD_ORIGINAL_TEXTURE_AND_GENERATE_MIP_MAPS:
			return GenKeyForSingleTexture( tRDesc->GetOrigTextureDesc() ) + toString( tRDesc->GetMipMapsDesc()->GetLevelsNum() ) + toString( (int)tRDesc->GetMipMapsDesc()->GetFilter() );
		case TextureResourceLoadingType::LOAD_ORIGINAL_TEXTURE_AND_MIP_MAPS:
		{
			auto ret = GenKeyForSingleTexture( tRDesc->GetOrigTextureDesc() );

			for( SizeType i = 0; tRDesc->GetMipMapsDesc()->GetLevelsNum(); ++i )
				ret += GenKeyForSingleTexture( tRDesc->GetMipMapsDesc()->GetLevelDesc( i ) );

			ret += toString( tRDesc->GetMipMapsDesc()->GetLevelsNum() ) + toString( (int)tRDesc->GetMipMapsDesc()->GetFilter() );

			return ret;
		}
		default:
			assert( !"Imposible enum value" );
			return "";
	}
}


// ******************************
//
std::string	TextureCache::GenKeyForGeneratedMipMap( const std::string & origPath, SizeType width, SizeType height, TextureFormat format, SizeType mmLevel, MipMapFilterType mmFiletType )
{
	return toString( origPath ) + toString( width ) + toString( height ) + toString( (int)format ) + toString( mmLevel ) + toString( (int)mmFiletType );
}

// ******************************
//
TextureResourceConstPtr	TextureCache::Find( const std::string & key ) const
{
	auto it = m_textures.find( key );
	if( it != m_textures.end() )
		return it->second;
	else
		return nullptr;
}

// ******************************
//
TextureCache::TextureCache()
{}

} //bv
