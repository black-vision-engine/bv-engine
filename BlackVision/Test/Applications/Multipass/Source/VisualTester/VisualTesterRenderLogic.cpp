#include "VisualTesterRenderLogic.h"
#include "LibImage.h"
#include "gtest\gtest.h"



double imageUINT8BitsPerChannel( const char* refImage, const char* renderedImage, unsigned char& step )
{
	double retValue = 0.0;
	step = 4;
	
	for( char channel = 0; channel < 4; ++channel )
		retValue += static_cast<double>( abs( refImage[channel] - renderedImage[channel] ) );
	return retValue;
}

namespace bv
{


VisualTesterRenderLogic::VisualTesterRenderLogic()
{
	currentCompareFunction = imageUINT8BitsPerChannel;
	errorTolerance = 0.0;
}


VisualTesterRenderLogic::~VisualTesterRenderLogic()
{
}


Texture2DConstPtr VisualTesterRenderLogic::renderImage( Renderer* renderer, SceneNode* node )
{
	RenderFrame( renderer, node );

	return m_offscreenRenderLogic->ReadDisplayTarget( renderer, 0 );
}

void VisualTesterRenderLogic::renderReferenceImage( Renderer* renderer, SceneNode* node, const std::string fileName )
{
	ASSERT_TRUE( renderer != nullptr );
	ASSERT_TRUE( node != nullptr );

	Texture2DConstPtr renderTarget = renderImage( renderer, node );

	bool succes = image::SaveBMPImage( fileName, renderTarget->GetData(), renderTarget->GetWidth(), renderTarget->GetHeight(), 32 );
	ASSERT_TRUE( succes );
}


void VisualTesterRenderLogic::renderCompareWithReferenceImage( Renderer* renderer, SceneNode* node, const std::string fileName )
{
	ASSERT_TRUE( renderer != nullptr );
	ASSERT_TRUE( node != nullptr );

	Texture2DConstPtr renderTarget = renderImage( renderer, node );

	UInt32 imageWidth;
	UInt32 imageHeight;
	UInt32 imageBPP;
	UInt32 channelNum;
	MemoryChunkConstPtr refImageMemChunk = image::LoadImage( fileName, &imageWidth, &imageHeight, &imageBPP, &channelNum, false );

	ASSERT_EQ( renderTarget->GetHeight(), imageHeight );
	ASSERT_EQ( renderTarget->GetWidth(), imageWidth );
	
	const char* refImage = refImageMemChunk->Get();
	const char* renderedImage = renderTarget->GetData()->Get();
	double imageError = 0.0;


	for( unsigned int i = 0; i < imageHeight * imageWidth; )
	{
		unsigned char step;

		imageError += currentCompareFunction( refImage + i, renderedImage + i, step );

		i += step;
	}
	

	EXPECT_EQ( errorTolerance, imageError );
}

} //bv
