#pragma once

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"
#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"

#include "VideoStreamDecoderThread.h"

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

	//static  std::string                     VertexShaderSource  ();
	//static  std::string                     PixelShaderSource   ();

	static  std::string                     TextureName         ();

};

// ***************************** PLUGIN ********************************** 
class DefaultVideoStreamDecoderPlugin : public BasePlugin< IPlugin >
{
private:
	DefaultPluginParamValModelPtr   m_paramValModel;

    DefaultPixelShaderChannelPtr    m_psc;
    DefaultVertexShaderChannelPtr   m_vsc;

    VertexAttributesChannelPtr      m_vaChannel;

    DefaultTexturesDataPtr          m_texturesData;

    unsigned int                    m_texCoordChannelIndex;

	VideoStreamDecoderThread *		m_decoderThread;
	UInt32							m_frameId;
	bool							m_started;

public:

    explicit							DefaultVideoStreamDecoderPlugin		( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
                                        ~DefaultVideoStreamDecoderPlugin    ();

    virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr ) override;

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
    virtual IPixelShaderChannelConstPtr         GetPixelShaderChannel       () const override;
    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;

    virtual void                                Update                      ( TimeType t ) override;

    virtual void								SetPrevPlugin               ( IPluginPtr plugin ) override;
private:

    void                                        InitAttributesChannel       ( IPluginPtr prev );

};

} }