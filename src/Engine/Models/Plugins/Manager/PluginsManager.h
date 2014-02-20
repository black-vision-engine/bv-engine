#pragma once

#include <string>
#include <hash_map>
#include <vector>


namespace bv { namespace model {

class IPluginDescriptor;
class IPlugin;

class PluginsManager
{
private:

    std::hash_map< std::string, const IPluginDescriptor * > m_descrMap;
    std::vector< const IPluginDescriptor * >                m_descrVec;

private:

    PluginsManager  ();
    PluginsManager  ( std::vector< IPluginDescriptor * > descriptors );

    ~PluginsManager ();

    void                                        RegisterDescriptors     ( std::vector< IPluginDescriptor * > descriptors );

public:

    bool                                        IsRegistered            ( const std::string & uid ) const;
    const IPluginDescriptor *                   GetDescriptor           ( const std::string & uid ) const;
    
    bool                                        CanBeAttachedTo         ( const std::string & uid, const IPlugin * prev ) const;
    const IPlugin *                             CreatePlugin            ( const std::string & uid, const std::string & name, const IPlugin * prev ) const;
    std::vector< const IPluginDescriptor * >    GetRegisteredDescriptors();

public:

    static const PluginsManager &               DefaultInstance         ( std::vector< IPluginDescriptor * > descriptors );

};

} //model
} //bv
