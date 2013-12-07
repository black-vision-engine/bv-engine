#include "AtlasCache.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>

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
FontAtlasCache*     FontAtlasCache::Load                ( const std::string& filePath )
{
    if( boost::filesystem::exists( filePath ) )
    {

    }
    else
    {


    }

    return nullptr;
}


} // model
} // bv