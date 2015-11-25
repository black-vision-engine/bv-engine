#pragma once

#include "CoreDEF.h"
#include "DataTypes/QueueConcurrent.h"
#include "Tools/Logger/LogMsgStruct.h"

#include "../../Applications/BlackVision/Source/EndUserAPI/IEventServer.h"


namespace bv
{

struct InitData
{
    unsigned int    SeverityLevel;
    unsigned int    LogModules;
};


enum SocketClientState
{
    SCS_Uninitialized,
    SCS_Running,
    SCS_Ended
};

class SocketClient
{
private:
    QueueConcurrent<ResponseMsg>    m_responseQueue;
    QueueEventCallback              m_sendCommandCallback;

    SOCKET                          m_socketID;

    QueueConcurrent<LogMsg>*        m_logQueue;
    int                             m_logID;

    SocketClientState               m_state;
public:
    SocketClient( SOCKET socketID, QueueEventCallback callback );
    ~SocketClient();

    void            QueueResponse       ( ResponseMsg&& message );
    void            MainThread          ();

    SocketClientState   GetState        ()      { return m_state; }
private:
    InitData        InitCommunication   ( SOCKET socketID );
    bool            Authorization       ( SOCKET socketID );

    void            OnEndMainThread     ();
};

DEFINE_PTR_TYPE( SocketClient );


} //bv
