#pragma once

#include "BVAppLogic.h"


namespace bv {

class BVTestAppLogic : public BVAppLogic
{
    friend class FrameworkTest;
public:
                    BVTestAppLogic      ( Renderer * renderer, audio::AudioRenderer * audioRenderer );
                    ~BVTestAppLogic     ();

    virtual void    OnUpdate            ( Renderer * renderer, audio::AudioRenderer * audioRenderer ) override;
    virtual void    LoadScene           () override;

    void            PostFrameLogic      ();
};

} //bv
