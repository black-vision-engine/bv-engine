#include "TextureCache.h"

#include "Assets/Cache/RawDataCache.h"

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
bool TextureCache::Add( const std::string & key, const TextureAssetConstPtr & textureRes )
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
bool TextureCache::Add( const TextureAssetDescConstPtr & textureDesc, const TextureAssetConstPtr & textureRes )
{
	return Add( GenKeyForTextureAsset( textureDesc ), textureRes );
}


// ******************************
//
void TextureCache::Update( const std::string & key, const TextureAssetConstPtr & textureRes )
{
	m_textures[ key ] = textureRes;
	AddToRawDataCache( textureRes );
}

// ******************************
//
void TextureCache::AddToRawDataCache( const TextureAssetConstPtr & textureRes ) const
{
	auto orig = textureRes->GetOriginal();
	RawDataCache::GetInstance().Add( Hash::FromString( orig->GetKey()), orig->GetData(), orig->GetCacheOnHardDrive() );

	auto mm = textureRes->GetMipMaps();

	if( mm )
		for( SizeType i = 0; i < mm->GetLevelsNum(); ++i )
			RawDataCache::GetInstance().Add( Hash::FromString( mm->GetLevel( i )->GetKey()), mm->GetLevel( i )->GetData(), orig->GetCacheOnHardDrive() );
}

// ******************************
//
bool TextureCache::Exists	( const std::string & key ) const
{
	return Find( key ) != nullptr;
}

// ******************************
//
bool TextureCache::Exists( const TextureAssetDescConstPtr & textureDesc ) const
{
	return Exists( GenKeyForTextureAsset( textureDesc ) );
}

// ******************************
//
TextureAssetConstPtr	TextureCache::Get( const std::string & key ) const
{
	return Find( key );
}

// ******************************
//
TextureAssetConstPtr	TextureCache::Get( const TextureAssetDescConstPtr & textureDesc ) const
{
	auto ta = Find( GenKeyForTextureAsset( textureDesc ) );

	if( ta )
	{
		return ta;
	}
	else
	{
		return GetFromRawDataCache( textureDesc );
	}
}

// ******************************
//
SingleTextureAssetConstPtr	TextureCache::GetFromRawDataCache	( const SingleTextureAssetDescConstPtr & desc ) const
{
	auto key = GenKeyForSingleTexture( desc );
	auto mChunk = RawDataCache::GetInstance().Get( Hash::FromString( GenKeyForSingleTexture( desc ) ) );

	if( mChunk )
	{
		return SingleTextureAsset::Create( mChunk, key, desc->GetWidth(), desc->GetHeight(), desc->GetFormat(), true );
	}
	else
	{
		return nullptr;
	}
}

// ******************************
//
TextureAssetConstPtr	TextureCache::GetFromRawDataCache	( const TextureAssetDescConstPtr & desc ) const
{
	auto origAsset = GetFromRawDataCache( desc->GetOrigTextureDesc() );

	auto mmDesc = desc->GetMipMapsDesc();

	MipMapAssetConstPtr mmAsset = nullptr;

	if( mmDesc )
	{
		std::vector< SingleTextureAssetConstPtr > mms;
		for( SizeType i = 0; mmDesc->GetLevelsNum(); ++i )
		{
			auto singleTextAssetDesc = mmDesc->GetLevelDesc( i );
			auto key = GenKeyForSingleTexture( singleTextAssetDesc );
			auto mmAsset = GetFromRawDataCache( singleTextAssetDesc );
			mms.push_back( mmAsset ); //
		}
	}

	return TextureAsset::Create( origAsset, mmAsset );
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
std::string TextureCache::GenKeyForSingleTexture( const std::string & origPath, SizeType width, SizeType height, TextureFormat format )
{
	return toString( origPath ) + toString( width ) + toString( height ) + toString( (int)format );
}

// ******************************
//
std::string TextureCache::GenKeyForSingleTexture( const SingleTextureAssetDescConstPtr & sTRDesc )
{
	return GenKeyForSingleTexture( sTRDesc->GetImagePath(), sTRDesc->GetWidth(), sTRDesc->GetHeight(), sTRDesc->GetFormat() );
}

// ******************************
//
std::string TextureCache::GenKeyForTextureAsset( const TextureAssetDescConstPtr & tRDesc )
{
	switch( tRDesc->GetLoadingType() )
	{
		case TextureAssetLoadingType::LOAD_ONLY_ORIGINAL_TEXTURE:
			return GenKeyForSingleTexture( tRDesc->GetOrigTextureDesc() );
		case TextureAssetLoadingType::LOAD_ORIGINAL_TEXTURE_AND_GENERATE_MIP_MAPS:
			return GenKeyForSingleTexture( tRDesc->GetOrigTextureDesc() ) + toString( tRDesc->GetMipMapsDesc()->GetLevelsNum() ) + toString( (int)tRDesc->GetMipMapsDesc()->GetFilter() );
		case TextureAssetLoadingType::LOAD_ORIGINAL_TEXTURE_AND_MIP_MAPS:
		{
			auto ret = GenKeyForSingleTexture( tRDesc->GetOrigTextureDesc() );

			for( SizeType i = 0; i < tRDesc->GetMipMapsDesc()->GetLevelsNum(); ++i )
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
TextureAssetConstPtr	TextureCache::Find( const std::string & key ) const
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
