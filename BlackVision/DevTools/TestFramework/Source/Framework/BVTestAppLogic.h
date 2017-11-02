#pragma once

#include "BVAppLogic.h"



namespace bv {


// ***********************
//
class BVTestAppLogic : public BVAppLogic
{
    friend class FrameworkTest;
private:

    FrameworkTest *     m_test;

public:
                    BVTestAppLogic      ( Renderer * renderer, audio::AudioRenderer * audioRenderer, const std::string & testname );
                    BVTestAppLogic      ( Renderer * renderer, audio::AudioRenderer * audioRenderer );
                    ~BVTestAppLogic     ();

    virtual void    OnUpdate            ( Renderer * renderer, audio::AudioRenderer * audioRenderer ) override;
    virtual void    LoadScene           () override;

    virtual void    PostFrameLogic      () override;

    void            RestartTimer        ();

public:

    void            InjectTest          ( FrameworkTest * test );

private:

    void            EndExecution        ();
};

} //bv
