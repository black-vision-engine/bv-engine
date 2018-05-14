#pragma once

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/ResourceStateModel.h"

#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"

#include "Assets/Input/VideoInput/VideoInputAsset.h"

#include "Engine/Models/Plugins/Simple/ShaderPlugins/TexturePluginBase.h"



namespace bv {
namespace model {

// ***************************** DESCRIPTOR **********************************
class VideoInputPluginDesc : public BasePluginDescriptor
{
public:

    VideoInputPluginDesc                               				();

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;

    static  std::string                     UID                 ();

    static  std::string                     FillTextureName     ();
    static  std::string                     KeyTextureName      ();

};



/**@brief Video input plugin

Plugin can load video and audio input from video card.

<b>Name</b>: video input

<b>Name abbreviation</b>: vidin

<b>UID:</b> VIDEO_INPUT_PLUGIN

@section VideoInputPlugin_Parameters Parameters

Parameter name         	| Type      	| Default Value     | Description
----------------------- | -------------	| ----------------- | -----------
alpha                   | float         | 1.0f              | Alpha to attenuate loaded texture.
txMat                   | transform     |                   | Asset transformation applied only to texture loaded by this plugin.
gain                    | float         | 1.0f              | Audio from video input gain.
enableKey               | bool          | false             | Enable/Disable video input key texture.

@section VideoInputPlugin_Assets Assets

- <b>VideoInput0</b>

    VideoInputAsset to display.

@subsection VideoInputPlugin_Assets_Loading Asset Loading

Set <b>VideoFillIdx</b> and <b>VideoKeyIdx</b> field to input index configured as <b>linkedVideoInput</b> in config file.
@ref Assets_List_VideoInputAssets "Video input assets"


@ingroup PluginsList*/
class VideoInputPlugin : public TexturePluginBase
{
public:

    struct PARAMS
    {
        static const std::string        ALPHA;
        static const std::string        ENABLE_KEY;
        static const std::string        TX_MAT;
        static const std::string        GAIN;
    };

protected:

    DefaultVertexShaderChannelPtr   m_vsc;

    VertexAttributesChannelPtr      m_vaChannel;
    DefaultAudioChannelPtr          m_audioChannel;

    VideoInputAssetConstPtr         m_videoInputAsset;
    UInt64                          m_lastAudioUpdateID;

    ValueFloatPtr                   m_gainValue;

    bool                            m_deferredPlay;

public:

    explicit                                    VideoInputPlugin   ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
    ~VideoInputPlugin  ();

    virtual bool                                IsValid						() const override;
    virtual std::string                         GetTextureName              ( UInt32 idx = 0 ) const override;

    virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr ) override;

    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;
    virtual IAudioChannelPtr                    GetAudioChannel             () const override;

    virtual void                                Update                      ( TimeType t ) override;

private:

    void										InitVertexAttributesChannel ();

    virtual bool								SetPrevPlugin               ( IPluginPtr plugin ) override;

    void                                        LoadVideoInputTexture       ( VideoInputAssetConstPtr videoAsset, AssetDescConstPtr desc );
    void                                        LoadVideoInputTexture       ( VideoInputTextureAssetConstPtr videoAsset, AssetDescConstPtr desc, UInt32 idx );
    void                                        LoadVideoInputAudio         ( VideoInputAssetConstPtr videoAsset, AssetDescConstPtr desc );

    void                                        ReplaceVideoTexture         ( const AssetDescConstPtr & assetDesc, ITextureDescriptorPtr texDesc, UInt32 texIdx = 0 );

    void                                        UpdateAudio                 ();
    MemoryChunkPtr								ApplyGain					( MemoryChunkConstPtr audioFrameData ) const;

    void                                        TriggerAudioEvent           ( AssetTrackerInternalEvent::Command command );
};

DEFINE_PTR_TYPE( VideoInputPlugin )

} // model
} // bv


