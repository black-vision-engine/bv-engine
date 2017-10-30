#include "bvAppTest.h"


#pragma warning(disable :4996)
#include "bvApp.h"

#include "System/InitSubsystem.h"
#include "BVTestAppLogic.h"

#include "System/Path.h"


// ***********************
//
void            bv::BlackVisionAppFramework::MainFrameworkInitializer       ( int , char *[] )
{
    bv::ApplicationBase::MainFun = &bv::WindowedApplication::MainImpl;
    bv::ApplicationBase::ApplicationInstance = new bv::BlackVisionAppFramework();
}

// ***********************
//
void            bv::BlackVisionAppFramework::RegisterTestParams             ( int argc, char * argv[] )
{
    static_cast< bv::BlackVisionAppFramework * >( bv::ApplicationBase::ApplicationInstance )->ParseTestParameters( argc, argv );
}

// ***********************
//
bool            bv::BlackVisionAppFramework::RegisterFrameworkInitializer   ()
{
    bv::InitSubsystem::AddInitializer( bv::BlackVisionAppFramework::MainFrameworkInitializer );
    RegisterLogicInitializers();
    bv::InitSubsystem::AddInitializer( bv::BlackVisionAppFramework::RegisterTestParams );

    return true;
}


namespace bv
{


// ***********************
//
bool                BlackVisionAppFramework::OverrideConfig     ( const std::string & configPath )
{
    Path newConfigPath( configPath );
    Path targetConfigPath( "config.xml" );

    Path::Remove( targetConfigPath );

    if( Path::Exists( newConfigPath ) )
    {
        Path::Copy( newConfigPath, targetConfigPath );
    }
    else
    {
        Path::Copy( "TestConfigs/DefaultConfig.xml", targetConfigPath );
    }

    return true;
}


// ***********************
//
BVAppLogic *        BlackVisionAppFramework::CreateAppLogic     ( bv::Renderer * renderer, audio::AudioRenderer * audioRenderer ) const
{
    return new BVTestAppLogic( renderer, audioRenderer );
}


// ***********************
//
void            BlackVisionAppFramework::PostFrame()
{
    // Note: we ommit frame stats here.
    m_app->PostFrameLogic();
}



// ***********************
//
void            BlackVisionAppFramework::ParseTestParameters    ( int argc, char * argv[] )
{
    // Note: This is empty for future use.

    { argc; argv; }

    // FIXME: It would be better to use TCLAP library for parsing command line arguments.
    // but there's a problem - in such case we should make two fases of initialization. First would
    // add possible cmd args to TCLAP::CmdLine object and second would take parsed values and initialize
    // subsystems. This looks horible.
}



}	// bv

