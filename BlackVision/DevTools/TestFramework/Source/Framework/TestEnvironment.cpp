#include "TestEnvironment.h"

#include "System/InitSubsystem.h"
#include "Application/ApplicationBase.h"

#include "bvAppTest.h"
#include "BVTestAppLogic.h"


namespace bv
{


TestEnvironment *    TestEnvironment::gEnvironment = nullptr;;



// ***********************
//
TestEnvironment::TestEnvironment        ( int argc, char * argv[] )
    :   ArgC( argc )
    ,   ArgV( argv )
    ,   m_app( nullptr )
{}

// ***********************
//
void            TestEnvironment::SetUp      ()
{
    bv::InitSubsystem::CallInitializers( ArgC, ArgV );
    assert( bv::ApplicationBase::ApplicationInstance != nullptr );

    m_app = static_cast< BlackVisionAppFramework * >( bv::ApplicationBase::ApplicationInstance );

    m_app->Initialize( ArgC, ArgV );
}

// ***********************
//
void            TestEnvironment::TearDown   ()
{
    m_app->TerminateStep();
    delete bv::ApplicationBase::ApplicationInstance;
}

// ***********************
//
bool            TestEnvironment::MainLoopStep   ()
{
    return m_app->MainLoopStep();
}

// ***********************
//
void            TestEnvironment::InjectTest     ( FrameworkTest * test )
{
    GetAppLogic()->InjectTest( test );
}

// ***********************
//
BVTestAppLogic *    TestEnvironment::GetAppLogic()
{
    return static_cast< BVTestAppLogic * >( m_app->GetAppLogic() );
}

// ***********************
//
void                TestEnvironment::SetEnvironment     ( TestEnvironment * environment )
{
    gEnvironment = environment;
}

// ***********************
//
TestEnvironment *   TestEnvironment::GetEnvironment     ()
{
    return gEnvironment;
}


}	// bv

