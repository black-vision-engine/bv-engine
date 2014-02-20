#include "BasePluginDescriptor.h"


namespace bv { namespace model {

// *********************************
//
BasePluginDescriptor::BasePluginDescriptor    ( const std::string & uid )
    : m_uid( uid )
{
}

// *********************************
//
const std::string &     BasePluginDescriptor::GetPluginTypeUID() const
{
    return m_uid;
}

// *********************************
//
bool                    BasePluginDescriptor::CanBeAttachedTo ( const IPlugin * plugin )  const
{
    //By default assume that plugin can be connected just to any other plugin
    return true;
}

} //model
} //bv
