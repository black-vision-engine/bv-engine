#include "AnimationLoader.h"

#include "Assets/Assets.h"
#include "Assets/Thumbnail/Impl/AnimationAssetThumbnail.h"
#include "Assets/Texture/TextureUtils.h"
#include "Serialization/Json/JsonDeserializeObject.h"
#include "ProjectManager.h"
#include "LibImage.h"
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

namespace 
{

///////////////////////////////
//
TextureAssetDescVec ListSingleDescriptors( const AnimationAssetDescConstPtr & typedDesc, SizeType inc )
{
    auto p = ProjectManager::GetInstance()->ToAbsPath( typedDesc->GetPath() );

    auto files = bv::Path::List( p.Str(), false, typedDesc->GetFilter() );

    TextureAssetDescVec framesDesc;

    auto div = files.size();

    if( inc > 0 )
    {
        div = inc;
    }

    if ( files.size() > 0 )
    {
        auto s = files.size();

        for( SizeType i = 0; i < s; i += s / div )
	    {
            framesDesc.push_back( TextureAssetDesc::Create( "file:/" + files[ i ].Str(), true ) );
        }
    }

    return framesDesc;
}

///////////////////////////////
//
std::vector< SingleTextureAssetConstPtr > LoadFrames( const AnimationAssetDescConstPtr & typedDesc, SizeType inc = 0 )
{
    auto framesDesc = ListSingleDescriptors( typedDesc, inc );

    std::vector< SingleTextureAssetConstPtr > framesAssets;

	for( auto f : framesDesc )
	{
        auto t = TextureUtils::LoadSingleTexture( f->GetOrigTextureDesc(), false );
		framesAssets.push_back( t );
	}

    return framesAssets;
}

///////////////////////////////
//
Hash CalcHash( const AnimationAssetDescConstPtr & typedDesc, SizeType inc = 0 )
{
     auto framesDesc = ListSingleDescriptors( typedDesc, inc );

     std::string hashes = "";

     for( auto f : framesDesc )
     {
         hashes = hashes + Hash::FromFile( ProjectManager::GetInstance()->ToAbsPath( f->GetOrigTextureDesc()->GetImagePath() ).Str() ).Get();
     }

     return Hash::FromString( hashes );
}

} // anonymous

///////////////////////////////
//
ThumbnailConstPtr   AnimationLoader::LoadThumbnail      ( const AssetDescConstPtr & desc ) const
{
    auto typedDesc = QueryTypedDesc< AnimationAssetDescConstPtr >( desc );

	assert( typedDesc );
    
    auto p = ProjectManager::GetInstance()->ToAbsPath( typedDesc->GetPath() );

    auto thumbFileName = p.Str() + ".bvthumb";

    auto h = CalcHash( typedDesc, 3 );

    if( Path::Exists( thumbFileName ) )
    {
        JsonDeserializeObject deser;
        deser.LoadFile( thumbFileName );

        auto thumb = AnimationAssetThumbnail::Create( deser );

        File::Write( thumb->m_data[0]->Get(), thumb->m_data[0]->Size(), "dump.tga", false );

        if( h == thumb->GetHash() )
        {
            return thumb;
        }
    }

    auto framesAssets = LoadFrames( typedDesc, 3 );

    MemoryChunkVector mcVec;

    for( auto a : framesAssets )
    {
        auto resized = image::Resize( a->GetData(), a->GetWidth(), a->GetHeight(), TextureUtils::ToBPP( a->GetFormat() ), 128, 128, image::FilterType::FT_LANCZOS );

        auto compresed = image::SaveTGAToHandle( resized, 128, 128, 32 );

        mcVec.push_back( compresed );
    }

    auto thumb =  AnimationAssetThumbnail::Create( mcVec, h );

    JsonSerializeObject ser;
    thumb->Serialize( ser );
    ser.Save( thumbFileName );

    return thumb;
}


} // bv
