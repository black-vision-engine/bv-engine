#include "TextureAssetThumbnail.h"

namespace bv
{

// ******************************
//
TextureAssetThumbnail::TextureAssetThumbnail    ()
{
}

// ******************************
//
TextureAssetThumbnail::~TextureAssetThumbnail  ()
{
}

// ******************************
//
TextureAssetThumbnailConstPtr       TextureAssetThumbnail::Create      ()
{
    return TextureAssetThumbnailConstPtr( new TextureAssetThumbnail() );
}

// ******************************
//
const char *                        TextureAssetThumbnail::Data        () const
{
    return nullptr;
}

// ******************************
//
const char *                        TextureAssetThumbnail::DataBase64  () const
{
    return nullptr;
}

} // bv