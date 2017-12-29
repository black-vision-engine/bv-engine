#include "stdafx.h"

#include "Font2DLoader.h"
#include "Assets/Font/2D/FontAssetDescriptor.h"
#include "Assets/Font/2D/FontAsset2D.h"


#include "Assets/Thumbnail/Impl/FontAssetThumbnail.h"
#include "Assets/Texture/TextureUtils.h"
#include "Serialization/Json/JsonDeserializeObject.h"
#include "Serialization/Json/JsonSerializeObject.h"
#include "DataTypes/Hash.h"
#include "System/Path.h"
#include "IO/FileIO.h"
#include "ProjectManager.h"
#include "LibImage.h"




namespace bv {



// ***********************
//
AssetConstPtr       Font2DLoader::LoadAsset( const bv::AssetDescConstPtr & desc ) const
{
    auto typedDesc = QueryTypedDesc< FontAssetDescConstPtr >( desc );

    assert( typedDesc );

    auto filePath = ProjectManager::GetInstance()->ToAbsPath( typedDesc->GetFontFileName() ).Str();
    auto atlasCharSetFile = typedDesc->GetAtlasCharSetFile();
    auto fontSize = typedDesc->GetFontSize();
    auto blurSize = typedDesc->GetBlurSize();
    auto glowBlurSize = typedDesc->GetGlowBlurSize();
    auto outlineSize = typedDesc->GetOutlineSize();
	auto generateMipMaps = typedDesc->GetGenerateMipmaps();

    auto text = TryLoadFont( filePath, fontSize, blurSize, glowBlurSize, outlineSize, generateMipMaps, atlasCharSetFile );

    if( text )
    {
        return FontAsset2D::Create( text );
    }
    else
    {
        return nullptr;
    }
}


///////////////////////////////
//
AssetDescConstPtr Font2DLoader::CreateDescriptor	( const IDeserializer& deserializeObject ) const
{
    return std::static_pointer_cast<const AssetDesc>( FontAssetDesc::Create( deserializeObject ) );
}

///////////////////////////////
//
ThumbnailConstPtr Font2DLoader::LoadThumbnail     ( const AssetDescConstPtr & desc ) const
{
    auto typedDesc = QueryTypedDesc< FontAssetDescConstPtr >( desc );

    assert( typedDesc );

    auto filePath = ProjectManager::GetInstance()->ToAbsPath( typedDesc->GetFontFileName() );

    auto thumbFilePath = AssetAccessor::GetThumbnailPath( filePath );

    auto charSetFilePath = ProjectManager::GetInstance()->ToAbsPath( "fonts/ThumbnailCharSet.txt" ).Str();

    auto hChSet = Hash::FromFile( charSetFilePath );
    auto hData = Hash::FromFile( filePath.Str() );

    auto h = Hash::FromString( hChSet.Get() + hData.Get() );

    if( Path::Exists( thumbFilePath ) )
    {
        SimpleJsonDeserializeObject deser;
        deser.LoadFile( thumbFilePath.Str() );

        auto thumb = FontAssetThumbnail::Create( deser );

        if( thumb->GetHash() == h )
        {
            return thumb;
        }
    }

    auto text = TryLoadFont( filePath.Str(), 10, 0, 0, 0, false, L"fonts/ThumbnailCharSet.txt" );

    auto atlasTexture = text->BuildAtlas()->GetAsset()->GetOriginal();

    auto swaped = image::SwapChannels( atlasTexture->GetData(), 32, 0xff000000, 0xff000000, 0xff000000, 0xff000000 );
    swaped = image::FlipVertical( swaped, atlasTexture->GetWidth(), atlasTexture->GetHeight(), TextureUtils::ToBPP( atlasTexture->GetFormat() ) );

    auto tga = image::SaveTGAToHandle( swaped, atlasTexture->GetWidth(), atlasTexture->GetHeight(), TextureUtils::ToBPP( atlasTexture->GetFormat() ) );

    auto thumb = FontAssetThumbnail::Create( tga, h );

    SimpleJsonSerializeObject ser;

    thumb->Serialize( ser );

    ser.Save( thumbFilePath.Str() );

    return thumb;
}

} // bv



