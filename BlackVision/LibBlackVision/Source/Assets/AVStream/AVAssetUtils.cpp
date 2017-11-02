#include "stdafx.h"
#include "AVAssetUtils.h"

#include "Serialization/Json/JsonDeserializeObject.h"
#include "Serialization/Json/JsonSerializeObject.h"
#include "ProjectManager.h"

#include "DataTypes/Hash.h"
#include "LibImage.h"
#include "Assets/Thumbnail/Impl/VideoStreamAssetThumbnail.h"

#include "Engine/Models/Plugins/Simple/AVStreamDecoder/FFmpeg/FFmpegAVDecoder.h"
#include "Assets/Texture/TextureUtils.h"        // ToBPP function




#include "Memory/MemoryLeaks.h"



namespace bv
{

ThumbnailConstPtr            AVAssetUtils::LoadThumbnail                   ( const AVAssetDescConstPtr & desc )
{
    auto absTexPath = ProjectManager::GetInstance()->ToAbsPath( desc->GetStreamPath() );
    auto thumbPath = AssetAccessor::GetThumbnailPath( absTexPath );
    auto h = Hash::FromString( absTexPath.Str() + std::to_string( Path::GetTimestamp( absTexPath ) ) );


    if( Path::Exists( thumbPath ) )
    {
        SimpleJsonDeserializeObject deser;
        deser.LoadFile( thumbPath.Str() );

        auto thumb = VideoStreamAssetThumbnail::Create( deser );

        if( h == thumb->GetHash() )
        {
            return thumb;
        }

    }

    auto frameTex = LoadSingleFrame( desc, 0.3f );
    if( frameTex == nullptr )
        return nullptr;

    auto resized = image::MakeThumbnai( frameTex->GetData(), frameTex->GetWidth(), frameTex->GetHeight(), TextureUtils::ToBPP( frameTex->GetFormat() ), 128 );
    auto flipped = image::FlipVertical( resized, 128, 128, 32 );
    auto compresed = image::SaveTGAToHandle( flipped, 128, 128, 32 );

    /*assert( image::SaveBMPImage( absTexPath.Str() + ".bmp", flipped, 128, 128, 32 ) );*/

    auto thumb = VideoStreamAssetThumbnail::Create( compresed, h );

    SimpleJsonSerializeObject ser;

    thumb->Serialize( ser );

    ser.Save( thumbPath.Str() );

    return thumb;
}

// ***********************
//
SingleTextureAssetConstPtr  AVAssetUtils::LoadSingleFrame     ( const AVAssetDescConstPtr & desc, TimeType frameTime )
{
    auto videoAsset = LoadTypedAsset< AVAsset >( desc );

    if( videoAsset != nullptr )
    {
        auto decoder = std::make_shared< FFmpegAVDecoder >( videoAsset );

        auto frameChunk = decoder->GetSingleFrame( frameTime );
    
        if( frameChunk.frameData != nullptr )
            return SingleTextureAsset::Create( frameChunk.frameData, desc->GetKey(), decoder->GetWidth(), decoder->GetHeight(), desc->GetTextureFormat() );
    }

    return nullptr;
}


}	// bv