#pragma once

#include "BVBenchmark.h"
#include "FrameworkTest.h"



namespace bv
{

class BVTestAppLogic;
class BVProjectEditor;




// ***********************
/// Benchmark class can be injected to test framework and call normal rendering pipeline.
/// We need Framework to call main loop steps in benchmarks.
class Benchmark : public FrameworkTest
{
    friend class BVTestAppLogic;
private:
public:

    virtual void        Run             ();

public:

    explicit            Benchmark           ();
    

    virtual void        PreEvents           () {}
    virtual void        PreModelUpdate      () {}
    virtual void        PreRender           () {}
    virtual void        PostRender          () {}

public:

    using FrameworkTest::SetUp;
    using FrameworkTest::TearDown;
};



}	// bv


