#pragma once

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/Audio/DefaultAudioChannel.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"
#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegAVDecoder.h"


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

    enum DecoderMode : int { STOP, PLAY, PAUSE };

    struct PARAM
    {
        static const std::string ALPHA;
        static const std::string TX_MAT;
        static const std::string DECODER_STATE;
        static const std::string SEEK_OFFSET;
        static const std::string LOOP_ENABLED;
        static const std::string LOOP_COUNT;
        static const std::string MUTE;
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
    ParamVec2Ptr                        m_offsetParam;                 
    Float32                             m_prevOffsetCounter;

    ParamBoolPtr                        m_loopEnabledParam;                 
    ParamIntPtr                         m_loopCountParam;
    UInt32                              m_loopCount;

    ParamBoolPtr                        m_muteParam;                 

    AVAssetDescConstPtr                 m_assetDesc;

    bool                                m_isFinished;

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

    virtual void								SetPrevPlugin               ( IPluginPtr plugin ) override;

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

    void                                        MarkOffsetChanges           ();

    void                                        BroadcastHasFinishedEvent   ();
    void                                        TriggerEvent                ( AssetTrackerInternalEvent::Command command );

};

// ***********************
//
template<>
inline bool SetParameter< DefaultAVDecoderPlugin::DecoderMode >( IParameterPtr param, TimeType t, const DefaultAVDecoderPlugin::DecoderMode & val )
{
    typedef ParamEnum< DefaultAVDecoderPlugin::DecoderMode > ParamType;

    ParamType * typedParam = QueryTypedParam< std::shared_ptr< ParamType > >( param ).get();

    if( typedParam == nullptr )
    {
        return false;
    }

    typedParam->SetVal( val, t );

    return true;
}

} //model
} //bv