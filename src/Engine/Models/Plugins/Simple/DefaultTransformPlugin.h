#pragma once

#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"
#include "Engine/Models/Plugins/Channels/Transform/DefaultTransformChannel.h"
#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/Interfaces/IPluginDescriptor.h"


namespace bv { namespace model {

class DefaultTransformChannel;
class IPluginParamValModel;
class DefaultTransformPlugin;

// ***************************** Desc **********************************
class DefaultTransformPluginDesc : public IPluginDescriptor
{
private:

    std::string     m_uid;

public:

    virtual const std::string &     GetPluginTypeUID() const override;

    virtual bool                    CanBeAttachedTo ( const IPlugin * plugin )  const override;
    virtual IPlugin *               CreatePlugin    ( const std::string & name, const IPlugin * prev ) const override;
    virtual IPluginParamValModel *  CreateModel     () const override;

    static const char *                 GetName         ()  { return "default_transform_plugin"; }

    static DefaultPluginParamValModel * CreateModel     ( bool setDefaultValues );
    static DefaultTransformPlugin *     CreatePlugin    ( const IPlugin * prev, bool setDefaultValues = true );

};


// ***************************** PLUGIN ********************************** 
class DefaultTransformPlugin : public BasePlugin< IPlugin >
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
