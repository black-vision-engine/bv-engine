#pragma once

#include "Engine/Models/Plugins/Channels/Transform/DefaultTransformChannel.h"
#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Events/Events.h"



namespace bv { namespace model {

class DefaultTransformChannel;
class IPluginParamValModel;

// ***************************** UID **********************************
class DefaultTransformPluginUID
{
public:

    static const char * GetName ()    { return "default_transform_plugin"; }

};


// ***************************** PLUGIN ********************************** 
class DefaultTransformPlugin : public BasePlugin< IPlugin, DefaultTransformPluginUID >
{
private:

    DefaultTransformChannelPtr                  m_transformChannel;
    IPluginParamValModel *                      m_paramValModel;

private:

    explicit                                    DefaultTransformPlugin      ( const IPlugin * prev, IPluginParamValModel * model );

public:

                                                ~DefaultTransformPlugin     ();

    virtual const IDefaultTransformChannel *    GetDefaultTransformChannel  () const override;

    virtual void                                Update                      ( TimeType t ) override;

    static  DefaultTransformPlugin *            Create                      ( const IPlugin * prev );

};

} // model
} // bv
