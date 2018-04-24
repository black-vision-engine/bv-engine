#pragma once

#include <string>
#include "CoreDEF.h"
#include "Assets/FwdDecls.h"
#include "Memory/MemoryChunk.h"



/**@page Assets Assets

@section Assets_General General

Assets are used to load external (not generated) content like:
- Textures
- Animation Sequences
- Fonts
- Audio
- Video
- Meshes
- Vector Shapes
- Data
- Video Input

@section Assets_Loading Loading Assets

@subsection Assets_Loading_AssetsDescriptor Asset Descriptors

To load asset, external tool must send asset descriptor. Asset descriptor contains resource location
and other information needed to process this asset.

The best way to create @ref AssetDesc "AssetDescriptor", is to use @ref InfoEvent_Commands_GetAssetDescriptor "GetAssetDescriptor" command.
It returns chosen @ref AssetDesc "AssetDescriptor" template. This template is filled with default values. External tool should override
what's necessary.

@subsection Assets_Loading_LoadingCommands Loading commands



*/



namespace bv {



/**@brief Asset base class.
@ingroup Model*/
class Asset
{
protected:
    virtual VoidConstPtr					QueryThis	() const = 0;

public:

    virtual const std::string &				GetUID		() const = 0;

    virtual ~Asset(){}

    template< typename AssetTypeConstPtr >
    friend AssetTypeConstPtr  QueryTypedRes( AssetConstPtr asset );

};

// ****************************
//
template< typename AssetTypeConstPtr >
AssetTypeConstPtr  QueryTypedRes( AssetConstPtr asset )
{
    if( asset->GetUID() != AssetTypeConstPtr::element_type::UID() )
    {
        return nullptr;
    }

    return std::static_pointer_cast< AssetTypeConstPtr::element_type >( asset->QueryThis() );
}



} // bv
