#pragma once

#include <string>


namespace bv
{
class RemoteCommandsListener;


struct ResponseMsg
{
    std::wstring message;
    int socketID;
    bool sent;

    ResponseMsg()
    { sent = false; }
};


class IEventServer
{
private:
protected:
public:
    ~IEventServer() {}

    virtual bool            InitializeServer    ( RemoteCommandsListener* converter, int port ) = 0;
    virtual void            SendResponse        ( ResponseMsg& meesage ) = 0;
    /// For now I assume, there's only one one implementation and this function is enough.
    /// Maybe in future we should create a factory or something.
    static IEventServer*    CreateServerObject  ();
};



} //bv
