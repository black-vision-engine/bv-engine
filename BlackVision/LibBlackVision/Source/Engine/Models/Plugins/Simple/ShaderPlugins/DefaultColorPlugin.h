#pragma once

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/Transform/DefaultTransformChannel.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"
#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"
#include "Engine/Models/Plugins/Plugin.h"


namespace bv { namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultColorPluginDesc : public BasePluginDescriptor
{
public:
    DefaultColorPluginDesc                                      ();

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
   
    static  std::string                     UID                 ();

};


// ***************************** PLUGIN ********************************** 
class DefaultColorPlugin : public BasePlugin
{
public:
    
    static const std::string        PARAM_BLEND_ENABLE;
    static const std::string        PARAM_COLOR;

private:

    DefaultPixelShaderChannelPtr            m_pixelShaderChannel;
    DefaultPluginParamValModelPtr           m_paramValModel;

	ValueParamState< bool >					m_blendEnabled;

public:

    explicit                                DefaultColorPlugin          ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
                                            ~DefaultColorPlugin         ();

    virtual IPixelShaderChannelPtr          GetPixelShaderChannel       () const override;

    virtual void                            Update                      ( TimeType t ) override;

    virtual bool							SetPrevPlugin               ( IPluginPtr plugin ) override;
};

} // model
} // bv
