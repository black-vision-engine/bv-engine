#pragma once

#include <thread>

#include "DataTypes/QueueConcurrent.h"
#include "Tools/Logger/LogMsgStruct.h"

#include "Util/IEventServer.h"

#include "CoreDEF.h"

#include "win_sock.h"

#include <atomic>


namespace bv {

struct InitData
{
    unsigned int    SeverityLevel;
    unsigned int    LogModules;
};


enum SocketConnectionState
{
    SCS_Uninitialized,
    SCS_Running,
    SCS_Ended
};


class SocketConnection
{
private:

    QueueConcurrent< ResponseMsg >  m_responseQueue;
    QueueEventCallback              m_sendCommandCallback;

    SOCKET                          m_socketID;
    std::thread                     m_clientThread;

    QueueConcurrent< LogMsg > *     m_logQueue;
    int                             m_logID;

    SocketConnectionState           m_state;
    int                             m_numSocketErrors;      ///< Count errors so we can end thread if there's no hope for holding connection.

    std::atomic< bool >             m_end;

public:

                    SocketConnection    ( SOCKET socketID, QueueEventCallback callback );
                    ~SocketConnection   ();

    void            QueueResponse       ( const ResponseMsg & message );
    void            QueueResponse       ( ResponseMsg && message );
    void            KillClient          ();
    void            InitThread          ();

    SocketConnectionState   GetState    ()      { return m_state; }

private:

    void            MainThread          ();
    InitData        InitCommunication   ( SOCKET socketID );
    bool            Authorization       ( SOCKET socketID );

    void            OnEndMainThread     ();

};

DEFINE_PTR_TYPE( SocketConnection );

} //bv
