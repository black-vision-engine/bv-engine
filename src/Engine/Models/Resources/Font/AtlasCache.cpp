#include "AtlasCache.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <fstream>
#include "sqlite3.h"

namespace bv { namespace model {

// *********************************
//
FontAtlasCacheData::FontAtlasCacheData( const char* data, unsigned int width, unsigned int height, unsigned int bitsPerPixel )
    : m_data( data )
    , m_width( width )
    , m_height( height )
    , m_bitsPerPixel( bitsPerPixel )
{}

// *********************************
//
FontAtlasCacheEntry::FontAtlasCacheEntry( const FontAtlasCacheData& data, const std::string& fontName, unsigned int fontSize, const std::string& fontFilePath )
    : m_data( data )
    , m_fontName( fontName )
    , m_fontSize( fontSize )
    , m_fontFilePath( fontFilePath )
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
    //if( boost::filesystem::exists( filePath ) )
    //{
    //    return new FontAtlasCache( filePath );
    //}
    //else
    //{
    //    std::cout << "File does not exist: " << filePath << std::endl;
    //    std::cout << "Creating new atlas cache file: " << std::endl;

    //    std::ofstream file;
    //    file.open( filePath.c_str() );
    //    file.close();

        return new FontAtlasCache( filePath );
//    }
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

namespace
{

int GetEntryCallback( void* data, int argsNum, char** args, char** columnName )
{
    auto out = static_cast< FontAtlasCacheEntry* >( data );

    assert(argsNum == 6);

    //out->m_fontName = args[0];
    //out->m_fontSize = args[1];
    //out->m_data = new FontAtlasCacheData(  );
    //out->m_data->

    return 0;
}

}
// *********************************
//
FontAtlasCacheEntry *    FontAtlasCache::GetEntry        ( const std::string& fontName, unsigned int fontSize, const std::string& fontFileName )
{
    if( !m_dataBase )
    {
        m_dataBase = OpenDataBase( m_cacheFile );
    }

    auto ret = new FontAtlasCacheEntry();

    std::string sql = "SELECT * from cached_fonts";

    char* err = nullptr;

    auto res = sqlite3_exec( m_dataBase, sql.c_str(), GetEntryCallback, ret, &err );

    if( res != SQLITE_OK )
    {
        std::cerr << "SQL Error: " << *err << std::endl;
        sqlite3_free(err);
    }

    return ret;
}

namespace
{

int AddEntryCallback( void* data, int argsNum, char** args, char** columnName )
{
    return 0;
}

}

// *********************************
//
void                    FontAtlasCache::AddEntry        ( const FontAtlasCacheData& data, const std::string& fontName, unsigned int fontSize, const std::string& fontFilePath, bool forceInvalidate )
{
    if( !m_dataBase )
    {
        m_dataBase = OpenDataBase( m_cacheFile );
    }

    static std::string sql = "CREATE TABLE IF NOT EXISTS cached_fonts(font_name TEXT, font_size INTEGER \
                        , atlas_width INTEGER, atlas_height INTEGER, atlas_bpp INTEGER, atlas_data BLOB, PRIMARY KEY( font_name , font_size) )";

    char* err = nullptr;

    auto res = sqlite3_exec( m_dataBase, sql.c_str(), nullptr, nullptr, &err );

    if( res != SQLITE_OK )
    {
        std::cerr << "SQL Error: " << *err << std::endl;
        sqlite3_free(err);
        return;
    }

    std::string sqlAdd = std::string("INSERT OR REPLACE INTO cached_fonts VALUES(") 
        + "\'" + fontName + "\'" + ", " 
        + std::to_string(fontSize) + ", " 
        + std::to_string(data.m_width) + ", " 
        + std::to_string(data.m_height) + ", " 
        + std::to_string(data.m_bitsPerPixel) + ", " 
        + "?)";

    sqlite3_stmt* stmt;
    const char* parsed;

    res = sqlite3_prepare( m_dataBase, sqlAdd.c_str(), sqlAdd.size(), &stmt, &parsed);
    if( res != SQLITE_OK )
    {
        std::cerr << "SQL Error: " << "prepare" << std::endl;
        sqlite3_free(err);
        return;
    }

    res = sqlite3_bind_blob(stmt, 1, data.m_data, data.m_width * data.m_height * data.m_bitsPerPixel, SQLITE_TRANSIENT);

    if( res != SQLITE_OK )
    {
        std::cerr << "SQL Error: " << "bind" << std::endl;
        sqlite3_free(err);
        return;
    }

    res = sqlite3_step(stmt); 

    if( res != SQLITE_DONE )
    {
        std::cerr << "SQL Error: " << "step" << std::endl;
        sqlite3_free(err);
        return;
    }

    sqlite3_close( m_dataBase );
    m_dataBase = nullptr;
}

} // model
} // bv