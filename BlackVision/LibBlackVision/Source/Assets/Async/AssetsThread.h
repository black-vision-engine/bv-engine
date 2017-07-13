#pragma once

#include "LoadAssetRequest.h"

#include "DataTypes/QueueConcurrent.h"

#include <thread>
#include <atomic>


namespace bv
{

// ***********************
//
class AssetsThread
{
private:

    QueueConcurrent< LoadAssetRequest >         m_assetRequests;
    
    std::thread                 m_thread;
    std::atomic< bool >         m_endMessage;

public:

    explicit        AssetsThread        ();
                    ~AssetsThread       ();

    void			QueueRequest		( LoadAssetRequest&& request );
    void            EndThread           ();

private:

    void			LoadingThread		();
};


}	// bv


