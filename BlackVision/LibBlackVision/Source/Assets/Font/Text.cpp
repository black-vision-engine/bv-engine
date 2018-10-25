#include "stdafx.h"

#include "boost/filesystem/path.hpp"

#include "Text.h"
#include "2D/TextAtlas.h"

#include "AtlasCache.h"
#include "LibEffect.h"
#include "Assets/Font/Engines/FreeTypeEngine.h"
#include "Assets/Texture/TextureUtils.h"


namespace bv { 


// *********************************
//
TextConstPtr Text::Create(const std::wstring & supportedCharsSet
                        , const std::string & fontFile
                        , UInt32 fontSize
                        , UInt32 blurSize
                        , UInt32 glowBlurSize
                        , UInt32 outlineSize
                        , bool withMipmaps )
{
    return std::make_shared< Text >( supportedCharsSet, fontFile, fontSize, blurSize, glowBlurSize, outlineSize, withMipmaps );
}


// *********************************
//
Text::Text( const std::wstring & supportedCharsSetFile, const std::string& fontFile, UInt32 fontSize, UInt32 blurSize, UInt32 glowBlurSize, UInt32 outlineSize, bool withMipmaps )
    : m_supportedCharsSetFile( supportedCharsSetFile )
    , m_fontFile( fontFile )
    , m_fontSize( fontSize )
    , m_blurSize( blurSize )
    , m_glowBlurSize( glowBlurSize )
    , m_outlineWidth( outlineSize )
    , m_withMipmaps( withMipmaps )
{
    m_fontEngine = FreeTypeEngine::Create( fontFile, fontSize );
}

//#define GENERATE_TEST_BMP_FILE

// *********************************
//
TextAtlasPtr Text::LoadFromCache() const
{
    boost::filesystem::path fontPath( m_fontFile );
    auto fontName = fontPath.filename().string();

    auto entry = FontAtlasCache::GetInstance().GetEntry( fontName, m_fontSize, m_blurSize, m_glowBlurSize, m_outlineWidth, 
                                                         m_withMipmaps, m_supportedCharsSetFile );

    if( entry )
    {
        auto textAtlas = entry->GetTextAtlas();
        if( textAtlas )
        {
            auto atlasTextureDesc = TextAtlas::GenerateTextAtlasAssetDescriptor( entry->GetFontFilePath(), entry->GetAtlasWidth(),
                                                                                 entry->GetAtlasHeight(), entry->GetFontSize(), 
                                                                                 entry->GetBlurSize(), entry->GetGlowBlurSize(),
                                                                                 entry->GetOutlineWidth(), entry->GetLevelsNum(),
                                                                                 entry->GetCharSet() );

            auto asset = AssetManager::GetInstance().LoadAsset( atlasTextureDesc );
            if( asset )
            {
                textAtlas->m_textureAsset = std::static_pointer_cast< const TextureAsset >( asset );
            }
        }

        return textAtlas;
    }

    return nullptr;
}

// *********************************
//
void                Text::AddToCache( TextAtlasPtr atlas ) const
{
    boost::filesystem::path fontPath( m_fontFile );
    auto fontName = fontPath.filename().string();

    UInt32 mmLevelsNum = 0;
    if( atlas->m_textureAsset->HasMipMaps() )
    {
        mmLevelsNum = ( UInt32 )atlas->m_textureAsset->GetMipMaps()->GetLevelsNum();
    }

    auto descriptor = TextAtlas::GenerateTextAtlasAssetDescriptor( m_fontFile, atlas->GetWidth(), atlas->GetHeight(),
                                                                   m_fontSize, m_blurSize, m_glowBlurSize, m_outlineWidth, mmLevelsNum,
                                                                   m_supportedCharsSetFile );

    //Add to AssetCache
    auto assetDesc = std::static_pointer_cast< const AssetDesc >( descriptor );
    AssetManager::GetInstance().AddToCache( assetDesc, atlas->m_textureAsset );

    //Add to RawDataCache
    TextureUtils::AddToRawDataCache( atlas->m_textureAsset );

    auto entry = FontAtlasCacheEntry::Create( atlas, fontName, m_fontSize, m_blurSize, m_glowBlurSize, m_outlineWidth, m_fontFile, 
                                              mmLevelsNum, m_supportedCharsSetFile );
    FontAtlasCache::GetInstance().AddEntry( entry );
}

// *********************************
//
namespace
{

// *********************************
//
UInt32 GetMMLevelsNum( UInt32 fontSize )
{
    return ( UInt32 )std::floor( std::log( fontSize ) / std::log( 2 ) );
}

// *********************************
//
UInt32 CalculatePadding( UInt32 fontSize, UInt32 blurSize, UInt32 glowBlurSize, bool withMipMaps )
{
    UInt32 padding = std::max( blurSize, glowBlurSize ) + 1;

    if( withMipMaps )
    {
        auto mmLevelsNum = GetMMLevelsNum( fontSize );
        padding += 2 * mmLevelsNum;
    }

    return padding;
}

} // anonymous

// *********************************
//
TextAtlasPtr Text::BuildAtlas        () const
{
    auto atlas = LoadFromCache();

    if( !atlas || !atlas->GetAsset() || !atlas->GetAsset()->GetOriginal() )
    {
        auto  padding = CalculatePadding( m_fontSize, m_blurSize, m_glowBlurSize, m_withMipmaps ); // Update padding in case of bluring the atlas.

        atlas = m_fontEngine->CreateAtlas( padding, m_outlineWidth, m_supportedCharsSetFile, m_withMipmaps );

        BlurAtlas( atlas );
        GenerateMipMaps( atlas );
        AddTexturesKey( atlas );
        AddToCache( atlas );
    }

    
#ifdef GENERATE_TEST_BMP_FILE

    image::SaveBMPImage( "test.bmp", atlas->GetData(), (unsigned int) atlas->GetWidth(), (unsigned int) atlas->GetHeight(), (unsigned int) atlas->GetBitsPerPixel() );
#endif // GENERATE_TEST_BMP_FILE

#ifdef MAKE_FREETYPE_TESTING_TEXT
    GenrateTestFreeTypeText( L"AV::AVAVA", face );
#endif // MAKE_FREETYPE_TESTING_TEXT

    return atlas;
}

// ***********************
//
TextGeometryPtr             Text::BuildGeometry       () const
{
    TextGeometryPtr textGeom = std::make_shared< TextGeometry >( m_fontEngine );
    m_fontEngine->FillTextGeometry( textGeom, m_supportedCharsSetFile );

    return textGeom;
}


// *********************************
//
void Text::GenerateMipMaps( TextAtlasPtr atlas ) const
{
    if( m_withMipmaps )
    {
        UInt32 numLevels = GetMMLevelsNum( m_fontSize );

        auto texWithMipMaps = TextureUtils::GenerateMipMaps( atlas->m_textureAsset->GetOriginal(), numLevels, MipMapFilterType::BOX );

        std::const_pointer_cast< TextAtlas >( atlas )->m_textureAsset = texWithMipMaps;
    }
}

// *********************************
//
void Text::AddTexturesKey( TextAtlasPtr atlas ) const
{
    auto oldTA = atlas->m_textureAsset;
    auto atlasW = atlas->GetWidth();
    auto atlasH = atlas->GetHeight();

    UInt32 levelsNum = m_withMipmaps ? GetMMLevelsNum( m_fontSize ) : 0;

    auto atlasAssetDesc = TextAtlas::GenerateTextAtlasAssetDescriptor( m_fontFile, atlasW, atlasH, m_fontSize, m_blurSize, m_glowBlurSize, m_outlineWidth, levelsNum, m_supportedCharsSetFile );

    auto origKey = atlasAssetDesc->GetOrigTextureDesc()->GetKey();

    auto newOrigTexture = SingleTextureAsset::Create( oldTA->GetOriginal()->GetData(), origKey, atlasW, atlasH, TextureFormat::F_A8R8G8B8, true );

    auto mmAssetDesc = atlasAssetDesc->GetMipMapsDesc();

    if( mmAssetDesc )
    {
        std::vector< SingleTextureAssetConstPtr > mmsSTAs;

        for( SizeType i = 0; i < mmAssetDesc->GetLevelsNum(); ++i )
        {
            auto key            = mmAssetDesc->GetLevelDesc( i )->GetKey();

            auto mm = oldTA->GetMipMaps()->GetLevel( i );
            auto w = mm->GetWidth();
            auto h = mm->GetHeight();
            auto f = mm->GetFormat();

            mmsSTAs.push_back( SingleTextureAsset::Create( mm->GetData(), key, w, h, f, true ) );
        }

        std::const_pointer_cast< TextAtlas >( atlas )->m_textureAsset = TextureAsset::Create( newOrigTexture, MipMapAsset::Create( mmsSTAs ) );
    }
    else
    {
        std::const_pointer_cast< TextAtlas >( atlas )->m_textureAsset = TextureAsset::Create( newOrigTexture, nullptr );
    }
}

// *********************************
//
void Text::BlurAtlas( TextAtlasPtr atlas ) const
{
    if( m_blurSize > 0 || m_glowBlurSize > 0 )
    {
        auto atlasW = atlas->GetWidth();
        auto atlasH = atlas->GetHeight();
        auto oldData = std::const_pointer_cast< MemoryChunk >( atlas->m_textureAsset->GetOriginal()->GetData() );
        
        MemoryChunkConstPtr finalAtlas = nullptr;

        {
            auto bluredData = image::BlurImage( oldData, atlasW, atlasH, atlas->GetBitsPerPixel(), (Float32)m_blurSize, image::BlurType::BT_GAUSSIAN );

            auto swapped = image::SwapChannels( bluredData, atlas->GetBitsPerPixel(), 0x00ff0000, 0xff000000, 0x00000000, 0x00000000 );

            finalAtlas = image::AddImages( oldData, swapped );
        }

        {
            auto bluredData = image::BlurImage( oldData, atlasW, atlasH, atlas->GetBitsPerPixel(), (Float32)m_glowBlurSize, image::BlurType::BT_GAUSSIAN );

            auto swapped = image::SwapChannels( bluredData, atlas->GetBitsPerPixel(), 0x00ff0000, 0xff000000, 0x00000000, 0x00000000 );

            auto added = image::AddImages( oldData, swapped );

            char * concatenated = new char[ finalAtlas->Size() * 2 ]; 

            memcpy( concatenated, finalAtlas->Get(), finalAtlas->Size() );
            memcpy( concatenated + finalAtlas->Size(), added->Get(), added->Size() );

            finalAtlas = MemoryChunk::Create( concatenated, finalAtlas->Size() * 2 );
        }


        auto newSingleTextureRes = SingleTextureAsset::Create( finalAtlas, "", atlasW, atlasH * 2, TextureFormat::F_A8R8G8B8, true );
        
        std::const_pointer_cast< TextAtlas >( atlas )->m_textureAsset = TextureAsset::Create( newSingleTextureRes, nullptr );
    }
}

} // bv
