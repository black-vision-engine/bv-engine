#pragma once

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/ResourceStateModel.h"

#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"

#include "Engine/Models/Plugins/Simple/ShaderPlugins/BlendHelper.h"
#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"



namespace bv { namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultTexturePluginDesc : public BasePluginDescriptor
{
public:

    DefaultTexturePluginDesc                                    ();
	DefaultTexturePluginDesc                                    ( const std::string & uid, const std::string & defaultName, const std::string & abbrv );

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
   
    static  std::string                     UID                 ();

    static  std::string                     TextureName         ();

};

// ***************************** PLUGIN ********************************** 
class DefaultTexturePlugin : public BasePlugin
{
public:

	struct PARAM
	{
		static const std::string        ALPHA;
        static const std::string		TX_MAT;
    };

protected:

    DefaultPixelShaderChannelPtr    m_psc;
    DefaultVertexShaderChannelPtr   m_vsc;

    VertexAttributesChannelPtr      m_vaChannel;

    SizeType	                    m_textureWidth;
    SizeType	                    m_textureHeight;

	ValueParamState< bool >						m_blendEnabled;
	ValueParamState< BlendHelper::BlendMode >	m_blendMode;

public:

    explicit                                    DefaultTexturePlugin        ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
                                                ~DefaultTexturePlugin       ();

    virtual bool                                IsValid						() const override;

    virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr ) override;

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
    virtual IPixelShaderChannelPtr              GetPixelShaderChannel       () const override;
    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;

    virtual mathematics::RectConstPtr			GetAABB						( const glm::mat4 & ) const override;

    SizeType									GetTextureWidth             () const;
    SizeType									GetTextureHeight            () const;

    virtual void                                Update                      ( TimeType t ) override;

private:
    void										InitVertexAttributesChannel ();

    virtual bool								SetPrevPlugin               ( IPluginPtr plugin ) override;
};

} // model
} // bv
