#pragma once

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/ResourceStateModel.h"

#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"



namespace bv { namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultTexturePluginDesc : public BasePluginDescriptor
{
public:

	DefaultTexturePluginDesc                                    ();

	virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
	virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
   
	virtual bool                            CanBeAttachedTo     ( IPluginConstPtr plugin )  const override;

	static  std::string                     UID                 ();

	static  std::string                     TextureName         ();

};

// ***************************** PLUGIN ********************************** 
class DefaultTexturePlugin : public BasePlugin< IPlugin >
{
protected:

	DefaultPixelShaderChannelPtr    m_psc;
	DefaultVertexShaderChannelPtr   m_vsc;

	VertexAttributesChannelPtr      m_vaChannel;

    SizeType	                    m_textureWidth;
    SizeType	                    m_textureHeight;

public:

	explicit                                    DefaultTexturePlugin        ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
												~DefaultTexturePlugin       ();

    virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr ) override;
	virtual IParamValModelPtr					GetResourceStateModel		( const std::string & name ) const override;

	virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
	virtual IPixelShaderChannelConstPtr         GetPixelShaderChannel       () const override;
	virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;

	virtual mathematics::RectConstPtr			GetAABB						( const glm::mat4 & ) const override;

    SizeType									GetTextureWidth             () const;
    SizeType									GetTextureHeight            () const;

	virtual void                                Update                      ( TimeType t ) override;

private:
	void										InitVertexAttributesChannel ();

    virtual void								SetPrevPlugin               ( IPluginPtr plugin ) override;
};

} // model
} // bv
