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
void                    AssetTracker::RegisterAsset         ( ITextureDescriptorConstPtr asset )
{
    //FIXME: register only cached textures
    auto tex = GTexture2DCache.GetTexture( asset.get() );
    if( GTexture2DCache.IsRegistered( asset.get() ) )
    {
        RegisterAsset( tex );
    }
}

// *************************************
//
void                    AssetTracker::UnregisterAsset     ( ITextureDescriptorConstPtr asset )
{
    if( GTexture2DCache.IsRegistered( asset.get() ) )
    {
        auto tex = GTexture2DCache.GetTexture( asset.get() );
        UnregisterAsset( tex );
    }
}

// *************************************
//
std::vector< TextureConstPtr >      AssetTracker::GetUnusedAssets       ()
{
    std::vector< TextureConstPtr > ret( m_unregisteredAssets.begin(), m_unregisteredAssets.end() );
    m_unregisteredAssets.clear();
    return ret;
}

// *************************************
//
void                                AssetTracker::ProcessEvent          ( IEventPtr evt )
{
    if( evt->GetEventType() == AssetTrackerInternalEvent::Type() )
    {
        auto typedEvent = std::static_pointer_cast< AssetTrackerInternalEvent >( evt );
        switch( typedEvent->EventCommand )
        {
        case AssetTrackerInternalEvent::Command::RegisterAsset:
            {
                RegisterAsset( typedEvent->TextureAsset );
            }
            break;

        case AssetTrackerInternalEvent::Command::UnregisterAsset:
            {
                UnregisterAsset( typedEvent->TextureAsset );
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
void                    AssetTracker::RegisterAsset         ( TextureConstPtr asset )
{
    if( m_registeredAssetsMap.count( asset ) == 0 )
    {
        m_registeredAssetsMap[ asset ] = 0;
    }

    if( m_unregisteredAssets.count( asset ) )
    {
        m_unregisteredAssets.erase( asset );
    }

    m_registeredAssetsMap[ asset ] += 1;
}

// *************************************
//
void                    AssetTracker::UnregisterAsset       ( TextureConstPtr asset )
{
    if( m_registeredAssetsMap.empty() || m_registeredAssetsMap.count( asset ) == 0 )
    {
        return;
    }

    m_registeredAssetsMap[ asset ] -= 1;

    if( m_registeredAssetsMap[ asset ] == 0 )
    {
        m_registeredAssetsMap.erase( asset );
        m_unregisteredAssets.insert( asset );
    }
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
