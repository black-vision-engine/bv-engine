#pragma once

#include "Assets/Font/FontLoader.h"



namespace bv {

class Font3DLoader : public FontLoader
{
private:
public:
    AssetConstPtr			LoadAsset			( const bv::AssetDescConstPtr & ) const override;
    AssetDescConstPtr		CreateDescriptor	( const IDeserializer& deserializeObject ) const override;

    ThumbnailConstPtr       LoadThumbnail       ( const AssetDescConstPtr & desc ) const override;


    explicit                Font3DLoader    () {}
    virtual                 ~Font3DLoader   () {}
};



} // bv


