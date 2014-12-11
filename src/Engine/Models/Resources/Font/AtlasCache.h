#pragma once

#include "System/BasicTypes.h"

#include <string>

struct sqlite3;

#define CACHE_DIRECTORY std::string("cache/")
#define CACHE_DB_FILE_NAME std::string("fontscache.db")

namespace bv { namespace model {

class TextAtlas;

class FontAtlasCacheEntry
{
public:
    const TextAtlas*            m_textAtlas;
    std::string                 m_fontName;
    unsigned int                m_fontSize;
    unsigned int                m_blurSize;
    std::string                 m_fontFilePath;
    bool                        m_bold;
    bool                        m_italic;

    FontAtlasCacheEntry ();
    FontAtlasCacheEntry ( const TextAtlas* textAtlas
                        , const std::string& fontName
                        , SizeType fontSize
                        , SizeType blurSize
                        , const std::string& fontFilePath
                        , bool bold = false
                        , bool italic = false );
};

class FontAtlasCache
{
private:
    std::string     m_cacheFile;
    sqlite3 *       m_dataBase;


    sqlite3 *                           OpenDataBase        ( const std::string& dbFilePath );
    void                                InitFontCachedTable ();

    FontAtlasCache();
    explicit FontAtlasCache( const std::string& cacheDataFile );

public:

    FontAtlasCacheEntry *               GetEntry            ( const std::string& fontName, SizeType fontSize, SizeType blurSize, const std::string& fontFileName, bool bold, bool italic );
    void                                AddEntry            ( const FontAtlasCacheEntry& data, bool forceInvalidate = true );

    void                                Update              ();
    void                                Clear               ();


    static FontAtlasCache*              Load                ( const std::string& dbFilePath );
};

} // model
} // bv