#include "DefaultPluginListFinalized.h"


namespace bv { namespace model {

// *******************************
//
DefaultPluginListFinalized::DefaultPluginListFinalized      ( const std::string & shadersDir )
    : m_finalizePlugin( new DefaultFinalizePlugin( shadersDir ) )
{
}

// *******************************
//
DefaultPluginListFinalized::DefaultPluginListFinalized      ( const std::vector< IPluginPtr > & plugins, const std::string & shadersDir )
    : m_finalizePlugin( new DefaultFinalizePlugin( shadersDir ) )
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
IPluginPtr              DefaultPluginListFinalized::GetPlugin           ( unsigned int idx ) const
{
    assert( idx < m_plugins.size() );

    return m_plugins[ idx ];
}

// *******************************
//
IPluginPtr              DefaultPluginListFinalized::GetLastPlugin       () const
{
    assert( m_plugins.size() > 0 );

    return m_plugins.back();
}

// *******************************
//
unsigned int            DefaultPluginListFinalized::NumPlugins          () const
{
    return m_plugins.size();
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
IPluginPtr                DefaultPluginListFinalized::GetPlugin           ( unsigned int idx )
{
    assert( idx < m_plugins.size() );

    return m_plugins[ idx ];
}

// *******************************
//
void                    DefaultPluginListFinalized::AttachPlugin        ( IPluginPtr plugin )
{
    assert( plugin != nullptr );

    m_plugins.push_back( plugin );

    m_finalizePlugin->SetPrevPlugin( plugin );
}

} // model
} // bv
