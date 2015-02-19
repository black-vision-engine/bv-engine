#pragma once

#include "Prototypes/Engine/Common/ScenePrototype.h"


namespace bv {

class ScenePrototype1 : public ScenePrototype
{
public:

    // TODO: implemnt if necessary

    ScenePrototype1( Renderer * renderer );

private:

    virtual SceneNode *         BuildSceneImpl      ();

};

} //bv
