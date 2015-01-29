#pragma once

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/Transform/DefaultTransformChannel.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"
#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
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

    static  std::string                     PixelShaderSource   ();

};


// ***************************** PLUGIN ********************************** 
class DefaultColorPlugin : public BasePlugin< IPlugin >
{
private:

    DefaultPixelShaderChannelPtr            m_pixelShaderChannel;
    DefaultPluginParamValModelPtr           m_paramValModel;

public:

    explicit                                DefaultColorPlugin          ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
                                            ~DefaultColorPlugin         ();

    virtual IPixelShaderChannelConstPtr     GetPixelShaderChannel       () const override;

    virtual void                            Update                      ( TimeType t ) override;

};

} // model
} // bv