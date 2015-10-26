#pragma once


#include "Engine/Events/Events.h"

namespace bv
{

class RemoteCommandsConverter
{
private:
public:
    RemoteCommandsConverter();
    ~RemoteCommandsConverter();

    void            QueueEvent          ( std::string&& eventString );
    std::string     PollEvent           ();
};


} //bv
