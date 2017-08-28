#include "TestNodesCreator.h"



namespace bv
{


const Float32		TestNodesCreator::SPEED = 1.f;
const UInt32		TestNodesCreator::ANIM_NUM = 10;




// ****************************
//
model::BasicNodePtr		TestNodesCreator::ColoredRectangle			( model::ITimeEvaluatorPtr timeEval, const std::string & name, Float32 width, Float32 height, glm::vec4 color, const std::string & alphaMask )
{
    auto node = model::BasicNode::Create( name, nullptr );

    std::vector< std::string > plugins;
    plugins.push_back( "DEFAULT_TRANSFORM" );
    plugins.push_back( "DEFAULT_RECTANGLE" );
    plugins.push_back( "DEFAULT_COLOR" );
    if( !alphaMask.empty() )
    {
        plugins.push_back( "DEFAULT_ALPHA_MASK" );
    }

    auto success = node->AddPlugins( plugins, timeEval );
    { success; }
    assert( success );

    SetParameter( node->GetPlugin( "solid color" )->GetParameter( "color" ), 0.f, color );

    SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "width" ), TimeType( 0.f ), width );
    SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "height" ), TimeType( 0.f ), height );

    if( !alphaMask.empty() )
    {
        LoadTexture( node->GetPlugin( "alpha_mask" ), alphaMask );
    }

    return node;
}

// ****************************
//
model::BasicNodePtr		TestNodesCreator::ColoredGeometry				( model::ITimeEvaluatorPtr timeEval, const std::string & name, const std::string & plugin, glm::vec4 color, const std::string & alphaMask )
{
    auto node = model::BasicNode::Create( name, nullptr );

    std::vector< std::string > plugins;
    plugins.push_back( "DEFAULT_TRANSFORM" );
    plugins.push_back( plugin );
    plugins.push_back( "DEFAULT_COLOR" );
    if( !alphaMask.empty() )
    {
        plugins.push_back( "DEFAULT_ALPHA_MASK" );
    }

    auto success = node->AddPlugins( plugins, timeEval );
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
model::BasicNodePtr		TestNodesCreator::TexturedRectangle		( model::ITimeEvaluatorPtr timeEval, const std::string & name, Float32 width, Float32 height, const std::string & path, const std::string & alphaMask )
{
    auto node = model::BasicNode::Create( name, nullptr );

    std::vector< std::string > plugins;
    plugins.push_back( "DEFAULT_TRANSFORM" );
    plugins.push_back( "DEFAULT_RECTANGLE" );
    plugins.push_back( "DEFAULT_TEXTURE" );
    if( !alphaMask.empty() )
    {
        plugins.push_back( "DEFAULT_ALPHA_MASK" );
    }

    auto success = node->AddPlugins( plugins, timeEval );

    assert( success ); { success; }

    SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "width" ), TimeType( 0.f ), width );
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
model::BasicNodePtr		TestNodesCreator::TexturedGeometry		( model::ITimeEvaluatorPtr timeEval, const std::string & name, const std::string & plugin, const std::string & path, const std::string & alphaMask )
{
    auto node = model::BasicNode::Create( name, nullptr );

    std::vector< std::string > plugins;
    plugins.push_back( "DEFAULT_TRANSFORM" );
    plugins.push_back( plugin );
    plugins.push_back( "DEFAULT_TEXTURE" );
    if( !alphaMask.empty() )
    {
        plugins.push_back( "DEFAULT_ALPHA_MASK" );
    }

    auto success = node->AddPlugins( plugins, timeEval );

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
model::BasicNodePtr		TestNodesCreator::AnimatedRectangle		( model::ITimeEvaluatorPtr timeEval, const std::string & name, Float32 width, Float32 height, const std::string & path, const std::string & alphaMask )
{
    auto node = model::BasicNode::Create( name, nullptr );

    std::vector< std::string > plugins;
    plugins.push_back( "DEFAULT_TRANSFORM" );
    plugins.push_back( "DEFAULT_RECTANGLE" );
    plugins.push_back( "DEFAULT_ANIMATION" );
    if( !alphaMask.empty() )
    {
        plugins.push_back( "DEFAULT_ALPHA_MASK" );
    }

    auto success = node->AddPlugins( plugins, timeEval );

    assert( success ); { success; }

    SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "width" ), TimeType( 0.f ), width );
    SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "height" ), TimeType( 0.f ), height );

    model::LoadAnimation( node->GetPlugin( "animation" ), path, "" );

    auto time = model::TimelineManager::GetInstance()->GetRootTimeline()->GetLocalTime();
    SetParameter( node->GetPlugin( "animation" )->GetParameter( "frameNum" ), time, 0.f );
    SetParameter( node->GetPlugin( "animation" )->GetParameter( "frameNum" ), time + TimeType( 0.25f * ( Float32 )ANIM_NUM / SPEED ), ( Float32 )ANIM_NUM );
    node->GetPlugin( "animation" )->GetParameter( "frameNum" )->SetTimeEvaluator( timeEval );

    if( !alphaMask.empty() )
    {
        LoadTexture( node->GetPlugin( "alpha_mask" ), alphaMask );
    }

    return node;
}

