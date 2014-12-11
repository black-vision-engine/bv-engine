#include "AtlasCache.h"

#include "System/FileIO.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cassert>

#include "Text.h"

#include "sqlite3.h"

#include "Engine/Models/Resources/TextureHelpers.h"
#include "Engine/Models/Resources/TextureLoader.h"
#include "Engine/Models/Resources/ModelTextureManager.h"
#include "Engine/Models/Resources/IResource.h"

#include "hashlibpp.h"

namespace bv { namespace model {

// *********************************
//
FontAtlasCacheEntry::FontAtlasCacheEntry()
    : m_textAtlas( nullptr )
    , m_fontSize( 0 )
{}

// *********************************
//
FontAtlasCacheEntry::FontAtlasCacheEntry(   const TextAtlas* textAtlas
                                         ,  const std::string& fontName
                                         ,  SizeType fontSize
                                         ,  SizeType blurSize
                                         ,  const std::string& fontFilePath
										 ,	const std::string & atlasFilePath
                                         ,  bool bold
                                         ,  bool italic )
    : m_textAtlas( textAtlas )
    , m_fontName( fontName )
    , m_fontSize( fontSize )
    , m_blurSize( blurSize )
    , m_fontFilePath( fontFilePath )
	, m_atlasFilePath( atlasFilePath )
    , m_bold( bold )
    , m_italic( italic )
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
    if( File::Exists( filePath ) )
    {
        return new FontAtlasCache( filePath );
    }
    else
    {
        std::cout << "File does not exist: " << filePath << std::endl;
        std::cout << "Creating new atlas cache file: " << filePath << std::endl;

        auto dir = File::GetDirName( filePath );

        if( (! File::Exists( dir ) ) && ( ! dir.empty() ) )
            File::CreateDir( dir );

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
        static std::string sql = "CREATE TABLE IF NOT EXISTS cached_fonts(font_name TEXT, font_size INTEGER, blur_size INTEGER, font_file_name TEXT \
                            , bold_flag BOOL, italic_flag BOOL, text_atlas BLOB, test_atlas_data_file TEXT, PRIMARY KEY( font_name , font_size, blur_size, bold_flag, italic_flag ) )";

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

int GetEntryCallback( void* data, int argsNum, char** args, char** columnName )
{
    auto out = static_cast< FontAtlasCacheEntry * >( data );

    assert( argsNum == 8 );

    out->m_fontName     = args[ 0 ];
    out->m_fontSize     = std::atoi( args[ 1 ] );
    out->m_blurSize     = std::atoi( args[ 2 ] );
    out->m_fontFilePath = args[ 3 ];
    out->m_bold         = std::atoi( args[ 4 ] ) == 0 ? false : true;
    out->m_italic       = std::atoi( args[ 5 ] ) == 0 ? false : true;
    out->m_textAtlas    = new TextAtlas();
    std::stringstream str(  args[6] );
	out->m_atlasFilePath = args[ 7 ];

	auto textAtlas		= const_cast< TextAtlas* >( out->m_textAtlas );

	textAtlas->Load( str );    

    return 0;
}

}
// *********************************
//
FontAtlasCacheEntry *    FontAtlasCache::GetEntry        ( const std::string& fontName, SizeType fontSize, SizeType blurSize, const std::string& fontFileName, bool bold, bool italic )
{
    if( !m_dataBase )
    {
        m_dataBase = OpenDataBase( m_cacheFile );
    }

    auto ret = new FontAtlasCacheEntry();

    std::string sql = "SELECT * FROM cached_fonts WHERE font_name=\'" + fontName + "\'" +
                            " AND font_size = " + std::to_string( fontSize ) +
                            " AND blur_Size = " + std::to_string( blurSize ) +
                            " AND bold_flag = " + std::to_string( bold ) +
                            " AND italic_flag = " + std::to_string( italic ) + ";";

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
		auto width	= ret->m_textAtlas->GetWidth();
		auto height = ret->m_textAtlas->GetHeight();
		auto bbp	= ret->m_textAtlas->GetBitsPerPixel();

		TextureFormat tf;

		if( bbp == 8 )
			tf = TextureFormat::F_A8;
		else if ( bbp = 32 )
			tf = TextureFormat::F_A8R8G8B8;
		else
			assert(false);

		auto resExtra = new TextureExtraData( width, height, bbp, tf, TextureType::T_2D );
		auto memChunk = TextureHelper::LoadRAW( ret->m_atlasFilePath );

		const_cast< TextAtlas* >( ret->m_textAtlas )->m_textureHandle = std::make_shared< ResourceHandle >( memChunk, width * height * bbp / 8, resExtra );

		return ret;
	}
    else
        return nullptr;
    
}

namespace
{

int AddEntryCallback( void* data, int argsNum, char** args, char** columnName )
{
    return 0;
}

} // anonymous

// *********************************
//
void                    FontAtlasCache::AddEntry        ( const FontAtlasCacheEntry& data, bool forceInvalidate )
{
    if( !m_dataBase )
    {
        m_dataBase = OpenDataBase( m_cacheFile );
    }

    std::stringstream textAtlasStream;
    data.m_textAtlas->Save( textAtlasStream );

    auto textAtlasStr =  textAtlasStream.str();

    auto sha1 = sha1wrapper();
    auto fontAtlasTextureFileName = CACHE_DIRECTORY + sha1.getHashFromString( textAtlasStr ) + ".raw";

    std::string sqlAdd = std::string( "INSERT OR REPLACE INTO cached_fonts VALUES(" ) 
        + "\'" + data.m_fontName + "\'" + ", " 
        + std::to_string( data.m_fontSize ) + ", " 
        + std::to_string( data.m_blurSize ) + ", " 
        + "\'" + data.m_fontFilePath + "\'" + ", " 
        + std::to_string( data.m_bold ) + ", " 
        + std::to_string( data.m_italic ) + ", " 
        + "?, "
        + "\'" + fontAtlasTextureFileName + "\'" + ")";


    if( ! File::Exists( CACHE_DIRECTORY ) )
        File::CreateDir( CACHE_DIRECTORY );

    TextureHelper::WriteRAW( fontAtlasTextureFileName, data.m_textAtlas->GetData() );

    sqlite3_stmt* stmt;
    const char* parsed;

    auto res = sqlite3_prepare( m_dataBase, sqlAdd.c_str(), sqlAdd.size(), &stmt, &parsed);
    if( res != SQLITE_OK )
    {
        std::cerr << "SQL Error: " << "prepare" << std::endl;
        return;
    }

    res = sqlite3_bind_blob(stmt, 1, textAtlasStr.c_str(), textAtlasStr.size(), SQLITE_TRANSIENT);

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

} // model
} // bv