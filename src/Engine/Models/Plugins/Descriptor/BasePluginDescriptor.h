#pragma once

#include "Engine/Models/Plugins/Interfaces/IPluginDescriptor.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"


namespace bv { namespace model {

class BasePluginDescriptor : public IPluginDescriptor
{
private:

    std::string     m_uid;
    std::string     m_defaultName;

protected:

    BasePluginDescriptor    ( const std::string & uid, const std::string & defaultName );

public:

    virtual const std::string &             GetPluginTypeUID    () const override;
    virtual const std::string &             DefaultPluginName   () const override;

    virtual bool                            CanBeAttachedTo     ( const IPlugin * plugin )  const override;
    virtual IPluginParamValModel *          CreateModel         () const override;

    virtual DefaultPluginParamValModel *    CreateDefaultModel  () const = 0;

    template< typename PluginType >
    PluginType *                           CreatePluginTyped    ( const std::string & name, const IPlugin * prev ) const
    {
        return new PluginType( name, GetPluginTypeUID(), prev, DefaultPluginParamValModelPtr( CreateDefaultModel() ) );
    }

    virtual ~BasePluginDescriptor () {}

};

} //model
} //bv
