#include "stdafx.h"

#include "AssetTracker.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Audio/AudioRenderer.h"

#include "Engine/Editors/BVProjectEditor.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Engine/Graphics/Resources/Textures/Texture2DCache.h"
#include "Assets/Cache/RawDataCache.h"

#include "Engine/Events/Events.h"
#include "Engine/Events/Interfaces/IEventManager.h"


namespace bv {


// *************************************
//
                        AssetTracker::AssetTracker          ( Renderer * renderer, audio::AudioRenderer * audioRenderer, BVProjectEditor * projectEditor )
    : m_renderer( renderer )
    , m_audioRenderer( audioRenderer )
    , m_projectEditor( projectEditor )
{
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &AssetTracker::ProcessEvent ), AssetTrackerInternalEvent::Type() );
}

// *************************************
//
                        AssetTracker::~AssetTracker         ()
{
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &AssetTracker::ProcessEvent ), AssetTrackerInternalEvent::Type() );
}

// *************************************
//
void                    AssetTracker::ClearCache            ()
{
    for( auto it = m_uidsRegisterOrder.cbegin(); it != m_uidsRegisterOrder.cend(); )
    {
        auto uid = *it;
        auto count = m_registeredUIDs[ uid ];
        if( count == 0 )
        {
            GTexture2DCache.ClearAsset( uid );
            m_registeredUIDs.erase( uid );
            it = m_uidsRegisterOrder.erase( it );
        }
        else
        {
            ++it;
        }
    }

    // assets needs to be erased in the reversed order that was registered (reason: FontAssets stores TextureAsset)
    for( auto rit = m_keysRegisterOrder.crbegin(); rit != m_keysRegisterOrder.crend(); )
    {
        auto key = *rit;
        auto keyHash = Hash::FromString( key );

        //FIXME: unregister assets and check count instead of pointer count
        auto asset = AssetManager::GetInstance().GetFromCache( key );
        auto memChunk = RawDataCache::GetInstance().Get( keyHash );
        if( ( asset && ( asset.use_count() == 2 ) ) ||
            ( memChunk && ( memChunk.use_count() == 2 ) ) )
        {
            AssetManager::GetInstance().RemoveFromCache( key );
            RawDataCache::GetInstance().Remove( keyHash );

            m_registeredKeys.erase( key );
            rit = std::vector< AssetKey >::reverse_iterator( m_keysRegisterOrder.erase( rit.base() - 1 ) );
        }
        else
        {
            ++rit;
        }
    }
}

// *************************************
//
std::vector< AssetTracker::AssetUID >   AssetTracker::GetUnusedAssetUIDs    () const
{
    std::vector< AssetUID > ret;

    for( auto & uid : m_registeredUIDs )
    {
        if( uid.second == 0 )
        {
            ret.push_back( uid.first );
        }
    }

    return ret;
}

// *************************************
//
void                                    AssetTracker::ProcessEvent          ( IEventPtr evt )
{
    if( evt->GetEventType() == AssetTrackerInternalEvent::Type() )
    {
        auto typedEvent = std::static_pointer_cast< AssetTrackerInternalEvent >( evt );
        switch( typedEvent->EventCommand )
        {
        case AssetTrackerInternalEvent::Command::RegisterAsset:
            {
                if( typedEvent->HasUID() )
                {
                    RegisterAsset( typedEvent->AssetUID );
                }
                else if( typedEvent->HasKey() )
                {
                    RegisterAsset( typedEvent->AssetKey );
                }
            }
            break;

        case AssetTrackerInternalEvent::Command::UnregisterAsset:
            {
                if( typedEvent->HasUID() )
                {
                    UnregisterAsset( typedEvent->AssetUID );
                }
                else if( typedEvent->HasKey() )
                {
                    UnregisterAsset( typedEvent->AssetKey );
                }
            }
            break;

        case AssetTrackerInternalEvent::Command::PlayAudio:
            {
                auto audio = GetAudio( typedEvent->PluginOwner );
                if( audio )
                {
                    m_audioRenderer->Play( audio );
                }
            }
            break;


        case AssetTrackerInternalEvent::Command::PauseAudio:
            {
                auto audio = GetAudio( typedEvent->PluginOwner );
                if( audio )
                {
                    m_audioRenderer->Pause( audio );
                }
            }
            break;

        case AssetTrackerInternalEvent::Command::StopAudio:
            {
                auto audio = GetAudio( typedEvent->PluginOwner );
                if( audio )
                {
                    m_audioRenderer->Stop( audio );
                }
            }
            break;
            
        case AssetTrackerInternalEvent::Command::EOFAudio:
            {
                auto audio = GetAudio( typedEvent->PluginOwner );
                if( audio )
                {
                    m_audioRenderer->EndOfFile( audio );
                }
            }
            break;

        case AssetTrackerInternalEvent::Command::ReleaseAudioResource:
            {
                auto audio = GetAudio( typedEvent->SceneNodeOwner );
                if( audio )
                {
                    m_audioRenderer->DeletePDR( audio );
                }
            }
            break;

        }
    }
}

// *************************************
//
void                    AssetTracker::RegisterAsset                     ( AssetUID uid )
{
    RegisterAsset( m_registeredUIDs, uid, m_uidsRegisterOrder );
}

// *************************************
//
void                    AssetTracker::RegisterAsset                     ( AssetKey key )
{
    RegisterAsset( m_registeredKeys, key, m_keysRegisterOrder );
}

// *************************************
//
void                    AssetTracker::UnregisterAsset                   ( AssetUID uid )
{
    UnregisterAsset( m_registeredUIDs, uid );
}

// *************************************
//
void                    AssetTracker::UnregisterAsset                   ( AssetKey key )
{
    UnregisterAsset( m_registeredKeys, key );
}

// *************************************
//
audio::AudioEntity *    AssetTracker::GetAudio              ( const model::IPlugin * plugin )
{
    audio::AudioEntity * audio = nullptr;

    auto modelNode = m_projectEditor->FindModelNodeByPlugin( plugin );
    if( modelNode )
    {
        auto sceneNode = m_projectEditor->GetEngineNode( modelNode );
        audio = sceneNode->GetAudio();
    }

    return audio;
}

// *************************************
//
audio::AudioEntity *    AssetTracker::GetAudio              ( const SceneNode * sceneNode )
{
    audio::AudioEntity * audio = nullptr;

    if( sceneNode )
    {
        audio = sceneNode->GetAudio();
    }

    return audio;
}

} //bv
