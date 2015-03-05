#pragma once

#include "TextAtlas.h"
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
    TextAtlasConstPtr           m_textAtlas;
    std::string                 m_fontName;
    SizeType					m_fontSize;
    SizeType					m_blurSize;
	SizeType					m_outlineWidth;
    std::string                 m_fontFilePath;
	std::string                 m_atlasFilePath;
    bool                        m_bold;
    bool                        m_italic;

    FontAtlasCacheEntry ();
    FontAtlasCacheEntry ( const TextAtlasConstPtr & textAtlas
                        , const std::string & fontName
                        , SizeType fontSize
                        , SizeType blurSize
						, SizeType outlineWidth
                        , const std::string & fontFilePath
						, const std::string & atlasFilePath
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

    FontAtlasCacheEntry *               GetEntry            ( const std::string& fontName, SizeType fontSize, SizeType blurSize, SizeType outlineWidth, bool bold, bool italic );
    void                                AddEntry            ( const FontAtlasCacheEntry& data, bool forceInvalidate = true );

    void                                Update              ();
    void                                Clear               ();

	static std::string					GenerateTextAtlasCacheFileName( const TextAtlasConstPtr & textAtlas );

    static FontAtlasCache*              Load                ( const std::string& dbFilePath );
};

} // model
} // bv