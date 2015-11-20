#pragma once

#include <string>
#include <hash_map>
#include <vector>

#include "Engine/Models/Interfaces/ITimeEvaluator.h"
#include "Engine/Models/Plugins/Interfaces/IPlugin.h"


namespace bv { namespace model {

class IPluginDescriptor;
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

    unsigned int                                        RegisterDescriptors     ( std::vector< IPluginDescriptor * > descriptors );

    bool                                                IsRegistered            ( const std::string & uid ) const;
    const IPluginDescriptor *                           GetDescriptor           ( const std::string & uid ) const;
    
    IPluginPtr                                          CreatePlugin            ( const std::string & uid, const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const;
    IPluginPtr                                          CreatePlugin            ( const std::string & uid, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const;
    const std::vector< const IPluginDescriptor * > &    GetRegisteredDescriptors() const;

    IPluginPtr                                          CreatePlugin            ( const std::string & uid, const std::string & name, ITimeEvaluatorPtr timeEvaluator ) const;
    IPluginPtr                                          CreatePlugin            ( const std::string & uid, ITimeEvaluatorPtr timeEvaluator ) const;

    IPluginListFinalized *                              CreatePlugins           ( const std::vector< std::string > & uids, ITimeEvaluatorPtr timeEvaluator ) const;
    IPluginListFinalized *                              CreatePlugins           ( const std::vector< std::string > & uids, const std::vector< std::string > & names, ITimeEvaluatorPtr timeEvaluator ) const;
    IPluginListFinalized *                              CreatePlugins           ( const std::vector< std::pair< std::string, std::string > > & plugins, ITimeEvaluatorPtr timeEvaluator ) const;

    DefaultPluginListFinalized *                        CreatePluginsDefaultImpl( const std::vector< std::string > & uids, ITimeEvaluatorPtr timeEvaluator ) const;
    DefaultPluginListFinalized *                        CreatePluginsDefaultImpl( const std::vector< std::string > & uids, const std::vector< std::string > & names, ITimeEvaluatorPtr timeEvaluator ) const;
    DefaultPluginListFinalized *                        CreatePluginsDefaultImpl( const std::vector< std::pair< std::string, std::string > > & plugins, ITimeEvaluatorPtr timeEvaluator ) const;

public:

    static PluginsManager &                             DefaultInstanceRef      ();
    static const PluginsManager &                       DefaultInstance         ();

};

} //model
} //bv
