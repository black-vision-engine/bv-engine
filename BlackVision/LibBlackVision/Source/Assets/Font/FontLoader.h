#pragma once

#include "Assets/AssetLoader.h"
#include "Assets/Font/Text.h"
#include "Engine/Types/Enums.h"


namespace bv { namespace model {

class FontLoader : public ResourceLoader
{
private:

    TextConstPtr            TryLoadFont         ( const std::string & file, UInt32 size, UInt32 blurSize, UInt32 oulineSize, const std::wstring & atlasCharSetFile ) const;

public:
	ResourceConstPtr		LoadResource		( const bv::ResourceDescConstPtr & ) const override;

    explicit                FontLoader( ){}

    virtual ~FontLoader(){}
};

} // model
} // bv