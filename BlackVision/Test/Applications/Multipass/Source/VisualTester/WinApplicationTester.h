//#pragma once
//#include "Application\WindowedApplication.h"
//#include "VisualTesterRenderLogic.h"
//#include "gtest/gtest.h"
//
//namespace bv
//{
//
//class Renderer;
//
//
///**@brief Application class used to visual testing of some parts of engine.
//
//When you want to use it with google test library, do following:
//-	include header WinApplicationTester.h and add .h and .cpp files to your project
//-	delete main fuction from google test template. Main Function is already defined in WinApplicationTester.cpp
//-	write all your test following google test rules
//In macros defining tests you can use global variable named application, to get Renderer
//and then to render single frame.
//
//To make image comparing test, call function testRender. Give the name of file containig
//reference image.
//
//If you want to render reference image, you can set last parameter of testRender function to true.
//*/
//class WinApplicationTester	:	public WindowedApplication
//{
//private:
//
//	VisualTesterRenderLogic *		m_renderLogic;
//
//public:
//
//    static bool			m_sWindowedApplicationInitialized;
//
//public:
//
//    static void			MainInitializer	();
//    static bool			RegisterInitializer ();
//
//protected:
//
//    WinApplicationTester();
//	~WinApplicationTester();
//
//	virtual int		MainFun( int argc, char ** argv ) override;
//	virtual void	OnIdle();
//	virtual bool	OnInitialize();
//
//	void InitCamera( unsigned int w, unsigned int h );
//
//public:
//
//	void TestRender( const std::string fileName, SceneNode * node, bool makeReferenceImage = false );
//
//
//	bv::Renderer *              GetRenderer();
//	
//    VisualTesterRenderLogic *   GetRenderLogic() { return m_renderLogic; }
//
//};
//
//
//extern WinApplicationTester * application;
//
//} //bv
