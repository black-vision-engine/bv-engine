#pragma once

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/ResourceStateModel.h"

#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"

#include "Engine/Models/Plugins/Simple/SpecialPlugins/BlendHelper.h"



namespace bv {
namespace model {

// ***************************** DESCRIPTOR **********************************
class ExpertPluginDesc : public BasePluginDescriptor
{
public:

    ExpertPluginDesc                               				();

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;

    static  std::string                     UID                 ();
};

// ***************************** PLUGIN ********************************** 
/// @ingroup PluginsList
class ExpertPlugin : public BasePlugin
{
public:

    struct PARAMS
    {
        static const std::string        COLOR_BLENDING_MODE;
        static const std::string        ALPHA_BLENDING_MODE;
        static const std::string        BLEND_ENABLE;

        static const std::string        CULLING_ENABLE;
        static const std::string        CC_CULL_ORDER;
        static const std::string        ENABLE_DEPTH_TEST;
        static const std::string        ENABLE_DEPTH_WRITE;
        static const std::string        FILL_MODE;

        static const std::string        RESET_SETTINGS;
    };

protected:

    DefaultPixelShaderChannelPtr        m_psc;

    ValueParamState< bool >						m_blendEnabled;
    ValueParamState< BlendHelper::BlendMode >	m_colorBlendMode;
    ValueParamState< BlendHelper::BlendMode >	m_alphaBlendMode;

    ValueParamState< bool >						m_cullingEnabled;
    ValueParamState< bool >						m_ccCullOrder;
    ValueParamState< bool >						m_enableDepthTest;
    ValueParamState< bool >						m_enableDepthWrite;

    ValueParamState< FillContext::Mode >	    m_fillMode;

    ValueParamState< bool >                     m_resetSettings;

    bool                                        m_firstAttach;

public:

    explicit                                    ExpertPlugin   ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
    virtual                                     ~ExpertPlugin  ();

    virtual bool                                IsValid						() const override;

    virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr ) override;

    virtual IPixelShaderChannelPtr              GetPixelShaderChannel       () const override;
    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;

    virtual void                                Update                      ( TimeType t ) override;

private:
    virtual bool								SetPrevPlugin               ( IPluginPtr plugin ) override;

    void                                        ApplyFromPrevious           ();
    void                                        UpdateContext               ();
};

DEFINE_ENUM_SET_PARAMETER( FillContext::Mode );


} // model
} // bv


