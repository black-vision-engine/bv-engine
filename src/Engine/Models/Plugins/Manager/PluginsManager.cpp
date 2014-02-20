#include "PluginsManager.h"

#include "Engine\Models\Plugins\Interfaces\IPluginDescriptor.h"
#include "Engine\Models\Plugins\Interfaces\IPlugin.h"


namespace bv { namespace model {

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
void                                        PluginsManager::RegisterDescriptors     ( std::vector< IPluginDescriptor * > descriptors )
{
    for( auto descr : descriptors )
    {
        if( !IsRegistered( descr->GetPluginTypeUID() ) )
        {
            m_descrMap[ descr->GetPluginTypeUID() ] = descr;
        }

        m_descrVec.push_back( descr );
    }
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
const IPlugin *                             PluginsManager::CreatePlugin            ( const std::string & uid, const std::string & name, const IPlugin * prev ) const
{
    if( !CanBeAttachedTo( uid, prev ) )
    {
        return nullptr;
    }

    return GetDescriptor( uid )->CreatePlugin( name, prev );
}

// *********************************
//
std::vector< const IPluginDescriptor * >    PluginsManager::GetRegisteredDescriptors()
{
    return m_descrVec;
}

// *********************************
//
const PluginsManager &                      PluginsManager::DefaultInstance         ( std::vector< IPluginDescriptor * > descriptors )
{
    static bool initialized = false;
    static PluginsManager instance;

    if( !initialized )
    {
        instance.RegisterDescriptors( descriptors );

        initialized = true;
    }

    return instance;

}

} //model
} //bv
