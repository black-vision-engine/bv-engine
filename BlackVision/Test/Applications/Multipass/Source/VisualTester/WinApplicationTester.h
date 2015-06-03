#pragma once
#include "Application\WindowedApplication.h"
#include "VisualTesterRenderLogic.h"
//#include "gtest/gtest.h"

namespace bv
{

class Renderer;


/**@brief Application class used to visual testing of some parts of engine.

When you want to use it with google test library, do following:
-	include header WinApplicationTester.h and add .h and .cpp files to your project
-	delete main fuction from google test template. Main Function is already defined in WinApplicationTester.cpp
-	write all your test following google test rules
In macros defining tests you can use global variable named application, to get Renderer
and then to render single frame.*/
class WinApplicationTester	:	public WindowedApplication
{
private:
	VisualTesterRenderLogic*		m_renderLogic;

	bool							makeReferenceImage;

public:
    static bool			m_sWindowedApplicationInitialized;
public:
    static void			StaticInitializer	();
    static bool			RegisterInitializer ();


protected:
	WinApplicationTester();
	~WinApplicationTester();

	virtual int		MainFun( int argc, char ** argv ) override;
	virtual void	OnIdle();
	virtual bool	OnInitialize();
public:
	void setMakeReferenceImage( bool value );

	void testRender( const std::string fileName, SceneNode* node );


	bv::Renderer* getRenderer();
};


extern WinApplicationTester* application;

}
