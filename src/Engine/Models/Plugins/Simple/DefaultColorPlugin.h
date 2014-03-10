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

    virtual IPlugin *                       CreatePlugin        ( const std::string & name, const IPlugin * prev ) const override;
    virtual DefaultPluginParamValModel *    CreateDefaultModel  () const override;
   
    static  std::string                     UID                 ();

    static  std::string                     PixelShaderSource   ();

};


// ***************************** PLUGIN ********************************** 
class DefaultColorPlugin : public BasePlugin< IPlugin >
{
private:

    DefaultPixelShaderChannelPtr            m_pixelShaderChannel;
    DefaultVertexShaderChannelPtr           m_vertexShaderChannel;
    DefaultPluginParamValModelPtr           m_paramValModel;

public:

    explicit                                DefaultColorPlugin          ( const std::string & name, const std::string & uid, const IPlugin * prev, DefaultPluginParamValModelPtr model );
                                            ~DefaultColorPlugin         ();

    virtual const IPixelShaderChannel *     GetPixelShaderChannel       () const override;

    virtual void                            Update                      ( TimeType t ) override;

};

} // model
} // bv
