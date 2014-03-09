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
void UpdatersManager::UpdateStep        ()
{
    for( auto updater : m_updaters )
    {
        updater->DoUpdate();
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
