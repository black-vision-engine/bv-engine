#include "FontLoader.h"
#include "Text.h"
#include "Assets/Font/FontAssetDescriptor.h"
#include "Assets/Font/FontAsset.h"
#include "System/Path.h"
#include "IO/FileIO.h"
#include "ProjectManager.h"

#include <boost/filesystem/convenience.hpp>
#include <assert.h>

namespace bv {

///////////////////////////////
//
AssetConstPtr FontLoader::LoadAsset( const bv::AssetDescConstPtr & desc ) const
{
	auto typedDesc = QueryTypedDesc< FontAssetDescConstPtr >( desc );

	assert( typedDesc );

    auto filePath			= ProjectManager::GetInstance()->ToAbsPath( typedDesc->GetFontFileName() ).Str();
    auto atlasCharSetFile	= typedDesc->GetAtlasCharSetFile();
    auto fontSize			= typedDesc->GetFontSize();
    auto blurSize			= typedDesc->GetBlurSize();
	auto outlineSize		= typedDesc->GetOutlineSize();
	auto generateMipMaps	= typedDesc->GetGenerateMipmaps();

    auto text				= TryLoadFont( filePath, fontSize, blurSize, outlineSize, generateMipMaps, atlasCharSetFile );

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
TextConstPtr        LoadFontFile( const std::string & file, UInt32 size, UInt32 blurSize, UInt32 outlineSize, bool generateMipMaps, const std::wstring & atlasCharSetFile )
{
	return Text::Create( atlasCharSetFile, file, size, blurSize, outlineSize, generateMipMaps ); // FIXME: Text constructor makes to much.
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
TextConstPtr		FontLoader::TryLoadFont( const std::string & file, UInt32 size, UInt32 blurSize, UInt32 oulineSize, bool generateMipMaps, const std::wstring & atlasCharSetFile ) const
{
    if( Path::Exists(file) )
    {
        return LoadFontFile( file, size, blurSize, oulineSize, generateMipMaps, atlasCharSetFile );
    }
    else
    {
        return nullptr;
    }
}


AssetDescConstPtr FontLoader::CreateDescriptor	( ISerializer& deserializeObject ) const
{
	return std::static_pointer_cast<const AssetDesc>( FontAssetDesc::Create( deserializeObject ) );
}

} // bv