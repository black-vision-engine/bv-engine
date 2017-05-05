#include "stdafx.h"

#include "DefaultPluginListFinalized.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {

// *******************************
//
DefaultPluginListFinalized::DefaultPluginListFinalized      ()
    : m_finalizePlugin( new DefaultFinalizePlugin() )
{
}

// *******************************
//
DefaultPluginListFinalized::DefaultPluginListFinalized      ( const std::vector< IPluginPtr > & plugins )
    : m_finalizePlugin( new DefaultFinalizePlugin() )
{
    assert( plugins.size() > 0 );
   
    m_finalizePlugin->SetPrevPlugin( plugins.back() );
}

// *******************************
//
DefaultPluginListFinalized::~DefaultPluginListFinalized     ()
{
}

// *******************************
//
IPluginPtr              DefaultPluginListFinalized::GetPlugin           ( const std::string & name ) const
{
    for( auto plugin : m_plugins )
    {
        if( plugin->GetName() == name )
        {
            return plugin;
        }
    }

    return nullptr;
}

// *******************************
//
IPluginPtr              DefaultPluginListFinalized::GetPlugin           ( UInt32 idx ) const
{
    assert( idx < m_plugins.size() );

    return m_plugins[ idx ];
}

// *******************************
//
IPluginPtr              DefaultPluginListFinalized::GetLastPlugin       () const
{
    return m_plugins.empty() ? nullptr : m_plugins.back();
}

// *******************************
//
unsigned int            DefaultPluginListFinalized::NumPlugins          () const
{
    return (unsigned int) m_plugins.size();
}

// *******************************
//
IFinalizePluginPtr      DefaultPluginListFinalized::GetFinalizePlugin   () const
{
    return m_finalizePlugin;
}

// *******************************
//
void                    DefaultPluginListFinalized::Update              ( TimeType t )
{
    //UPDATE ORDER IS CRUCIAL HERE - following plugins must be able to read their predcessor's state
    for( auto plugin : m_plugins )
    {
        plugin->Update( t );
    }

    m_finalizePlugin->Update( t );
}

// *******************************
//
IPluginPtr               DefaultPluginListFinalized::GetPlugin           ( const std::string & name )
{
    for( auto plugin : m_plugins )
    {
        if( plugin->GetName() == name )
        {
            return plugin;
        }
    }

    return nullptr;
}

// *******************************
//
IPluginPtr                DefaultPluginListFinalized::GetPlugin           ( UInt32 idx )
{
    assert( idx < m_plugins.size() );

    return m_plugins[ idx ];
}

// *******************************
//
void                    DefaultPluginListFinalized::AttachPlugin        ( IPluginPtr plugin )
{
    UInt32 idx = (UInt32)m_plugins.size();
    AttachPlugin( plugin, idx );
}

// *******************************
// idx will contain real index of attached plugin. idx can change when you provide number greater then plugins list length.
bool                    DefaultPluginListFinalized::AttachPlugin        ( IPluginPtr plugin, UInt32 & idx )
{
    if( plugin == nullptr )
    {
        return false;
    }

    if( idx < m_plugins.size() )
    {
        m_plugins.insert( m_plugins.begin() + idx, plugin );
    }
    else
    {
        m_plugins.push_back( plugin );
        idx = ( UInt32 )( m_plugins.size() - 1 );
    }

    bool noError = true;

    for( UInt32 i = idx; i < m_plugins.size() && noError; ++i )
    {
        if( i == 0 )
        {
            noError = noError && m_plugins[ i ]->SetPrevPlugin( nullptr );
        }
        else
        {
            noError = noError && m_plugins[ i ]->SetPrevPlugin( m_plugins[ i - 1 ] );
        }
    }

    if( !noError )
    {
        m_plugins.erase( m_plugins.begin() + idx ); // Cleaning up.
        LOG_MESSAGE( SeverityLevel::debug ) << "Cannot attach plugin " << plugin->GetName() << " on index " << idx;
        return false;
    }

    m_finalizePlugin->SetPrevPlugin( m_plugins.back() );

    return true; //m_finalizePlugin->IsValid();
}

// *******************************
//
PluginWithIdx				DefaultPluginListFinalized::DetachPlugin	   ( const std::string & name )
{
    for( unsigned int i = 0; i < m_plugins.size(); ++i )
    {
        if( m_plugins[ i ]->GetName() == name )
        {
            return std::make_pair( DetachPlugin( i ), i );
        }
    }
    return std::make_pair( nullptr, -1 );
}

// *******************************
//
IPluginPtr				DefaultPluginListFinalized::DetachPlugin	   ( UInt32 idx )
{
    if( idx >= m_plugins.size() )
    {
        return nullptr;
    }

    auto plugin =  m_plugins[ idx ];
    m_plugins.erase( m_plugins.begin() + idx );
    plugin->SetPrevPlugin( nullptr );

    for( UInt32 i = idx; i < m_plugins.size(); ++i )
    {
        if( i == 0 )
        {
            m_plugins[ i ]->SetPrevPlugin( nullptr );
        }
        else
        {
            m_plugins[ i ]->SetPrevPlugin( m_plugins[ i - 1 ] );
        }
    }

    m_finalizePlugin->SetPrevPlugin( m_plugins.empty() ? nullptr : m_plugins.back() );

    //assert( m_finalizePlugin->IsValid() );

    return plugin;
}

} // model
} // bv
