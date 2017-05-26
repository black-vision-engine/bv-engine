#include "bvAppTest.h"


#pragma warning(disable :4996)
#include "bvApp.h"

#include "System/InitSubsystem.h"
#include "EndUserAPI/RemoteController.h"

#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Events/Interfaces/IEventManager.h"

#include "Tools/Profiler/HerarchicalProfiler.h"
#include "Application/ApplicationContext.h"

#include "Statistics/StatsFormatters.h"


#include "BVTestAppLogic.h"
#include "BVConfig.h"

#include "Application/Win32/DisableCrashReport.h"

// Log initializer
#include "bvAppLogInitializer.inl"


// *********************************
// FIXME: move it to valid BV windowed version of engine and wrap with a macro
bool			bv::BlackVisionApp::RegisterInitializer	()
{
    bv::InitSubsystem::AddInitializer( bv::BlackVisionApp::MainInitializer );
    bv::InitSubsystem::AddInitializer( bv::BlackVisionApp::LoggerInitializer );

    return true;
}


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


namespace bv
{

// ***********************
//
BVAppLogic *        BlackVisionAppFramework::CreateAppLogic( bv::Renderer * renderer, audio::AudioRenderer * audioRenderer ) const
{
    return new BVTestAppLogic( renderer, audioRenderer );
}


}	// bv