#pragma once

#include "Engine/Models/Plugins/Channels/Audio/DefaultAudioChannel.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"
#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"

#include "Engine/Models/Plugins/Simple/AVStreamDecoder/Interfaces/IAVDecoder.h"

#include "Assets/AVStream/AVAssetDescriptor.h"

#include "Engine/Events/Events.h"


namespace bv {
namespace model {

// ***************************** DESCRIPTOR **********************************
class FakeAudioPlayerPluginDesc : public BasePluginDescriptor
{
public:

    FakeAudioPlayerPluginDesc   ();

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;

    static  std::string                     UID                 ();

    static  std::string                     AudioName           ();

};

// ***************************** PLUGIN ********************************** 
class FakeAudioPlayerPlugin : public BasePlugin
{
public:

    struct PARAM
    {
        static const std::string LOOP_ENABLED;
        static const std::string LOOP_COUNT;
        static const std::string GAIN;
    };

private:

    DefaultAudioChannelPtr              m_audioChannel;

    ParamBoolPtr                        m_loopEnabledParam;
    ParamIntPtr                         m_loopCountParam;
    ParamFloatPtr						m_gainParam;

    UInt32                              m_loopCount;
    SizeType                            m_signalOffset;

    MemoryChunkPtr                      m_signalSource;

public:

    explicit							        FakeAudioPlayerPlugin   	( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
    ~FakeAudioPlayerPlugin  ();

    virtual bool                                IsValid						() const override;

    virtual IAudioChannelPtr                    GetAudioChannel             () const override;

    virtual void                                Update                      ( TimeType t ) override;

    virtual bool								SetPrevPlugin               ( IPluginPtr plugin ) override;

private:

    void                                        UploadAudioFrame            ();

    SizeType                                    ComputeFrameSamples         ();

    MemoryChunkPtr                              GenerateNextFrame           ();
    MemoryChunkPtr								ApplyGain					( const MemoryChunkPtr & audioFrameData ) const;

public:


    /**@brief Sets source of fake audio data.
    Memory chunk will be played until loop count will reach 0.*/
    void                    SetSignalSource     ( MemoryChunkPtr signal );

};

DEFINE_PTR_TYPE( FakeAudioPlayerPlugin )


} //model
} //bv