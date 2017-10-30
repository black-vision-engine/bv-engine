#pragma once

#include "CoreDEF.h"

#include "NameMangler.h"
#include "TestEnvironment.h"


namespace bv
{

class BVTestAppLogic;
class BVProjectEditor;




// ***********************
//
class FrameworkTest : public ::testing::Test
{
    friend class BVTestAppLogic;
    friend class TestExecutor;
private:

    BVTestAppLogic*         m_appLogic;
    SizeType                m_frameNum;
    TimeType                m_frameTime;
    bool                    m_isLastFrame;
    bool                    m_overrideTime;

public:

    //explicit    FrameworkTest       ( char const* testName, char const* suiteName = "DefaultSuite", char const* filename = "", int lineNumber = 0 )
    //    : UnitTest::Test( testName, suiteName, filename, lineNumber )
    //    , m_appLogic( nullptr )       // This will be set in future.
    //    , m_isLastFrame( true )
    //    , m_overrideTime( false )
    //    , m_frameNum( 0 )
    //    , m_frameTime( 0 )
    //{}

    // Run test from BVTesterAppLogic

    virtual void        RunImpl         ();
    virtual void        Run             ();

protected:

    // gtest overrides

    virtual void        TestBody    () override;

    virtual void        SetUp       () override;
    virtual void        TearDown    () override;

public:

    // Interface for test creator.

    virtual void        PreEvents           () {}
    virtual void        PreModelUpdate      () {}
    virtual void        PreRender           () {}
    virtual void        PostRender          () {}

    virtual TimeType    ComputeFrameTime    () { return 0.0f; }

    BVTestAppLogic*     GetAppLogic         () const { return m_appLogic; }
    BVProjectEditor*    GetProjectEditor    () const;
    SizeType            GetFrameNumber      () const { return m_frameNum; }
    TimeType            GetFrameTime        () const { return m_frameTime; }

    bool                IsLastFrame         () const { return m_isLastFrame; }

private:

    void        SetAppLogic             ( BVTestAppLogic* logic );
    bool        HandleFailure           ();

protected:

    // Functions for user interaction.
    void        EndTestAfterThisFrame   ( bool value );
    void        UseOverridenTime        ( bool value );

private:

    // Private internal functions.
    TimeType    ComputeFrameTimeImpl    ();

    void        PreRunImpl              ();
    void        PostRunImpl             ();
};


}	// bv


// ========================================================================= //
// Framework Helper macros
// ========================================================================= //



#define SIMPLE_FRAMEWORK_TEST_IN_SUITE_IMPL( suite, name )    \
class name : public bv::FrameworkTest   \
{                                       \
public:                                 \
    name() : bv::FrameworkTest( ::bv::MangleName< name >( #name ).c_str(), suite, __FILE__, __LINE__ ) {}       \
                                                                                                                \
    virtual void        PreEvents           () override;                                                        \
} name ## Instance;                                                                                             \
                                                                                                                \
UnitTest::ListAdder adder ## name ( UnitTest::Test::GetTestList(), &name ## Instance );                         \
void        name::PreEvents           ()


#define SIMPLE_FRAMEWORK_TEST_IN_SUITE( suite, name )   SIMPLE_FRAMEWORK_TEST_IN_SUITE_IMPL( #suite, name )
#define SIMPLE_FRAMEWORK_TEST( name )                   SIMPLE_FRAMEWORK_TEST_IN_SUITE_IMPL( UnitTestSuite::GetSuiteName(), name )
