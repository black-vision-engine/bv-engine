/**
@file LoadBarrier.h
@author nieznanysprawiciel
Implementation based on https://github.com/nieznanysprawiciel/swGraphicAPI/blob/extResourceManager/ResourceManager/AsyncLoad/LoadBarrier.cpp
*/

#include "stdafx.h"

#include "LoadBarrier.h"


namespace bv
{

// ================================ //
//
WaitingAsset::WaitingAsset	( AssetDescConstPtr assetDesc )
    : m_assetDesc( assetDesc )
    , m_numWaiting( 0 )
    , m_ready( false )
{}

// ================================ //
//
WaitingAsset::~WaitingAsset()
{
    // @todo It won't work :(
    LoadingCompleted();
}

// ================================ //
//
bool			WaitingAsset::WaitUntilLoaded		()
{
    std::unique_lock< std::mutex > lock( m_lock );
    while( !m_ready )
        m_condition.wait( lock );

    lock.unlock();

    auto remaining = --m_numWaiting;
    if( remaining == 0 )
        return true;
    return false;
}

// ================================ //
//
void			WaitingAsset::RequestAsset			()
{
    m_numWaiting++;
}

// ================================ //
//
void			WaitingAsset::LoadingCompleted		()
{
    m_lock.lock();
    m_ready = true;
    m_lock.unlock();

    m_condition.notify_all();
}

// ================================ //
//
bool			WaitingAsset::Compare				( AssetDescConstPtr assetDesc )
{
    assert( !"Implement me" );
    return false;
}


//====================================================================================//
//				LoadBarrier
//====================================================================================//

// ================================ //
//
LoadBarrier::~LoadBarrier()
{
    for( auto& asset : m_waitingAssets )
        delete asset;
}

// ================================ //
//
std::pair< WaitingAsset*, bool >		LoadBarrier::RequestAsset		( AssetDescConstPtr assetDesc )
{
    std::unique_lock< std::mutex > lock( m_lock );

    WaitingAsset* assetWait = nullptr;
    for( auto asset : m_waitingAssets )
    {
        if( asset->Compare( assetDesc ) )
        {
            assert( assetWait == nullptr );		// Should be only one file in waiting vector.
            assetWait = asset;
        }
    }

    if( assetWait )
    {
        assetWait->RequestAsset();
        return std::make_pair< WaitingAsset*, bool >( std::move( assetWait ), true );
    }
    else
    {
        auto iter = m_waitingAssets.insert( m_waitingAssets.end(), new WaitingAsset( assetDesc ) );
        return std::make_pair< WaitingAsset*, bool >( std::move( *iter ), false );
    }
}

// ================================ //
//
void									LoadBarrier::WaitUntilLoaded	( WaitingAsset* asset )
{
    bool isLast = asset->WaitUntilLoaded();
    if( isLast )
    {
        std::unique_lock< std::mutex > lock( m_lock );

        bool removed = false;
        for( auto iter = m_waitingAssets.begin(); iter != m_waitingAssets.end(); ++iter )
        {
            if( *iter == asset )
            {
                delete *iter;
                m_waitingAssets.erase( iter );
                removed = true;
                break;
            }
        }

        assert( removed );
    }
}

// ================================ //
//
void									LoadBarrier::LoadingCompleted	( AssetDescConstPtr assetDesc )
{
    std::unique_lock< std::mutex > lock( m_lock );

    for( auto asset : m_waitingAssets )
    {
        if( asset->Compare( assetDesc ) )
            asset->LoadingCompleted();

        // WaitingAsset will be removed when last thread will leave waiting lock.
    }
}

}	// sw
