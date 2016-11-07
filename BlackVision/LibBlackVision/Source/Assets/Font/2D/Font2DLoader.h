#pragma once

#include "Assets/Font/FontLoader.h"



namespace bv {


class Font2DLoader : public FontLoader
{
private:
public:

    AssetConstPtr			LoadAsset			( const bv::AssetDescConstPtr & ) const override;
    AssetDescConstPtr		CreateDescriptor	( const IDeserializer& deserializeObject ) const override;

    ThumbnailConstPtr       LoadThumbnail       ( const AssetDescConstPtr & desc ) const override;


    explicit                Font2DLoader    () {}
    virtual                 ~Font2DLoader   () {}
};



} // bv


