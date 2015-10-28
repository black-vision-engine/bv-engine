#pragma once

#include "../RemoteCommandsConverter.h"


namespace bv
{

class JsonCommandsConverter : public RemoteCommandsConverter
{
public:
    JsonCommandsConverter();
    ~JsonCommandsConverter();

    void                QueueEvent          ( std::wstring&& eventString ) override;
    std::wstring        PollEvent           () override;
};

} //bv
