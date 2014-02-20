#pragma once

#include <string>

namespace bv { namespace model {

class IPlugin;
class IPluginParamValModel;

class IPluginDescriptor
{
public:

    virtual const std::string &     GetPluginTypeUID() const                                                    = 0;

    virtual bool                    CanBeAttachedTo ( const IPlugin * plugin )  const                           = 0;
    virtual IPlugin *               CreatePlugin    ( const std::string & name, const IPlugin * prev ) const    = 0;
    virtual IPluginParamValModel *  CreateModel     () const                                                    = 0;

    virtual ~IPluginDescriptor() {}

};

} //model
} //bv
