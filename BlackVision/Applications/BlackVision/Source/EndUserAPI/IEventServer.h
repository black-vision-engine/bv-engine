#pragma once

#include <string>

#pragma warning( push )
#pragma warning( disable : 4100 )

#include "FastDelegate.h"

#pragma warning( pop )

namespace bv
{
class RemoteController;


struct ResponseMsg
{
    std::wstring message;
    int socketID;
    bool sent;

    ResponseMsg()
    { sent = false; }
};


typedef fastdelegate::FastDelegate2<const std::wstring&, int> QueueEventCallback;

class IEventServer
{
private:
protected:
public:
    virtual ~IEventServer() {}

    virtual bool            InitializeServer        ( const QueueEventCallback& callback, int port ) = 0;
    virtual void            SendResponse            ( ResponseMsg& meesage ) = 0;
    /// For now I assume, there's only one one implementation and this function is enough.
    /// Maybe in future we should create a factory or something.
    static IEventServer*    CreateServerObject      ();
};



} //bv
