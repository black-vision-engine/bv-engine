#pragma once

#include "Engine/Models/Resources/ResourceLoader.h"
#include "Engine/Models/Resources/Font/Text.h"
#include "Engine/Types/Enums.h"


namespace bv { namespace model {

class FontLoader : public ResourceLoader
{
private:

    TextConstPtr            TryLoadFont         ( const std::string & file, size_t size, size_t blurSize, size_t oulineSize, const std::wstring & atlasCharSetFile ) const;

public:
	IResourceNEWConstPtr	LoadResource		( const bv::ResourceDescConstPtr & ) const override;

    explicit                FontLoader( ){}

    virtual ~FontLoader(){}
};

} // model
} // bv