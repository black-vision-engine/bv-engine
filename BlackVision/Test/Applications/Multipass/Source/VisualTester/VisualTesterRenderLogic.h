#pragma once


#include "RenderLogic.h"
#include "OffscreenRenderLogic.h"
#include <string>

namespace bv
{

/**@brief Function comparing to samples od images: renderedImage with refImage.

Format of image is undefined. Thats why there's third parameter that returns
offset to next sample.*/
typedef double (*CompareFunction)( const char* refImage, const char* renderedImage, unsigned char& step );		

class VisualTesterRenderLogic :
	public RenderLogic
{
private:
	CompareFunction currentCompareFunction;


	Texture2DConstPtr renderImage( Renderer* renderer, SceneNode* node );
public:
	VisualTesterRenderLogic();
	~VisualTesterRenderLogic();

	void setOwnCompareFunction( CompareFunction newFunction ) { currentCompareFunction = newFunction; }

	void renderReferenceImage( Renderer* renderer, SceneNode* node, const std::string fileName );
	void renderCompareWithReferenceImage( Renderer* renderer, SceneNode* node, const std::string fileName );
public:
	double errorTolerance;
};



}