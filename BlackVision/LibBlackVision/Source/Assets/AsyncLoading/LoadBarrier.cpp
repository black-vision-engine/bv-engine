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
WaitingAsset::WaitingAsset    ( AssetDescConstPtr assetDesc )
    : m_assetDesc( assetDesc )
    , m_numWaiting( 0 )
    , m_ready( false )
{}

// ================================ //
//
WaitingAsset::~WaitingAsset()
{
    // @todo It won't work :(
    LoadingCompleted( nullptr );
}

// ================================ //
//
bool            WaitingAsset::WaitUntilLoaded        ()
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
void            WaitingAsset::RequestAsset            ()
{
    m_numWaiting++;
}

// ================================ //
//
bool            WaitingAsset::LoadingCompleted        ( AssetConstPtr asset )
{
    m_lock.lock();

    m_loadedAsset = asset;
    m_ready = true;
    bool noOneWaits = m_numWaiting == 0;

    m_lock.unlock();

    m_condition.notify_all();

    if( noOneWaits )
        return true;
    return false;
}

// ================================ //
//
bool            WaitingAsset::Compare                ( AssetDescConstPtr assetDesc )
{
    if( m_assetDesc->GetUID() == assetDesc->GetUID() )
        return m_assetDesc->GetKey() == assetDesc->GetKey();
    return false;
}


//====================================================================================//
//                LoadBarrier
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
std::pair< WaitingAsset*, bool >        LoadBarrier::RequestAsset        ( AssetDescConstPtr assetDesc )
{
    std::unique_lock< std::mutex > lock( m_lock );

    WaitingAsset* assetWait = nullptr;
    for( auto asset : m_waitingAssets )
    {
        if( asset->Compare( assetDesc ) )
        {
            assert( assetWait == nullptr );        // Should be only one file in waiting vector.
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
AssetConstPtr                           LoadBarrier::WaitUntilLoaded    ( WaitingAsset* asset )
{
    bool isLast = asset->WaitUntilLoaded();
    auto loadedAsset = asset->GetAsset();

    if( isLast )
    {
        std::unique_lock< std::mutex > lock( m_lock );
        bool removed = RemoveWaitingAsset( asset );

        assert( removed );
    }

    return loadedAsset;
}

// ================================ //
//
void                                    LoadBarrier::LoadingCompleted    ( AssetDescConstPtr assetDesc, AssetConstPtr loadedAsset )
{
    std::unique_lock< std::mutex > lock( m_lock );

    WaitingAsset* asset = nullptr;

    for( int i = 0; i < m_waitingAssets.size(); ++i )
    {
        if( m_waitingAssets[ i ]->Compare( assetDesc ) )
        {
            asset = m_waitingAssets[ i ];
            break;
        }
    }

    
    if( asset )
    {
        bool noOneWaits = asset->LoadingCompleted( loadedAsset );

        // Remove if there're no waiting threads
        if( noOneWaits )
            RemoveWaitingAsset( asset );

        // In other cases WaitingAsset will be removed when last thread will leave waiting lock.
    }
}

// ***********************
//
bool                                    LoadBarrier::RemoveWaitingAsset ( WaitingAsset* asset )
{
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

    return removed;
}

}    // sw
