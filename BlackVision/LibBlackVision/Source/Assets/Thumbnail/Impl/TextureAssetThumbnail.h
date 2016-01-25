#pragma once

#include "Assets/Thumbnail/Thumbnail.h"

namespace bv
{

class TextureAssetThumbnail;
DEFINE_CONST_PTR_TYPE( TextureAssetThumbnail )

class TextureAssetThumbnail : public Thumbnail
{
private:
    explicit    TextureAssetThumbnail   ();

public:
    virtual const char *                    Data        () const override;
    virtual const char *                    DataBase64  () const override;

    static TextureAssetThumbnailConstPtr    Create      ();

    virtual     ~TextureAssetThumbnail  ();
};

} // bv