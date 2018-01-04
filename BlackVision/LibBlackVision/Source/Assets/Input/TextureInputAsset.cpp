#include "stdafx.h"
#include "TextureInputAsseth.h"




namespace bv
{

const std::string       TextureInputAsset::uid = "TEXTURE_INPUT_ASSET";



// ***********************
//
const std::string &         TextureInputAsset::GetUID       () const
{
    return UID();
}

// ***********************
//
const std::string &         TextureInputAsset::UID          ()
{
    return uid;
}


}	// bv


