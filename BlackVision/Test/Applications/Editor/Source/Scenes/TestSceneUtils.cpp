#include "TestSceneUtils.h"

#include <random>
#include <cassert>

#include "LibImage.h"

namespace bv {

const std::string	TestSceneUtils::TEXTURE_PATH	= "Assets/checkboard.bmp";
const std::string	TestSceneUtils::ALPHA_MASK_PATH	= "Assets/checkboard_am.bmp";
const std::string	TestSceneUtils::ANIM_PATH		= "Assets/anim";

const UInt32		TestSceneUtils::SPEED			= 2;
const UInt32		TestSceneUtils::ANIM_NUM		= 10;
const UInt32		TestSceneUtils::IMG_SIZE		= 256;
const UInt32		TestSceneUtils::AM_SIZE			= 1024;

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

    assert( success );

	model::SetParameter( node->GetPlugin( "solid color" )->GetParameter( "color" ), 0.f, color );

    model::SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "width" ), TimeType( 0.f ),  width );
	model::SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "height" ), TimeType( 0.f ), height );

	if( !alphaMask.empty() )
	{
		model::LoadTexture( node->GetPlugin( "alpha_mask" ), alphaMask );
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

    assert( success );

    model::SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "width" ), TimeType( 0.f ),  width );
	model::SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "height" ), TimeType( 0.f ), height );
	
	model::LoadTexture( node->GetPlugin( "texture" ), path );

	if( !alphaMask.empty() )
	{
		model::LoadTexture( node->GetPlugin( "alpha_mask" ), alphaMask );
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

    assert( success );

    SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "width" ), TimeType( 0.f ),  width );
	SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "height" ), TimeType( 0.f ), height );
	
	model::LoadAnimation( node->GetPlugin( "animation" ), path, "*.bmp" );
	
	auto time = timeEvaluator->GetLocalTime();
    SetParameter( node->GetPlugin( "animation" )->GetParameter( "frameNum" ), time, 0.f );
    SetParameter( node->GetPlugin( "animation" )->GetParameter( "frameNum" ), time + TimeType( 0.25f * ( Float32 )( ANIM_NUM / SPEED ) ), ( Float32 )( ANIM_NUM - 1 ) );
    node->GetPlugin( "animation" )->GetParameter( "frameNum" )->SetTimeEvaluator( timeEvaluator );

	if( !alphaMask.empty() )
	{
		model::LoadTexture( node->GetPlugin( "alpha_mask" ), alphaMask );
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

    assert( success );

	SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "width" ), TimeType( 0.f ),  width );
	SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "height" ), TimeType( 0.f ), height );
	
	SetParameter( node->GetPlugin( "linear_gradient" )->GetParameter( "color1" ), TimeType( 0.f ), c1 );
	SetParameter( node->GetPlugin( "linear_gradient" )->GetParameter( "color2" ), TimeType( 0.f ), c2 );
	
	if( !alphaMask.empty() )
	{
		model::LoadTexture( node->GetPlugin( "alpha_mask" ), alphaMask );
	}

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
void					TestSceneUtils::GenerateCheckboardAlphaMaskTex		( const std::string & name, UInt32 width, UInt32 height )
{
	SizeType size = width * height;
	char * data = new char[ size ];
	for ( unsigned int i = 0; i < width; ++i ) 
	{
		for ( unsigned int j = 0; j < height; ++j ) 
		{
			unsigned int idx = i * height + j;
			unsigned char c = static_cast< unsigned char >( (( ( (i&32)==0 ) ^ ( (j&32)==0 ) ))*255 );
			data[ idx ] = (GLubyte)c;
		}
	}
	image::SaveBMPImage( name, MemoryChunk::Create( data, size ), width, height, 8 );
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

} // bv