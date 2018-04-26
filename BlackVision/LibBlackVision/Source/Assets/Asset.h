#pragma once

#include <string>
#include "CoreDEF.h"
#include "Assets/FwdDecls.h"
#include "Memory/MemoryChunk.h"



/**@page Assets Assets

[TOC]

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

Assets are loaded from ProjectManager directory. This directory can be set in @ref Config "config" file as <b>PMFolder</b>.

@subsection Assets_Loading_AssetsDescriptor Asset Descriptors

To load asset, external tool must send asset descriptor. Asset descriptor contains resource location
and other information needed to process this asset.

The best way to create @ref bv::AssetDesc "AssetDescriptor", is to use @ref InfoEvent_Commands_GetAssetDescriptor "GetAssetDescriptor" command.
It returns chosen @ref bv::AssetDesc "AssetDescriptor" template. This template is filled with default values. External tool should override
what's necessary.

@subsection Assets_Loading_LoadingCommands Loading commands

To load asset to @ref Plugins "Plugin", you must send @ref LoadAssetEvent_Commands_LoadAsset "LoadAsset" command.

@section Assets_List List of Assets

Serialized descriptors below contain content returned by @ref InfoEvent_Commands_GetAssetDescriptor "GetAssetDescriptor" command.
Some values aren't required to load asset and exist only to inform editor about asset being loaded.

@subsection Assets_List_Texture Texture Asset
@copydetails bv::TextureAsset

@subsection Assets_List_Font2D Font 2D Asset
@copydetails bv::FontAsset2D

@subsection Assets_List_Font3D Font 3D Asset
@copydetails bv::FontAsset3D

@subsection Assets_List_MeshAsset Mesh Asset
@copydetails bv::MeshAsset

@subsection Assets_List_SVGAsset SVG Asset
@copydetails bv::SVGAsset

@subsection Assets_List_AVAsset Audio/Video Asset
@copydetails bv::AVAsset

@subsection Assets_List_DataArrayAsset DataArray Asset
@copydetails bv::DataArrayAsset

@subsection Assets_List_InputSlotsAssets Input Slots Assets

Input Slots Assets allow to access global table of texture and audio sources.
Slots can be accessed by index or by name.

<b>Binding by index Example:</b>

@code{.json}
{
    "asset" :
    {
        "slotBinding" :
        {
           "index" : "0",
           "type" : "ByIndex"
        },
        "type" : "TEXTURE_INPUT_ASSET_DESC"
    }
}
@endcode

<b>Binding by name Example:</b>

@code{.json}
{
    "asset" :
    {
        "slotBinding" :
        {
           "name" : "Input",
           "type" : "ByName"
        },
        "type" : "TEXTURE_INPUT_ASSET_DESC"
    }
}
@endcode


-                       | UID                       | Class
----------------------- | ------------------------- | -----------
Descriptor              | TEXTURE_INPUT_ASSET_DESC  | @ref bv::TextureInputAssetDesc
Asset                   | TEXTURE_INPUT_ASSET       | @ref bv::TextureInputAsset
Descriptor              | AUDIO_INPUT_ASSET_DESC    | @ref bv::AudioInputAssetDesc
Asset                   | AUDIO_INPUT_ASSET         | @ref bv::AudioInputAsset


@subsection Assets_List_VideoInputAssets Video Input Assets

There are special descriptors for @ref EngineInputs_VideoInput "Video Input".
They use abstraction layer, which maps VideoInputIdx defined in config to Input Slots.

-                       | UID                               | Class
----------------------- | --------------------------------- | -----------
Descriptor              | VIDEO_INPUT_TEXTURE_ASSET_DESC    | @ref bv::VideoInputTextureAssetDesc
Asset                   | VIDEO_INPUT_TEXTURE_ASSET         | @ref bv::VideoInputTextureAsset
Descriptor              | VIDEO_INPUT_AUDIO_ASSET_DESC      | @ref bv::VideoInputAudioAssetDesc
Asset                   | VIDEO_INPUT_AUDIO_ASSET           | @ref bv::VideoInputAudioAsset
Descriptor              | VIDEO_INPUT_ASSET_DESC            | @ref bv::VideoInputAssetDesc
Asset                   | VIDEO_INPUT_ASSET                 | @ref bv::VideoInputAsset


First two descriptors (@ref bv::VideoInputTextureAssetDesc and @ref bv::VideoInputAudioAssetDesc) can be used in all @ref Plugins "Plugins"
that support @ref Assets_List_InputSlotsAssets "Input Slot Asset".
@ref bv::VideoInputAsset can be used only with @ref bv::model::VideoInputPlugin and it connects Fill and Key textures
of Video Input. Audio is taken from Fill texture.


<b>Audio slot:</b>

@code{.json}
{
    "asset" : 
    {
        "VideoInputIdx" : "0",
        "type" : "VIDEO_INPUT_AUDIO_ASSET_DESC"
    }
}
@endcode

<b>Texture slot:</b>

@code{.json}
{
    "asset" : 
    {
        "VideoInputIdx" : "0",
        "VideoType" : "Fill",
        "type" : "VIDEO_INPUT_TEXTURE_ASSET_DESC"
    }
}
@endcode

<b>Example of Video Input Asset descriptor used by @ref bv::model::VideoInputPlugin.</b>

@code{.json}
{
    "asset" : 
    {
        "VideoFillIdx" : "0",
        "VideoKeyIdx" : "1",
        "type" : "VIDEO_INPUT_ASSET_DESC"
    }
}
@endcode


*/



namespace bv {



/**@brief Asset base class.
@ingroup Assets
@see Assets*/
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
