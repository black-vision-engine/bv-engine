#include "VisualTesterRenderLogic.h"
#include "LibImage.h"




namespace bv
{


VisualTesterRenderLogic::VisualTesterRenderLogic()
{
}


VisualTesterRenderLogic::~VisualTesterRenderLogic()
{
}


void VisualTesterRenderLogic::renderReferenceImage( Renderer* renderer, SceneNode* node, const std::string fileName )
{
	assert( renderer );
	assert( node );

	RenderFrame( renderer, node );

	Texture2DConstPtr renderTarget = m_offscreenRenderLogic->ReadDisplayTarget( renderer, 0 );
	bool succes = image::SaveBMPImage( fileName, renderTarget->GetData(), (UInt32)renderTarget->GetWidth(), (UInt32)renderTarget->GetHeight(), 32 );
	{ succes; }
	assert( succes );
}


void VisualTesterRenderLogic::renderCompareWithReferenceImage( Renderer* renderer, SceneNode* node, const std::string fileName )
{
	node;
	renderer;
	fileName;
}

} //bv