#pragma once

#include "Threading/Thread.h"
#include "Engine/Processes/Process.h"


namespace bv
{

class ProcessThreaded : public Process, public Thread
{
public:

                    ProcessThreaded     ();
    virtual         ~ProcessThreaded    ();

protected:

    virtual void    OnUpdate            ( unsigned long millis ) override;

};

} //bv
