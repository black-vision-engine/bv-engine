#pragma once

#include <vector>
#include <hash_map>

#include "Engine/Interfaces/IUpdater.h"


namespace bv {

namespace model {
class IModelNode;
} // model

class UpdatersManager
{
private:

    std::hash_map< const model::IModelNode *, IUpdaterPtr > m_updatersMapping;
    std::vector< IUpdaterPtr >                              m_updaters;

public:

                                UpdatersManager     ();
                                ~UpdatersManager    ();

    void                        UpdateStep          ();

    bool                        IsRegistered        ( const model::IModelNode * node );

    void                        RegisterUpdater     ( const model::IModelNode * node,  IUpdaterPtr updater );
    void                        RemoveNodeUpdater   ( const model::IModelNode * node );

    void                        RemoveAllUpdaters   ();

    static UpdatersManager &    Get                 ();

};

} // bv
