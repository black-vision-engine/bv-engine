#pragma once

#include "Common/ScenePrototype.h"


namespace bv {

class ScenePrototype2 : public ScenePrototype
{
public:

    // TODO: implemnt if necessary

    ScenePrototype2( Renderer * renderer );

private:

    virtual SceneNode *         BuildSceneImpl      ();

};

} //bv
