#include "TextureTest.h"

namespace bv {

// *****************************
//
TextureTest::TextureTest      ( Renderer * renderer )
	: m_renderer( renderer )
	, m_camera( new Camera( true ) )
{
	BaseTextureTest::Initialize();
}

// *****************************
//
TextureTest::~TextureTest     ()
{
}

// *****************************
//
void    TextureTest::Initialize          ()
{
    BVGL::bvglClearColor( 0.f, 0.f, 0.f, 0.f );

	//m_test = BaseTextureTest::GetTest( 0 );
	//m_test = BaseTextureTest::GetTest( 1 );
	//m_test = BaseTextureTest::GetTest( 2 );
	//m_test = BaseTextureTest::GetTest( 3 );
	m_test = BaseTextureTest::GetTest( 4 );
}

// *****************************
//
void    TextureTest::Update              ( TimeType t )
{
	{ t; }
}

// *****************************
//
void    TextureTest::Key                 ( unsigned char c )
{
	{ c; }
}

// *****************************
//
void    TextureTest::Resize              ( UInt32 w, UInt32 h )
{
    BVGL::bvglViewport( 0, 0, w, h );
	m_camera->SetPerspective( 90.0f, w, h, 0.1f, 100.0f );
	m_projMat = m_camera->GetProjectionMatrix();

	m_test->Initialize( m_renderer, m_projMat );
}

// *****************************
//
void    TextureTest::Render              ()
{
    BVGL::bvglClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	m_test->Render();
}

} // bv
