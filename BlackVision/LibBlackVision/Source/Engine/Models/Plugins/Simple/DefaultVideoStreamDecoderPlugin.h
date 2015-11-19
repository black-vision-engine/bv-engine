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

	virtual bool                            CanBeAttachedTo     ( IPluginConstPtr plugin )  const override;

	static  std::string                     UID                 ();

	static  std::string                     TextureName         ();

};

// ***************************** PLUGIN ********************************** 
class DefaultVideoStreamDecoderPlugin : public BasePlugin< IPlugin >
{
private:
    DefaultPixelShaderChannelPtr    m_psc;
    DefaultVertexShaderChannelPtr   m_vsc;

    VertexAttributesChannelPtr      m_vaChannel;

	IVideoDecoderPtr				m_decoder;

public:

    explicit							DefaultVideoStreamDecoderPlugin		( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
                                        ~DefaultVideoStreamDecoderPlugin    ();

    virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr ) override;

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
    virtual IPixelShaderChannelConstPtr         GetPixelShaderChannel       () const override;
    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;

    virtual void                                Update                      ( TimeType t ) override;

    virtual void								SetPrevPlugin               ( IPluginPtr plugin ) override;

//---controlling events

	void										Start						();
	void										Pause						();
	void										Stop						();
	/** @param[time] in seconds from the beginning of video */
	void										Seek						( Float64 time );

    static bool									Start						( IPluginPtr plugin );
    static bool									Pause						( IPluginPtr plugin );
    static bool									Stop						( IPluginPtr plugin );
    static bool									Seek						( IPluginPtr plugin, Float64 time );

private:
    void                                        InitVertexAttributesChannel ();
};

} }