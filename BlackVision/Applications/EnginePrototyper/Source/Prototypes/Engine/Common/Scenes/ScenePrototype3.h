#pragma once


#include "Prototypes/Engine/Common/ScenePrototype.h"


namespace bv {

class ScenePrototype3 : public ScenePrototype
{
public:

    ScenePrototype3( Renderer * renderer );

    virtual void                Update              ( TimeType t );
    virtual void                OnKey               ( unsigned char c );

private:

    virtual SceneNode *         BuildSceneImpl      ();

};

} //bv



