#pragma once

#include "Engine/Processes/ProcessThreaded.h"

namespace bv
{

class InitProcess : public ProcessThreaded
{
public:

                    InitProcess     ();
    virtual         ~InitProcess    ();

protected:

    virtual void    Run             ();
    
    virtual void    Init            () = 0;

};

} //bv
