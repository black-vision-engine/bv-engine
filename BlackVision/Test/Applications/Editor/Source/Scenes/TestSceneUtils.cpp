#include "TestSceneUtils.h"

#include <random>
#include <cassert>

#include "Assets/Font/TextHelper.h"

#include "LibImage.h"

namespace bv {

const std::string	TestSceneUtils::TEXTURE_PATH	= "Assets/checkboard.bmp";
const std::string	TestSceneUtils::ALPHA_MASK0_PATH	= "Assets/checkboard_am0.bmp";
const std::string	TestSceneUtils::ALPHA_MASK_PATH	= "Assets/checkboard_am.bmp";
const std::string	TestSceneUtils::ANIM_PATH		= "Assets/anim";

const Float32		TestSceneUtils::SPEED			= 1.f;
const UInt32		TestSceneUtils::ANIM_NUM		= 10;
const UInt32		TestSceneUtils::IMG_SIZE		= 128;
const UInt32		TestSceneUtils::AM_SIZE			= 2048;

// ****************************
//
const UInt32		TestSceneUtils::GEOM_PLUGINS_NUM = 15;
const std::string	TestSceneUtils::PluginsArr[] = { "DEFAULT_TRIANGLE", "DEFAULT_CIRCLE", "DEFAULT_ELLIPSE", "DEFAULT_ROUNDEDRECT", "DEFAULT_CUBE", "DEFAULT_SIMPLE_CUBE", "DEFAULT_SPHERE", "DEFAULT_GEOSPHERE", "DEFAULT_CYLINDER", "DEFAULT_CONE", "DEFAULT_TORUS", "DEFAULT_SPRING", "DEFAULT_COGWHEEL", "DEFAULT_PIECHART", "DEFAULT_PRISM" };
const std::string	TestSceneUtils::PluginsNameArr[] = { "triangle", "circle", "ellipse", "rounded rect", "cube", "simple cube", "sphere", "sphere", "cylinder", "cone", "torus", "spring", "cog wheel", "piechart", "prism" };

// ****************************
//
model::BasicNodePtr		TestSceneUtils::ColoredRectangle			( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::string & name, Float32 width, Float32 height, glm::vec4 color, const std::string & alphaMask )
{
	auto localTimeline = timelineManager->CreateOffsetTimeEvaluator( "timeline0" , TimeType( 1.0 ) );
    timeEvaluator->AddChild( localTimeline );

    auto node = model::BasicNode::Create( name, timeEvaluator );

	std::vector< std::string > plugins;
	plugins.push_back( "DEFAULT_TRANSFORM" );
	plugins.push_back( "DEFAULT_RECTANGLE" );
	plugins.push_back( "DEFAULT_COLOR" );
	if( !alphaMask.empty() )
	{
		plugins.push_back( "DEFAULT_ALPHA_MASK" );
	}

    auto success = node->AddPlugins( plugins, localTimeline );
	{ success; }
    assert( success );

	SetParameter( node->GetPlugin( "solid color" )->GetParameter( "color" ), 0.f, color );

    SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "width" ), TimeType( 0.f ),  width );
	SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "height" ), TimeType( 0.f ), height );

	if( !alphaMask.empty() )
	{
		LoadTexture( node->GetPlugin( "alpha_mask" ), alphaMask );
	}

    return node;
}

// ****************************
//
model::BasicNodePtr		TestSceneUtils::ColoredGeometry				( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::string & name, const std::string & plugin, glm::vec4 color, const std::string & alphaMask )
{
	auto localTimeline = timelineManager->CreateOffsetTimeEvaluator( "timeline0" , TimeType( 1.0 ) );
    timeEvaluator->AddChild( localTimeline );

    auto node = model::BasicNode::Create( name, timeEvaluator );

	std::vector< std::string > plugins;
	plugins.push_back( "DEFAULT_TRANSFORM" );
	plugins.push_back( plugin );
	plugins.push_back( "DEFAULT_COLOR" );
	if( !alphaMask.empty() )
	{
		plugins.push_back( "DEFAULT_ALPHA_MASK" );
	}

    auto success = node->AddPlugins( plugins, localTimeline );
	{ success; }
    assert( success );

	SetParameter( node->GetPlugin( "solid color" )->GetParameter( "color" ), 0.f, color );

	if( !alphaMask.empty() )
	{
		LoadTexture( node->GetPlugin( "alpha_mask" ), alphaMask );
	}

    return node;
}

