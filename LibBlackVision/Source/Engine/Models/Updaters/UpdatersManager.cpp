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
void UpdatersManager::RegisterUpdater    ( IUpdaterPtr updater )
{
    m_updaters.push_back( updater );
}

// *******************************
//
void UpdatersManager::RemoveAllUpdaters   ()
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
