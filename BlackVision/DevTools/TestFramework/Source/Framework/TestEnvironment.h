#pragma once

#include "gtest/gtest.h"


#include "bvAppTest.h"


namespace bv
{

class FrameworkTest;
class BVTestAppLogic;


// ***********************
//
class TestEnvironment : public ::testing::Environment
{
private:

    static TestEnvironment *    gEnvironment;

private:

    BlackVisionAppFramework *       m_app;

    int *       ArgC;   ///< Google Test removes recognized flags. We need reference to number of arguments.
    char **     ArgV;

public:

    explicit    TestEnvironment     ( int * argc, char * argv[] );
    virtual     ~TestEnvironment    () {}

    // Override this to define how to set up the environment.
    virtual void        SetUp       () override;

    // Override this to define how to tear down the environment.
    virtual void        TearDown    () override;

public:

    bool                MainLoopStep    ();
    void                InjectTest      ( FrameworkTest * test );

    BVTestAppLogic *    GetAppLogic     ();

public:

    static void                 SetEnvironment  ( TestEnvironment * environment );
    static TestEnvironment *    GetEnvironment  ();
};


}	// bv


