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
    //assert( m_plugins.size() > 0 );

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
IPluginPtr                DefaultPluginListFinalized::GetPlugin           ( unsigned int idx )
{
    assert( idx < m_plugins.size() );

    return m_plugins[ idx ];
}

// *******************************
//
void                    DefaultPluginListFinalized::AttachPlugin        ( IPluginPtr plugin )
{
	AttachPlugin( plugin, (unsigned int)m_plugins.size() );
}

// *******************************
//
bool                    DefaultPluginListFinalized::AttachPlugin        ( IPluginPtr plugin, unsigned int idx )
{
    assert( plugin != nullptr );

	if( idx >= m_plugins.size() )
	{
		plugin->SetPrevPlugin( m_plugins.empty() ? nullptr : m_plugins.back() );
		m_plugins.push_back( plugin );
	} 
	else 
	{
		m_plugins[idx]->SetPrevPlugin( plugin );
		plugin->SetPrevPlugin( idx == 0 ? nullptr : m_plugins[idx - 1]);
		m_plugins.insert(m_plugins.begin() + idx, plugin);
	}

	m_finalizePlugin->SetPrevPlugin( m_plugins.empty() ? nullptr : m_plugins.back() );

	assert( m_finalizePlugin->IsValid() );
	return m_finalizePlugin->IsValid();
}

// *******************************
//
IPluginPtr				DefaultPluginListFinalized::DetachPlugin	   ( const std::string & name )
{
	for( unsigned int i = 0; i < m_plugins.size(); ++i )
    {
		if( m_plugins[ i ]->GetName() == name )
			return DetachPlugin( i );
	}
	return nullptr;
}

// *******************************
//
IPluginPtr				DefaultPluginListFinalized::DetachPlugin	   ( unsigned int idx )
{
    assert( idx < m_plugins.size() );

	auto plugin =  m_plugins[ idx ];
    m_plugins.erase( m_plugins.begin() + idx );

	if( idx < m_plugins.size() )
		m_plugins[idx]->SetPrevPlugin( idx == 0 ? nullptr : m_plugins[idx - 1]);

	plugin->SetPrevPlugin( nullptr );

	m_finalizePlugin->SetPrevPlugin( m_plugins.empty() ? nullptr : m_plugins.back() );

	assert( m_finalizePlugin->IsValid() );

    return plugin;
}

} // model
} // bv
