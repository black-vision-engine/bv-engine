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
                auto modelNode = m_projectEditor->FindModelNodeByPlugin( typedEvent->PluginOwner );
                if( modelNode )
                {
                    auto sceneNode = m_projectEditor->GetEngineNode( modelNode );
                    m_audioRenderer->Pause( sceneNode->GetAudio() );
                }
            }
            break;
        case AssetTrackerInternalEvent::Command::StopAudio:
            {
                auto modelNode = m_projectEditor->FindModelNodeByPlugin( typedEvent->PluginOwner );
                if( modelNode )
                {
                    auto sceneNode = m_projectEditor->GetEngineNode( modelNode );
                    m_audioRenderer->Stop( sceneNode->GetAudio() );
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
    if( m_registeredAssetsMap.empty() || m_registeredAssetsMap.count( asset ) == 0 ) return;

    if( m_registeredAssetsMap[ asset ] <= 2 ) // asset is registered twice (plugin & finalize plugin)
    {
        m_registeredAssetsMap.erase( asset );
        m_unregisteredAssets.insert( asset );
        return;
    }
    
    m_registeredAssetsMap[ asset ] -= 1;
}

} //bv
