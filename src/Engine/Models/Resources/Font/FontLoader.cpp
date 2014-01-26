#include "FontLoader.h"
#include "Text.h"
#include "Engine/Models/Resources/TextHelpers.h"
#include "System/FileIO.h"


#include <boost/filesystem/convenience.hpp>
#include <assert.h>

namespace bv { namespace model {

///////////////////////////////
//
FontResource::FontResource( const std::string& filePath, size_t fontSize, const std::wstring& atlasCharSetFile )
    : Resource( filePath, filePath )
    , m_fontSize( fontSize )
    , m_atlasCharSetFile( atlasCharSetFile )
{}

///////////////////////////////
//
size_t                  FontResource::GetFontSize     ()   const
{
    return m_fontSize;
}

///////////////////////////////
//
const std::wstring&     FontResource::GetAtlasCharSetFile     ()  const
{
    return m_atlasCharSetFile;
}

///////////////////////////////
//
ResourceHandle *        FontLoader::LoadResource        ( IResource* res )  const
{
    assert( dynamic_cast< FontResource* >(res) );

    auto fontRes = static_cast< FontResource* >(res);

    auto filePath = res->GetFilePath();
    auto atlasCharSetFile = fontRes->GetAtlasCharSetFile();
    auto fontSize = fontRes->GetFontSize();

    const Text*       font        = TryLoadFont             ( filePath, fontSize, atlasCharSetFile );
    const Text*       fontB       = TryLoadFontBold         ( filePath, fontSize, atlasCharSetFile );
    const Text*       fontBI      = TryLoadFontBoldItalic   ( filePath, fontSize, atlasCharSetFile );
    const Text*       fontI       = TryLoadFontItalic       ( filePath, fontSize, atlasCharSetFile );

    if( font || fontB || fontBI || fontI )
    {
        auto res = new ResourceHandle( nullptr, 0, new FontExtraData( font, fontB, fontI, fontBI, nullptr, fontRes->GetFontSize() ) );
        return res;
    }
    else
    {
        return nullptr;
    }
}

namespace
{

const Text*         LoadFontFile( const std::string& file, size_t size, const std::wstring& atlasCharSetFile )
{
    auto t = TextHelper::LoadUtf8FileToString( atlasCharSetFile );
    return new Text( t, file, int( size * (1.25f) ) ); /* points to pixel proportion */ // FIXME: Text constructor makes to much.
}

std::string         AddPostfixToFileName( const std::string& file, const std::string& postfix )
{
    std::string newFileName = boost::filesystem::basename( file ) + postfix;

    auto newPath = boost::filesystem::path( file );
    newPath.remove_filename() /= newFileName;

    return newPath.replace_extension( boost::filesystem::extension( file ) ).string();
}

}

///////////////////////////////
//
const Text*             FontLoader::TryLoadFont             ( const std::string& file, size_t size, const std::wstring& atlasCharSetFile ) const
{
    if( File::Exists(file) )
    {
        return LoadFontFile( file, size, atlasCharSetFile );
    }
    else
    {
        return nullptr;
    }
}

///////////////////////////////
//
const Text*             FontLoader::TryLoadFontBold         ( const std::string& file, size_t size, const std::wstring& atlasCharSetFile ) const
{
    auto filebd = AddPostfixToFileName( file, "bd" );
    if( File::Exists( filebd ) )
    {
        return LoadFontFile( filebd, size, atlasCharSetFile  );
    }
    else
    {
        return nullptr;
    }
}

///////////////////////////////
//
const Text*             FontLoader::TryLoadFontBoldItalic   ( const std::string& file, size_t size, const std::wstring& atlasCharSetFile ) const
{
    auto filebd = AddPostfixToFileName( file, "bi" );
    if( File::Exists( filebd ) )
    {
        return LoadFontFile( filebd, size, atlasCharSetFile  );
    }
    else
    {
        return nullptr;
    }
}

///////////////////////////////
//
const Text*             FontLoader::TryLoadFontItalic       ( const std::string& file, size_t size, const std::wstring& atlasCharSetFile ) const
{
    auto filebd = AddPostfixToFileName( file, "i" );
    if( File::Exists( filebd ) )
    {
        return LoadFontFile( filebd, size, atlasCharSetFile  );
    }
    else
    {
        return nullptr;
    }
}

///////////////////////////////
//
const Text*             FontExtraData::GetFont             () const
{
    return m_font;
}

///////////////////////////////
//
const Text*             FontExtraData::GetFontBold         () const
{
    return m_fontBold;
}

///////////////////////////////
//
const Text*             FontExtraData::GetFontBoldItalic   () const
{
    return m_fontBoldItalic;
}

///////////////////////////////
//
const Text*             FontExtraData::GetFontItalic       () const
{
    return m_fontItalic;
}

///////////////////////////////
//
const Text*             FontExtraData::GetFontUnderlined   () const
{
    return m_fontBoldItalic;
}

///////////////////////////////
//
FontExtraData::FontExtraData       ( const Text* font, const Text* fontBold, const Text* fontItalic, const Text* fontBoldItalic, const Text* fontUnderlined, size_t fontSize )
    : ResourceExtraData( ResourceExtraKind::RE_FONT )
    , m_font( font )
    , m_fontBold( fontBold )
    , m_fontItalic( fontItalic )
    , m_fontBoldItalic( fontBoldItalic )
    , m_fontUnderlined( fontUnderlined )
    , m_fontSize( fontSize )
{}


} // model
} // bv