// ****************************
//
model::BasicNodePtr		TestSceneUtils::TexturedRectangle		( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::string & name, Float32 width, Float32 height, const std::string & path, const std::string & alphaMask )
{
	auto localTimeline = timelineManager->CreateOffsetTimeEvaluator( "timeline0" , TimeType( 1.0 ) );
    timeEvaluator->AddChild( localTimeline );

    auto node = model::BasicNode::Create( name, timeEvaluator );

	std::vector< std::string > plugins;
	plugins.push_back( "DEFAULT_TRANSFORM" );
	plugins.push_back( "DEFAULT_RECTANGLE" );
	plugins.push_back( "DEFAULT_TEXTURE" );
	if( !alphaMask.empty() )
	{
		plugins.push_back( "DEFAULT_ALPHA_MASK" );
	}

    auto success = node->AddPlugins( plugins, localTimeline );

	assert( success ); { success; }

    SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "width" ), TimeType( 0.f ),  width );
	SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "height" ), TimeType( 0.f ), height );
	
	LoadTexture( node->GetPlugin( "texture" ), path );

	if( !alphaMask.empty() )
	{
		LoadTexture( node->GetPlugin( "alpha_mask" ), alphaMask );
	}

    return node;
}

// ****************************
//
model::BasicNodePtr		TestSceneUtils::TexturedGeometry		( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::string & name, const std::string & plugin, const std::string & path, const std::string & alphaMask )
{
	auto localTimeline = timelineManager->CreateOffsetTimeEvaluator( "timeline0" , TimeType( 1.0 ) );
    timeEvaluator->AddChild( localTimeline );

    auto node = model::BasicNode::Create( name, timeEvaluator );

	std::vector< std::string > plugins;
	plugins.push_back( "DEFAULT_TRANSFORM" );
	plugins.push_back( plugin );
	plugins.push_back( "DEFAULT_TEXTURE" );
	if( !alphaMask.empty() )
	{
		plugins.push_back( "DEFAULT_ALPHA_MASK" );
	}

    auto success = node->AddPlugins( plugins, localTimeline );

	assert( success ); { success; }

	LoadTexture( node->GetPlugin( "texture" ), path );

	if( !alphaMask.empty() )
	{
		LoadTexture( node->GetPlugin( "alpha_mask" ), alphaMask );
	}

    return node;
}

// ****************************
//
model::BasicNodePtr		TestSceneUtils::AnimatedRectangle		( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::string & name, Float32 width, Float32 height, const std::string & path, const std::string & alphaMask )
{
	auto localTimeline = timelineManager->CreateOffsetTimeEvaluator( "timeline0" , TimeType( 1.0 ) );
    timeEvaluator->AddChild( localTimeline );

    auto node = model::BasicNode::Create( name, timeEvaluator );

	std::vector< std::string > plugins;
	plugins.push_back( "DEFAULT_TRANSFORM" );
	plugins.push_back( "DEFAULT_RECTANGLE" );
	plugins.push_back( "DEFAULT_ANIMATION" );
	if( !alphaMask.empty() )
	{
		plugins.push_back( "DEFAULT_ALPHA_MASK" );
	}

    auto success = node->AddPlugins( plugins, localTimeline );

	assert( success ); { success; }

    SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "width" ), TimeType( 0.f ),  width );
	SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "height" ), TimeType( 0.f ), height );
	
	model::LoadAnimation( node->GetPlugin( "animation" ), path, "*.bmp" );
	
	auto time = timeEvaluator->GetLocalTime();
    SetParameter( node->GetPlugin( "animation" )->GetParameter( "frameNum" ), time, 0.f );
    SetParameter( node->GetPlugin( "animation" )->GetParameter( "frameNum" ), time + TimeType( 0.25f * ( Float32 )ANIM_NUM / SPEED ), ( Float32 )ANIM_NUM );
    node->GetPlugin( "animation" )->GetParameter( "frameNum" )->SetTimeEvaluator( timeEvaluator );

	if( !alphaMask.empty() )
	{
		LoadTexture( node->GetPlugin( "alpha_mask" ), alphaMask );
	}

    return node;
}

