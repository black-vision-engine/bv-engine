#pragma once

#include <vector>

#include "Engine/Interfaces/IUpdater.h"


namespace bv
{

class UpdatersManager
{
private:

    std::vector< IUpdater * >   m_updaters;

public:

                                UpdatersManager     ();
                                ~UpdatersManager    ();

    void                        UpdateStep          ( TimeType t );

    void                        RegisterUpdater     ( IUpdater * updater );

    static UpdatersManager &    Get                 ();

};


} // bv
