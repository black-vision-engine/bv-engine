#pragma once


#include "Common/ScenePrototype.h"


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
	SceneNode*					CreateScene( std::string& file1, std::string& file2 );
private:

    virtual SceneNode *         BuildSceneImpl      ();
};

} //bv



