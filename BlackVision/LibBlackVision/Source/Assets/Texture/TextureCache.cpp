#include "TextureCache.h"

#include "Assets/Cache/RawDataCache.h"

#include <cassert>

namespace bv
{

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
	return Add( textureDesc->GetKey(), textureRes );
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
	{
		for( SizeType i = 0; i < mm->GetLevelsNum(); ++i )
		{
			RawDataCache::GetInstance().Add( Hash::FromString( mm->GetLevel( i )->GetKey()), mm->GetLevel( i )->GetData(),  mm->GetLevel( i )->GetCacheOnHardDrive() );
		}
	}
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
	return Exists( textureDesc->GetKey() );
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
	auto ta = Find( textureDesc->GetKey() );

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
	auto key = desc->GetKey();
	auto mChunk = RawDataCache::GetInstance().Get( Hash::FromString( key ) );

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

	if( !origAsset )
	{
		return nullptr;
	}

	auto mmDesc = desc->GetMipMapsDesc();

	MipMapAssetConstPtr mmAsset = nullptr;

	if( mmDesc )
	{
		std::vector< SingleTextureAssetConstPtr > mms;
		for( SizeType i = 0; i < mmDesc->GetLevelsNum(); ++i )
		{
			auto singleTextAssetDesc = mmDesc->GetLevelDesc( i );
			auto mmSibleTextureAssetAsset = GetFromRawDataCache( singleTextAssetDesc );

			if( !mmSibleTextureAssetAsset )
			{
				return nullptr;
			}

			mms.push_back( mmSibleTextureAssetAsset );
		}

		mmAsset = MipMapAsset::Create( mms );
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