// ****************************
//
model::BasicNodePtr		TestSceneUtils::AnimatedGeometry		( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::string & name, const std::string & plugin, const std::string & path, const std::string & alphaMask )
{
	auto localTimeline = timelineManager->CreateOffsetTimeEvaluator( "timeline0" , TimeType( 1.0 ) );
    timeEvaluator->AddChild( localTimeline );

    auto node = model::BasicNode::Create( name, timeEvaluator );

	std::vector< std::string > plugins;
	plugins.push_back( "DEFAULT_TRANSFORM" );
	plugins.push_back( plugin );
	plugins.push_back( "DEFAULT_ANIMATION" );
	if( !alphaMask.empty() )
	{
		plugins.push_back( "DEFAULT_ALPHA_MASK" );
	}

    auto success = node->AddPlugins( plugins, localTimeline );

	assert( success ); { success; }

	model::LoadAnimation( node->GetPlugin( "animation" ), path, "*.bmp" );
	
	auto time = timeEvaluator->GetLocalTime();
    SetParameter( node->GetPlugin( "animation" )->GetParameter( "frameNum" ), time, 0.f );
    SetParameter( node->GetPlugin( "animation" )->GetParameter( "frameNum" ), time + TimeType( 0.25f * ( Float32 )ANIM_NUM / SPEED ), ( Float32 )ANIM_NUM );
    node->GetPlugin( "animation" )->GetParameter( "frameNum" )->SetTimeEvaluator( timeEvaluator );

	if( !alphaMask.empty() )
	{
		LoadTexture( node->GetPlugin( "alpha_mask" ), alphaMask );
	}

    return node;
}

// ****************************
//
model::BasicNodePtr		TestSceneUtils::GradientRectangle		( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::string & name, Float32 width, Float32 height, glm::vec4 c1, glm::vec4 c2, const std::string & alphaMask )
{
	auto localTimeline = timelineManager->CreateOffsetTimeEvaluator( "timeline0" , TimeType( 1.0 ) );
    timeEvaluator->AddChild( localTimeline );

    auto node = model::BasicNode::Create( name, timeEvaluator );

	std::vector< std::string > plugins;
	plugins.push_back( "DEFAULT_TRANSFORM" );
	plugins.push_back( "DEFAULT_RECTANGLE" );
	plugins.push_back( "DEFAULT_LINEAR_GRADIENT" );
	if( !alphaMask.empty() )
	{
		plugins.push_back( "DEFAULT_ALPHA_MASK" );
	}

    auto success = node->AddPlugins( plugins, localTimeline );

	assert( success ); { success; }

	SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "width" ), TimeType( 0.f ),  width );
	SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "height" ), TimeType( 0.f ), height );
	
	SetParameter( node->GetPlugin( "linear_gradient" )->GetParameter( "color1" ), TimeType( 0.f ), c1 );
	SetParameter( node->GetPlugin( "linear_gradient" )->GetParameter( "color2" ), TimeType( 0.f ), c2 );
	
	if( !alphaMask.empty() )
	{
		LoadTexture( node->GetPlugin( "alpha_mask" ), alphaMask );
	}

    return node;
}

