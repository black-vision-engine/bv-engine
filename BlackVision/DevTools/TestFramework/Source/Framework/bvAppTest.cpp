#include "bvAppTest.h"


#pragma warning(disable :4996)
#include "bvApp.h"

#include "System/InitSubsystem.h"
#include "BVTestAppLogic.h"




// ***********************
//
void            bv::BlackVisionAppFramework::MainFrameworkInitializer       ( int , char *[] )
{
    bv::ApplicationBase::MainFun = &bv::WindowedApplication::MainImpl;
    bv::ApplicationBase::ApplicationInstance = new bv::BlackVisionAppFramework();
}

// ***********************
//
bool            bv::BlackVisionAppFramework::RegisterFrameworkInitializer   ()
{
    bv::InitSubsystem::AddInitializer( bv::BlackVisionAppFramework::MainFrameworkInitializer );
    RegisterLogicInitializers();

    return true;
}

// ***********************
//
void            bv::BlackVisionAppFramework::PostFrame()
{
    // Note: we ommit frame stats here.
    m_app->PostFrameLogic();
}


namespace bv
{

// ***********************
//
BVAppLogic *        BlackVisionAppFramework::CreateAppLogic( bv::Renderer * renderer, audio::AudioRenderer * audioRenderer ) const
{
    return new BVTestAppLogic( renderer, audioRenderer );
}


}	// bv