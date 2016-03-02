#pragma once

#include <map>
#include <set>

namespace bv 
{

class BVProject;

namespace model 
{

class IModelNode;
struct NodeState;

class ModelState
{
public:

    const std::string &                 QueryNodeScene  ( const IModelNode * node ) const;
    const IModelNode *                  QueryNodeParent ( const IModelNode * node ) const;
    const std::string &                 QueryNodePath   ( const IModelNode * node ) const;

    bool                                RegisterNode    ( const IModelNode * node, const IModelNode * parent );
    bool                                UnregisterNode  ( const IModelNode * node );

    static ModelState &                 GetInstance     ();

    bool                                RegisterBVProject ( const BVProject * project );

    // Node selection
    bool                                IsSelected      ( IModelNode * node );
    void                                Select          ( IModelNode * node );
    bool                                Unselect        ( IModelNode * node );
    void                                UnselectAll     ();

private:
    
    std::string                         RestoreNodePath ( const IModelNode * node, std::string * sceneName ) const;


    ModelState  ();
    ~ModelState ();

    typedef std::map< const IModelNode *, NodeState * > NodeStatesType;
    NodeStatesType                  m_nodeStates;
    const BVProject *               m_project;
    std::set< IModelNode * >        m_selectedNodes;
};

} // model
} // bv