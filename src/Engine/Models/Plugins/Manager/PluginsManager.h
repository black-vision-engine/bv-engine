#pragma once

#include <string>
#include <hash_map>
#include <vector>


namespace bv { namespace model {

class IPluginDescriptor;
class IPlugin;
class IPluginListFinalized;

class DefaultPluginListFinalized;

class PluginsManager
{
private:

    static PluginsManager                                   m_instance;

    std::hash_map< std::string, const IPluginDescriptor * > m_descrMap;
    std::vector< const IPluginDescriptor * >                m_descrVec;

private:

    PluginsManager  ();
    PluginsManager  ( std::vector< IPluginDescriptor * > descriptors );

    ~PluginsManager ();

public:

    unsigned int                                RegisterDescriptors     ( std::vector< IPluginDescriptor * > descriptors );

    bool                                        IsRegistered            ( const std::string & uid ) const;
    const IPluginDescriptor *                   GetDescriptor           ( const std::string & uid ) const;
    
    bool                                        CanBeAttachedTo         ( const std::string & uid, const IPlugin * prev ) const;
    IPlugin *                                   CreatePlugin            ( const std::string & uid, const std::string & name, const IPlugin * prev ) const;
    IPlugin *                                   CreatePlugin            ( const std::string & uid, const IPlugin * prev ) const;
    std::vector< const IPluginDescriptor * >    GetRegisteredDescriptors();

    IPluginListFinalized *                      CreatePlugins           ( const std::vector< std::string > & uids ) const;
    IPluginListFinalized *                      CreatePlugins           ( const std::vector< std::string > & uids, const std::vector< std::string > & names ) const;
    IPluginListFinalized *                      CreatePlugins           ( const std::vector< std::pair< std::string, std::string > > & plugins ) const;

    DefaultPluginListFinalized *                CreatePluginsDefaultImpl( const std::vector< std::string > & uids ) const;
    DefaultPluginListFinalized *                CreatePluginsDefaultImpl( const std::vector< std::string > & uids, const std::vector< std::string > & names ) const;
    DefaultPluginListFinalized *                CreatePluginsDefaultImpl( const std::vector< std::pair< std::string, std::string > > & plugins ) const;

public:

    static PluginsManager &                     DefaultInstanceRef      ();
    static const PluginsManager &               DefaultInstance         ();

};

} //model
} //bv
