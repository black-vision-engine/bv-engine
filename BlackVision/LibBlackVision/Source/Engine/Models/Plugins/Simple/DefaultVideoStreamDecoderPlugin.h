#pragma once

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"
#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegVideoDecoder.h"


namespace bv { namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultVideoStreamDecoderPluginDesc : public BasePluginDescriptor
{
public:

	DefaultVideoStreamDecoderPluginDesc                         ();

	virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
	virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;

	static  std::string                     UID                 ();

	static  std::string                     TextureName         ();

};

// ***************************** PLUGIN ********************************** 
class DefaultVideoStreamDecoderPlugin : public BasePlugin< IPlugin >
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
    };

private:

    typedef std::shared_ptr< ParamEnum< DecoderMode > >     DecoderModeParamPtr;

private:

    DefaultPixelShaderChannelPtr        m_psc;
    DefaultVertexShaderChannelPtr       m_vsc;

    VertexAttributesChannelPtr          m_vaChannel;

	IVideoDecoderPtr				    m_decoder;
	DecoderMode				            m_decoderMode;

    TimeType                            m_prevDecoderModeTime;
    TimeType                            m_prevOffsetTime;

	DecoderModeParamPtr                 m_decoderModeParam;

    UInt32                              m_prevFrameIdx;

	/** time in seconds from the beginning of video */
    ParamVec2Ptr                        m_offsetParam;                 
    Float32                             m_prevOffsetCounter;

    ParamBoolPtr                        m_loopEnabledParam;                 
    ParamIntPtr                         m_loopCountParam;
    UInt32                              m_loopCount;

    VideoStreamAssetDescConstPtr        m_assetDesc;

    bool                                m_isFinished;

public:

    explicit							DefaultVideoStreamDecoderPlugin		( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
                                        ~DefaultVideoStreamDecoderPlugin    ();

    virtual bool                                IsValid						() const override;

    virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr ) override;

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
    virtual IPixelShaderChannelPtr		        GetPixelShaderChannel       () const override;
    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;

    virtual void                                Update                      ( TimeType t ) override;

    virtual void								SetPrevPlugin               ( IPluginPtr plugin ) override;

private:

    void                                        UpdateDecoder               ();
    void                                        UploadVideoFrame            ();

    void                                        InitVertexAttributesChannel ();

    void                                        MarkOffsetChanges           ();

};

// ***********************
//
template<>
inline bool SetParameter< DefaultVideoStreamDecoderPlugin::DecoderMode >( IParameterPtr param, TimeType t, const DefaultVideoStreamDecoderPlugin::DecoderMode & val )
{
    typedef ParamEnum< DefaultVideoStreamDecoderPlugin::DecoderMode > ParamType;

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