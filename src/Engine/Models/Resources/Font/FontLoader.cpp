#include "FontLoader.h"
#include "Text.h"
#include "Engine/Models/Resources/TextHelpers.h"
#include "System/FileIO.h"


#include <boost/filesystem/convenience.hpp>
#include <assert.h>

namespace bv { namespace model {

ResourceHandle *        LoadFont( const std::string& fontFileName, size_t size )
{
    auto fRes = FontResource( fontFileName, size );

    FontLoader fLoader;

    auto ret = fLoader.LoadResource( &fRes );

    assert( ret->GetExtra() );
    assert( ret->GetExtra()->GetResourceExtraKind() == ResourceExtraKind::RE_FONT );

    return ret;
}

///////////////////////////////
//
FontResource::FontResource( const std::string& filePath, size_t fontSize )
    : Resource( filePath, filePath )
    , m_fontSize( fontSize )
{}

///////////////////////////////
//
size_t FontResource::GetFontSize     ()   const
{
    return m_fontSize;
}

///////////////////////////////
//
ResourceHandle *        FontLoader::LoadResource        ( IResource* res )  const
{
    assert( dynamic_cast< FontResource* >(res) );

    auto fontRes = static_cast< FontResource* >(res);

    auto filePath = res->GetFilePath();

    const Text*       font        = TryLoadFont( filePath, fontRes->GetFontSize() );
    const Text*       fontB       = TryLoadFontBold( filePath, fontRes->GetFontSize() );
    const Text*       fontBI      = TryLoadFontBoldItalic( filePath, fontRes->GetFontSize() );
    const Text*       fontI       = TryLoadFontItalic( filePath, fontRes->GetFontSize() );

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

#define SUPPROTED_CHARS_FILE L"../dep/Media/fonts/SupportedChars.txt"

namespace
{

const Text*         LoadFontFile( const std::string& file, size_t size )
{
    auto t = TextHelper::LoadUtf8FileToString( SUPPROTED_CHARS_FILE );
    return new Text( t, file, int( size * (1.25f) ) ); /* points to pixel proportion */
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
const Text*             FontLoader::TryLoadFont             ( const std::string& file, size_t size ) const
{
    if( File::Exists(file) )
    {
        return LoadFontFile( file, size );
    }
    else
    {
        return nullptr;
    }
}

///////////////////////////////
//
const Text*             FontLoader::TryLoadFontBold         ( const std::string& file, size_t size ) const
{
    auto filebd = AddPostfixToFileName( file, "bd" );
    if( File::Exists( filebd ) )
    {
        return LoadFontFile( filebd, size );
    }
    else
    {
        return nullptr;
    }
}

///////////////////////////////
//
const Text*             FontLoader::TryLoadFontBoldItalic   ( const std::string& file, size_t size ) const
{
    auto filebd = AddPostfixToFileName( file, "bi" );
    if( File::Exists( filebd ) )
    {
        return LoadFontFile( filebd, size );
    }
    else
    {
        return nullptr;
    }
}

///////////////////////////////
//
const Text*             FontLoader::TryLoadFontItalic       ( const std::string& file, size_t size ) const
{
    auto filebd = AddPostfixToFileName( file, "i" );
    if( File::Exists( filebd ) )
    {
        return LoadFontFile( filebd, size );
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