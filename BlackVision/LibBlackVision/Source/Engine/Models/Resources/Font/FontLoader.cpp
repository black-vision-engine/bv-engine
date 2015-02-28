#include "FontLoader.h"
#include "Text.h"
#include "Engine/Models/Resources/Font/FontResourceDescriptor.h"
#include "Engine/Models/Resources/Font/FontResource.h"
#include "Engine/Models/Resources/TextHelpers.h"
#include "System/FileIO.h"


#include <boost/filesystem/convenience.hpp>
#include <assert.h>

namespace bv { namespace model {

///////////////////////////////
//
IResourceNEWConstPtr FontLoader::LoadResource( const bv::ResourceDescConstPtr & desc ) const
{
	auto typedDesc = QueryTypedDesc< FontResourceDescConstPtr >( desc );

	assert( typedDesc );

	auto filePath			= typedDesc->GetFontFileName();
    auto atlasCharSetFile	= typedDesc->GetAtlasCharSetFile();
    auto fontSize			= typedDesc->GetFontSize();
    auto blurSize			= typedDesc->GetBlurSize();
	auto outlineSize		= typedDesc->GetOutlineSize();

    auto text				= TryLoadFont( filePath, fontSize, blurSize, outlineSize, atlasCharSetFile );

    if( text )
    {
		return FontResource::Create( text );
    }
    else
    {
        return nullptr;
    }
}

namespace
{

TextConstPtr        LoadFontFile( const std::string & file, size_t size, size_t blurSize, size_t outlineSize, const std::wstring & atlasCharSetFile )
{
    auto t = TextHelper::LoadUtf8FileToString( atlasCharSetFile );
	return Text::Create( t, file, size, blurSize, outlineSize ); // FIXME: Text constructor makes to much.
}

std::string         AddPostfixToFileName( const std::string & file, const std::string & postfix )
{
    std::string newFileName = boost::filesystem::basename( file ) + postfix;

    auto newPath = boost::filesystem::path( file );
    newPath.remove_filename() /= newFileName;

    return newPath.replace_extension( boost::filesystem::extension( file ) ).string();
}

}

///////////////////////////////
//
TextConstPtr		FontLoader::TryLoadFont( const std::string & file, size_t size, size_t blurSize, size_t oulineSize, const std::wstring & atlasCharSetFile ) const
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

} // model
} // bv