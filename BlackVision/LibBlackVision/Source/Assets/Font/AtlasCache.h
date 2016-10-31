#pragma once

#include "FontAtlasCacheEntry.h"


namespace bv { 

class IDatabase;

class FontAtlasCache
{
private:

    static const std::string        m_sCacheDataPath;
    static const std::string        m_sCachedFontsTableName;

    IDatabase *                     m_database;

    explicit                        FontAtlasCache      ();
                                    ~FontAtlasCache     ();

public:

    FontAtlasCacheEntryPtr          GetEntry            ( const std::string & fontName, SizeType fontSize, SizeType blurSize, 
                                                          SizeType outlineWidth, bool withMipMaps, const std::wstring & charSetFileName );

    void                            AddEntry            ( FontAtlasCacheEntryConstPtr data );

    static FontAtlasCache &         GetInstance         ();

};

} // bv