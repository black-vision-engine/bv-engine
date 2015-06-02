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
            return "DEFAULT_TRANSFORM"; // FIXME
        else if( serialName == "solid" )
            return "DEFAULT_COLOR";
        else if( serialName == "text" )
            return "DEFAULT_TEXT";
        else if( serialName == "mask" )
            return "DEFAULT_ALPHA_MASK";
        else if( serialName == "sequence" )
            return "DEFAULT_ANIMATION";
        else if( serialName == "replicator" )
            return "DEFAULT_TRANSFORM"; // FIXME
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

// params
    auto params = doc.LoadProperties< AbstractModelParameter >( "property" );
    
    ISerializablePtr serializablePlugin = std::static_pointer_cast< ISerializable >( plugin );
    return serializablePlugin;
}

} }