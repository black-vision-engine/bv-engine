#pragma once

#include <vector>
#include <hash_map>

#include "Engine/Interfaces/IUpdater.h"

#include "EngineResources.h"



namespace bv {

namespace model {
class IModelNode;
class SceneModel;
} // model

class UpdatersManager
{
private:

    std::hash_map< const model::IModelNode *, IUpdaterPtr > m_nodeUpdatersMapping;
    std::hash_map< const model::SceneModel *, IUpdaterPtr > m_sceneUpdatersMapping;

    std::vector< IUpdaterPtr >      m_updaters;

    EngineResources                 m_engineResources;

public:

                                UpdatersManager     ();
                                ~UpdatersManager    ();

    void                        UpdateStep          ();

    bool                        IsRegistered        ( const model::IModelNode * node );
    void                        RegisterUpdater     ( const model::IModelNode * node,  IUpdaterPtr updater );
    void                        RemoveNodeUpdater   ( const model::IModelNode * node );

    bool                        IsRegistered        ( const model::SceneModel * scene );
    void                        RegisterUpdater     ( const model::SceneModel * scene,  IUpdaterPtr updater );
    void                        RemoveSceneUpdater  ( const model::SceneModel * scene );

    void                        RemoveAllUpdaters   ();

    static UpdatersManager &    Get                 ();

private:

    template< typename T >
    void                        RegisterUpdater     ( const T * obj, IUpdaterPtr updater, std::hash_map< const T *, IUpdaterPtr > & mapping );

    template< typename T >
    void                        RemoveUpdater       ( const T * obj, std::hash_map< const T *, IUpdaterPtr > & mapping );

};

} // bv
