#include "bvAppTest.h"


#pragma warning(disable :4996)
#include "bvApp.h"

#include "System/InitSubsystem.h"
#include "BVTestAppLogic.h"


#include "tclap/CmdLine.h"


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
BVAppLogic *        BlackVisionAppFramework::CreateAppLogic     ( bv::Renderer * renderer, audio::AudioRenderer * audioRenderer ) const
{
    return new BVTestAppLogic( renderer, audioRenderer, m_testName );
}


// ***********************
//
void            BlackVisionAppFramework::ParseTestParameters    ( int argc, char * argv[] )
{
    try
    {
        TCLAP::CmdLine cmd( "Black Vision Test Framework" );

        TCLAP::ValueArg< std::string > testOutput( "o", "Output", "Output file", true, "", "string" );
        cmd.add( testOutput );
        
        cmd.parse( argc, argv );

        m_testName = testOutput.getValue();
    }
    catch( TCLAP::ArgException & e )
    {
        std::cerr << "Parsing arguments error: " << e.error() << " for arg " << e.argId() << std::endl;
    }
}

// ***********************
//
void            BlackVisionAppFramework::PostFrame()
{
    // Note: we ommit frame stats here.
    m_app->PostFrameLogic();
}

}	// bv