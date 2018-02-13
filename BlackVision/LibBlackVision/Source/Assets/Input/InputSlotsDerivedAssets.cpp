#include "stdafx.h"

#include "Assets/Input/TextureInputAssetDesc.h"
#include "Assets/Input/VideoInput/VideoInputTextureAssetDesc.h"


// ========================================================================= //
// For TextureInputAsset base classes
// ========================================================================= //


namespace bv
{

// ***********************
//
bool                    IsTextureSlotAssetDesc          ( const std::string & uid )
{
    if( uid == TextureInputAssetDesc::UID() )
        return true;

    if( uid == VideoInputTextureAssetDesc::UID() )
        return true;

    return false;
}



}	// bv

