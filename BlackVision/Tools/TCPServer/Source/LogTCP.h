#pragma once

#include <iostream>
#include <thread>
#include <string>
#include <vector>

#include "DataTypes/QueueConcurrent.h"
#include "Tools/Logger/LogMsgStruct.h"


namespace bv
{
#define LOG_TCP_NUM_SOCKETS 1

class LogTCP
{
private:
    LogTCP& operator=( LogTCP& );

private:
    QueueConcurrent<bv::LogMsg>&        m_queue;
    std::thread                         m_mainThread;

    std::string             m_address;
    unsigned short          m_port;
    volatile bool           m_end;

    SOCKET                  ahSocket[ LOG_TCP_NUM_SOCKETS ];
    bool                    m_validConnection;
public:
    LogTCP( QueueConcurrent<bv::LogMsg>& queue )
        : m_queue( queue ),
        m_end( false ),
        m_validConnection( false )
    {}
    ~LogTCP();

    void        Initialize      ( const std::string& address, unsigned short port );
private:
    void        MainThread      ();
    void        ConnectThread   ();
    SOCKET      ConnectToPort   ();
};

} //bv

