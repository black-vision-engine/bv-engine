#pragma once

#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"
#include "Engine/Models/Plugins/Channels/Transform/DefaultTransformChannel.h"
#include "Engine/Models/Plugins/Plugin.h"


namespace bv { namespace model {

class DefaultTransformChannel;
class IPluginParamValModel;
class DefaultTransformPlugin;

// ***************************** Desc **********************************
class DefaultTransformPluginDesc
{
public:

    static const char *                 GetName         ()  { return "default_transform_plugin"; }

    static DefaultPluginParamValModel * CreateModel     ( bool setDefaultValues );
    static DefaultTransformPlugin *     CreatePlugin    ( const IPlugin * prev, bool setDefaultValues = true );

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

    virtual const ITransformChannel *           GetTransformChannel         () const override;

    virtual void                                Update                      ( TimeType t ) override;

    friend class DefaultTransformPluginDesc;

};

} // model
} // bv
