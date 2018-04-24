#pragma once

#include "Assets/Asset.h"
#include "Assets/Font/FontAsset.h"
#include "Assets/Font/3D/TextGeometry.h"

#include "Assets/AssetManager.h"			// Only for LoadTypedAsset template specialization

namespace bv
{

class FontAsset3D;
DEFINE_CONST_PTR_TYPE( FontAsset3D )



// *******************************
/// Implements font asset with full size atlas and all its mimmaps.
/// @ingroup Assets
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

