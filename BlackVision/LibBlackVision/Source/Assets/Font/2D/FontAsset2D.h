#pragma once

#include "Assets/Asset.h"
#include "Assets/Font/FontAsset.h"
#include "Assets/Font/2D/TextAtlas.h"

#include "Assets/AssetManager.h"			// Only for LoadTypedAsset template specialization

namespace bv
{

class FontAsset2D;
DEFINE_CONST_PTR_TYPE( FontAsset2D )



// *******************************
/// Implements font asset with full size atlas and all its mimmaps.
/// @ingroup Assets
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


