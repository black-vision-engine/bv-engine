#pragma once

#include "MeshAssetAccessor.h"

namespace bv
{

class SVGAssetAccessor;
DEFINE_CONST_PTR_TYPE( SVGAssetAccessor );

class SVGAssetAccessor : public MeshAssetAccessor
{
public:
    static SVGAssetAccessorConstPtr Create          ( const Path & rootPath, const StringVector & fileExts );
    
    virtual AssetDescConstPtr	GetAssetDesc		( const Path & path ) const override;
private:
    explicit                    SVGAssetAccessor    ( const Path & rootPath, const StringVector & fileExts );
};

}
