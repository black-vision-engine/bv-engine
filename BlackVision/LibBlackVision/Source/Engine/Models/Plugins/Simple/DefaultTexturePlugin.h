#pragma once

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"
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

	DefaultPluginParamValModelPtr   m_paramValModel;

	DefaultPixelShaderChannelPtr    m_psc;
	DefaultVertexShaderChannelPtr   m_vsc;

	VertexAttributesChannelPtr      m_vaChannel;

	DefaultTexturesDataPtr          m_texturesData;

    SizeType						m_texCoordChannelIndex;

    ParamFloatPtr                   m_paramWrapModeX;
    ParamFloatPtr                   m_paramWrapModeY;
    ParamFloatPtr                   m_paramFilteringMode;
    ParamFloatPtr                   m_paramAttachMode;
    
    SizeType	                    m_textureWidth;
    SizeType	                    m_textureHeight;

	TextureWrappingMode             m_lastTextureWrapModeX;
	TextureWrappingMode             m_lastTextureWrapModeY;
	TextureFilteringMode            m_lastTextureFilteringMode;
	TextureAttachmentMode           m_lastTextureAttachMode;
	glm::vec4                       m_lastBorderColor;
	

public:

	explicit                                    DefaultTexturePlugin        ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
												~DefaultTexturePlugin       ();

    virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr ) override;

	virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
	virtual IPixelShaderChannelConstPtr         GetPixelShaderChannel       () const override;
	virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;

	virtual mathematics::RectConstPtr			GetAABB						( const glm::mat4 & ) const override;

    SizeType									GetTextureWidth             () const;
    SizeType									GetTextureHeight            () const;

	virtual void                                Update                      ( TimeType t ) override;

private:

	void                                        InitAttributesChannel       ( IPluginPtr prev );

	TextureWrappingMode                         GetWrapModeX                () const;
	TextureWrappingMode                         GetWrapModeY                () const;
	TextureFilteringMode                        GetFilteringMode            () const;
	TextureAttachmentMode                       GetAttachementMode          () const;
	glm::vec4                                   GetBorderColor              () const;

	bool                                        StateChanged                ( TextureWrappingMode wmX, TextureWrappingMode wmY, TextureFilteringMode fm, TextureAttachmentMode am/*, glm::vec4 bc*/ ) const;
	void                                        UpdateState                 ( TextureWrappingMode wmX, TextureWrappingMode wmY, TextureFilteringMode fm, TextureAttachmentMode am/*, glm::vec4 bc*/ );

};

} // model
} // bv
