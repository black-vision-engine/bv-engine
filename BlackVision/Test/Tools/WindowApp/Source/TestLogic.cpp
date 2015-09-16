#include "TestLogic.h"

namespace bv {

// ****************************
//
bool TestLogic::ms_appInitialized = BasicWindowApp::RegisterInitializer( &TestLogic::Create, "TEST", 800, 600 );

// ****************************
//
#ifdef _DEBUG

bool TestLogic::ms_debugConsole = BasicWindowApp::RegisterConsoleInitializer();

#endif

// ****************************
//
				TestLogic::TestLogic			( Renderer * renderer )
{
	{ renderer; }
}

// ****************************
//
				TestLogic::~TestLogic			()
{
}

// ****************************
//
void			TestLogic::Initialize			()
{
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

// **************************
//
IBasicLogicUnqPtr    TestLogic::Create  ( Renderer * renderer )
{
	return IBasicLogicUnqPtr( new TestLogic( renderer ) );
}

} // bv
