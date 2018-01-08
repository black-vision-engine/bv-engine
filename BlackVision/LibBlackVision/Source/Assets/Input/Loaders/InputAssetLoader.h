#pragma once


#include "Assets/AssetLoader.h"

#include "Engine/Graphics/InputSlots/InputSlots.h"


namespace bv
{


// ***********************
//
class InputAssetLoader : public AssetLoader
{
private:

    InputSlotsPtr       m_inputSlots;

public:

    explicit            InputAssetLoader    ( InputSlotsPtr slots );
    virtual             ~InputAssetLoader   (){}


    AssetConstPtr						LoadAsset			( const AssetDescConstPtr & desc )          const override;
    AssetDescConstPtr					CreateDescriptor	( const IDeserializer& deserializeObject )  const override;

    ThumbnailConstPtr                   LoadThumbnail       ( const AssetDescConstPtr & desc )          const override;

    
};



}	// bv
