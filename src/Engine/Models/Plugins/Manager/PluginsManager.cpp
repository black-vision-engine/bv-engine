#include "PluginsManager.h"

#include "Engine/Models/Plugins/Interfaces/IPluginDescriptor.h"
#include "Engine/Models/Plugins/Interfaces/IPlugin.h"

#include "Engine/Models/Plugins/DefaultPluginListFinalized.h"


namespace bv { namespace model {

//Default instance
PluginsManager PluginsManager::m_instance;

// *********************************
//
PluginsManager::PluginsManager  ()
{
}

// *********************************
//
PluginsManager::PluginsManager  ( std::vector< IPluginDescriptor * > descriptors )
{
    RegisterDescriptors( descriptors );
}

// *********************************
//
PluginsManager::~PluginsManager ()
{
    for( auto descr : m_descrVec )
    {
        delete descr;
    }
}

// *********************************
//
unsigned int                                PluginsManager::RegisterDescriptors     ( std::vector< IPluginDescriptor * > descriptors )
{
    unsigned int i = 0;

    for( auto descr : descriptors )
    {
        if( !IsRegistered( descr->GetPluginTypeUID() ) )
        {
            m_descrMap[ descr->GetPluginTypeUID() ] = descr;
            
            ++i;
        }

        m_descrVec.push_back( descr );
    }

    return i;
}

// *********************************
//
bool                                        PluginsManager::IsRegistered            ( const std::string & uid ) const
{
    return m_descrMap.find( uid ) != m_descrMap.end();
}

// *********************************
//
const IPluginDescriptor *                   PluginsManager::GetDescriptor           ( const std::string & uid ) const
{
    if( IsRegistered( uid ) )
    {
        return m_descrMap.find( uid )->second;
    }

    return nullptr;
}

// *********************************
//
bool                                        PluginsManager::CanBeAttachedTo         ( const std::string & uid, const IPlugin * prev ) const
{
    if( !IsRegistered( uid ) )
    {
        return false;
    }

    return GetDescriptor( uid )->CanBeAttachedTo( prev );
}

// *********************************
//
IPlugin *                                   PluginsManager::CreatePlugin            ( const std::string & uid, const std::string & name, const IPlugin * prev ) const
{
    if( !CanBeAttachedTo( uid, prev ) )
    {
        return nullptr;
    }

    return GetDescriptor( uid )->CreatePlugin( name, prev );
}

// *********************************
//
IPlugin *                                   PluginsManager::CreatePlugin            ( const std::string & uid, const IPlugin * prev ) const
{
    if( !CanBeAttachedTo( uid, prev ) )
    {
        return nullptr;
    }

    return CreatePlugin( uid, GetDescriptor( uid )->DefaultPluginName(), prev );
}

// *********************************
//
std::vector< const IPluginDescriptor * >    PluginsManager::GetRegisteredDescriptors()
{
    return m_descrVec;
}

// *********************************
//
IPluginListFinalized *                      PluginsManager::CreatePlugins           ( const std::vector< std::string > & uids ) const
{
    return CreatePluginsDefaultImpl( uids );
}

// *********************************
//
IPluginListFinalized *                      PluginsManager::CreatePlugins           ( const std::vector< std::string > & uids, const std::vector< std::string > & names ) const
{
    return CreatePluginsDefaultImpl( uids, names );
}

// *********************************
//
IPluginListFinalized *                      PluginsManager::CreatePlugins           ( const std::vector< std::pair< std::string, std::string > > & plugins ) const
{
    return CreatePluginsDefaultImpl( plugins );
}

// *********************************
//
DefaultPluginListFinalized *                PluginsManager::CreatePluginsDefaultImpl( const std::vector< std::string > & uids ) const
{
    std::vector< std::pair< std::string, std::string > > plugins;

    for( auto uid : uids )
    {
        if( !IsRegistered( uid ) )
        {
            printf( "PLUGIN UID %s not registered\n", uid.c_str() );

            return nullptr;
        }

        plugins.push_back( std::make_pair( uid, GetDescriptor( uid )->DefaultPluginName() ) );
    }

    return CreatePluginsDefaultImpl( plugins );
}

// *********************************
//
DefaultPluginListFinalized *                PluginsManager::CreatePluginsDefaultImpl( const std::vector< std::string > & uids, const std::vector< std::string > & names ) const
{
    assert( uids.size() == names.size() );

    if( uids.size() != names.size() )
    {
        printf( "Number of uids %d differs from number of names %d \n", uids.size(), names.size() );

        return nullptr;
    }

    std::vector< std::pair< std::string, std::string > > plugins;

    for( unsigned int i = 0; i < uids.size(); ++i )
    {
        plugins.push_back( std::make_pair( uids[ i ], names[ i ] ) );
    }    

    return CreatePluginsDefaultImpl( plugins );
}

// *********************************
//
DefaultPluginListFinalized *                PluginsManager::CreatePluginsDefaultImpl( const std::vector< std::pair< std::string, std::string > > & plugins ) const
{
    std::vector< IPluginPtr > tmpList;
    const IPlugin * prev = nullptr;

    for( auto pair : plugins )
    {
        const std::string & uid     = pair.first;
        const std::string & name    = pair.second;

        if( !IsRegistered( uid ) )
        {
            printf( "PLUGIN UID %s not registered\n", uid.c_str() );
            return nullptr;
        }
    
        if( !CanBeAttachedTo( uid, prev ) )
        {
            std::string prevpl = prev ? prev->GetName() : "NULL";

            printf( "PLIUGIN with UID %s cannot be attached to %s\n", uid.c_str(), prevpl.c_str() );

            return nullptr;
        }
    
        IPluginPtr plugin( CreatePlugin( uid, name, prev ) );

        tmpList.push_back( plugin );

        prev = plugin.get();
    }

    return new DefaultPluginListFinalized( tmpList );
}

// *********************************
//
PluginsManager &                            PluginsManager::DefaultInstanceRef      ()
{
    return m_instance;
}

// *********************************
//
const PluginsManager &                      PluginsManager::DefaultInstance         ()
{
    return m_instance;
}

} //model
} //bv
