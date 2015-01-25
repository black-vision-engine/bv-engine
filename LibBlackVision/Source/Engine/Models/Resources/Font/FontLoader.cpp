#include "FontLoader.h"
#include "Text.h"
#include "Engine/Models/Resources/TextHelpers.h"
#include "System/FileIO.h"


#include <boost/filesystem/convenience.hpp>
#include <assert.h>

namespace bv { namespace model {

///////////////////////////////
//
FontResource::FontResource( const std::string& filePath, size_t fontSize, size_t blurSize, size_t outlineSize, const std::wstring& atlasCharSetFile )
    : Resource( filePath, filePath )
    , m_fontSize( fontSize )
    , m_blurSize( blurSize )
	, m_outlineSize( outlineSize )
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
size_t                  FontResource::GetBlurSize     ()   const
{
    return m_blurSize;
}

///////////////////////////////
//
size_t                  FontResource::GetOutlineSize     ()   const
{
    return m_outlineSize;
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
    auto blurSize = fontRes->GetBlurSize();
	auto outlineSize = fontRes->GetOutlineSize();

    const Text*       font        = TryLoadFont             ( filePath, fontSize, blurSize, outlineSize, atlasCharSetFile );

    if( font )
    {
		auto res = new ResourceHandle( nullptr, 0, new FontExtraData( font, nullptr, nullptr, nullptr, nullptr, fontRes->GetFontSize() ) );
        return res;
    }
    else
    {
        return nullptr;
    }
}

namespace
{

const Text*         LoadFontFile( const std::string& file, size_t size, size_t blurSize, size_t outlineSize, const std::wstring& atlasCharSetFile )
{
    auto t = TextHelper::LoadUtf8FileToString( atlasCharSetFile );
    return new Text( t, file, size, blurSize, outlineSize ); /* points to pixel proportion */ // FIXME: Text constructor makes to much.
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
const Text*             FontLoader::TryLoadFont             ( const std::string& file, size_t size, size_t blurSize, size_t oulineSize, const std::wstring& atlasCharSetFile ) const
{
    if( File::Exists(file) )
    {
        return LoadFontFile( file, size, blurSize, oulineSize, atlasCharSetFile );
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
FontExtraData::FontExtraData       ( const Text* font, const Text* fontBold, const Text* fontItalic, const Text* fontBoldItalic, const Text* fontUnderlined, size_t fontSize )
    : ResourceExtraData( ResourceExtraKind::RE_FONT )
    , m_font( font )
    , m_fontSize( fontSize )
{}


} // model
} // bv