#pragma once

#include <vector>

#include "Engine/Interfaces/IUpdater.h"


namespace bv
{

class UpdatersManager
{
private:

    std::vector< IUpdaterPtr >   m_updaters;

public:

                                UpdatersManager     ();
                                ~UpdatersManager    ();

    void                        UpdateStep          ();

    void                        RegisterUpdater     ( IUpdaterPtr updater );
    void                        RemoveAllUpdaters   ();

    static UpdatersManager &    Get                 ();

};

} // bv
