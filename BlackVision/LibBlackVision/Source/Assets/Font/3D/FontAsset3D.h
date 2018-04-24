#pragma once

#include "Assets/Asset.h"
#include "Assets/Font/FontAsset.h"
#include "Assets/Font/3D/TextGeometry.h"

#include "Assets/AssetManager.h"			// Only for LoadTypedAsset template specialization

namespace bv
{

class FontAsset3D;
DEFINE_CONST_PTR_TYPE( FontAsset3D )



/**@brief Implements font asset with full size atlas and all its mimmaps.
@details
Loads geometric font.

-                       | UID                       | Class
----------------------- | ------------------------- | -----------
Descriptor              | FONT_ASSET_3D_DESC        | @ref bv::FontAssetDesc
Asset                   | FONT_ASSET_3D             | @ref bv::FontAsset2D

<b>Example serialized descriptor:</b>

@code{.json}
{
    "asset" :
    {
        "path" : "fonts\\New Walt Disney.ttf",
        "size" : "0",
        "type" : "FONT_ASSET_DESC"
    }
}
@endcode

<b>Parameters</b>

Parameter name         	| Type                              | Required   | Description
----------------------- | --------------------------------- | ---------- | --------------------------
type                    | string                            | yes        | Asset descriptor UID.
size                    | UInt32                            | yes        | Font size.
path                    | string                            | yes        | Path to asset.


@ingroup Assets*/
class FontAsset3D : public FontAsset, public std::enable_shared_from_this< FontAsset3D >
{
    static const std::string		uid;

protected:
    virtual VoidConstPtr			QueryThis		() const override;

public:

    static FontAsset3DConstPtr		Create			( const TextConstPtr & textAtlas );

    virtual const std::string &		GetUID			() const override;
    static const std::string &		UID				();

    TextGeometryConstPtr            GetTextGeometry () const;

private:

    explicit						FontAsset3D		( const TextConstPtr & textAtlas );
};



} // bv

