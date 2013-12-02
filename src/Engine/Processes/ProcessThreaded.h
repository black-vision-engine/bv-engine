#pragma once

#include "System/Threading/Thread.h"
#include "Engine/Processes/Process.h"


namespace bv
{

class ProcessThreaded : public Process, public Thread
{
private:

public:

                    ProcessThreaded     ();
    virtual         ~ProcessThreaded    ();

protected:

    virtual void    OnUpdate            ( unsigned long millis ) override;

};

} //bv
