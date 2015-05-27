#include "Plugin.h"

namespace bv { namespace model {

namespace PluginsSerialization
{
    //std::shared_ptr< BasePlugin< IPlugin > >                                            CreatePluginByName( std::string name, ITimeEvaluatorPtr te );
    std::string SerialNameToUID( std::string serialName )
    {
        if( serialName == "geometry" // FIXME: deprecated, to remove
            || serialName == "transform" )
            return "DEFAULT_TRANSFORM";
        else if( serialName == "rectangle" )
            return "DEFAULT_RECTANGLE";
        else if( serialName == "texture" )
            return "DEFAULT_TEXTURE";
        else if( serialName == "alpha" )
            //return "DEFAULT_ALPHA_MASK";
            return "DEFAULT_TRANSFORM";
        else if( serialName == "solid" )
            return "DEFAULT_COLOR";
        else if( serialName == "text" )
            return "DEFAULT_TEXT";
        else
        {
            std::cout << "[ERROR] Unable to deserialize plugin: " << serialName << std::endl;
            assert( false );
            return "";
        }
    }
};

// *******************************
//
template <>
ISerializablePtr BasePlugin< IPlugin >::Create( DeserializeObject& doc )
{
    std::string pluginType = PluginsSerialization::SerialNameToUID( doc.GetValue( "name" ) );

    std::string pluginName = ""; // FIXME

    IPluginPtr plugin__ = doc.m_pm->CreatePlugin( pluginType, pluginName, doc.m_tm->GetRootTimeline() );
    std::shared_ptr< BasePlugin< IPlugin > > plugin = std::static_pointer_cast< BasePlugin< IPlugin > >( plugin__ );

// TODO: params

    
    ISerializablePtr serializablePlugin = std::static_pointer_cast< ISerializable >( plugin );
    return serializablePlugin;
}

} }