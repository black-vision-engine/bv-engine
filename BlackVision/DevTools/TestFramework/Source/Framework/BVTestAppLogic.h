#pragma once

#include "BVAppLogic.h"

#include "UnitTest++.h"
#include "TestExecutor.h"



namespace bv {


// ***********************
//
class BVTestAppLogic : public BVAppLogic
{
    friend class FrameworkTest;
private:

    TestExecutor *      m_testExecutor;

public:
                    BVTestAppLogic      ( Renderer * renderer, audio::AudioRenderer * audioRenderer, const std::string & testname );
                    BVTestAppLogic      ( Renderer * renderer, audio::AudioRenderer * audioRenderer );
                    ~BVTestAppLogic     ();

    virtual void    OnUpdate            ( Renderer * renderer, audio::AudioRenderer * audioRenderer ) override;
    virtual void    LoadScene           () override;

    virtual void    PostFrameLogic      () override;

private:

    void            EndExecution        ();
};

} //bv