// ****************************
//
model::BasicNodePtr		TestNodesCreator::AnimatedGeometry		( model::ITimeEvaluatorPtr timeEval, const std::string & name, const std::string & plugin, const std::string & path, const std::string & alphaMask )
{
    auto node = model::BasicNode::Create( name, nullptr );

    std::vector< std::string > plugins;
    plugins.push_back( "DEFAULT_TRANSFORM" );
    plugins.push_back( plugin );
    plugins.push_back( "DEFAULT_ANIMATION" );
    if( !alphaMask.empty() )
    {
        plugins.push_back( "DEFAULT_ALPHA_MASK" );
    }

    auto success = node->AddPlugins( plugins, timeEval );

    assert( success ); { success; }

    model::LoadAnimation( node->GetPlugin( "animation" ), path, "*.bmp" );

    auto time = model::TimelineManager::GetInstance()->GetRootTimeline()->GetLocalTime();
    SetParameter( node->GetPlugin( "animation" )->GetParameter( "frameNum" ), time, 0.f );
    SetParameter( node->GetPlugin( "animation" )->GetParameter( "frameNum" ), time + TimeType( 0.25f * ( Float32 )ANIM_NUM / SPEED ), ( Float32 )ANIM_NUM );
    node->GetPlugin( "animation" )->GetParameter( "frameNum" )->SetTimeEvaluator( timeEval );

    if( !alphaMask.empty() )
    {
        LoadTexture( node->GetPlugin( "alpha_mask" ), alphaMask );
    }

    return node;
}

