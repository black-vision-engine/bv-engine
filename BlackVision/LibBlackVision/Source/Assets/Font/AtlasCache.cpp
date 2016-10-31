#include "stdafx.h"

#include "AtlasCache.h"
#include "Database/SQLite/SQLiteDatabase.h"


namespace bv { 


// *********************************
//
const std::string        FontAtlasCache::m_sCacheDataPath = "cache/fontscache.db";
const std::string        FontAtlasCache::m_sCachedFontsTableName = "cached_fonts";


// *********************************
//
FontAtlasCache::FontAtlasCache      ()
    : m_database( new SQLiteDatabase( m_sCacheDataPath ) )
{
    m_database->CreateTable( m_sCachedFontsTableName, FontAtlasCacheEntry::ColumnTypes(), FontAtlasCacheEntry::PrimaryKeys() );
}

// *********************************
//
FontAtlasCache::~FontAtlasCache     ()
{
    delete m_database;
}

// ******************************
//
FontAtlasCache &          FontAtlasCache::GetInstance       ()
{
    static FontAtlasCache instance = FontAtlasCache();
    return instance;
}

// *********************************
//
FontAtlasCacheEntryPtr      FontAtlasCache::GetEntry        ( const std::string & fontName, SizeType fontSize, SizeType blurSize, 
                                                              SizeType outlineWidth, bool withMipMaps, const std::wstring & charSet )
{
    std::string sql = "SELECT * FROM " + m_sCachedFontsTableName + " WHERE " +
        FontAtlasCacheEntry::CN::FONT_NAME + "=\'" + fontName + "\'" +
        " AND " + FontAtlasCacheEntry::CN::FONT_SIZE + " = " + std::to_string( fontSize ) +
        " AND " + FontAtlasCacheEntry::CN::BLUR_SIZE + " = " + std::to_string( blurSize ) +
        " AND " + FontAtlasCacheEntry::CN::OUTLINE_WIDTH + " = " + std::to_string( outlineWidth ) +
        " AND " + FontAtlasCacheEntry::CN::LEVELS_NUM + " " + ( withMipMaps  ? " > 0 " : " = 0 " ) +
        " AND " + FontAtlasCacheEntry::CN::CHAR_SET + " = \'" + std::string( charSet.begin(), charSet.end() ) + "\';";

    auto entries = m_database->Load< FontAtlasCacheEntry >( sql );
    if( !entries.empty() )
    {
        return std::static_pointer_cast< FontAtlasCacheEntry >( entries[ 0 ] );
    }

    return nullptr;
}

// *********************************
//
void                    FontAtlasCache::AddEntry        ( FontAtlasCacheEntryConstPtr data )
{
    m_database->Save( m_sCachedFontsTableName, data );
}

} // bv