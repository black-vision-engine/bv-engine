#include "AnimationLoader.h"

#include "Assets/Assets.h"
#include "Assets/Thumbnail/Impl/AnimationAssetThumbnail.h"
#include "Assets/Texture/TextureUtils.h"
#include "Serialization/Json/JsonDeserializeObject.h"
#include "ProjectManager.h"
#include "IO/DirIO.h"

#include <cassert>

namespace bv {

// *******************************
//
TextureAssetConstPtr	AnimationLoader::LoadFrame( const TextureAssetDescConstPtr & frameDesc ) const
{
    auto asset = AssetManager::GetInstance().LoadAsset( frameDesc );

	auto texRes = QueryTypedRes< TextureAssetConstPtr >( asset );

    if ( texRes == nullptr )
    {
        return nullptr;
    }

    return texRes;
}


// ******************************
//
AssetConstPtr AnimationLoader::LoadAsset( const AssetDescConstPtr & desc ) const
{
	auto typedDesc = QueryTypedDesc< AnimationAssetDescConstPtr >( desc );

	assert( typedDesc );

    auto p = ProjectManager::GetInstance()->ToAbsPath( typedDesc->GetPath() );
    auto files = bv::Path::List( p.Str(), false, typedDesc->GetFilter() );

    TextureAssetDescVec framesDesc;

    if ( files.size() > 0 )
    {
	    for( auto f : files )
	    {
            framesDesc.push_back( TextureAssetDesc::Create( "file:/" + f.Str(), true ) );
        }
    }

	if ( framesDesc.size() == 0 )
    {
        return nullptr;
    }

	std::vector< TextureAssetConstPtr > framesAssets;
	
    printf( "Loading animation\n" );

	auto i = 0;
	for( auto f : framesDesc )
	{
		framesAssets.push_back( LoadFrame( f ) );
		printf( "\rLoaded %d out of %d total frames                ", i + 1, framesDesc.size() );
		++i;
	}

	printf( "\n" );

	return AnimationAsset::Create( framesAssets );
}

// ******************************
//
AssetDescConstPtr	AnimationLoader::CreateDescriptor	( const IDeserializer& deserializeObject ) const
{
	return std::static_pointer_cast<const AssetDesc>( AnimationAssetDesc::Create( deserializeObject ) );
}

///////////////////////////////
//
ThumbnailConstPtr   AnimationLoader::LoadThumbnail      ( const AssetDescConstPtr & desc ) const
{
    auto typedDesc = QueryTypedDesc< AnimationAssetDescConstPtr >( desc );

	assert( typedDesc );
    
    auto p = ProjectManager::GetInstance()->ToAbsPath( typedDesc->GetPath() );

    auto thumbFileName = p.Str() + ".bvthumb";

    if( Path::Exists( thumbFileName ) )
    {
        JsonDeserializeObject deser;
        deser.LoadFile( thumbFileName );

        return AnimationAssetThumbnail::Create( deser );
    }

    auto files = bv::Path::List( p.Str(), false, typedDesc->GetFilter() );

    TextureAssetDescVec framesDesc;

    if ( files.size() > 0 )
    {
        auto s = files.size();

        for( SizeType i = 0; i < s; i += s / 3 )
	    {
            framesDesc.push_back( TextureAssetDesc::Create( "file:/" + files[ i ].Str(), true ) );
        }
    }

	if ( framesDesc.size() == 0 )
    {
        return nullptr;
    }

    std::vector< SingleTextureAssetConstPtr > framesAssets;

	for( auto f : framesDesc )
	{
        auto t = TextureUtils::LoadSingleTexture( f->GetOrigTextureDesc(), false );
		framesAssets.push_back( t );
	}

    MemoryChunkVector mcVec;

    SizeType w = 0;
    SizeType h = 0;
    UInt32 bpp = 0;

    for( auto a : framesAssets )
    {
        mcVec.push_back( a->GetData() );

        if( w && h && bpp )
        {
            w = a->GetWidth();
            h = a->GetWidth();
            bpp = TextureUtils::ToBPP( a->GetFormat() );
        }
    }

    auto thumb =  AnimationAssetThumbnail::Create( mcVec, w, h, bpp, Hash( "" ) );

    JsonSerializeObject ser;
    thumb->Serialize( ser );
    ser.Save( thumbFileName );

    return thumb;
}


} // bv
