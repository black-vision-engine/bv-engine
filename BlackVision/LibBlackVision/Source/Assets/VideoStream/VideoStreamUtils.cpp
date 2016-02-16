#include "stdafx.h"
#include "VideoStreamUtils.h"

#include "Serialization/Json/JsonDeserializeObject.h"
#include "Serialization/Json/JsonSerializeObject.h"
#include "ProjectManager.h"

#include "DataTypes/Hash.h"
#include "LibImage.h"
#include "Assets/Thumbnail/Impl/VideoStreamAssetThumbnail.h"

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegVideoDecoder.h"
#include "Assets/Texture/TextureUtils.h"        // ToBPP function


namespace bv
{

ThumbnailConstPtr            VideoStreamUtils::LoadThumbnail                   ( const VideoStreamAssetDescConstPtr & desc )
{
    auto absTexPath = ProjectManager::GetInstance()->ToAbsPath( desc->GetStreamPath() );
    auto thumbPath = Path( absTexPath.Str() + ".bvthumb" );
    auto h = Hash::FromFile( absTexPath.Str() );


    if( Path::Exists( thumbPath ) )
    {
        JsonDeserializeObject deser;
        deser.LoadFile( thumbPath.Str() );

        auto thumb = VideoStreamAssetThumbnail::Create( deser );

        if( h == thumb->GetHash() )
        {
            return thumb;
        }

    }

    auto frameTex = LoadSingleFrame( desc, 3.0f );

    auto resized = image::Resize( frameTex->GetData(), frameTex->GetWidth(), frameTex->GetHeight(), TextureUtils::ToBPP( frameTex->GetFormat() ), 128, 128, image::FilterType::FT_LANCZOS );
    auto compresed = image::SaveTGAToHandle( resized, 128, 128, 32 );
    auto thumb = VideoStreamAssetThumbnail::Create( compresed, h );

    JsonSerializeObject ser;

    thumb->Serialize( ser );

    ser.Save( thumbPath.Str() );

    return thumb;
}

// ***********************
//
SingleTextureAssetConstPtr  VideoStreamUtils::LoadSingleFrame     ( const VideoStreamAssetDescConstPtr & desc, TimeType frameTime )
{
    auto decoder = std::make_shared< FFmpegVideoDecoder >( desc );

    auto frameChunk = decoder->GetSingleFrame( frameTime );
    return SingleTextureAsset::Create( frameChunk.frameData, desc->GetKey(), desc->GetWidth(), desc->GetHeight(), desc->GetTextureFormat() );
}


}	// bv