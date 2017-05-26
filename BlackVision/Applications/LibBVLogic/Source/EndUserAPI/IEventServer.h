#pragma once

#include <string>
#include <limits>

#pragma warning( push )
#pragma warning( disable : 4100 )

#include "FastDelegate.h"

#pragma warning( pop )


namespace bv {


class RemoteController;

struct ResponseMsg
{
    std::string     message;
    int             socketID;
};


typedef fastdelegate::FastDelegate2< const std::string &, int > QueueEventCallback;


class IEventServer
{
public:

    virtual                 ~IEventServer           () {}

    virtual bool            InitializeServer        ( const QueueEventCallback & callback, int port ) = 0;
    virtual void            DeinitializeServer      () = 0;
    virtual void            SendResponse            ( ResponseMsg & meesage ) = 0;

    /// For now I assume, there's only one one implementation and this function is enough.
    /// Maybe in future we should create a factory or something.
    static IEventServer *   CreateServerObject      ();

};



} //bv
