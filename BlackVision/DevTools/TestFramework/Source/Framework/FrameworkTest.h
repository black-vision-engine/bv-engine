#pragma once

#include "CoreDEF.h"

#include "UnitTest++.h"


namespace bv
{

class BVTestAppLogic;


class FrameworkTest : public UnitTest::Test
{
    friend class BVTestAppLogic;
private:

    BVTestAppLogic*         m_appLogic;
    SizeType                m_frameNum;
    bool                    m_isLastFrame;
    bool                    m_overrideTime;

public:

    explicit    FrameworkTest       ( char const* testName, char const* suiteName = "DefaultSuite", char const* filename = "", int lineNumber = 0 )
        :   UnitTest::Test( testName, suiteName, filename, lineNumber )
        ,   m_appLogic( nullptr )       // This will be set in future.
        ,   m_isLastFrame( true )
        ,   m_overrideTime( false )
        ,   m_frameNum( 0 )
    {}

    virtual void        RunImpl         () const;
    virtual void        RunImplNotConst ();

public:

    // Interface for test creator.

    virtual void        PreEvents           () {}
    virtual void        PreModelUpdate      () {}
    virtual void        PreRender           () {}
    virtual void        PostRender          () {}

    virtual TimeType    ComputeFrameTime    () { return 0.0f; }


    SizeType            GetFrameNumber      () const { return m_frameNum; }

private:

    // Call only from BVTestAppLogic

    void        SetAppLogic             ( BVTestAppLogic* logic );
    bool        IsLastFrame             () const                        { return m_isLastFrame; }

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


