#include "DefaultPluginListFinalized.h"


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
   
    m_finalizePlugin->SetPrevPlugin( plugins.back().get() );
}

// *******************************
//
DefaultPluginListFinalized::~DefaultPluginListFinalized     ()
{
}

// *******************************
//
const IPlugin * DefaultPluginListFinalized::GetPlugin           ( const std::string & name ) const
{
    for( auto plugin : m_plugins )
    {
        if( plugin->GetName() == name )
        {
            return plugin.get();
        }
    }

    return nullptr;
}

// *******************************
//
const IPlugin * DefaultPluginListFinalized::GetPlugin           ( unsigned int idx ) const
{
    assert( idx < m_plugins.size() );

    return m_plugins[ idx ].get();
}

// *******************************
//
const IPlugin * DefaultPluginListFinalized::GetLastPlugin       () const
{
    assert( m_plugins.size() > 0 );

    return m_plugins.back().get();
}

// *******************************
//
unsigned int    DefaultPluginListFinalized::NumPlugins          () const
{
    return m_plugins.size();
}

// *******************************
//
const IPlugin * DefaultPluginListFinalized::GetFinalizePlugin   () const
{
    return m_finalizePlugin.get();
}

// *******************************
//
void            DefaultPluginListFinalized::Update              ( TimeType t )
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
void            DefaultPluginListFinalized::AttachPlugin        ( IPlugin * plugin )
{
    assert( plugin != nullptr );

    m_plugins.push_back( IPluginPtr( plugin ) );
}

} // model
} // bv
