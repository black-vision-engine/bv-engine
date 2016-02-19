#include "stdafx.h"

#include "ApplicationBase.h"

#include <cassert>

#include "System/InitSubsystem.h"


bv::ApplicationBase * bv::ApplicationBase::ApplicationInstance		= nullptr;
bv::ApplicationBase::ApplicationMain bv::ApplicationBase::MainFun	= nullptr;


namespace bv {

// *********************************
//
ApplicationBase::ApplicationBase	()
{
}

// *********************************
//
ApplicationBase::~ApplicationBase	()
{
}

} //bv


// *********************************
// Entry point for all applications derived from this class
int main( int argc, char * argv[] )
{

//TODO: Initialize here all generic subsystems here (loggers, generic managers, system wide subsystems)
//TODO: Register (and/or) initialize all (possibly already registered) components
//TODO: Load all generic resources

    bv::InitSubsystem::CallInitializers();

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
