#pragma once

#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"
#include "Engine/Models/Plugins/Channels/Transform/DefaultTransformChannel.h"
#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Events/Events.h"


namespace bv { namespace model {

class DefaultTransformChannel;
class IPluginParamValModel;

// ***************************** Desc **********************************
class DefaultTransformPluginDesc
{
public:

    static const char *                 GetName     ()  { return "default_transform_plugin"; }
    static DefaultPluginParamValModel * CreateModel ( bool setDefaultValues );

};


// ***************************** PLUGIN ********************************** 
class DefaultTransformPlugin : public BasePlugin< IPlugin, DefaultTransformPluginDesc >
{
private:

    DefaultTransformChannelPtr                  m_transformChannel;
    DefaultPluginParamValModelPtr               m_paramValModel;

private:

    explicit                                    DefaultTransformPlugin      ( const IPlugin * prev, DefaultPluginParamValModelPtr model );

public:

                                                ~DefaultTransformPlugin     ();

    virtual const IDefaultTransformChannel *    GetDefaultTransformChannel  () const override;

    virtual void                                Update                      ( TimeType t ) override;

public:

    static  DefaultTransformPlugin *            Create                      ( const IPlugin * prev, bool setDefaultValues = true );

};

} // model
} // bv
