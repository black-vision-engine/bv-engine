#include "FontLoader.h"
#include "Text.h"
#include "Assets/Font/FontAssetDescriptor.h"
#include "Assets/Font/FontAsset.h"
#include "IO/FileIO.h"


#include <boost/filesystem/convenience.hpp>
#include <assert.h>

namespace bv {

///////////////////////////////
//
AssetConstPtr FontLoader::LoadAsset( const bv::AssetDescConstPtr & desc ) const
{
	auto typedDesc = QueryTypedDesc< FontAssetDescConstPtr >( desc );

	assert( typedDesc );

	auto filePath			= typedDesc->GetFontFileName();
    auto atlasCharSetFile	= typedDesc->GetAtlasCharSetFile();
    auto fontSize			= typedDesc->GetFontSize();
    auto blurSize			= typedDesc->GetBlurSize();
	auto outlineSize		= typedDesc->GetOutlineSize();

    auto text				= TryLoadFont( filePath, fontSize, blurSize, outlineSize, atlasCharSetFile );

    if( text )
    {
		return FontAsset::Create( text );
    }
    else
    {
        return nullptr;
    }
}

namespace
{

// *******************************
//
size_t GetSizeOfFile( const std::wstring& path )
{
	struct _stat fileinfo;
	_wstat(path.c_str(), &fileinfo);
	return fileinfo.st_size;
}

// *******************************
//
std::wstring LoadUtf8FileToString(const std::wstring& filename)
{
	std::wstring buffer;            // stores file contents
	FILE* f = nullptr;
    _wfopen_s(&f, filename.c_str(), L"rtS, ccs=UTF-8");

	// Failed to open file
	if (f == NULL)
	{
		// ...handle some error...
		return buffer;
	}

	size_t filesize = GetSizeOfFile(filename);

	// Read entire file contents in to memory
	if (filesize > 0)
	{
		buffer.resize(filesize);
		size_t wchars_read = fread(&(buffer.front()), sizeof(wchar_t), filesize, f);
		buffer.resize(wchars_read);
		buffer.shrink_to_fit();
	}

	fclose(f);

	return buffer;
}

// *******************************
//
TextConstPtr        LoadFontFile( const std::string & file, UInt32 size, UInt32 blurSize, UInt32 outlineSize, const std::wstring & atlasCharSetFile )
{
    auto t = LoadUtf8FileToString( atlasCharSetFile );
	return Text::Create( t, file, size, blurSize, outlineSize ); // FIXME: Text constructor makes to much.
}

// *******************************
//
std::string         AddPostfixToFileName( const std::string & file, const std::string & postfix )
{
    std::string newFileName = boost::filesystem::basename( file ) + postfix;

    auto newPath = boost::filesystem::path( file );
    newPath.remove_filename() /= newFileName;

    return newPath.replace_extension( boost::filesystem::extension( file ) ).string();
}

} // anonymous

///////////////////////////////
//
TextConstPtr		FontLoader::TryLoadFont( const std::string & file, UInt32 size, UInt32 blurSize, UInt32 oulineSize, const std::wstring & atlasCharSetFile ) const
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

} // bv