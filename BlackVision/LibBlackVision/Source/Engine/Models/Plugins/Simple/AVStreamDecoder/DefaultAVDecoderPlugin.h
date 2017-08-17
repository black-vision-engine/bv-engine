#pragma once

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/Audio/DefaultAudioChannel.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"
#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"

#include "Engine/Models/Plugins/Simple/SpecialPlugins/BlendHelper.h"

#include "Engine/Models/Plugins/Simple/AVStreamDecoder/Interfaces/IAVDecoder.h"

#include "Assets/AVStream/AVAssetDescriptor.h"

namespace bv { namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultAVDecoderPluginDesc : public BasePluginDescriptor
{
public:

	                                DefaultAVDecoderPluginDesc  ();

	virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
	virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;

	static  std::string                     UID                 ();

	static  std::string                     TextureName         ();

};

// ***************************** PLUGIN ********************************** 
class DefaultAVDecoderPlugin : public BasePlugin
{
public:

    enum DecoderMode : int { STOP, PLAY, PAUSE, RESTART };

    struct PARAM
    {
        static const std::string ALPHA;
        static const std::string TX_MAT;
        static const std::string DECODER_STATE;
        static const std::string SEEK_OFFSET;
        static const std::string LOOP_ENABLED;
        static const std::string LOOP_COUNT;
        static const std::string MUTE;
		static const std::string GAIN;
    };

private:

    typedef std::shared_ptr< ParamEnum< DecoderMode > >     DecoderModeParamPtr;

private:

    DefaultPixelShaderChannelPtr        m_psc;
    DefaultVertexShaderChannelPtr       m_vsc;

    VertexAttributesChannelPtr          m_vaChannel;

    DefaultAudioChannelPtr              m_audioChannel;

	IAVDecoderPtr				        m_decoder;
	DecoderMode				            m_decoderMode;

    TimeType                            m_prevDecoderModeTime;
    TimeType                            m_prevOffsetTime;

	DecoderModeParamPtr                 m_decoderModeParam;

	/** time in seconds from the beginning of video */
    ParamFloatPtr                       m_offsetParam;                 

    ParamBoolPtr                        m_loopEnabledParam;                 
    ParamIntPtr                         m_loopCountParam;
    UInt32                              m_loopCount;

    ParamBoolPtr                        m_muteParam;                 
	ParamFloatPtr                       m_gainParam;

    AVAssetDescConstPtr                 m_assetDesc;

    bool                                m_isFinished;

	ValueParamState< bool >						m_blendEnabled;
	ValueParamState< BlendHelper::BlendMode >	m_blendMode;

public:

    explicit							        DefaultAVDecoderPlugin		( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
                                                ~DefaultAVDecoderPlugin     ();

    virtual bool                                IsValid						() const override;

    virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr ) override;

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
    virtual IPixelShaderChannelPtr		        GetPixelShaderChannel       () const override;
    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;
    virtual IAudioChannelPtr                    GetAudioChannel             () const override;

    virtual void                                Update                      ( TimeType t ) override;

    virtual bool								SetPrevPlugin               ( IPluginPtr plugin ) override;

private:

    void                                        Play                        ();
    void                                        Stop                        ();
    void                                        Pause                       ();

    void                                        HandlePerfectLoops          ();

    void                                        UpdateDecoderState          ( DecoderMode mode );

    void                                        UpdateDecoder               ();
    void                                        UploadVideoFrame            ();
    void                                        UploadAudioFrame            ();

    void                                        InitVertexAttributesChannel ();

	MemoryChunkPtr								ApplyGain					( const MemoryChunkPtr & audioFrameData ) const;

    void                                        BroadcastHasFinishedEvent   ();
    void                                        TriggerEvent                ( AssetTrackerInternalEvent::Command command );

};


DEFINE_ENUM_SET_PARAMETER( DefaultAVDecoderPlugin::DecoderMode );


} //model
} //bv