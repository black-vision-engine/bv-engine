#include "UpdatersManager.h"

#include "Engine/Models/Interfaces/IModelNode.h"


namespace bv {

// *******************************
//
UpdatersManager::UpdatersManager        ()
{
}

// *******************************
//
UpdatersManager::~UpdatersManager       ()
{
}

// *******************************
//
void UpdatersManager::UpdateStep        ()
{
    for( auto updater : m_updaters )
    {
        updater->DoUpdate();
    }
}

// *******************************
//
bool    UpdatersManager::IsRegistered       ( const model::IModelNode * node )
{
    return m_updatersMapping.find( node ) != m_updatersMapping.end();
}

// *******************************
//
void    UpdatersManager::RegisterUpdater    ( const model::IModelNode * node, IUpdaterPtr updater )
{
    assert( m_updatersMapping.find( node ) == m_updatersMapping.end() );

    m_updatersMapping[ node ] = updater;
    m_updaters.push_back( updater );
}

// *******************************
//
void    UpdatersManager::RemoveNodeUpdater  ( const model::IModelNode * node )
{
    auto itToRemove = m_updatersMapping.find( node );
    assert( itToRemove != m_updatersMapping.end() );

    auto updaterToRemove = itToRemove->second;

    for( auto it = m_updaters.begin(); it != m_updaters.end(); ++it )
    {
        if( updaterToRemove == *it )
        {
            m_updaters.erase( it );
            m_updatersMapping.erase( itToRemove );
        
            return;
        }
    }

    assert( false );
}

// *******************************
//
void UpdatersManager::RemoveAllUpdaters     ()
{
    m_updaters.clear();
}

// *******************************
//
UpdatersManager & UpdatersManager::Get  ()
{
    static UpdatersManager instance = UpdatersManager();
    return instance;
}

} //bv
