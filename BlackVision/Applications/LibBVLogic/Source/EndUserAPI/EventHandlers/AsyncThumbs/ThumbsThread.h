#pragma once

#include "DataTypes/QueueConcurrentLimited.h"

#include <thread>
#include <atomic>
#include <string>


namespace bv
{


// ***********************
//
struct LoadThumbRequest
{
    std::string         ProjectName;
    std::string         Category;
    std::string         Path;

    int                 EventID;
    int                 SocketID;
    bool                EndThread;

    // ***********************
    //
    LoadThumbRequest()
        : EndThread( false )
    {}
};




// ***********************
//
class ThumbsThread
{
private:

    QueueConcurrentLimited< LoadThumbRequest >         m_assetRequests;

    std::thread                 m_thread;

public:

    explicit        ThumbsThread        ();
                    ~ThumbsThread       ();

    void			QueueRequest		( LoadThumbRequest && request );
    void            EndThread           ();

private:

    void			LoadingThread		();
    void            GenerateThumbnails  ( const LoadThumbRequest & request );
};


}	// bv


