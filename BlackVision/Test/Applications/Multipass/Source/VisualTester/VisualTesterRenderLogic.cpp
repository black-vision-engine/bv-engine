#include "VisualTesterRenderLogic.h"
#include "LibImage.h"
#include <limits.h>
#include "gtest\gtest.h"
#include "System\Env.h"


const std::string AssetsPath = "../../../../Test/Assets/Multipass/";
const std::string TestsResultsDir = "";
const std::string file_ext = ".bmp";
const std::string ref_image = "ReferenceImage";
const std::string diff_image = "DiffToReference";


double imageRGBA8BitsPerChannelIgnoreAlpha( const char* refImage, const char* renderedImage )
{
	double retValue = 0.0;
	
	for( char channel = 0; channel < 3; ++channel )
	{
		short refVal = refImage[channel] & 0xFF;
		short rendVal = renderedImage[channel] & 0xFF;
		retValue += static_cast<double>( abs( refVal - rendVal ) );
	}
	return retValue;
}

void imageRGBA8BitsPerChannelDiffIgnoreAlpha( double imageError, char* diffImage )
{
	unsigned char alpha = UCHAR_MAX;
	unsigned char diff = unsigned char( imageError / 3 );			// UCHAR_MAX * imageError / (3 * UCHAR_MAX); alpha channel ignored

	for( char channel = 0; channel < 3; ++channel )
		diffImage[channel] = diff & 0xFF;
	diffImage[3] = (char)alpha;
}



namespace bv
{


VisualTesterRenderLogic::VisualTesterRenderLogic()
{
	currentCompareFunction = imageRGBA8BitsPerChannelIgnoreAlpha;
	currentDiffWriteFunction = imageRGBA8BitsPerChannelDiffIgnoreAlpha;
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

	std::string fullName = AssetsPath + fileName + ref_image + file_ext;
	bool succes = image::SaveBMPImage( fullName, renderTarget->GetData(), (UInt32)renderTarget->GetWidth(), (UInt32)renderTarget->GetHeight(), 32 );
	{ succes; }
	ASSERT_TRUE( succes );
}


void VisualTesterRenderLogic::renderCompareWithReferenceImage( Renderer* renderer, SceneNode* node, const std::string fileName )
{
	ASSERT_TRUE( renderer != nullptr );
	ASSERT_TRUE( node != nullptr );

	Texture2DConstPtr renderTarget = renderImage( renderer, node );

	std::string fullName = AssetsPath + fileName + ref_image + file_ext;
	UInt32 imageWidth;
	UInt32 imageHeight;
	UInt32 imageBPP;
	UInt32 channelNum;
	MemoryChunkConstPtr refImageMemChunk = image::LoadImage( fullName, &imageWidth, &imageHeight, &imageBPP, &channelNum, false );

	ASSERT_NE( refImageMemChunk->Get(), nullptr );
	ASSERT_EQ( renderTarget->GetHeight(), imageHeight );
	ASSERT_EQ( renderTarget->GetWidth(), imageWidth );


	UInt32 BytesPP = imageBPP / 8;
	char* diffImage = nullptr;
	if( makeDiffImage )
	{
		diffImage  = new char[imageHeight * imageWidth * BytesPP];
	}
	
	const char* refImage = refImageMemChunk->Get();
	const char* renderedImage = renderTarget->GetData()->Get();
	double imageError = 0.0;


	for( unsigned int i = 0; i < imageHeight * imageWidth * BytesPP; )
	{
		double stepError = currentCompareFunction( refImage + i, renderedImage + i );
		imageError += stepError;


		if( makeDiffImage )
			currentDiffWriteFunction( stepError, diffImage + i );

		i += BytesPP;
	}
	

	EXPECT_LE( imageError, errorTolerance );

	if( makeDiffImage && imageError > errorTolerance )
	{
		fullName = TestsResultsDir + fileName + diff_image + file_ext;
		MemoryChunkConstPtr diffImageChunk = std::make_shared<MemoryChunk>( diffImage, imageHeight * imageWidth * BytesPP);

		image::SaveBMPImage( fullName, diffImageChunk, imageWidth, imageHeight, imageBPP );
	}
}

} //bv
