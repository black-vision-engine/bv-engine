#pragma once

#include "CoreDEF.h"

#include "Assets/AssetManager.h"

namespace bv
{

// ***********************
//
class TEST_ACCESSOR( AssetManager )
{
public:

    static AssetLoaderConstPtr      FindLoader      ( AssetManager & assetManager, const std::string & descUID )
    {
        auto iter = assetManager.m_loaders.find( descUID );
        if( iter != assetManager.m_loaders.end() )
        {
            return iter->second;
        }
        else
            return nullptr;
    }


};




}	// bv

