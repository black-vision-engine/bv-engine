#include "TextureCache.h"

#include <sstream>

namespace bv
{

namespace 
{
	// FIXME: Should be in some core module.
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
TextureResourceConstPtr	TextureCache::Get( const std::string & key ) const
{
	return Find( key );
}

// ******************************
//
TextureCache & TextureCache::GetInstance()
{
	static auto * innstance = new TextureCache();
	return *innstance;
}

// ******************************
//
std::string TextureCache::GenKeyForSingleTexture( const std::string & path, UInt32 width, UInt32 height, TextureFormat format )
{
	return toString( path ) + toString( width ) + toString( height ) + toString( (int)format );
}

// ******************************
//
std::string	TextureCache::GenKeyForGeneratedMipMap( const std::string & origPath, UInt32 width, UInt32 height, TextureFormat format, UInt32 mmLevel, MipMapFilterType mmFiletType )
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
