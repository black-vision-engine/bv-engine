#pragma once


#include "Assets/AssetLoader.h"

#include "Engine/Graphics/InputSlots/SlotsLogic/VideoInput/VideoInputSlots.h"


namespace bv
{


// ***********************
//
class VideoInputAssetLoader : public AssetLoader
{
private:

    VideoInputSlotsPtr      m_inputSlots;

public:

    explicit            VideoInputAssetLoader    ( VideoInputSlotsPtr slots );
    virtual             ~VideoInputAssetLoader   (){}


    AssetConstPtr						LoadAsset			( const AssetDescConstPtr & desc )          const override;
    AssetDescConstPtr					CreateDescriptor	( const IDeserializer& deserializeObject )  const override;

    ThumbnailConstPtr                   LoadThumbnail       ( const AssetDescConstPtr & desc )          const override;


};



}	// bv
