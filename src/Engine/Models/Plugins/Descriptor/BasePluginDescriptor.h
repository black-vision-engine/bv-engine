#pragma once

#include "Engine/Models/Plugins/Interfaces/IPluginDescriptor.h"


namespace bv { namespace model {

class BasePluginDescriptor : public IPluginDescriptor
{
private:

    std::string     m_uid;

protected:

    BasePluginDescriptor    ( const std::string & uid );

public:

    virtual const std::string &     GetPluginTypeUID() const override;
    virtual bool                    CanBeAttachedTo ( const IPlugin * plugin )  const override;

    virtual ~BasePluginDescriptor () {}

};

} //model
} //bv
