#pragma once

#include "Common/ScenePrototype.h"

namespace bv {

class ScenePrototype0 : public ScenePrototype
{
private:

    unsigned int m_n0;
    unsigned int m_n1;
    unsigned int m_n2;

    SceneNode * m_detachedNode;

public:

                                ScenePrototype0     ( Renderer * renderer );

    virtual void                Update              ( TimeType t );
    virtual void                OnKey               ( unsigned char c );

private:

    virtual SceneNode *         BuildSceneImpl      ();

private:

    void                        PrintStructure      ();
    void                        PrintStructure      ( SceneNode * node, unsigned int idx, unsigned int indent );

    void                        DeleteNodeExperimental( SceneNode * node );

    bool                        DetachNode          ( SceneNode * node, SceneNode * detachNode );

    SceneNode *                 GetNode             ( SceneNode * node, unsigned int * pos, unsigned int numElts );

    void                        ToggleNode332       ();

    SceneNode *                 RootNode            ();

    SceneNode *                 Node1               ();
    SceneNode *                 Node2               ();
    SceneNode *                 Node3               ();

    SceneNode *                 Node21              ();
    SceneNode *                 Node22              ();

    SceneNode *                 Node31              ();
    SceneNode *                 Node32              ();
    SceneNode *                 Node33              ();

    SceneNode *                 Node331             ();
    SceneNode *                 Node332             ();
};

} //bv
