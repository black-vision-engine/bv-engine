#include "Plugin.h"

namespace bv { namespace model {

namespace PluginsSerialization
{
    std::shared_ptr< BasePlugin< IPlugin > >                                            CreatePluginByName( std::string name, ITimeEvaluatorPtr te );
};

// *******************************
//
template <>
ISerializablePtr BasePlugin< IPlugin >::Create( DeserializeObject& doc )
{
    std::string pluginType = "DEFAULT_TRANSFORM"; //doc.GetValue( "name" );

    std::string pluginName = ""; // FIXME

    IPluginPtr plugin__ = doc.m_pm->CreatePlugin( pluginType, pluginName, nullptr, doc.m_tm->GetRootTimeline() );
    std::shared_ptr< BasePlugin< IPlugin > > plugin = std::static_pointer_cast< BasePlugin< IPlugin > >( plugin__ );

// TODO: params

    
    ISerializablePtr serializablePlugin = std::static_pointer_cast< ISerializable >( plugin );
    return serializablePlugin;
}

} }