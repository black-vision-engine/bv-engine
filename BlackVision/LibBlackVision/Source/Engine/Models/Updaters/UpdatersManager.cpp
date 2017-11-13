#include "stdafx.h"

#include "UpdatersManager.h"

#include "Engine/Models/Interfaces/IModelNode.h"
#include "Engine/Models/SceneModel.h"

#include "Engine/Graphics/Resources/Textures/Texture2DCache.h"


#include "Memory/MemoryLeaks.h"



namespace bv
{

// ***********************
//
UpdatersManagerPtr       UpdatersManager::sInstancePtr = nullptr;


// *******************************
//
UpdatersManager::UpdatersManager        ()
{
    // FIXME: Maybe Textrue2DCache should be owned by UpdatersManager.
    m_engineResources.TexturesMap = &GTexture2DCache;
}

// *******************************
//
UpdatersManager::~UpdatersManager       ()
{}

// *******************************
//
void UpdatersManager::UpdateStep        ()
{
    for( auto updater : m_updaters )
    {
        updater->DoUpdate( m_engineResources );
    }
}

// *******************************
//
bool    UpdatersManager::IsRegistered       ( const model::IModelNode * node )
{
    return m_nodeUpdatersMapping.find( node ) != m_nodeUpdatersMapping.end();
}

// *******************************
//
void    UpdatersManager::RegisterUpdater    ( const model::IModelNode * node, IUpdaterPtr updater )
{
    RegisterUpdater< model::IModelNode >( node, updater, m_nodeUpdatersMapping );
}

// *******************************
//
void            UpdatersManager::RemoveNodeUpdater  ( const model::IModelNode * node )
{
    RemoveUpdater( node, m_nodeUpdatersMapping );
}

// *******************************
//
bool            UpdatersManager::IsRegistered       ( const model::SceneModel * scene )
{
    return m_sceneUpdatersMapping.find( scene ) != m_sceneUpdatersMapping.end();
}

// *******************************
//
void            UpdatersManager::RegisterUpdater    ( const model::SceneModel * scene, IUpdaterPtr updater )
{
    RegisterUpdater( scene, updater, m_sceneUpdatersMapping );
}

// *******************************
//
void            UpdatersManager::RemoveSceneUpdater ( const model::SceneModel * scene )
{
    RemoveUpdater( scene, m_sceneUpdatersMapping );
}

// *******************************
//
void                        UpdatersManager::RemoveAllUpdaters     ()
{
    m_updaters.clear();
}

// *******************************
//
UpdatersManager &           UpdatersManager::Get  ()
{
    return *GetShared().get();
}

// ***********************
//
UpdatersManagerPtr          UpdatersManager::GetShared()
{
    if( !UpdatersManager::sInstancePtr )
        UpdatersManager::sInstancePtr = std::make_shared< UpdatersManager >();

    return sInstancePtr;
}

// *******************************
//
template< typename T >
void            UpdatersManager::RegisterUpdater    ( const T * obj, IUpdaterPtr updater, std::hash_map< const T *, IUpdaterPtr > & mapping )
{
    assert( mapping.find( obj ) == mapping.end() );

    mapping[ obj ] = updater;
    m_updaters.push_back( updater );
}

// *******************************
//
template< typename T >
void            UpdatersManager::RemoveUpdater      ( const T * obj, std::hash_map< const T *, IUpdaterPtr > & mapping )
{
    auto itToRemove = mapping.find( obj );
    assert( itToRemove != mapping.end() );

    auto updaterToRemove = itToRemove->second;

    for( auto it = m_updaters.begin(); it != m_updaters.end(); ++it )
    {
        if( updaterToRemove == *it )
        {
            m_updaters.erase( it );
            mapping.erase( itToRemove );
        
            return;
        }
    }

    assert( false );
}

} //bv
