#include "stdafx.h"

#include "Font3DLoader.h"
#include "Assets/Font/3D/FontAsset3DDesc.h"
#include "Assets/Font/3D/FontAsset3D.h"

#include "Serialization/Json/JsonDeserializeObject.h"
#include "Serialization/Json/JsonSerializeObject.h"
#include "DataTypes/Hash.h"
#include "System/Path.h"
#include "IO/FileIO.h"
#include "ProjectManager.h"


namespace bv {


// ***********************
//
AssetConstPtr       Font3DLoader::LoadAsset( const bv::AssetDescConstPtr & desc ) const
{
    auto typedDesc = QueryTypedDesc< FontAsset3DDescConstPtr >( desc );

    assert( typedDesc );

    auto filePath = ProjectManager::GetInstance()->ToAbsPath( typedDesc->GetFontFileName() ).Str();
    auto atlasCharSetFile = typedDesc->GetAtlasCharSetFile();
    auto fontSize = typedDesc->GetFontSize();
    auto blurSize = 0;
    auto glowBlurSize = 0;
    auto outlineSize = 0;
    auto generateMipMaps = false;

    auto text = TryLoadFont( filePath, fontSize, blurSize, glowBlurSize, outlineSize, generateMipMaps, atlasCharSetFile );

    if( text )
    {
        return FontAsset3D::Create( text );
    }
    else
    {
        return nullptr;
    }
}

// ***********************
//
AssetDescConstPtr   Font3DLoader::CreateDescriptor	( const IDeserializer& deserializeObject ) const
{
    return std::static_pointer_cast< const AssetDesc >( FontAsset3DDesc::Create( deserializeObject ) );
}

// ***********************
//
ThumbnailConstPtr   Font3DLoader::LoadThumbnail     ( const AssetDescConstPtr & ) const
{
    // Use thumbnail from FontAsset2D.
    return nullptr;
}


} // bv