// ****************************
//
model::BasicNodePtr		TestSceneUtils::GradientGeometry		( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::string & name, const std::string & plugin, glm::vec4 c1, glm::vec4 c2, const std::string & alphaMask )
{
	auto localTimeline = timelineManager->CreateOffsetTimeEvaluator( "timeline0" , TimeType( 1.0 ) );
    timeEvaluator->AddChild( localTimeline );

    auto node = model::BasicNode::Create( name, timeEvaluator );

	std::vector< std::string > plugins;
	plugins.push_back( "DEFAULT_TRANSFORM" );
	plugins.push_back( plugin );
	plugins.push_back( "DEFAULT_LINEAR_GRADIENT" );
	if( !alphaMask.empty() )
	{
		plugins.push_back( "DEFAULT_ALPHA_MASK" );
	}

    auto success = node->AddPlugins( plugins, localTimeline );

	assert( success ); { success; }

	SetParameter( node->GetPlugin( "linear_gradient" )->GetParameter( "color1" ), TimeType( 0.f ), c1 );
	SetParameter( node->GetPlugin( "linear_gradient" )->GetParameter( "color2" ), TimeType( 0.f ), c2 );
	
	if( !alphaMask.empty() )
	{
		LoadTexture( node->GetPlugin( "alpha_mask" ), alphaMask );
	}

    return node;
}

// ****************************
//
model::BasicNodePtr		TestSceneUtils::ColoredText				( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::string & name, glm::vec4 color, UInt32 fontSize, const std::string & alphaMask )
{
	auto localTimeline = timelineManager->CreateOffsetTimeEvaluator( "timeline0" , TimeType( 1.0 ) );
    timeEvaluator->AddChild( localTimeline );

    auto node = model::BasicNode::Create( name, timeEvaluator );

	std::vector< std::string > plugins;
	plugins.push_back( "DEFAULT_TRANSFORM" );
	plugins.push_back( "DEFAULT_COLOR" );
	plugins.push_back( "DEFAULT_TEXT" );
	if( !alphaMask.empty() )
	{
		plugins.push_back( "DEFAULT_ALPHA_MASK" );
	}

    auto success = node->AddPlugins( plugins, localTimeline );

	assert( success ); { success; }

	SetParameter( node->GetPlugin( "solid color" )->GetParameter( "color" ), 0.f, color );

    SetParameter( node->GetPlugin( "text" )->GetParameter( "spacing" ), TimeType( 0.0 ), 0.f );
	SetParameter( node->GetPlugin( "text" )->GetParameter( "alignment" ), TimeType( 0.0 ), float( TextAlignmentType::Left ) );
    
	LoadFont( node->GetPlugin( "text" ), "Assets/Fonts/arial.TTF", fontSize, 0, 0, true );
	model::DefaultTextPlugin::SetText( node->GetPlugin( "text" ), L"tekst\n1234" );
	
	if( !alphaMask.empty() )
	{
		LoadTexture( node->GetPlugin( "alpha_mask" ), alphaMask );
	}

    return node;
}

// ****************************
//
model::BasicNodePtr		TestSceneUtils::GradientText			( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::string & name, glm::vec4 c1, glm::vec4 c2, UInt32 fontSize, const std::string & alphaMask )
{
	auto localTimeline = timelineManager->CreateOffsetTimeEvaluator( "timeline0" , TimeType( 1.0 ) );
    timeEvaluator->AddChild( localTimeline );

    auto node = model::BasicNode::Create( name, timeEvaluator );

	std::vector< std::string > plugins;
	plugins.push_back( "DEFAULT_TRANSFORM" );
	plugins.push_back( "DEFAULT_TEXT" );
	plugins.push_back( "DEFAULT_LINEAR_GRADIENT" );
	if( !alphaMask.empty() )
	{
		plugins.push_back( "DEFAULT_ALPHA_MASK" );
	}

    auto success = node->AddPlugins( plugins, localTimeline );

	assert( success ); { success; }

	SetParameter( node->GetPlugin( "linear_gradient" )->GetParameter( "color1" ), TimeType( 0.f ), c1 );
	SetParameter( node->GetPlugin( "linear_gradient" )->GetParameter( "color2" ), TimeType( 0.f ), c2 );

    SetParameter( node->GetPlugin( "text" )->GetParameter( "spacing" ), TimeType( 0.0 ), 0.f );
	SetParameter( node->GetPlugin( "text" )->GetParameter( "alignment" ), TimeType( 0.0 ), float( TextAlignmentType::Center ) );
    
	LoadFont( node->GetPlugin( "text" ), "Assets/Fonts/arial.TTF", fontSize, 0, 0, true );
	model::DefaultTextPlugin::SetText( node->GetPlugin( "text" ), L"tekst\n1234" );
	
	if( !alphaMask.empty() )
	{
		LoadTexture( node->GetPlugin( "alpha_mask" ), alphaMask );
	}

    return node;
}

