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

    explicit    FrameworkTest       ()
        : m_appLogic( nullptr )       // This will be set in future.
        , m_isLastFrame( true )
        , m_overrideTime( false )
        , m_frameNum( 0 )
        , m_frameTime( 0 )
    {}

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





// ***********************
// Creates and registers test class instance.
#define REGISTER_TEST_INFO( testClassName, test_case_name, test_name, parent_class, parent_id )\
::testing::TestInfo* const testClassName\
::test_info_ = \
::testing::internal::MakeAndRegisterTestInfo( \
    #test_case_name, #test_name, NULL, NULL, \
    ::testing::internal::CodeLocation( __FILE__, __LINE__ ), \
    ( parent_id ), \
    parent_class::SetUpTestCase, \
    parent_class::TearDownTestCase, \
    new ::testing::internal::TestFactoryImpl<\
    testClassName> );


// ***********************
// Creates and registers test class instance based on bv framework.
#define REGISTER_FRAMEWORK_GTEST_INFO( testClassName, test_case_name, test_name )\
    REGISTER_TEST_INFO( testClassName, test_case_name, test_name, ::bv::FrameworkTest, ::testing::internal::GetTestTypeId() )


// ***********************
// Declares TestInfo structure and other gtest things inside of test class.
#define DECALRE_GTEST_INFO( testClassName )\
private:\
    static ::testing::TestInfo* const test_info_ GTEST_ATTRIBUTE_UNUSED_; \
    GTEST_DISALLOW_COPY_AND_ASSIGN_( testClassName );

// ***********************
// Declares TestInfo structure and other gtest things inside of test class.
#define DECALRE_GTEST_INFO_WITH_CONSTRUCTOR( testClassName )\
public:\
    testClassName() {}\
private:\
    static ::testing::TestInfo* const test_info_ GTEST_ATTRIBUTE_UNUSED_; \
    GTEST_DISALLOW_COPY_AND_ASSIGN_( testClassName );


// ========================================================================= //
// Test creation macros
// ========================================================================= //


// ***********************
//
#define SIMPLE_FRAMEWORK_TEST_IN_SUITE_IMPL( suite, name )    \
class GTEST_TEST_CLASS_NAME_( suite, name ) : public bv::FrameworkTest   \
{                                       \
    DECALRE_GTEST_INFO_WITH_CONSTRUCTOR( GTEST_TEST_CLASS_NAME_( suite, name ) )  \
public:                                 \
                                                                \
    virtual void        PreEvents           () override;        \
};                                                              \
                                                                \
REGISTER_FRAMEWORK_GTEST_INFO( GTEST_TEST_CLASS_NAME_( suite, name ), suite, name )     \
void        GTEST_TEST_CLASS_NAME_( suite, name )::PreEvents           ()


#define SIMPLE_FRAMEWORK_TEST_IN_SUITE( suite, name )   SIMPLE_FRAMEWORK_TEST_IN_SUITE_IMPL( suite, name )
