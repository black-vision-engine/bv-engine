#pragma once

#include "Assets/Asset.h"
#include "Assets/Font/FontAsset.h"
#include "Assets/Font/2D/TextAtlas.h"

#include "Assets/AssetManager.h"			// Only for LoadTypedAsset template specialization

namespace bv
{

class FontAsset2D;
DEFINE_CONST_PTR_TYPE( FontAsset2D )



/**@brief Implements font asset with full size atlas and all its mimmaps.
@details
Loads bitmap font based on atlas.

<b>Supported formats:</b> ttf

-                       | UID                       | Class
----------------------- | ------------------------- | -----------
Descriptor              | FONT_ASSET_DESC           | @ref bv::FontAssetDesc
Asset                   | FONT_ASSET                | @ref bv::FontAsset2D

<b>Example serialized descriptor:</b>

@code{.json}
{
    "asset" :
    {
        "blur" : "0",
        "glowBlur" : "0",
        "mipmaps" : "true",
        "outline" : "0",
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
blur                    | UInt32                            | yes        | Blur size.
outline                 | UInt32                            | yes        | Outline size.
size                    | UInt32                            | yes        | Font size.
mipmaps                 | bool                              | yes        | Use mipmaps. It's recomended to set true.
path                    | string                            | yes        | Path to asset.


@ingroup Assets*/
class FontAsset2D : public FontAsset, public std::enable_shared_from_this< FontAsset2D >
{
    static const std::string		uid;

protected:
    virtual VoidConstPtr			QueryThis		() const override;

public:

    static FontAsset2DConstPtr		Create			( const TextConstPtr & textAtlas );

    virtual const std::string &		GetUID			() const override;
    static const std::string &		UID				();

    TextAtlasConstPtr               GetTextAtlas    () const;

private:

    explicit						FontAsset2D		( const TextConstPtr & textAtlas );
};



} // bv


