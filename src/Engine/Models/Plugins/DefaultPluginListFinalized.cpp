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
unsigned int    DefaultPluginListFinalized::NumPlugins          () const
{
    return m_plugins.size();
}

// *******************************
//
const IPlugin * DefaultPluginListFinalized::GetFinalizePlugin   ( const std::string & name ) const
{
    return m_finalizePlugin.get();
}

// *******************************
//
void            DefaultPluginListFinalized::Update              ( TimeType t )
{
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
    m_plugins.push_back( IPluginPtr( plugin ) );
}

} // model
} // bv
