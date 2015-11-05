#pragma once

#include "../RemoteCommandsConverter.h"


namespace bv
{
class IDeserializer;

class JsonCommandsConverter : public RemoteCommandsConverter
{
private:
public:
    JsonCommandsConverter();
    ~JsonCommandsConverter();

    void                QueueEvent          ( const std::wstring& eventString ) override;
    std::wstring        PollEvent           () override;
};

} //bv
