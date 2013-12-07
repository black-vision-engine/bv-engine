#pragma once

#include <string>

namespace bv { namespace model {

struct FontAtlasCacheData
{
    const char*     m_data; // Cache is an owner. Can free this memmory.
    unsigned int    m_width;
    unsigned int    m_height;
    unsigned int    m_bitsPerPixel;

    FontAtlasCacheData  ( const char* data, unsigned int width, unsigned int height, unsigned int bitPerPixel );
};

class FontAtlasCacheEntry
{
    FontAtlasCacheData          m_data;
    std::string                 m_fontName;
    unsigned int                m_fontSize;
    std::string                 m_fontFilePath;

    FontAtlasCacheEntry ( const FontAtlasCacheData& data
                        , const std::string& fontName
                        , unsigned int fontSize
                        , const std::string& fontFilePath );
};

class FontAtlasCache
{
private:



public:

    void                                AddEntry            ( const FontAtlasCacheData& data
                                                            , const std::string& fontName
                                                            , unsigned int fontSize
                                                            , const std::string& fontFilePath
                                                            , bool forceInvalidate = true );

    void                                Update              ();
    void                                Clear               ();


    static FontAtlasCache*              Load                ( const std::string& dbFilePath );
};

} // model
} // bv