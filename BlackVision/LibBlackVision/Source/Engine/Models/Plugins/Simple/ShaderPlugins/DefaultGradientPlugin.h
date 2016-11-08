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
class DefaultGradientPluginDesc : public BasePluginDescriptor
{
public:

    DefaultGradientPluginDesc                                    ();

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
   
    static  std::string                     UID                 ();

    static  std::string                     TextureName         ();

};

// ***************************** PLUGIN ********************************** 
class DefaultGradientPlugin : public BasePlugin
{
public:
    static const std::string        PARAM_BLEND_ENABLE;
    static const std::string        PARAM_ALPHA;
    
    static const std::string        PARAM_POINT1;
    static const std::string        PARAM_POINT2;
    static const std::string        PARAM_COLOR1;
    static const std::string        PARAM_COLOR2;

private:

    DefaultPixelShaderChannelPtr    m_psc;
    DefaultVertexShaderChannelPtr   m_vsc;

    VertexAttributesChannelPtr      m_vaChannel;

public:

    explicit                                    DefaultGradientPlugin        ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
                                                ~DefaultGradientPlugin       ();

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
    virtual IPixelShaderChannelPtr              GetPixelShaderChannel       () const override;
    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;

    virtual void                                Update                      ( TimeType t ) override;

private:

    void                                        InitVertexAttributesChannel ();
    virtual void								SetPrevPlugin               ( IPluginPtr plugin ) override;
};

} // model
} // bv
