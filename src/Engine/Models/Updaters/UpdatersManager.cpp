#include "UpdatersManager.h"


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
    for( auto u : m_updaters )
    {
        delete u;
    }
}

// *******************************
//
void UpdatersManager::UpdateStep        ( TimeType t )
{
    for( auto updater : m_updaters )
    {
        updater->Update( t );
    }
}

// *******************************
//
void UpdatersManager::RegisterUpdater    ( IUpdater * updater )
{
    m_updaters.push_back( updater );
}

// *******************************
//
UpdatersManager & UpdatersManager::Get  ()
{
    static UpdatersManager instance = UpdatersManager();
    return instance;
}

} //bv
