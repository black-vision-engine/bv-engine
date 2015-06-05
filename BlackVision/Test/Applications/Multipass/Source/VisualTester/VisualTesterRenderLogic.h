#pragma once


#include "RenderLogic.h"
#include "OffscreenRenderLogic.h"
#include <string>

namespace bv
{

/**@brief Function comparing to samples od images: renderedImage with refImage.

@todo Format of image is undefined. What if render target isn't 32 bit RGBA?*/
typedef double (*SampleCompareFunction)( const char* refImage, const char* renderedImage );		


typedef void (*SampleDiffWriteFunction)( double stepError, char* diffImage );

class VisualTesterRenderLogic :
	public RenderLogic
{
private:
	SampleCompareFunction		currentCompareFunction;
	SampleDiffWriteFunction		currentDiffWriteFunction;

	Texture2DConstPtr renderImage( Renderer* renderer, SceneNode* node );
public:
	VisualTesterRenderLogic();
	~VisualTesterRenderLogic();

	void setOwnCompareFunction( SampleCompareFunction newFunction ) { currentCompareFunction = newFunction; }
	void setOwnDiffWriteFunction( SampleDiffWriteFunction newFunction ) { currentDiffWriteFunction = newFunction; }

	void renderReferenceImage( Renderer* renderer, SceneNode* node, const std::string fileName );
	void renderCompareWithReferenceImage( Renderer* renderer, SceneNode* node, const std::string fileName );
public:
	double errorTolerance;
	bool makeDiffImage;
};



}