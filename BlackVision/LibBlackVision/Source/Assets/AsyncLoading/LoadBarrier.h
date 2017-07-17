#pragma once
/**
@file LoadBarrier.h
@author nieznanysprawiciel

Implementation based on https://github.com/nieznanysprawiciel/swGraphicAPI/blob/extResourceManager/ResourceManager/AsyncLoad/LoadBarrier.h
*/


#include "CoreDEF.h"
#include "Assets/AssetDescriptor.h"

#include <set>
#include <atomic>
#include <condition_variable>


namespace bv
{

/**@brief Class describing asset waiting for beeing load.*/
class WaitingAsset
{
    friend class LoadBarrier;
private:

    AssetDescConstPtr			m_assetDesc;
    std::atomic< UInt16	>		m_numWaiting;
    bool						m_ready;

    std::condition_variable		m_condition;
    std::mutex					m_lock;

public:
    WaitingAsset		() = default;
    WaitingAsset		( AssetDescConstPtr assetDesc );
    ~WaitingAsset		();


private:

    /**@brief Function blocks until resource will be loaded.
    @return Returns true if it was last waiting thread.*/
    bool				WaitUntilLoaded		();

    /**@brief Increments loading threads count.*/
    void				RequestAsset		();

    /**@brief Notify all threads waiting for this asset.*/
    void				LoadingCompleted	();

    /**@brief Check if file is during laoding.*/
    bool				Compare				( AssetDescConstPtr assetDesc );
};



/**@brief Class prevents ResourceManager from loading Assets multiple times.*/
class LoadBarrier
{
private:

    std::vector< WaitingAsset* >	m_waitingAssets;
    std::mutex						m_lock;

public:
    ~LoadBarrier();

    /**@brief Tries to access asset described by filePath. If asset exists, function increments waiting threads counter.
    Otherwise new WaitingAsset object is created to block future loads.
    @return Function returns true if asset already existed. If WaitingAsset was created in this function call function returns true.
    Note: WaitingAsset should never be nullptr.*/
    std::pair< WaitingAsset*, bool >		RequestAsset		( AssetDescConstPtr assetDesc );

    /**@brief Function waits until asset will be loaded.*/
    void									WaitUntilLoaded		( WaitingAsset* asset );

    /**@brief Notify all threads waiting for this asset.*/
    void									LoadingCompleted	( AssetDescConstPtr assetDesc );
};


}	// sw
