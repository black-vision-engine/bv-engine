#include "stdafx.h"

#include "AssetTracker.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Audio/AudioRenderer.h"

#include "Engine/Models/BVProjectEditor.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Engine/Graphics/Resources/Textures/Texture2DCache.h"

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
    for( auto it = m_registeredUIDs.cbegin(); it != m_registeredUIDs.cend(); )
    {
        if( it->second == 0 )
        {
            GTexture2DCache.ClearAsset( it->first );
            m_registeredUIDs.erase( it++ );
        }
        else
        {
            ++it;
        }
    }

    for( auto it = m_registeredKeys.cbegin(); it != m_registeredKeys.cend(); )
    {
        auto key = it->first;

        //FIXME: unregister assets and check count instead of pointer count
        auto asset = AssetManager::GetInstance().GetFromCache( key );
        if( asset && ( asset.use_count() == 2 ) )
        {
            AssetManager::GetInstance().RemoveFromCache( key );
            m_registeredKeys.erase( it++ );
        }
        else
        {
            ++it;
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
                if( typedEvent->TextureDesc )
                {
                    RegisterAsset( typedEvent->TextureDesc );
                }
                else if( typedEvent->AssetDesc )
                {
                    RegisterAsset( typedEvent->AssetDesc );
                }
            }
            break;

        case AssetTrackerInternalEvent::Command::UnregisterAsset:
            {
                if( typedEvent->TextureDesc )
                {
                    UnregisterAsset( typedEvent->TextureDesc );
                }
                else if( typedEvent->AssetDesc )
                {
                    UnregisterAsset( typedEvent->AssetDesc );
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
void                    AssetTracker::RegisterAsset                     ( ITextureDescriptorConstPtr & texDesc )
{
    RegisterAsset( m_registeredUIDs, texDesc->GetUID() );
}

// *************************************
//
void                    AssetTracker::RegisterAsset                     ( AssetDescConstPtr & assetDesc )
{
    RegisterAsset( m_registeredKeys, assetDesc->GetKey() );
}

// *************************************
//
void                    AssetTracker::UnregisterAsset                   ( ITextureDescriptorConstPtr & texDesc )
{
    UnregisterAsset( m_registeredUIDs, texDesc->GetUID() );
}

// *************************************
//
void                    AssetTracker::UnregisterAsset                   ( AssetDescConstPtr & assetDesc )
{
    UnregisterAsset( m_registeredKeys, assetDesc->GetKey() );
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
