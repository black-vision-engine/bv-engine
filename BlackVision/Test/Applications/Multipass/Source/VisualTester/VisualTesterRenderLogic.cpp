#include "VisualTesterRenderLogic.h"
#include "LibImage.h"
#include <limits.h>
#include "gtest\gtest.h"



double imageUINT8BitsPerChannel( const char* refImage, const char* renderedImage, unsigned char& step )
{
	double retValue = 0.0;
	step = 4;
	
	for( char channel = 0; channel < 4; ++channel )
		retValue += static_cast<double>( abs( refImage[channel] - renderedImage[channel] ) );
	return retValue;
}

void imageUINT8BitsPerChannelDiff( double imageError, char* diffImage )
{
	unsigned char diff = unsigned char( imageError / 4 );			// UCHAR_MAX * imageError / (4 * UCHAR_MAX);
	for( char channel = 0; channel < 4; ++channel )
		diffImage[channel] = diff;
}

const std::string file_ext = ".bmp";
const std::string ref_image = "ReferenceImage";
const std::string diff_image = "DiffToReference";

namespace bv
{


VisualTesterRenderLogic::VisualTesterRenderLogic()
{
	currentCompareFunction = imageUINT8BitsPerChannel;
	currentDiffWriteFunction = imageUINT8BitsPerChannelDiff;
	errorTolerance = 0.0;
	makeDiffImage = true;
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

	std::string fullName = fileName + ref_image + file_ext;

	bool succes = image::SaveBMPImage( fullName, renderTarget->GetData(), renderTarget->GetWidth(), renderTarget->GetHeight(), 32 );
	ASSERT_TRUE( succes );
}


void VisualTesterRenderLogic::renderCompareWithReferenceImage( Renderer* renderer, SceneNode* node, const std::string fileName )
{
	ASSERT_TRUE( renderer != nullptr );
	ASSERT_TRUE( node != nullptr );

	Texture2DConstPtr renderTarget = renderImage( renderer, node );

	std::string fullName = fileName + ref_image + file_ext;
	UInt32 imageWidth;
	UInt32 imageHeight;
	UInt32 imageBPP;
	UInt32 channelNum;
	MemoryChunkConstPtr refImageMemChunk = image::LoadImage( fullName, &imageWidth, &imageHeight, &imageBPP, &channelNum, false );

	ASSERT_EQ( renderTarget->GetHeight(), imageHeight );
	ASSERT_EQ( renderTarget->GetWidth(), imageWidth );

	char* diffImage = nullptr;
	if( makeDiffImage )
	{
		diffImage  = new char[imageHeight * imageWidth * imageBPP / 8];
	}
	
	const char* refImage = refImageMemChunk->Get();
	const char* renderedImage = renderTarget->GetData()->Get();
	double imageError = 0.0;


	for( unsigned int i = 0; i < imageHeight * imageWidth; )
	{
		unsigned char step;		// currentCompareFunction will put here our step.
		double stepError = currentCompareFunction( refImage + i, renderedImage + i, step );
		imageError += stepError;


		if( makeDiffImage )
			currentDiffWriteFunction( stepError, diffImage + i );

		i += step;
	}
	

	EXPECT_LE( errorTolerance, imageError );

	if( makeDiffImage && imageError > errorTolerance )
	{
		fullName = fileName + diff_image + file_ext;
		MemoryChunkConstPtr diffImageChunk = std::make_shared<MemoryChunk>( diffImage, imageHeight * imageWidth * imageBPP / 8 );

		image::SaveBMPImage( fullName, diffImageChunk, imageWidth, imageHeight, imageBPP );
	}
}

} //bv
