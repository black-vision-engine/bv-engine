#pragma once

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/ResourceStateModel.h"

#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"

#include "Engine/Models/Plugins/Simple/ShaderPlugins/BlendHelper.h"



namespace bv { namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultBlendTexturePluginDesc : public BasePluginDescriptor
{
public:

    DefaultBlendTexturePluginDesc                               ();

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
   
    static  std::string                     UID                 ();

    static  std::string                     TextureName         ();

};

// ***************************** PLUGIN ********************************** 
class DefaultBlendTexturePlugin : public BasePlugin
{
public:

    struct PARAMS
    {
        static const std::string        ALPHA;
        static const std::string        BLENDING_MODE;
    };

protected:

    DefaultPixelShaderChannelPtr    m_psc;
    DefaultVertexShaderChannelPtr   m_vsc;

    SizeType	                    m_textureWidth;
    SizeType	                    m_textureHeight;

public:

    explicit                                    DefaultBlendTexturePlugin   ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
                                                ~DefaultBlendTexturePlugin  ();

    virtual bool                                IsValid						() const override;

    virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr ) override;

    virtual IPixelShaderChannelPtr              GetPixelShaderChannel       () const override;
    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;

    SizeType									GetTextureWidth             () const;
    SizeType									GetTextureHeight            () const;

    virtual void                                Update                      ( TimeType t ) override;

private:
    virtual void								SetPrevPlugin               ( IPluginPtr plugin ) override;
};


} // model
} // bv


