#pragma once

#include <map>
#include <set>

namespace bv 
{

class BVProject;

namespace model 
{

class IModelNode;
DEFINE_PTR_TYPE( IModelNode );
struct NodeState;

class ModelState
{
public:

    std::string                         QueryNodeScene  ( const IModelNode * node ) const;
    const IModelNode *                  QueryNodeParent ( const IModelNode * node ) const;
    std::string                         QueryNodePath   ( const IModelNode * node ) const;
    std::string                         BuildIndexPath  ( const IModelNode * node );

    bool                                RegisterNode    ( const IModelNode * node, const IModelNode * parent );
    bool                                UnregisterNode  ( const IModelNode * node );

    static ModelState &                 GetInstance     ();

    bool                                RegisterBVProject ( const BVProject * project );

    // Node selection
    bool                                IsSelected          ( IModelNodePtr node );
    void                                Select              ( IModelNodePtr node );
    bool                                Unselect            ( IModelNodePtr node );
    void                                UnselectAll         ();
    void                                UnselectRecursive   ( IModelNodePtr node );
    std::set< IModelNodePtr > &         GetSelectedNodes    ();

private:
    
    std::string                         RestoreNodePath ( const IModelNode * node, std::string * sceneName ) const;
    const NodeState *                   GetNodeState    ( const IModelNode * node ) const;


    ModelState  ();
    ~ModelState ();

    typedef std::map< const IModelNode *, NodeState * > NodeStatesType;
    NodeStatesType                  m_nodeStates;
    const BVProject *               m_project;
    std::set< IModelNodePtr >       m_selectedNodes;
};

} // model
} // bv