// ****************************
//
model::BasicNodePtr		TestNodesCreator::GradientRectangle		( model::ITimeEvaluatorPtr timeEval, const std::string & name, Float32 width, Float32 height, glm::vec4 c1, glm::vec4 c2, const std::string & alphaMask )
{
    auto node = model::BasicNode::Create( name, nullptr );

    std::vector< std::string > plugins;
    plugins.push_back( "DEFAULT_TRANSFORM" );
    plugins.push_back( "DEFAULT_RECTANGLE" );
    plugins.push_back( "DEFAULT_LINEAR_GRADIENT" );
    if( !alphaMask.empty() )
    {
        plugins.push_back( "DEFAULT_ALPHA_MASK" );
    }

    auto success = node->AddPlugins( plugins, timeEval );

    assert( success ); { success; }

    SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "width" ), TimeType( 0.f ), width );
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
model::BasicNodePtr		TestNodesCreator::GradientGeometry		( model::ITimeEvaluatorPtr timeEval, const std::string & name, const std::string & plugin, glm::vec4 c1, glm::vec4 c2, const std::string & alphaMask )
{
    auto node = model::BasicNode::Create( name, nullptr );

    std::vector< std::string > plugins;
    plugins.push_back( "DEFAULT_TRANSFORM" );
    plugins.push_back( plugin );
    plugins.push_back( "DEFAULT_LINEAR_GRADIENT" );
    if( !alphaMask.empty() )
    {
        plugins.push_back( "DEFAULT_ALPHA_MASK" );
    }

    auto success = node->AddPlugins( plugins, timeEval );

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
model::BasicNodePtr		TestNodesCreator::ColoredText				( model::ITimeEvaluatorPtr timeEval, const std::string & name, glm::vec4 color, UInt32 fontSize, const std::string & alphaMask )
{
    auto node = model::BasicNode::Create( name, nullptr );

    std::vector< std::string > plugins;
    plugins.push_back( "DEFAULT_TRANSFORM" );
    plugins.push_back( "DEFAULT_COLOR" );
    plugins.push_back( "DEFAULT_TEXT" );
    if( !alphaMask.empty() )
    {
        plugins.push_back( "DEFAULT_ALPHA_MASK" );
    }

    auto success = node->AddPlugins( plugins, timeEval );

    assert( success ); { success; }

    SetParameter( node->GetPlugin( "solid color" )->GetParameter( "color" ), 0.f, color );

    SetParameter( node->GetPlugin( "text" )->GetParameter( "spacing" ), TimeType( 0.0 ), 0.f );
    SetParameter( node->GetPlugin( "text" )->GetParameter( "alignment" ), TimeType( 0.0 ), float( TextAlignmentType::Center ) );

    LoadFont( node->GetPlugin( "text" ), "fonts/arial.TTF", fontSize, 0, 0, 0, true );
    SetParameter( node->GetPlugin( "text" )->GetParameter( "text" ), 0.0, std::wstring( L"tekst\n1234" ) );

    if( !alphaMask.empty() )
    {
        LoadTexture( node->GetPlugin( "alpha_mask" ), alphaMask );
    }

    return node;
}

// ****************************
//
model::BasicNodePtr		TestNodesCreator::GradientText			( model::ITimeEvaluatorPtr timeEval, const std::string & name, glm::vec4 c1, glm::vec4 c2, UInt32 fontSize, const std::string & alphaMask )
{
    auto node = model::BasicNode::Create( name, nullptr );

    std::vector< std::string > plugins;
    plugins.push_back( "DEFAULT_TRANSFORM" );
    plugins.push_back( "DEFAULT_TEXT" );
    plugins.push_back( "DEFAULT_LINEAR_GRADIENT" );
    if( !alphaMask.empty() )
    {
        plugins.push_back( "DEFAULT_ALPHA_MASK" );
    }

    auto success = node->AddPlugins( plugins, timeEval );

    assert( success ); { success; }

    SetParameter( node->GetPlugin( "linear_gradient" )->GetParameter( "color1" ), TimeType( 0.f ), c1 );
    SetParameter( node->GetPlugin( "linear_gradient" )->GetParameter( "color2" ), TimeType( 0.f ), c2 );

    SetParameter( node->GetPlugin( "text" )->GetParameter( "spacing" ), TimeType( 0.0 ), 0.f );
    SetParameter( node->GetPlugin( "text" )->GetParameter( "alignment" ), TimeType( 0.0 ), float( TextAlignmentType::Center ) );

    LoadFont( node->GetPlugin( "text" ), "fonts/arial.TTF", fontSize, 0, 0, 0, true );
    SetParameter( node->GetPlugin( "text" )->GetParameter( "text" ), 0.0, std::wstring( L"tekst\n1234" ) );

    if( !alphaMask.empty() )
    {
        LoadTexture( node->GetPlugin( "alpha_mask" ), alphaMask );
    }

    return node;
}

// ****************************
//
model::BasicNodePtr		TestNodesCreator::ColoredTimer				( model::ITimeEvaluatorPtr timeEval, const std::string & name, glm::vec4 color, UInt32 fontSize, const std::string & alphaMask )
{
    auto node = model::BasicNode::Create( name, nullptr );

    std::vector< std::string > plugins;
    plugins.push_back( "DEFAULT_TRANSFORM" );
    plugins.push_back( "DEFAULT_COLOR" );
    plugins.push_back( "DEFAULT_TIMER" );
    if( !alphaMask.empty() )
    {
        plugins.push_back( "DEFAULT_ALPHA_MASK" );
    }

    auto success = node->AddPlugins( plugins, timeEval );

    assert( success ); { success; }

    SetParameter( node->GetPlugin( "solid color" )->GetParameter( "color" ), 0.f, color );

    SetParameter( node->GetPlugin( "timer" )->GetParameter( "spacing" ), TimeType( 0.0 ), 0.f );
    SetParameter( node->GetPlugin( "timer" )->GetParameter( "alignment" ), TimeType( 0.0 ), float( TextAlignmentType::Center ) );

    LoadFont( node->GetPlugin( "timer" ), "fonts/arial.TTF", fontSize, 0, 0, 0, true );

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
model::BasicNodePtr		TestNodesCreator::GradientTimer			( model::ITimeEvaluatorPtr timeEval, const std::string & name, glm::vec4 c1, glm::vec4 c2, UInt32 fontSize, const std::string & alphaMask )
{
    auto node = model::BasicNode::Create( name, nullptr );

    std::vector< std::string > plugins;
    plugins.push_back( "DEFAULT_TRANSFORM" );
    plugins.push_back( "DEFAULT_TIMER" );
    plugins.push_back( "DEFAULT_LINEAR_GRADIENT" );
    if( !alphaMask.empty() )
    {
        plugins.push_back( "DEFAULT_ALPHA_MASK" );
    }

    auto success = node->AddPlugins( plugins, timeEval );

    assert( success ); { success; }

    SetParameter( node->GetPlugin( "linear_gradient" )->GetParameter( "color1" ), TimeType( 0.f ), c1 );
    SetParameter( node->GetPlugin( "linear_gradient" )->GetParameter( "color2" ), TimeType( 0.f ), c2 );

    SetParameter( node->GetPlugin( "timer" )->GetParameter( "spacing" ), TimeType( 0.0 ), 0.f );
    SetParameter( node->GetPlugin( "timer" )->GetParameter( "alignment" ), TimeType( 0.0 ), float( TextAlignmentType::Center ) );

    LoadFont( node->GetPlugin( "timer" ), "fonts/arial.TTF", fontSize, 0, 0, 0, true );

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
model::BasicNodePtr		TestNodesCreator::VideoStreamDecoder			( model::ITimeEvaluatorPtr timeEval, const std::string & name, const std::string & plugin, const std::string & videoPath, const std::string & alphaMask )
{
    auto node = model::BasicNode::Create( name, nullptr );

    std::vector< std::string > plugins;
    plugins.push_back( "DEFAULT_TRANSFORM" );
    plugins.push_back( plugin );
    plugins.push_back( "DEFAULT_AV_DECODER" );
    if( !alphaMask.empty() )
    {
        plugins.push_back( "DEFAULT_ALPHA_MASK" );
    }

    auto success = node->AddPlugins( plugins, timeEval );

    assert( success );

    success = model::LoadAVStream( node->GetPlugin( "av_decoder" ), videoPath, TextureFormat::F_A8R8G8B8 );

    if( !alphaMask.empty() )
    {
        LoadTexture( node->GetPlugin( "alpha_mask" ), alphaMask );
    }

    assert( success );

    return node;
}



}	// bv


