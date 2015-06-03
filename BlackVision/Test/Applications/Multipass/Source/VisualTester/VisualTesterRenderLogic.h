#pragma once


#include "RenderLogic.h"
#include "OffscreenRenderLogic.h"
#include <string>

namespace bv
{


class VisualTesterRenderLogic :
	public RenderLogic
{
private:

public:
	VisualTesterRenderLogic();
	~VisualTesterRenderLogic();


	void renderReferenceImage( Renderer* renderer, SceneNode* node, const std::string fileName );
	void renderCompareWithReferenceImage( Renderer* renderer, SceneNode* node, const std::string fileName );
};



}