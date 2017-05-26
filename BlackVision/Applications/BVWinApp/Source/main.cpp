//#include "stdafx.h"
//
#include "Application/ApplicationBase.h"
#include "../BlackVision/Source/bvApp.h"
//
#include <cassert>
//
#include "System/InitSubsystem.h"

static bool registered = bv::BlackVisionApp::RegisterInitializer();


// *********************************
// Entry point for all applications derived from this class
int main( int argc, char * argv[] )
{
    //TODO: Initialize here all generic subsystems here (loggers, generic managers, system wide subsystems)
    //TODO: Register (and/or) initialize all (possibly already registered) components
    //TODO: Load all generic resources

    bv::InitSubsystem::CallInitializers( argc, argv );

    assert( bv::ApplicationBase::ApplicationInstance != nullptr );

    //FIXME: Why not use polymorphism here??
    int exitCode = bv::ApplicationBase::MainFun( argc, argv );

    //TODO: deinitialize all subsystems
    //TODO: Free all generic resources
    //TODO: make sure that all logs are closed and no data is corrupted and/or saved 

    //FIXME: this should be done in some kind of termination subsystem
    delete bv::ApplicationBase::ApplicationInstance;

    return exitCode;
}
