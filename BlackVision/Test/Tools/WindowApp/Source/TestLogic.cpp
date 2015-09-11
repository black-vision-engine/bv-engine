#include "TestLogic.h"

namespace bv {

// ****************************
//
bool TestLogic::ms_appInitialized = BasicWindowApp::RegisterInitializer( new TestLogic(), "TEST", 800, 600 );

// ****************************
//
				TestLogic::~TestLogic			()
{
}

// ****************************
//
void			TestLogic::Initialize			( Renderer * renderer )
{
	{ renderer; }

    BVGL::bvglClearColor( 0.f, 0.f, 0.f, 0.f );
}

// ****************************
//
void			TestLogic::Render				()
{
    BVGL::bvglClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

// ****************************
//
void			TestLogic::Update				( TimeType t )
{
	{ t; }
}

// ****************************
//
void			TestLogic::Key					( unsigned char c )
{
	{ c; }
}

// ****************************
//
void			TestLogic::Resize				( UInt32 w, UInt32 h )
{
    BVGL::bvglViewport( 0, 0, w, h );
}

} // bv