// ****************************
//
model::BasicNodePtr		TestSceneUtils::ColoredTimer				( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::string & name, glm::vec4 color, UInt32 fontSize, const std::string & alphaMask )
{
	auto localTimeline = timelineManager->CreateOffsetTimeEvaluator( "timeline0" , TimeType( 1.0 ) );
    timeEvaluator->AddChild( localTimeline );

    auto node = model::BasicNode::Create( name, timeEvaluator );

	std::vector< std::string > plugins;
	plugins.push_back( "DEFAULT_TRANSFORM" );
	plugins.push_back( "DEFAULT_COLOR" );
	plugins.push_back( "DEFAULT_TIMER" );
	if( !alphaMask.empty() )
	{
		plugins.push_back( "DEFAULT_ALPHA_MASK" );
	}

    auto success = node->AddPlugins( plugins, localTimeline );

	assert( success ); { success; }

	SetParameter( node->GetPlugin( "solid color" )->GetParameter( "color" ), 0.f, color );

    SetParameter( node->GetPlugin( "timer" )->GetParameter( "spacing" ), TimeType( 0.0 ), 0.f );
	SetParameter( node->GetPlugin( "timer" )->GetParameter( "alignment" ), TimeType( 0.0 ), float( TextAlignmentType::Center ) );
    
	LoadFont( node->GetPlugin( "timer" ), "Assets/Fonts/arial.TTF", fontSize, 0, 0, true );
    
	if( !alphaMask.empty() )
	{
		LoadTexture( node->GetPlugin( "alpha_mask" ), alphaMask );
	}

	model::SetTimeTimerPlugin( node->GetPlugin( "timer" ), 12333.0f );
    model::StartTimerPlugin( node->GetPlugin( "timer" ) );

    return node;
}

// ****************************
//
model::BasicNodePtr		TestSceneUtils::GradientTimer			( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::string & name, glm::vec4 c1, glm::vec4 c2, UInt32 fontSize, const std::string & alphaMask )
{
	auto localTimeline = timelineManager->CreateOffsetTimeEvaluator( "timeline0" , TimeType( 1.0 ) );
    timeEvaluator->AddChild( localTimeline );

    auto node = model::BasicNode::Create( name, timeEvaluator );

	std::vector< std::string > plugins;
	plugins.push_back( "DEFAULT_TRANSFORM" );
	plugins.push_back( "DEFAULT_TIMER" );
	plugins.push_back( "DEFAULT_LINEAR_GRADIENT" );
	if( !alphaMask.empty() )
	{
		plugins.push_back( "DEFAULT_ALPHA_MASK" );
	}

    auto success = node->AddPlugins( plugins, localTimeline );

	assert( success ); { success; }

	SetParameter( node->GetPlugin( "linear_gradient" )->GetParameter( "color1" ), TimeType( 0.f ), c1 );
	SetParameter( node->GetPlugin( "linear_gradient" )->GetParameter( "color2" ), TimeType( 0.f ), c2 );

    SetParameter( node->GetPlugin( "timer" )->GetParameter( "spacing" ), TimeType( 0.0 ), 0.f );
	SetParameter( node->GetPlugin( "timer" )->GetParameter( "alignment" ), TimeType( 0.0 ), float( TextAlignmentType::Center ) );
    
	LoadFont( node->GetPlugin( "timer" ), "Assets/Fonts/arial.TTF", fontSize, 0, 0, true );
    
	if( !alphaMask.empty() )
	{
		LoadTexture( node->GetPlugin( "alpha_mask" ), alphaMask );
	}

	model::SetTimeTimerPlugin( node->GetPlugin( "timer" ), 12333.0f );
    model::StartTimerPlugin( node->GetPlugin( "timer" ) );
	
    return node;
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
    descriptors.push_back( new model::DefaultRectPluginDesc() );
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
	descriptors.push_back( new model::DefaultCube::PluginDesc() );
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

    return descriptors;
}

} // bv