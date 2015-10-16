#pragma once

#include <vector>

#include "Engine/Models/Plugins/Interfaces/IPluginDescriptor.h"
#include "Engine/Models/BVScene.h"

namespace bv {

class TestScene
{
private:
    BVScenePtr              m_scene;

public:
							TestScene			( Renderer * renderer );
							~TestScene			();
	
	static std::vector< model::IPluginDescriptor * >  DefaultBVPluginDescriptors  ();

	BVScenePtr				GetScene			();

//test scenes
	
	model::BasicNodePtr		ColoredRectangles	( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator );

};

DEFINE_PTR_TYPE( TestScene )
DEFINE_CONST_PTR_TYPE( TestScene )


} // bv
