#pragma once

#include "BVAppLogic.h"


namespace bv {

class BVTestAppLogic : public BVAppLogic
{
public:
                    BVTestAppLogic      ( Renderer * renderer, audio::AudioRenderer * audioRenderer );
                    ~BVTestAppLogic     ();

    virtual void    OnUpdate            ( Renderer * renderer, audio::AudioRenderer * audioRenderer ) override;
    virtual void    LoadScene           () override;

    void            Update              ( unsigned int time );
};

} //bv
