//#include "VisualTesterRenderLogic.h"
//#include "LibImage.h"
//#include <limits.h>
//#include "gtest\gtest.h"
//#include "System\Env.h"
//
////FIXME: CO TO JEST? TO JUZ NIE MOZNA KOSZERNIE SKONFIGUROWAC KATALOGOW TESTOWYCH?
//
////const std::string TestDir = bv::Env::GetVar("TestDir");
//const std::string TestDir = "../../../../Test";
//const std::string AssetsPath = TestDir + "/Assets/";
////const std::string TestsResultsDir = bv::Env::GetVar("TestsResultsDir");
//const std::string TestsResultsDir = "../../../../_TestsResults/";
//const std::string FileExt = ".bmp";
//const std::string FileExtLoad = ".png";
//const std::string RefImageString = "ReferenceImage";
//const std::string DiffImageString = "DiffToReference";
//
//
//double imageRGBA8BitsPerChannelIgnoreAlpha( const char* refImage, const char* renderedImage )
//{
//	double retValue = 0.0;
//	
//	for( char channel = 0; channel < 3; ++channel )
//	{
//		short refVal = refImage[channel] & 0xFF;
//		short rendVal = renderedImage[channel] & 0xFF;
//		retValue += static_cast<double>( abs( refVal - rendVal ) );
//	}
//	return retValue;
//}
//
//void imageRGBA8BitsPerChannelDiffIgnoreAlpha( double imageError, char* diffImage )
//{
//	unsigned char alpha = UCHAR_MAX;
//	unsigned char diff = unsigned char( imageError / 3 );			// UCHAR_MAX * imageError / (3 * UCHAR_MAX); alpha channel ignored
//
//	for( char channel = 0; channel < 3; ++channel )
//		diffImage[channel] = diff & 0xFF;
//	diffImage[3] = (char)alpha;
//}
//
//
//
//namespace bv
//{
//
//
//VisualTesterRenderLogic::VisualTesterRenderLogic()
//{
//	currentCompareFunction = imageRGBA8BitsPerChannelIgnoreAlpha;
//	currentDiffWriteFunction = imageRGBA8BitsPerChannelDiffIgnoreAlpha;
//	errorTolerance = 0.0;
//	makeDiffImage = true;
//}
//
//
//VisualTesterRenderLogic::~VisualTesterRenderLogic()
//{
//}
//
//
//Texture2DConstPtr VisualTesterRenderLogic::RenderImage( Renderer * renderer, SceneNode * node )
//{
//    assert( false );
//
//    return nullptr;
//
//	//RenderFrame( renderer, node );
//
//	//return m_offscreenRenderLogicTst->ReadDisplayTarget( renderer, 0 );
//}
//
//void VisualTesterRenderLogic::RenderReferenceImage( Renderer* renderer, SceneNode* node, const std::string fileName )
//{
//	ASSERT_TRUE( renderer != nullptr );
//	ASSERT_TRUE( node != nullptr );
//
//	Texture2DConstPtr renderTarget = RenderImage( renderer, node );
//
//	std::string fullName = AssetsPath + fileName + RefImageString + FileExt;
//	bool succes = image::SaveBMPImage( fullName, renderTarget->GetData(), (UInt32)renderTarget->GetWidth(), (UInt32)renderTarget->GetHeight(), 32 );
//	{ succes; }
//	ASSERT_TRUE( succes );
//}
//
//
//void VisualTesterRenderLogic::RenderCompareWithReferenceImage( Renderer* renderer, SceneNode* node, const std::string fileName )
//{
//	ASSERT_TRUE( renderer != nullptr );
//	ASSERT_TRUE( node != nullptr );
//
//	Texture2DConstPtr renderTarget = RenderImage( renderer, node );
//
//	std::string fullName = AssetsPath + fileName + RefImageString + FileExt;		// In future maybe it will be FileExtLoad. We want to load .png, because it has smaller size.
//	UInt32 imageWidth;
//	UInt32 imageHeight;
//	UInt32 imageBPP;
//	UInt32 channelNum;
//	MemoryChunkConstPtr refImageMemChunk = image::LoadImage( fullName, &imageWidth, &imageHeight, &imageBPP, &channelNum, false );
//
//	ASSERT_NE( refImageMemChunk->Get(), nullptr );
//	ASSERT_EQ( renderTarget->GetHeight(), imageHeight );
//	ASSERT_EQ( renderTarget->GetWidth(), imageWidth );
//
//
//	UInt32 BytesPP = imageBPP / 8;
//	char* diffImage = nullptr;
//	if( makeDiffImage )
//	{
//		diffImage  = new char[imageHeight * imageWidth * BytesPP];
//	}
//	
//	const char* refImage = refImageMemChunk->Get();
//	const char* renderedImage = renderTarget->GetData()->Get();
//	double imageError = 0.0;
//
//
//	for( unsigned int i = 0; i < imageHeight * imageWidth * BytesPP; )
//	{
//		double stepError = currentCompareFunction( refImage + i, renderedImage + i );
//		imageError += stepError;
//
//
//		if( makeDiffImage )
//			currentDiffWriteFunction( stepError, diffImage + i );
//
//		i += BytesPP;
//	}
//	
//
//	EXPECT_LE( imageError, errorTolerance );
//
//	if( makeDiffImage && imageError > errorTolerance )
//	{
//		fullName = TestsResultsDir + fileName + DiffImageString + FileExt;
//		MemoryChunkConstPtr diffImageChunk = std::make_shared<MemoryChunk>( diffImage, imageHeight * imageWidth * BytesPP);
//
//		image::SaveBMPImage( fullName, diffImageChunk, imageWidth, imageHeight, imageBPP );
//	}
//}
//
//} //bv
