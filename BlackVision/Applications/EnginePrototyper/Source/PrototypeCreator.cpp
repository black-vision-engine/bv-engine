#include "PrototypeCreator.h"

#include "Engine/Graphics/Renderers/Renderer.h"



namespace bv {

namespace  {

    unsigned int    DefaultWidth                = 800;
    unsigned int    DefaultHeight               = 600;
    bool            DefaultFullscreenSetting    = false;

} // anonymous


// ****************************
//
IAppLogicPrototype * CreateDefaultPrototype             ( Renderer * renderer )
{
    { renderer; }
    //return new SimpleVAOPrototype0();
    // return new PboUpdatesInspector( renderer );

    //return new MemManagementInspector( renderer );
	//return new FBOProfilingPrototype0( renderer );
	//return new FBOProfilingPrototype1( renderer );
	//return new FBOProfilingPrototype2( renderer );
	//return new FBOProfilingPrototype3( renderer );

    //return new DrawingPrototype( renderer );
	return new MultipassTest( renderer );
}

// ****************************
// FIXME: fatalna imitacja nedznej namiastki configa
unsigned int        DefaultPrototypeWidth               ()
{
    return DefaultWidth;
}

// ****************************
// FIXME: fatalna imitacja nedznej namiastki configa
unsigned int        DefaultPrototypeHeight              ()
{
    return DefaultHeight;
}

// ****************************
// FIXME: fatalna imitacja nedznej namiastki configa
WindowMode       DefaultPrototypeWindowModeSetting             ()
{
	return WindowMode::WINDOWED;
}

// ****************************
// FIXME: fatalna imitacja nedznej namiastki configa
RendererInput       DefaultPrototypeRendererInput             ()
{
	RendererInput ri;
	ri.m_WindowHandle			= nullptr;
    ri.m_PixelFormat			= 0;
    ri.m_RendererDC				= 0;
    ri.m_DisableVerticalSync	= true;
	return ri;
}

// ****************************
// FIXME: fatalna imitacja nedznej namiastki configa
bool                DefaultPrototypeFullscreenSetting   ()
{
    return DefaultFullscreenSetting;
}

} // bv
