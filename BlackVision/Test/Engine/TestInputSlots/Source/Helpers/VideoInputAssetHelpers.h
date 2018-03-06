#pragma once


#include "InputAssetHelpers.h"

#include "Engine/Graphics/InputSlots/SlotsLogic/VideoInput/VideoInputSlots.h"

#include "Assets/Input/VideoInput/VideoInputAsset.h"
#include "Assets/Input/VideoInput/VideoInputAssetDesc.h"
#include "Assets/Input/VideoInput/VideoInputTextureAsset.h"
#include "Assets/Input/VideoInput/VideoInputTextureAssetDesc.h"
#include "Assets/Input/VideoInput/VideoInputAudioAssetDesc.h"

#include "Assets/Input/Loaders/VideoInputAssetLoader.h"


namespace bv
{

// ***********************
//
struct VideoInputsContext : public InputsContext
{
    VideoInputSlotsPtr      videoSlots;
};


// ***********************
//
inline VideoInputsContext           CreateVideoInputContext     ()
{
    auto inputContext = CreateInputContext();
    
    VideoInputsContext context;
    context.assetManager = inputContext.assetManager;
    context.slots = inputContext.slots;
    context.videoSlots = std::make_shared< VideoInputSlots >( inputContext.slots );

    auto loader = std::make_shared< VideoInputAssetLoader >( context.videoSlots );

    context.assetManager->RegisterLoader( VideoInputAssetDesc::UID(), loader );
    context.assetManager->RegisterLoader( VideoInputTextureAssetDesc::UID(), loader );
    context.assetManager->RegisterLoader( VideoInputAudioAssetDesc::UID(), loader );

    return context;
}




}	// bv


