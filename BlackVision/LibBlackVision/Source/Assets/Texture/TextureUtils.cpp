#include "stdafx.h"

#include "TextureUtils.h"

#include "MipMapBuilder.h"
#include "Assets/Cache/RawDataCache.h"
#include "ProjectManager.h"
#include "Assets/Thumbnail/Impl/TextureAssetThumbnail.h"
#include "Serialization/Json/JsonSerializeObject.h"
#include "Serialization/Json/JsonDeserializeObject.h"
#include "DataTypes/Hash.h"

#include <cassert>



#include "Memory/MemoryLeaks.h"



namespace bv {

// ******************************
// Anonymous namespace begin
namespace
{

// ******************************
//
image::FilterType ToMMBuilderFilterType( MipMapFilterType ft )
{
    switch( ft )
    {
    case MipMapFilterType::BOX:
        return image::FilterType::FT_BOX;
    case MipMapFilterType::BILINEAR:
        return image::FilterType::FT_BILINEAR;
    case MipMapFilterType::B_SPLINE:
        return image::FilterType::FT_B_SPLINE;
    case MipMapFilterType::BICUBIC:
        return image::FilterType::FT_BICUBIC;
    case MipMapFilterType::CATMULL_ROM:
        return image::FilterType::FT_CATMULL_ROM;
    case MipMapFilterType::LANCZOS:
        return image::FilterType::FT_LANCZOS;
    default:
        assert( !"Impossible enum value" );
        return image::FilterType::FT_BOX;
    }
}

} // anounymous


// ******************************
//
TextureAssetConstPtr    TextureUtils::GenerateMipMaps ( SingleTextureAssetConstPtr texture, Int32 numLevels, MipMapFilterType ft )
{
    if( numLevels > 0 )
    {
        tools::Image img = { texture->GetData(), texture->GetWidth(), texture->GetHeight(), ToBPP( texture->GetFormat() ) };
        auto mipmap = tools::GenerateMipmaps( img, numLevels, ToMMBuilderFilterType( ft ) );

        std::vector< SingleTextureAssetConstPtr > mipMapsRes;
        for( SizeType i = 0; i < mipmap.size(); ++i )
        {       
            assert( ToBPP( texture->GetFormat() ) == mipmap[ i ].bpp );  // Mipmaps should have same format.
            mipMapsRes.push_back( SingleTextureAsset::Create( mipmap[ i ].data, "", mipmap[ i ].width, mipmap[ i ].height, texture->GetFormat(), true ) );
        }

        auto mipmaps = MipMapAsset::Create( mipMapsRes );

        return TextureAsset::Create( texture, mipmaps );
    }
    else
    {
        return TextureAsset::Create( texture, nullptr );
    }
}

// ******************************
//
TextureFormat           TextureUtils::ToTextureFormat( UInt32 bpp, UInt32 channelNum )
{
    switch( bpp )
    {
    case 128:
        if( channelNum == 4 )
            return TextureFormat::F_A32FR32FG32FB32F;
    case 32:
        if( channelNum == 4 )
            return TextureFormat::F_A8R8G8B8;
        else if( channelNum == 1 )
            return TextureFormat::F_A32F;

    case 8:
        if( channelNum == 1 )
            return TextureFormat::F_A8;
    }

    assert( !"Should never be here" );
    return TextureFormat::F_TOTAL;
}


// ******************************
//
TextureFormat           TextureUtils::NearestSupportedTextureFormat ( TextureFormat format )
{
    switch( format )
    {
    case TextureFormat::F_R32FG32FB32F:
        return TextureFormat::F_A32FR32FG32FB32F;
    case TextureFormat::F_R8G8B8:
        return TextureFormat::F_A8R8G8B8;
    default:
        return format;
    }
}

// ******************************
//
UInt32                  TextureUtils::ToBPP                         ( TextureFormat format )
{
    switch( format )
    {
    case TextureFormat::F_A32FR32FG32FB32F:
        return 128;
    case TextureFormat::F_R32FG32FB32F:
        return 96;
    case TextureFormat::F_A32F:
        return 32;
    case TextureFormat::F_A8R8G8B8:
        return 32;
    case TextureFormat::F_R8G8B8:
        return 24;
    case TextureFormat::F_A8:
        return 8;
    case TextureFormat::F_L8:
        return 8;
    default:
        assert( !"Should never be here !" );
        return 0;
    }
}

// ******************************
//
UInt32                  TextureUtils::Channels                      ( TextureFormat format )
{
    switch( format )
    {
    case TextureFormat::F_A32FR32FG32FB32F:
    case TextureFormat::F_A8R8G8B8:
        return 4;
    case TextureFormat::F_R32FG32FB32F:
    case TextureFormat::F_R8G8B8:
        return 3;
    case TextureFormat::F_A32F:
    case TextureFormat::F_A8:
    case TextureFormat::F_L8:
        return 1;
    default:
        assert( !"Should never be here !" );
        return 0;
    }
}

// ******************************
//
SingleTextureAssetConstPtr  TextureUtils::LoadSingleTexture( const SingleTextureAssetDescConstPtr & singleTextureResDesc, bool cacheOnDisk )
{
    auto key        = singleTextureResDesc->GetKey();

    auto isCacheable = singleTextureResDesc->IsCacheable();

    MemoryChunkConstPtr mmChunk = isCacheable ? RawDataCache::GetInstance().Get( Hash::FromString( key ) ) : nullptr;
    
    if( mmChunk ) // if found in the cache
    {
        auto format     = TextureUtils::NearestSupportedTextureFormat( singleTextureResDesc->GetFormat() );
        return SingleTextureAsset::Create( mmChunk, key, singleTextureResDesc->GetWidth(), singleTextureResDesc->GetHeight(), format );
    }
    else
    {
        UInt32 w            = 0;
        UInt32 h            = 0;
        UInt32 bpp          = 0;
        UInt32 channelNum   = 0;

        auto imgPath        = ProjectManager::GetInstance()->ToAbsPath( singleTextureResDesc->GetImagePath() ).Str();

        mmChunk = LoadImage( imgPath, &w, &h, &bpp, &channelNum );
        
        if( !mmChunk )
        {
            return nullptr;
        }
        
        if( isCacheable )
        {
            if( RawDataCache::GetInstance().Add( Hash::FromString( key ), mmChunk, cacheOnDisk ) )
            {
                GetDefaultEventManager().TriggerEvent( std::make_shared< AssetTrackerInternalEvent >( AssetTrackerInternalEvent::Command::RegisterAsset, key ) );
            }
        }

        auto format     = TextureUtils::ToTextureFormat( bpp, channelNum );

        return SingleTextureAsset::Create( mmChunk, key, singleTextureResDesc->GetWidth(), singleTextureResDesc->GetHeight(), format, cacheOnDisk );
    }
}

// ******************************
//
MemoryChunkConstPtr TextureUtils::LoadImage( const std::string & path, UInt32 * width, UInt32 * height, UInt32 * bpp, UInt32 * channelNum )
{
    if( path.find( ".raw" ) != std::string::npos )
    {
        return image::LoadRAWImage( path );
    }
    else
    {
        return image::LoadImage( path, width, height, bpp, channelNum );
    }
}

// ******************************
//
TextureAssetConstPtr TextureUtils::LoadOrginalTextureOnly           ( const TextureAssetDescConstPtr & desc )
{
    assert( desc->GetLoadingType() == TextureAssetLoadingType::LOAD_ONLY_ORIGINAL_TEXTURE );

    auto origRes = LoadSingleTexture( desc->GetOrigTextureDesc(), false );
    if( origRes != nullptr )
        return TextureAsset::Create( origRes, nullptr );
    else
        return nullptr;
}

// ******************************
//
TextureAssetConstPtr TextureUtils::LoadTextureAndMipMaps            ( const TextureAssetDescConstPtr & desc )
{
    assert( desc->GetLoadingType() == TextureAssetLoadingType::LOAD_ORIGINAL_TEXTURE_AND_MIP_MAPS );

    auto origRes = LoadSingleTexture( desc->GetOrigTextureDesc(), false );
    if( origRes == nullptr )
        return nullptr;

    auto mipMapsSize = desc->GetMipMapsDesc()->GetLevelsNum();

    std::vector< SingleTextureAssetConstPtr > mipMapsRes;

    for( SizeType i = 0; i < mipMapsSize; ++i )
    {
        auto levelDesc = desc->GetMipMapsDesc()->GetLevelDesc( i );
        auto newMipMap = LoadSingleTexture( levelDesc, false );
        if( newMipMap != nullptr )
            mipMapsRes.push_back( newMipMap );
        else
            return nullptr;
    }

    auto mipMapRes = MipMapAsset::Create( mipMapsRes );

    return TextureAsset::Create( origRes, mipMapRes );


}

// ******************************
//
TextureAssetConstPtr TextureUtils::LoadTextureAndGenerateMipMaps    ( const TextureAssetDescConstPtr & desc )
{
    assert( desc->GetLoadingType() == TextureAssetLoadingType::LOAD_ORIGINAL_TEXTURE_AND_GENERATE_MIP_MAPS );

    if( desc->IsCacheable() )  // check if already in cache
    {
        auto cachedTextureAsset = GetFromRawDataCache( desc );

        if( cachedTextureAsset )
        {
            return cachedTextureAsset;
        }
    }

    SingleTextureAssetConstPtr origRes = LoadSingleTexture( desc->GetOrigTextureDesc(), false );
    if( origRes == nullptr )
        return nullptr;

    std::vector< SingleTextureAssetConstPtr > mipMapsRes;

    if( desc->IsCacheable() )  // check if already in cache
    {
        auto mmDesc = desc->GetMipMapsDesc();

        if( mmDesc )
        {
            mipMapsRes.resize( mmDesc->GetLevelsNum() );

            for( SizeType i = 0; i < mmDesc->GetLevelsNum(); ++i )
            {
                auto cachedMMAsset = LoadSingleTexture( mmDesc->GetLevelDesc( i ), true );
                if( cachedMMAsset )
                {
                    mipMapsRes[ i ] = cachedMMAsset;
                }
                else
                {
                    TextureAssetConstPtr textureWithMipMap = nullptr;
                    SingleTextureAssetConstPtr mm = nullptr;

                    if( i == 0 )
                    {
                        textureWithMipMap = TextureUtils::GenerateMipMaps( origRes, 1, mmDesc->GetFilter() );
                        mm = textureWithMipMap->GetMipMaps()->GetLevel( 0 );
                    }
                    else
                    {
                        textureWithMipMap = TextureUtils::GenerateMipMaps( mipMapsRes[ i - 1 ], 2, mmDesc->GetFilter() );
                        mm = textureWithMipMap->GetMipMaps()->GetLevel( 1 );
                    }
                    //auto data = mipMapsRes[ i - 1 ]->GetData();
                    //auto w        = mipMapsRes[ i - 1 ]->GetWidth();
                    //auto h        = mipMapsRes[ i - 1 ]->GetHeight();
                    //auto bpp  = TextureLoader::ToBPP( mipMapsRes[ i - 1 ]->GetFormat() );

                    //tools::Image img = { data, w, h, bpp };
                    //auto mm = tools::GenerateMipmaps( img,
                    //                                  2,
                    //                                  ToMMBuilderFilterType( desc->GetMipMapsDesc()->GetFilter() ) );

                    auto key = mmDesc->GetLevelDesc( i )->GetKey();

                    mipMapsRes[ i ] = SingleTextureAsset::Create( mm->GetData(), key,  mm->GetWidth(), mm->GetHeight(),
                                                                  mm->GetFormat(), mm->GetCacheOnHardDrive() );
                }
            }
        }
    }

    auto textureAsset = TextureAsset::Create( origRes, MipMapAsset::Create( mipMapsRes ) );

    if( desc->IsCacheable() )  // Cache the texture asset
    {
        AddToRawDataCache( textureAsset );
    }

    return textureAsset;
}

// ******************************
//
SingleTextureAssetConstPtr  TextureUtils::GetFromRawDataCache   ( const SingleTextureAssetDescConstPtr & desc )
{
    auto key = desc->GetKey();
    auto mChunk = RawDataCache::GetInstance().Get( Hash::FromString( key ) );

    if( mChunk )
    {
        return SingleTextureAsset::Create( mChunk, key, desc->GetWidth(), desc->GetHeight(), TextureUtils::NearestSupportedTextureFormat( desc->GetFormat() ), true );
    }
    else
    {
        return nullptr;
    }
}

// ******************************
//
TextureAssetConstPtr TextureUtils::GetFromRawDataCache             ( const TextureAssetDescConstPtr & desc )
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
void TextureUtils::AddToRawDataCache( const TextureAssetConstPtr & textureRes )
{
    auto orig = textureRes->GetOriginal();
    
    auto key = orig->GetKey();
    if( RawDataCache::GetInstance().Add( Hash::FromString( key ), orig->GetData(), orig->GetCacheOnHardDrive() ) )
    {
        GetDefaultEventManager().TriggerEvent( std::make_shared< AssetTrackerInternalEvent >( AssetTrackerInternalEvent::Command::RegisterAsset, key ) );
    }
    
    auto mm = textureRes->GetMipMaps();

    if( mm )
    {
        for( SizeType i = 0; i < mm->GetLevelsNum(); ++i )
        {
            key = mm->GetLevel( i )->GetKey();
            if( RawDataCache::GetInstance().Add( Hash::FromString( key ), mm->GetLevel( i )->GetData(), mm->GetLevel( i )->GetCacheOnHardDrive() ) )
            {
                GetDefaultEventManager().TriggerEvent( std::make_shared< AssetTrackerInternalEvent >( AssetTrackerInternalEvent::Command::RegisterAsset, key ) );
            }
        }
    }
}

// ******************************
//
ThumbnailConstPtr TextureUtils::LoadThumbnail( const TextureAssetDescConstPtr & desc )
{
    auto texPath = desc->GetOrigTextureDesc()->GetImagePath();

    auto absTexPath = ProjectManager::GetInstance()->ToAbsPath( texPath );

    auto thumbPath = AssetAccessor::GetThumbnailPath( absTexPath );

    auto h = Hash::FromString( absTexPath.Str() + std::to_string( Path::GetTimestamp( absTexPath ) ) );

    if( Path::Exists( thumbPath ) )
    {
        JsonDeserializeObject deser;
        deser.LoadFile( thumbPath.Str() );

        auto thumb = TextureAssetThumbnail::Create( deser );

        if( h == thumb->GetHash() )
        {
            return thumb;
        }

    }

    auto t = LoadSingleTexture( desc->GetOrigTextureDesc(), false );

    auto resized = image::MakeThumbnai( t->GetData(), t->GetWidth(), t->GetHeight(), ToBPP( t->GetFormat() ), 128 );

    auto compresed = image::SaveTGAToHandle( resized, 128, 128, 32 );

    auto thumb = TextureAssetThumbnail::Create( compresed, h );

    JsonSerializeObject ser;

    thumb->Serialize( ser );

    ser.Save( thumbPath.Str() );

    return thumb;
}

}  // bv