#pragma once

#include "Assets/AssetLoader.h"
#include "Assets/Font/Text.h"
#include "Engine/Types/Enums.h"
#include "Serialization/ISerializer.h"

namespace bv {

class FontLoader : public AssetLoader
{
private:

    TextConstPtr            TryLoadFont         (	const std::string & file, UInt32 size, UInt32 blurSize, UInt32 oulineSize,
													bool generateMipMaps, const std::wstring & atlasCharSetFile ) const;

public:
	AssetConstPtr			LoadAsset			( const bv::AssetDescConstPtr & ) const override;
	AssetDescConstPtr		CreateDescriptor	( ISerializer& deserializeObject ) const override;

    explicit                FontLoader( ){}

    virtual ~FontLoader(){}
};



} // bv