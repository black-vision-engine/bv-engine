#include "stdafx.h"

#include "boost/filesystem/path.hpp"

#include "Text.h"
#include "TextAtlas.h"

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
						, UInt32 outlineSize
						, bool withMipmaps )
{
	return std::make_shared< Text >( supportedCharsSet, fontFile, fontSize, blurSize, outlineSize, withMipmaps );
}


// *********************************
//
Text::Text( const std::wstring & supportedCharsSetFile, const std::string& fontFile, UInt32 fontSize, UInt32 blurSize, UInt32 outlineSize, bool withMipmaps )
    : m_supportedCharsSetFile( supportedCharsSetFile )
    , m_fontFile( fontFile )
    , m_fontSize( fontSize )
    , m_blurSize( blurSize )
	, m_outlineWidth( outlineSize )
	, m_withMipmaps( withMipmaps )
{
	m_fontEngine = FreeTypeEngine::Create( fontFile, fontSize );
    BuildAtlas();
}

//#define GENERATE_TEST_BMP_FILE

// *********************************
//
TextAtlasPtr Text::LoadFromCache()
{
    boost::filesystem::path fontPath( m_fontFile );
    auto fontName = fontPath.filename().string();

	auto entry = FontAtlasCache::GetInstance().GetEntry( fontName, m_fontSize, m_blurSize, m_outlineWidth, 
                                                         m_withMipmaps, m_supportedCharsSetFile );

    if( entry )
	{
        auto textAtlas = entry->GetTextAtlas();
        if( textAtlas )
        {
            auto atlasTextureDesc = TextAtlas::GenerateTextAtlasAssetDescriptor( entry->GetFontFilePath(), entry->GetAtlasWidth(),
                                                                                 entry->GetAtlasHeight(), entry->GetFontSize(), 
                                                                                 entry->GetBlurSize(), entry->GetOutlineWidth(),
                                                                                 entry->GetLevelsNum(), entry->GetCharSet() );

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
void				Text::AddToCache()
{
    boost::filesystem::path fontPath( m_fontFile );
    auto fontName = fontPath.filename().string();

    UInt32 mmLevelsNum = 0;
    if( m_atlas->m_textureAsset->HasMipMaps() )
    {
        mmLevelsNum = ( UInt32 )m_atlas->m_textureAsset->GetMipMaps()->GetLevelsNum();
    }

    auto descriptor = TextAtlas::GenerateTextAtlasAssetDescriptor( m_fontFile, m_atlas->GetWidth(), m_atlas->GetHeight(),
                                                                   m_fontSize, m_blurSize, m_outlineWidth, mmLevelsNum,
                                                                   m_supportedCharsSetFile );

    //Add to AssetCache
    auto assetDesc = std::static_pointer_cast< const AssetDesc >( descriptor );
    AssetManager::GetInstance().AddToCache( assetDesc, m_atlas->m_textureAsset );

    //Add to RawDataCache
    TextureUtils::AddToRawDataCache( m_atlas->m_textureAsset );

    auto entry = FontAtlasCacheEntry::Create( m_atlas, fontName, m_fontSize, m_blurSize, m_outlineWidth, m_fontFile, 
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
UInt32 CalculatePadding( UInt32 fontSize, UInt32 blurSize, bool withMipMaps )
{
	UInt32 padding = blurSize + 1;

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
void Text::BuildAtlas        ()
{
    m_atlas = LoadFromCache();

    if( m_atlas != nullptr )
	{
		assert( m_atlas->GetAsset()->GetOriginal() ); // If atlas is in db it should be also in raw data cache.
		return;
	}

	auto  padding = CalculatePadding( m_fontSize, m_blurSize, m_withMipmaps ); // Update padding in case of bluring the atlas.

	m_atlas = m_fontEngine->CreateAtlas( padding, m_outlineWidth, m_supportedCharsSetFile, m_withMipmaps );

	BlurAtlas();

	GenerateMipMaps();

	AddTexturesKey();

	AddToCache();

#ifdef GENERATE_TEST_BMP_FILE

    image::SaveBMPImage( "test.bmp", m_atlas->GetData(), (unsigned int) m_atlas->GetWidth(), (unsigned int) m_atlas->GetHeight(), (unsigned int) m_atlas->GetBitsPerPixel() );
#endif // GENERATE_TEST_BMP_FILE

#ifdef MAKE_FREETYPE_TESTING_TEXT
    GenrateTestFreeTypeText( L"AV::AVAVA", face );
#endif // MAKE_FREETYPE_TESTING_TEXT
}


// *********************************
//
void Text::GenerateMipMaps()
{
	if( m_withMipmaps )
	{
		UInt32 numLevels = GetMMLevelsNum( m_fontSize );

		auto texWithMipMaps = TextureUtils::GenerateMipMaps( m_atlas->m_textureAsset->GetOriginal(), numLevels, MipMapFilterType::BILINEAR );

		std::const_pointer_cast< TextAtlas >( m_atlas )->m_textureAsset = texWithMipMaps;
	}
}

// *********************************
//
void Text::AddTexturesKey()
{
	auto oldTA = m_atlas->m_textureAsset;
	auto atlasW = m_atlas->GetWidth();
	auto atlasH = m_atlas->GetHeight();

	UInt32 levelsNum = m_withMipmaps ? GetMMLevelsNum( m_fontSize ) : 0;

    auto atlasAssetDesc = TextAtlas::GenerateTextAtlasAssetDescriptor( m_fontFile, atlasW, atlasH, m_fontSize, m_blurSize, m_outlineWidth, levelsNum, m_supportedCharsSetFile );

	auto origKey = atlasAssetDesc->GetOrigTextureDesc()->GetKey();

	auto newOrigTexture = SingleTextureAsset::Create( oldTA->GetOriginal()->GetData(), origKey, atlasW, atlasH, TextureFormat::F_A8R8G8B8, true );

	auto mmAssetDesc = atlasAssetDesc->GetMipMapsDesc();

	if( mmAssetDesc )
	{
		std::vector< SingleTextureAssetConstPtr > mmsSTAs;

		for( SizeType i = 0; i < mmAssetDesc->GetLevelsNum(); ++i )
		{
			auto key			= mmAssetDesc->GetLevelDesc( i )->GetKey();

			auto mm = oldTA->GetMipMaps()->GetLevel( i );
			auto w = mm->GetWidth();
			auto h = mm->GetHeight();
			auto f = mm->GetFormat();

			mmsSTAs.push_back( SingleTextureAsset::Create( mm->GetData(), key, w, h, f, true ) );
		}

		std::const_pointer_cast< TextAtlas >( m_atlas )->m_textureAsset = TextureAsset::Create( newOrigTexture, MipMapAsset::Create( mmsSTAs ) );
	}
	else
	{
		std::const_pointer_cast< TextAtlas >( m_atlas )->m_textureAsset = TextureAsset::Create( newOrigTexture, nullptr );
	}
}

// *********************************
//
void Text::BlurAtlas()
{
	if( m_blurSize > 0 )
	{
		auto atlasW = m_atlas->GetWidth();
		auto atlasH = m_atlas->GetHeight();
		auto oldData = std::const_pointer_cast< MemoryChunk >( m_atlas->m_textureAsset->GetOriginal()->GetData() );

		//image::SaveBMPImage( "test.bmp", oldData, (unsigned int) m_atlas->GetWidth(), (unsigned int) m_atlas->GetHeight(), (unsigned int) m_atlas->GetBitsPerPixel() );


		//UInt32 w;
		//UInt32 h;
		//UInt32 bbp;
		//auto img = image::LoadImage( "100x100.png", &w, &h, &bbp );
		//auto bluredData = image::BlurImage( img, w, h, bbp, 5 );

		//image::SaveBMPImage( "testhost.bmp", bluredData, w, h, bbp );

		//bluredData = bv::effect::GLBlurImage( img, w, h, bbp, 5 );

		//image::SaveBMPImage( "testbgpu.bmp", bluredData, w, h, bbp );

		auto bluredData = bv::effect::GLBlurImage( oldData, m_atlas->GetWidth(), m_atlas->GetHeight(), m_atlas->GetBitsPerPixel(), m_blurSize );

		//image::SaveBMPImage( "testbgpu.bmp", bluredData, m_atlas->GetWidth(), m_atlas->GetHeight(), m_atlas->GetBitsPerPixel() );

		auto newSingleTextureRes = SingleTextureAsset::Create( bluredData, "", atlasW, atlasH, TextureFormat::F_A8R8G8B8, true );
		std::const_pointer_cast< TextAtlas >( m_atlas )->m_textureAsset = TextureAsset::Create( newSingleTextureRes, nullptr );
	}
}

// ***********************
//
std::vector< std::unique_ptr< FTContour > > Text::CreateCharacter3D    ( wchar_t ch, float size ) const
{
    return m_fontEngine->Create3dVerticies( ch, size );
}

} // bv
