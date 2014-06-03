#include "AtlasCache.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Text.h"

#include "sqlite3.h"


namespace bv { namespace model {

// *********************************
//
FontAtlasCacheEntry::FontAtlasCacheEntry()
    : m_textAtlas( nullptr )
    , m_fontSize( 0 )
{}

// *********************************
//
FontAtlasCacheEntry::FontAtlasCacheEntry(   TextAtlas* textAtlas
                                         ,  const std::string& fontName
                                         ,  unsigned int fontSize
                                         ,  const std::string& fontFilePath
                                         ,  bool bold
                                         ,  bool italic )
    : m_textAtlas( textAtlas )
    , m_fontName( fontName )
    , m_fontSize( fontSize )
    , m_fontFilePath( fontFilePath )
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
    if( boost::filesystem::exists( filePath ) )
    {
        return new FontAtlasCache( filePath );
    }
    else
    {
        std::cout << "File does not exist: " << filePath << std::endl;
        std::cout << "Creating new atlas cache file: " << std::endl;

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
        static std::string sql = "CREATE TABLE IF NOT EXISTS cached_fonts(font_name TEXT, font_size INTEGER, font_file_name TEXT \
                            , bold_flag BOOL, italic_flag BOOL, tesxt_atlas BLOB, PRIMARY KEY( font_name , font_size, bold_flag, italic_flag ) )";

        char* err = nullptr;

        auto res = sqlite3_exec( m_dataBase, sql.c_str(), nullptr, nullptr, &err );

        if( res != SQLITE_OK )
        {
            std::cerr << "SQL Error: " << *err << std::endl;
            sqlite3_free(err);
            return;
        }

    }
}

namespace
{

int GetEntryCallback( void* data, int argsNum, char** args, char** columnName )
{
    auto out = static_cast< FontAtlasCacheEntry* >( data );

    //assert(argsNum == 6);

    //out->m_fontName = args[0];
    //out->m_fontSize = args[1];
    //out->m_data = new FontAtlasCacheData(  );
    //out->m_data->

    return 0;
}

}
// *********************************
//
FontAtlasCacheEntry *    FontAtlasCache::GetEntry        ( const std::string& fontName, unsigned int fontSize, const std::string& fontFileName, bool bold, bool italic )
{
    if( !m_dataBase )
    {
        m_dataBase = OpenDataBase( m_cacheFile );
    }

    auto ret = new FontAtlasCacheEntry();

    static const char* sql = "SELECT * from cached_fonts";

    char* err = nullptr;

    auto res = sqlite3_exec( m_dataBase, sql, GetEntryCallback, ret, &err );


    if( res != SQLITE_OK )
    {
        std::cerr << "SQL Error: " << *err << std::endl;
        sqlite3_free(err);
    }

    if( ret->m_textAtlas != nullptr )
        return ret;
    else
        return nullptr;
    
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
void                    FontAtlasCache::AddEntry        ( const FontAtlasCacheEntry& data, bool forceInvalidate )
{
    if( !m_dataBase )
    {
        m_dataBase = OpenDataBase( m_cacheFile );
    }

    std::string sqlAdd = std::string( "INSERT OR REPLACE INTO cached_fonts VALUES(" ) 
        + "\'" + data.m_fontName + "\'" + ", " 
        + std::to_string( data.m_fontSize ) + ", " 
        + data.m_fontFilePath + ", " 
        + std::to_string( data.m_bold ) + ", " 
        + std::to_string( data.m_italic ) + ", " 
        + "?)";

    sqlite3_stmt* stmt;
    const char* parsed;

    auto res = sqlite3_prepare( m_dataBase, sqlAdd.c_str(), sqlAdd.size(), &stmt, &parsed);
    if( res != SQLITE_OK )
    {
        std::cerr << "SQL Error: " << "prepare" << std::endl;
        return;
    }

    std::stringstream textAtlasStream;
    data.m_textAtlas->Save( textAtlasStream );

    auto textAtlasStr =  textAtlasStream.str();

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