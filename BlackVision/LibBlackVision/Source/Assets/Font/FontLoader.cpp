#include "stdafx.h"

#include "FontLoader.h"
#include "Text.h"
#include "Assets/Font/2D/FontAssetDescriptor.h"
#include "Assets/Font/FontAsset.h"
#include "Assets/Thumbnail/Impl/FontAssetThumbnail.h"
#include "Assets/Texture/TextureUtils.h"
#include "Serialization/Json/JsonDeserializeObject.h"
#include "Serialization/Json/JsonSerializeObject.h"
#include "DataTypes/Hash.h"
#include "System/Path.h"
#include "IO/FileIO.h"
#include "ProjectManager.h"
#include "LibImage.h"

#include <boost/filesystem/convenience.hpp>
#include <assert.h>



#include "Memory/MemoryLeaks.h"



namespace bv {



namespace
{

// *******************************
//
TextConstPtr        LoadFontFile( const std::string & file, UInt32 size, UInt32 blurSize, UInt32 glowBlurSize, UInt32 outlineSize, bool generateMipMaps, const std::wstring & atlasCharSetFile )
{
	return Text::Create( atlasCharSetFile, file, size, blurSize, glowBlurSize, outlineSize, generateMipMaps ); // FIXME: Text constructor makes to much.
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
TextConstPtr		FontLoader::TryLoadFont( const std::string & file, UInt32 size, UInt32 blurSize, UInt32 glowBlurSize, UInt32 oulineSize, bool generateMipMaps, const std::wstring & atlasCharSetFile ) const
{
    if( Path::Exists(file) )
    {
        return LoadFontFile( file, size, blurSize, glowBlurSize, oulineSize, generateMipMaps, atlasCharSetFile );
    }
    else
    {
        return nullptr;
    }
}



} // bv