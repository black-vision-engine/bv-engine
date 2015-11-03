#pragma once


#include "Engine/Events/Events.h"

namespace bv
{

class RemoteCommandsConverter
{
private:
public:
    RemoteCommandsConverter() {};
    virtual ~RemoteCommandsConverter() {};

    virtual void                QueueEvent          ( std::wstring&& eventString ) = 0;
    virtual std::wstring        PollEvent           () = 0;
};


} //bv
