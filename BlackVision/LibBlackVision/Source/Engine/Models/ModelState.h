#pragma once

#include <map>
#include <set>

#include "Mathematics/glm_inc.h"
#include "CoreDEF.h"

namespace bv 
{

class BVProject;

namespace model 
{

class IModelNode;
DEFINE_PTR_TYPE( IModelNode );
DEFINE_CONST_PTR_TYPE( IModelNode );
struct NodeState;
class SceneModel;


class ModelState
{
public:

    std::string                         QueryNodeSceneName  ( const IModelNode * node ) const;
    const SceneModel *                  QueryNodeScene      ( const IModelNode * node ) const;
    const IModelNode *                  QueryNodeParent     ( const IModelNode * node ) const;
    std::string                         QueryNodePath       ( const IModelNode * node ) const;
    std::string                         BuildIndexPath      ( const IModelNode * node );
    UInt32                              GetNodeIndex        ( const IModelNode * node ) const;

    bool                                RegisterNode    ( const IModelNode * node, const IModelNode * parent );
    bool                                UnregisterNode  ( const IModelNode * node );

    static ModelState &                 GetInstance     ();

    bool                                RegisterBVProject ( const BVProject * project );

    // Node selection
    bool                                IsSelected          ( IModelNodeConstPtr node );
    glm::vec4                           GetSelectedNodeColor ( IModelNodeConstPtr node );
    void                                Select              ( IModelNodePtr node, glm::vec4 color );
    bool                                Unselect            ( IModelNodePtr node );
    void                                UnselectAll         ();
    void                                UnselectRecursive   ( IModelNodePtr node );
    std::set< IModelNodeConstPtr >      GetSelectedNodes    ();

    const BVProject *                   GetBVProject        () const;

private:
    
    std::string                         RestoreNodePath ( const IModelNode * node, std::string * sceneName ) const;
    const NodeState *                   GetNodeState    ( const IModelNode * node ) const;


    ModelState  ();
    ~ModelState ();

    typedef std::map< const IModelNode *, NodeState * > NodeStatesType;
    NodeStatesType                      m_nodeStates;
    const BVProject *                   m_project;
    std::map< IModelNodeConstPtr, glm::vec4 > m_selectedNodes;
};

} // model
} // bv