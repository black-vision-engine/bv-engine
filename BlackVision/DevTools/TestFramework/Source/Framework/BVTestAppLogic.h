#pragma once

#include "BVAppLogic.h"



namespace bv
{

class RenderLogic;



// ***********************
//
class BVTestAppLogic : public BVAppLogic
{
    friend class FrameworkTest;
    friend class Benchmark;
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


    void            RegisterMocks       ();

    void            RegisterMockVideoCard   ();
    void            RegisterMockPlugins     ();
    void            RegisterMockLogics      ();

public:

    void            InjectTest          ( FrameworkTest * test );

private:

    void            EndExecution        ();

public:

    ///@name Test Accessors
    /// Use these functions in your tests to access BVAppLogic members.
    ///{@

    audio::AudioRenderer *              GetAudioRenderer        () { return m_audioRenderer; }
    videocards::VideoCardManager *      GetVideoCardManager     () { return m_videoCardManager; }

    RenderLogic *                       GetRenderLogic          () { return m_renderLogic; }
    ///@}
};

} //bv
