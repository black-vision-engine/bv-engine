#pragma once

#include "../RemoteCommandsConverter.h"
#include "../IEventServer.h"

namespace bv
{
class IDeserializer;

class JsonCommandsConverter : public RemoteCommandsConverter
{
private:
    IEventServer*       m_eventServer;
public:
    JsonCommandsConverter();
    ~JsonCommandsConverter();

    void                QueueEvent          ( const std::wstring& eventString, int socketID ) override;
    ResponseEventPtr    PollEvent           () override;
    bool                InitializeServer    () override;
};

} //bv
