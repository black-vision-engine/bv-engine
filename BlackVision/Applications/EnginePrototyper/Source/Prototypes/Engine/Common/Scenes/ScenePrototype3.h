#pragma once


#include "Prototypes/Engine/Common/ScenePrototype.h"


namespace bv {

class ScenePrototype3 : public ScenePrototype
{
private:
	int numPasses;
public:

    ScenePrototype3( Renderer * renderer );

    virtual void                Update              ( TimeType t );
    virtual void                OnKey               ( unsigned char c );

	inline int getNumPasses() { return numPasses; }
private:

    virtual SceneNode *         BuildSceneImpl      ();

};

} //bv



