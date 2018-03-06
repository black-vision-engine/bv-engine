#pragma once

#include "Assets/Input/TextureInputAssetDesc.h"




namespace bv {
namespace model
{


// ***********************
//
class TextureInputAssetComparator
{
public:
    static bool         CompareStructure    ( TextureInputAssetDescConstPtr expected, const TextureInputAssetDescConstPtr actual )
    {
        auto & expectedBinding = expected->BindingInfo();
        auto & actualBinding = actual->BindingInfo();

        if( expectedBinding.GetType() != actualBinding.GetType() )
            return false;

        if( expectedBinding.GetName() != actualBinding.GetName() )
            return false;

        if( expectedBinding.GetIndex() != actualBinding.GetIndex() )
            return false;

        return true;
    }
};

}

}	// bv

