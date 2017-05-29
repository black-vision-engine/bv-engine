#pragma once


#include "UnitTest++.h"


namespace bv
{

class BVTestAppLogic;


class FrameworkTest : public UnitTest::Test
{
    friend class BVTestAppLogic;
private:

    BVTestAppLogic*         m_appLogic;
    bool                    m_isLastFrame;

public:

    explicit    FrameworkTest       ( char const* testName, char const* suiteName = "DefaultSuite", char const* filename = "", int lineNumber = 0 )
        :   UnitTest::Test( testName, suiteName, filename, lineNumber )
        ,   m_appLogic( nullptr )       // This will be set in future.
        ,   m_isLastFrame( true )
    {}

    virtual void        RunImpl         () const;
    virtual void        RunImplNotConst ();

public:

    // Interface for test creator.

    virtual void        PreEvents           () {}
    virtual void        PreModelUpdate      () {}
    virtual void        PreRender           () {}
    virtual void        PostRender          () {}


private:

    // Call only from BVTestAppLogic

    void        SetAppLogic             ( BVTestAppLogic* logic );
    bool        IsLastFrame             () const                        { return m_isLastFrame; }

protected:

    // Functions for user interaction.
    void        EndTestAfterThisFrame   ( bool value );
};

}	// bv


