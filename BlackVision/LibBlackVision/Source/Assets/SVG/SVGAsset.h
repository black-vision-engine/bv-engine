#pragma once

#include "Assets/Asset.h"

#include "Assets/Mesh/MeshAsset.h"

namespace bv {



/**@brief Vector graphics asset.

Loads SVG files.

-                       | UID                       | Class
----------------------- | ------------------------- | -----------
Descriptor              | SVG_ASSET_DESC            | @ref bv::SVGAssetDescriptor
Asset                   | MESH_ASSET                | @ref bv::SVGAsset

<b>Example serialized descriptor:</b>

@code{.json}
{
    "asset" :
    {
        "path" : "svgs\\heart.svg",
        "type" : "SVG_ASSET_DESC"
    }
}
@endcode

<b>Parameters</b>

Parameter name         	| Type                              | Required   | Description
----------------------- | --------------------------------- | ---------- | --------------------------
type                    | string                            | yes        | Asset descriptor UID.
path                    | string                            | yes        | Path to asset. 

@ingroup Assets*/
class SVGAsset : public MeshAsset
{
public:
                                        SVGAsset        ( const std::string & key );
};

DEFINE_PTR_TYPE( SVGAsset );
DEFINE_CONST_PTR_TYPE( SVGAsset );

}
