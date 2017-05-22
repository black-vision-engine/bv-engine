#pragma once

#include "Engine/Models/Plugins/Channels/Audio/DefaultAudioChannel.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"
#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"

#include "Engine/Models/Plugins/Simple/AVStreamDecoder/Interfaces/IAVDecoder.h"

#include "Assets/AVStream/AVAssetDescriptor.h"

#include "Engine/Events/Events.h"


namespace bv { namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultAudioDecoderPluginDesc : public BasePluginDescriptor
{
public:

                                DefaultAudioDecoderPluginDesc   ();

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;

    static  std::string                     UID                 ();

    static  std::string                     AudioName           ();

};

// ***************************** PLUGIN ********************************** 
class DefaultAudioDecoderPlugin : public BasePlugin
{
public:

    enum DecoderMode : int { STOP, PLAY, PAUSE, RESTART };

    struct PARAM
    {
        static const std::string DECODER_STATE;
        static const std::string SEEK_OFFSET;
        static const std::string LOOP_ENABLED;
        static const std::string LOOP_COUNT;
		static const std::string GAIN;
    };

private:

    typedef std::shared_ptr< ParamEnum< DecoderMode > >     DecoderModeParamPtr;

private:

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
	ParamFloatPtr						m_gainParam;

    AVAssetDescConstPtr                 m_assetDesc;

    bool                                m_isFinished;

public:

    explicit							        DefaultAudioDecoderPlugin	( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
                                                ~DefaultAudioDecoderPlugin  ();

    virtual bool                                IsValid						() const override;

    virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr ) override;

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
    void                                        UploadAudioFrame            ();

    void                                        BroadcastHasFinishedEvent   ();
    void                                        TriggerAudioEvent           ( AssetTrackerInternalEvent::Command command );

	MemoryChunkPtr								ApplyGain					( const MemoryChunkPtr & audioFrameData ) const;

};

// ***********************
//
template<>
inline bool SetParameter< DefaultAudioDecoderPlugin::DecoderMode >( IParameterPtr param, TimeType t, const DefaultAudioDecoderPlugin::DecoderMode & val )
{
    typedef ParamEnum< DefaultAudioDecoderPlugin::DecoderMode > ParamType;

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