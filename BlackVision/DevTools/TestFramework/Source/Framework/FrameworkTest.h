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

public:

    explicit    FrameworkTest       ( char const* testName, char const* suiteName = "DefaultSuite", char const* filename = "", int lineNumber = 0 )
        : UnitTest::Test( testName, suiteName, filename, lineNumber )
        , m_appLogic( nullptr )       // This will be set in future.
    {}

    virtual void        RunImpl         () const;
    virtual void        RunImplNotConst ();

public:

    // Interface for test creator.

    virtual void        PreEvents           () {}
    virtual void        PostEvents          () {}
    virtual void        PreModelUpdate      () {}
    virtual void        PreRender           () {}
    virtual void        PostRender          () {}


private:

    void        SetAppLogic         ( BVTestAppLogic* logic );
};

}	// bv


