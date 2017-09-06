#include "TestSceneUtils.h"

#include <random>
#include <cassert>

#include "Assets/Font/TextHelper.h"

#include "LibImage.h"

#include "Utils/Nodes/TestNodesCreator.h"



namespace bv {

const std::string	TestSceneUtils::TEXTURE_PATH	= "Assets/checkboard.bmp";
const std::string	TestSceneUtils::ALPHA_MASK0_PATH = "Assets/checkboard_am0.bmp";
const std::string	TestSceneUtils::ALPHA_MASK_PATH	= "Assets/checkboard_am.bmp";
const std::string	TestSceneUtils::ANIM_PATH		= "seq:/anim";

const std::string	TestSceneUtils::VIDEO_PATH0		= "Assets/cameraman.mp4";
const std::string	TestSceneUtils::VIDEO_PATH1		= "Assets/dxv.mov";

const Float32		TestSceneUtils::SPEED			= 1.f;
const UInt32		TestSceneUtils::ANIM_NUM		= 10;
const UInt32		TestSceneUtils::IMG_SIZE		= 128;
const UInt32		TestSceneUtils::AM_SIZE			= 2048;

// ****************************
//
const UInt32		TestSceneUtils::GEOM_PLUGINS_NUM = 16;
const std::string	TestSceneUtils::PluginsArr[] = { "DEFAULT_TRIANGLE", "DEFAULT_CIRCLE", "DEFAULT_ELLIPSE", "DEFAULT_ROUNDEDRECT", "DEFAULT_CUBE", "DEFAULT_SIMPLE_CUBE", "DEFAULT_SPHERE", "DEFAULT_GEOSPHERE", "DEFAULT_CYLINDER", "DEFAULT_CONE", "DEFAULT_TORUS", "DEFAULT_SPRING", "DEFAULT_COGWHEEL", "DEFAULT_PIECHART", "DEFAULT_PRISM", "DEFAULT_RECTANGLE" };
const std::string	TestSceneUtils::PluginsNameArr[] = { "triangle", "circle", "ellipse", "rounded rect", "cube", "simple cube", "sphere", "sphere", "cylinder", "cone", "torus", "spring", "cog wheel", "piechart", "prism", "rectangle" };



const std::string       FakeConfig::EmptyString = "";
const FakeConfig        FakeConfig::Config;


// ****************************
//
void	                TestSceneUtils::AddColoredRectangleScene	( BVProjectEditor * editor, const std::string & name, glm::vec4 color, glm::vec3 transform )
{
    auto sceneModel = model::SceneModel::Create( name );
	editor->AddScene( sceneModel );
    
    auto root = TestSceneUtils::ColoredRectangle( editor->GetSceneDefaultTimeline( sceneModel ), "root", 0.5f, 0.5f, color, ALPHA_MASK0_PATH );
    SetParameterTranslation( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, transform );
    editor->AddChildNode( sceneModel, nullptr, root );
}

// ****************************
//
model::BasicNodePtr		TestSceneUtils::ColoredRectangle			( model::ITimeEvaluatorPtr timeEval, const std::string & name, Float32 width, Float32 height, glm::vec4 color, const std::string & alphaMask )
{
    return TestNodesCreator::ColoredRectangle( timeEval, name, width, height, color, alphaMask );
}

// ****************************
//
model::BasicNodePtr		TestSceneUtils::ColoredGeometry				( model::ITimeEvaluatorPtr timeEval, const std::string & name, const std::string & plugin, glm::vec4 color, const std::string & alphaMask )
{
    return TestNodesCreator::ColoredGeometry( timeEval, name, plugin, color, alphaMask );
}

// ****************************
//
model::BasicNodePtr		TestSceneUtils::TexturedRectangle		( model::ITimeEvaluatorPtr timeEval, const std::string & name, Float32 width, Float32 height, const std::string & path, const std::string & alphaMask )
{
    return TestNodesCreator::TexturedRectangle( timeEval, name, width, height, path, alphaMask );
}

// ****************************
//
model::BasicNodePtr		TestSceneUtils::TexturedGeometry		( model::ITimeEvaluatorPtr timeEval, const std::string & name, const std::string & plugin, const std::string & path, const std::string & alphaMask )
{
    return TestNodesCreator::TexturedGeometry( timeEval, name, plugin, path, alphaMask );
}

// ****************************
//
model::BasicNodePtr		TestSceneUtils::AnimatedRectangle		( model::ITimeEvaluatorPtr timeEval, const std::string & name, Float32 width, Float32 height, const std::string & path, const std::string & alphaMask )
{
    return TestNodesCreator::AnimatedRectangle( timeEval, name, width, height, path, alphaMask );
}

// ****************************
//
model::BasicNodePtr		TestSceneUtils::AnimatedGeometry		( model::ITimeEvaluatorPtr timeEval, const std::string & name, const std::string & plugin, const std::string & path, const std::string & alphaMask )
{
    return TestNodesCreator::AnimatedGeometry( timeEval, name, plugin, path, alphaMask );
}

// ****************************
//
model::BasicNodePtr		TestSceneUtils::GradientRectangle		( model::ITimeEvaluatorPtr timeEval, const std::string & name, Float32 width, Float32 height, glm::vec4 c1, glm::vec4 c2, const std::string & alphaMask )
{
    return TestNodesCreator::GradientRectangle( timeEval, name, width, height, c1, c2, alphaMask );
}

// ****************************
//
model::BasicNodePtr		TestSceneUtils::GradientGeometry		( model::ITimeEvaluatorPtr timeEval, const std::string & name, const std::string & plugin, glm::vec4 c1, glm::vec4 c2, const std::string & alphaMask )
{
    return TestNodesCreator::GradientGeometry( timeEval, name, plugin, c1, c2, alphaMask );
}

// ****************************
//
model::BasicNodePtr		TestSceneUtils::ColoredText				( model::ITimeEvaluatorPtr timeEval, const std::string & name, glm::vec4 color, UInt32 fontSize, const std::string & alphaMask )
{
    return TestNodesCreator::ColoredText( timeEval, name, color, fontSize, alphaMask );
}

// ****************************
//
model::BasicNodePtr		TestSceneUtils::GradientText			( model::ITimeEvaluatorPtr timeEval, const std::string & name, glm::vec4 c1, glm::vec4 c2, UInt32 fontSize, const std::string & alphaMask )
{
    return TestNodesCreator::GradientText( timeEval, name, c1, c2, fontSize, alphaMask );
}

// ****************************
//
model::BasicNodePtr		TestSceneUtils::ColoredTimer				( model::ITimeEvaluatorPtr timeEval, const std::string & name, glm::vec4 color, UInt32 fontSize, const std::string & alphaMask )
{
    return TestNodesCreator::ColoredTimer( timeEval, name, color, fontSize, alphaMask );
}

// ****************************
//
model::BasicNodePtr		TestSceneUtils::GradientTimer			( model::ITimeEvaluatorPtr timeEval, const std::string & name, glm::vec4 c1, glm::vec4 c2, UInt32 fontSize, const std::string & alphaMask )
{
    return TestNodesCreator::GradientTimer( timeEval, name, c1, c2, fontSize, alphaMask );
}

// ****************************
//
model::BasicNodePtr		TestSceneUtils::VideoStreamDecoder			( model::ITimeEvaluatorPtr timeEval, const std::string & name, const std::string & plugin, const std::string & videoPath, const std::string & alphaMask )
{
    return TestNodesCreator::VideoStreamDecoder( timeEval, name, plugin, videoPath, alphaMask );
}

// *************************
//
void					TestSceneUtils::GenerateCheckboardTex	( const std::string & name, UInt32 width, UInt32 height, glm::uvec3 color )
{
	SizeType size = width * height * 4;
	char * data = new char[ size ];

	for ( unsigned int i = 0; i < width; ++i ) 
	{
		for ( unsigned int j = 0; j < height; ++j ) 
		{
			unsigned int idx = 4 * ( i * height + j );
			unsigned char c = static_cast< unsigned char >( (( ( (i&32)==0 ) ^ ( (j&32)==0 ) ))*255 );
			if( c == 255 )
			{
				data[ idx ] = (GLubyte)255;
				data[ idx + 1 ] = (GLubyte)255;
				data[ idx + 2 ] = (GLubyte)255;
			}
			else
			{
				data[ idx ] = (GLubyte)color.r;
				data[ idx + 1 ] = (GLubyte)color.g;
				data[ idx + 2 ] = (GLubyte)color.b;
			}
			data[ idx + 3 ] = (GLubyte)255;
		}
	}
	image::SaveBMPImage( name, MemoryChunk::Create( data, size ), width, height, 32 );
}

// *************************
//
void					TestSceneUtils::GenerateCheckboardAlphaMaskTex		( const std::string & name, UInt32 width, UInt32 height, UInt32 size )
{
	char * data = new char[ width * height ];
	for ( unsigned int i = 0; i < width; ++i ) 
	{
		for ( unsigned int j = 0; j < height; ++j ) 
		{
			unsigned int idx = i * height + j;
			unsigned char c = static_cast< unsigned char >( (( ( (i&size)==0 ) ^ ( (j&size)==0 ) ))*255 );
			data[ idx ] = (GLubyte)c;
		}
	}
	image::SaveBMPImage( name, MemoryChunk::Create( data, width * height ), width, height, 8 );
}

// *************************
//
void					TestSceneUtils::GenerateCheckboardAnim		( const std::string & path, UInt32 width, UInt32 height, UInt32 animNum )
{
    std::random_device rd;
    std::mt19937 gen( rd() );
    std::uniform_int_distribution<> dis( 0, 255 );

	for( UInt32 i = 0; i < animNum; ++i )
	{
		GenerateCheckboardTex( toString( path + "/f" + toString( i ) + ".bmp" ), width, height, glm::uvec3( dis( gen ), dis( gen ), dis( gen ) ) );
	}
}

// ****************************
//
std::vector< model::IPluginDescriptor * >  TestSceneUtils::DefaultBVPluginDescriptors  ()
{
    std::vector< model::IPluginDescriptor * > descriptors;

    descriptors.push_back( new model::DefaultTransformPluginDesc() );
    descriptors.push_back( new model::DefaultRect::PluginDesc() );
    descriptors.push_back( new model::DefaultColorPluginDesc() );
    descriptors.push_back( new model::DefaultTexturePluginDesc() );
    descriptors.push_back( new model::DefaultAnimationPluginDesc() );
    descriptors.push_back( new model::DefaultAlphaMaskPluginDesc() );
    descriptors.push_back( new model::DefaultGradientPluginDesc() );
    descriptors.push_back( new model::DefaultTextPluginDesc() );
    descriptors.push_back( new model::DefaultTimerPluginDesc() );
    
	descriptors.push_back( new model::DefaultTrianglePluginDesc() );
	descriptors.push_back( new model::DefaultCirclePluginDesc() );
	descriptors.push_back( new model::DefaultEllipsePluginDesc() );
	descriptors.push_back( new model::DefaultRoundedRectPluginDesc() );
	descriptors.push_back( new model::DefaultCubePluginDesc() );
	descriptors.push_back( new model::DefaultSimpleCube::PluginDesc() );
	descriptors.push_back( new model::DefaultSphere::PluginDesc() );
	descriptors.push_back( new model::DefaultGeosphere::PluginDesc() );
	descriptors.push_back( new model::DefaultCylinder::DefaultCylinderPluginDesc() );
	descriptors.push_back( new model::DefaultCone::DefaultConePluginDesc() );
	descriptors.push_back( new model::DefaultTorus::PluginDesc() );
	descriptors.push_back( new model::DefaultSpring::PluginDesc() );
	descriptors.push_back( new model::DefaultCogWheel::PluginDesc() );
	descriptors.push_back( new model::DefaultPieChartPluginDesc() );
	descriptors.push_back( new model::DefaultPrismPluginDesc() );

	descriptors.push_back( new model::DefaultAVDecoderPluginDesc() );

    return descriptors;
}

// ***********************
//
const IConfig *         TestSceneUtils::GetConfig()
{
    return &FakeConfig::Config;
}

} // bv