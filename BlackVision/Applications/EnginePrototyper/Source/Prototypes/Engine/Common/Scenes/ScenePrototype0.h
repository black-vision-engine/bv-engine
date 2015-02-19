#pragma once

#include "Prototypes/Engine/Common/ScenePrototype.h"

namespace bv {

class ScenePrototype0 : public ScenePrototype
{
private:

    unsigned int m_n0;
    unsigned int m_n1;
    unsigned int m_n2;

public:

                                ScenePrototype0     ();

    virtual void                Update              ( TimeType t );
    virtual void                OnKey               ( unsigned char c );

private:

    virtual SceneNode *         BuildSceneImpl      ();

private:

    void                        PrintStructure      ();
    void                        PrintStructure      ( SceneNode * node, unsigned int idx, unsigned int indent );

    SceneNode *                 GetNode             ( unsigned int * pos, unsigned int numElts );

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
