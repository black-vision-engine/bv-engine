#include "AtlasCache.h"

#include "System/Path.h"
#include "IO/FileIO.h"
#include "IO/DirIO.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cassert>

#include "sqlite3.h"

#include "LibImage.h"
#include "Assets/Texture/TextureLoader.h"
#include "Assets/Assets.h"
#include "Assets/Texture/TextureCache.h"

#pragma warning(push)
#pragma warning(disable : 4512)
#pragma warning(disable : 4267)
#pragma warning(disable : 4996)

#include "hashlibpp.h"

#pragma warning(pop)

namespace bv { 

// *********************************
//
FontAtlasCacheEntry::FontAtlasCacheEntry()
    : m_textAtlas( nullptr )
    , m_fontSize( 0 )
{}

// *********************************
//
FontAtlasCacheEntry::FontAtlasCacheEntry(   const TextAtlasConstPtr & textAtlas
                                         ,  const std::string& fontName
                                         ,  SizeType fontSize
                                         ,  SizeType blurSize
										 ,  SizeType outlineWidth
                                         ,  const std::string& fontFilePath
										 ,  UInt32 mmLevelsNum )
    : m_textAtlas( textAtlas )
    , m_fontName( fontName )
    , m_fontSize( fontSize )
    , m_blurSize( blurSize )
	, m_outlineWidth( outlineWidth )
    , m_fontFilePath( fontFilePath )
	, m_mmLevelsNum( mmLevelsNum )
{}

// *********************************
//
FontAtlasCache::FontAtlasCache( const std::string& cacheDataFile )
    : m_dataBase( nullptr )
    , m_cacheFile( cacheDataFile )
{

}

// *********************************
//
FontAtlasCache::FontAtlasCache()
    : m_dataBase( nullptr )
{

}

// *********************************
//
FontAtlasCache*     FontAtlasCache::Load            ( const std::string& filePath )
{
    if( Path::Exists( filePath ) )
    {
        return new FontAtlasCache( filePath );
    }
    else
    {
        std::cout << "File does not exist: " << filePath << std::endl;
        std::cout << "Creating new atlas cache file: " << filePath << std::endl;

        auto dir = File::GetDirName( filePath );

        if( ( !Path::Exists( dir ) ) && ( !dir.empty() ) )
            Dir::CreateDir( dir );

        std::ofstream file;
        file.open( filePath.c_str() );
        file.close();

        auto fac = new FontAtlasCache( filePath );

        fac->InitFontCachedTable();

        return fac;
    }
}

// *********************************
//
sqlite3 *               FontAtlasCache::OpenDataBase    ( const std::string& dbFilePath )
{
    sqlite3 * db = nullptr;

    auto res = sqlite3_open( dbFilePath.c_str(), &db );

    if( res )
    {
        std::cerr << "Cannot open data base file: " + dbFilePath << std::endl;
        return nullptr;
    }

    return db;
}

// *********************************
//
void                    FontAtlasCache::InitFontCachedTable ()
{
    m_dataBase = OpenDataBase( m_cacheFile );

    if ( m_dataBase != nullptr )
    {
        static std::string sql = "CREATE TABLE IF NOT EXISTS cached_fonts(font_name TEXT, font_size INTEGER, blur_size INTEGER, outline_width INTEGER, font_file_name TEXT \
                            , atlas_width INTEGER, atlas_height INTEGER, mm_levels_num INTEGER, text_atlas BLOB, PRIMARY KEY( font_name , font_size, blur_size, mm_levels_num ) )";

        char* err = nullptr;

        auto res = sqlite3_exec( m_dataBase, sql.c_str(), nullptr, nullptr, &err );

        if( res != SQLITE_OK )
        {
            std::cerr << "SQL Error: " << std::string( err ) << std::endl;
            sqlite3_free(err);
            return;
        }

    }
}

namespace
{

int GetEntryCallback( void * data, int argsNum, char ** args, char ** columnName )
{
    { columnName; } // FIXME: suppress unused warning
    { argsNum; } // FIXME: suppress unused warning

    auto out = static_cast< FontAtlasCacheEntry * >( data );

    assert( argsNum == 9 );

    out->m_fontName     = args[ 0 ];
    out->m_fontSize     = std::atoi( args[ 1 ] );
    out->m_blurSize     = std::atoi( args[ 2 ] );
	out->m_outlineWidth = std::atoi( args[ 3 ] );
    out->m_fontFilePath = args[ 4 ];
	out->m_atlasWidth   = std::atoi( args[ 5 ] );
    out->m_atlasHeight  = std::atoi( args[ 6 ] );
	out->m_mmLevelsNum  = std::atoi( args[ 7 ] );
	out->m_textAtlas    = TextAtlas::Create(0,0,0,0,0);
    std::stringstream str(  args[ 8 ] );

	std::const_pointer_cast< TextAtlas >( out->m_textAtlas )->Load( str );    

    return 0;
}

}
// *********************************
//
FontAtlasCacheEntry *    FontAtlasCache::GetEntry        ( const std::string & fontName, SizeType fontSize, SizeType blurSize, SizeType outlineWidth, bool withMipMaps )
{
    if( !m_dataBase )
    {
        m_dataBase = OpenDataBase( m_cacheFile );
    }

	auto ret = new FontAtlasCacheEntry();

    std::string sql = "SELECT * FROM cached_fonts WHERE font_name=\'" + fontName + "\'" +
                            " AND font_size = " + std::to_string( fontSize ) +
                            " AND blur_Size = " + std::to_string( blurSize ) +
							" AND outline_width = " + std::to_string( outlineWidth ) +
							" AND mm_levels_num " + ( withMipMaps  ? " > 0 " : " = 0 " ) + ";";

    char* err = nullptr;

    auto res = sqlite3_exec( m_dataBase, sql.c_str(), GetEntryCallback, ret, &err );

    if( res != SQLITE_OK )
    {
        if( err != nullptr )
        {
            std::cerr << "SQL Error: " << std::string( err ) << std::endl;
            sqlite3_free(err);
        }
        else
            std::cerr << "SQL Error: " << res << std::endl;
    }

	if( ret->m_textAtlas != nullptr )
	{
		auto atlasTextureDesc = TextAtlas::GenerateTextAtlasAssetDescriptor(	ret->m_fontFilePath,
																				ret->m_atlasWidth,
																				ret->m_atlasHeight,
																				ret->m_fontSize,
																				ret->m_blurSize,
																				ret->m_outlineWidth,
																				ret->m_mmLevelsNum );

		auto asset = std::static_pointer_cast<const TextureAsset>( AssetManager::GetInstance().LoadAsset( atlasTextureDesc ) );

		if( asset != nullptr )
		{
			std::const_pointer_cast< TextAtlas >( ret->m_textAtlas )->m_textureAsset = asset; // FIXME: Remove const_pointer_cast
		}

		return ret;
	}
    else
    {
        return nullptr;
    }
}

namespace
{

int AddEntryCallback( void * data, int argsNum, char ** args, char ** columnName )
{
    { columnName; args; data; argsNum; } // FIXME: suppress unused warning
    return 0;
}

} // anonymous

// *********************************
//
std::string				FontAtlasCache::GenerateTextAtlasCacheFileName( const TextAtlasConstPtr & textAtlas )
{
	std::stringstream textAtlasStream;
    textAtlas->Save( textAtlasStream );

    auto textAtlasStr =  textAtlasStream.str();

    auto sha1 = sha1wrapper();
    auto fontAtlasTextureFileName = CACHE_DIRECTORY + sha1.getHashFromString( textAtlasStr ) + ".bmp";
	return fontAtlasTextureFileName;
}

// *********************************
//
void                    FontAtlasCache::AddEntry        ( const FontAtlasCacheEntry & data, bool forceInvalidate )
{
    { forceInvalidate; } // FIXME: suppress unused warning
    if( !m_dataBase )
    {
        m_dataBase = OpenDataBase( m_cacheFile );
    }

	auto mmLevelsNum = data.m_textAtlas->m_textureAsset->GetMipMaps() ? data.m_textAtlas->m_textureAsset->GetMipMaps()->GetLevelsNum() : 0;

    std::string sqlAdd = std::string( "INSERT OR REPLACE INTO cached_fonts VALUES(" ) 
        + "\'" + data.m_fontName + "\'" + ", " 
        + std::to_string( data.m_fontSize ) + ", " 
        + std::to_string( data.m_blurSize ) + ", " 
		+ std::to_string( data.m_outlineWidth ) + ", " 
        + "\'" + data.m_fontFilePath + "\'" + ", " 
		+ std::to_string( data.m_textAtlas->GetWidth() ) + ", " 
		+ std::to_string( data.m_textAtlas->GetHeight() ) + ", " 
		+ std::to_string( mmLevelsNum ) + ", " 
        + "?)";

	
	auto atlasTextureDesc = TextAtlas::GenerateTextAtlasAssetDescriptor(	data.m_fontFilePath,
																			data.m_textAtlas->GetWidth(),
																			data.m_textAtlas->GetHeight(),
																			data.m_fontSize,
																			data.m_blurSize,
																			data.m_outlineWidth,
																			mmLevelsNum );

	TextureCache::GetInstance().Add( atlasTextureDesc, data.m_textAtlas->m_textureAsset );

    sqlite3_stmt * stmt = nullptr;
    const char * parsed = nullptr;

    auto res = sqlite3_prepare( m_dataBase, sqlAdd.c_str(), (int) sqlAdd.size(), &stmt, &parsed);
    if( res != SQLITE_OK )
    {
        std::cerr << "SQL Error: " << "prepare" << std::endl;
        return;
    }

	std::stringstream textAtlasStream;
    data.m_textAtlas->Save( textAtlasStream );

	auto textAtlasStr = textAtlasStream.str();

    res = sqlite3_bind_blob( stmt, 1, textAtlasStr.c_str(), (int) textAtlasStr.size(), SQLITE_TRANSIENT);

    if( res != SQLITE_OK )
    {
        std::cerr << "SQL Error: " << "bind" << std::endl;
        return;
    }

    res = sqlite3_step(stmt); 

    if( res != SQLITE_DONE )
    {
        std::cerr << "SQL Error: " << "step" << std::endl;
        return;
    }

    sqlite3_close( m_dataBase );
    m_dataBase = nullptr;
}


} // bv