#include "TextureTest.h"

#include "Engine/Audio/AudioRenderer.h"


namespace bv {

// *****************************
//
TextureTest::TextureTest      ( Renderer * renderer, audio::AudioRenderer * )
	: m_renderer( renderer )
	, m_camera( new Camera( true ) )
	, m_width( 0 )
	, m_height( 0 )
	, m_test( nullptr )
	, m_currTestIdx( 0 )
{
	BaseTextureTest::Initialize();
}

// *****************************
//
TextureTest::~TextureTest     ()
{
	if( m_test ) delete m_test;
}

// *****************************
//
void    TextureTest::Initialize          ()
{
    BVGL::bvglClearColor( 0.f, 0.f, 0.f, 0.f );
}

// *****************************
//
void    TextureTest::Update              ( TimeType t )
{
	{ t; }

	if( m_test )
	{
		m_test->Update( t );
	}
	else if( m_width > 0 && m_height > 0 )
	{
		m_test = BaseTextureTest::GetTest( m_currTestIdx );
		m_test->Initialize( m_renderer, m_projMat );
	}
}

// *****************************
//
void    TextureTest::Key                 ( unsigned char c )
{
	if( c > 47 && c < 55 ) {
		if( m_test ) delete m_test;
		m_test = nullptr;
		m_currTestIdx = ( UInt32 )c - 48;
	}
}

// *****************************
//
void    TextureTest::Resize              ( UInt32 w, UInt32 h )
{
	m_width = w; 
	m_height = h;

    BVGL::bvglViewport( 0, 0, w, h );
	m_camera->SetPerspective( 90.0f, (float)w, (float)h, 0.1f, 100.0f );
	m_projMat = m_camera->GetProjectionMatrix();
}

// *****************************
//
void    TextureTest::Render              ()
{
    BVGL::bvglClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	if( m_test )
	{
		m_test->Render();
	}
}

// **************************
//
IBasicLogicUnqPtr    TextureTest::Create  ( Renderer * renderer, audio::AudioRenderer * audioRenderer )
{
	return IBasicLogicUnqPtr( new TextureTest( renderer, audioRenderer ) );
}

} // bv
