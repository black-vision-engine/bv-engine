//#pragma once
//
//#include <string>
//
//#include "RenderLogicTst.h"
//
//
//namespace bv
//{
//
///**@brief Function comparing to samples od images: renderedImage with refImage.
//
//@todo Format of image is undefined. What if render target isn't 32 bit RGBA?*/
//typedef double (*SampleCompareFunction)( const char* refImage, const char* renderedImage );		
//
//
//typedef void (*SampleDiffWriteFunction)( double stepError, char* diffImage );
//
//class VisualTesterRenderLogic : public RenderLogicTst
//{
//private:
//
//    SampleCompareFunction		currentCompareFunction;
//	SampleDiffWriteFunction		currentDiffWriteFunction;
//
//	Texture2DConstPtr RenderImage( Renderer* renderer, SceneNode* node );
//
//public:
//	
//    VisualTesterRenderLogic();
//	~VisualTesterRenderLogic();
//
//	void SetOwnCompareFunction( SampleCompareFunction newFunction ) { currentCompareFunction = newFunction; }
//	void SetOwnDiffWriteFunction( SampleDiffWriteFunction newFunction ) { currentDiffWriteFunction = newFunction; }
//
//	void RenderReferenceImage( Renderer* renderer, SceneNode* node, const std::string fileName );
//	void RenderCompareWithReferenceImage( Renderer* renderer, SceneNode* node, const std::string fileName );
//
//public:
//
//	double  errorTolerance;
//	bool    makeDiffImage;
//};
//
//} //bv
