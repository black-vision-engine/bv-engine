#pragma once

#include "Engine/Processes/Process.h"


namespace bv
{

class MainLoopProcess : public Process
{
private:



public:

    virtual void    OnUpdate    ( unsigned long millis ) = 0;

};

} //bv
