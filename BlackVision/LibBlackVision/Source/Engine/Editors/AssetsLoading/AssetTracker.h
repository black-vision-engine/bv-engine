#pragma once

#include <map>
#include <set>

#include "Engine/Models/Interfaces/ITextureDescriptor.h"
#include "Engine/Graphics/Resources/Textures/Texture.h"

#include "Engine/Events/Events.h"
#include "Engine/Events/EventManager.h"

#include <mutex>



namespace bv { 

class Renderer;
namespace audio {
    class AudioEntity;
    class AudioRenderer;
}

class AssetTracker;
DEFINE_UPTR_TYPE(AssetTracker)

//FIXME: AssetTracker tracks only textures (engine). There is no need to track anything more for now, but it should be extended to all kind of assets.
class AssetTracker
{
private:

    typedef ITextureDescriptor::uid_t       AssetUID;
    typedef std::string                     AssetKey;

private:
    
    mutable std::recursive_mutex            m_lock;

    Renderer *                              m_renderer;
    audio::AudioRenderer *                  m_audioRenderer;

    BVProjectEditor *                       m_projectEditor;

    std::map< AssetUID, UInt32 >            m_registeredUIDs;
    std::map< AssetKey, UInt32 >            m_registeredKeys;

    // assets needs to be erased in the reversed order that was registered (reason: FontAssets stores TextureAsset)
    std::vector< AssetUID >                 m_uidsRegisterOrder;
    std::vector< AssetKey >                 m_keysRegisterOrder;

public:

                                            AssetTracker        ( Renderer * renderer, audio::AudioRenderer * audioRenderer, BVProjectEditor * projectEditor );
                                            ~AssetTracker       ();

    void                                    RegisterAsset       ( AssetUID uid );
    void                                    UnregisterAsset     ( AssetUID uid );
    
    void                                    RegisterAsset       ( AssetKey key );
    void                                    UnregisterAsset     ( AssetKey key );

    void                                    ClearCache          ();

    std::vector< AssetUID >                 GetUnusedAssetUIDs  () const;

    void                                    ProcessEvent        ( IEventPtr evt );

private:

    template< typename KEYTYPE >
    void                                    RegisterAsset       ( std::map< KEYTYPE, UInt32 > & keysMap, KEYTYPE key, std::vector< KEYTYPE > & order );

    template< typename KEYTYPE >
    void                                    UnregisterAsset     ( std::map< KEYTYPE, UInt32 > & keysMap, KEYTYPE key );

    audio::AudioEntity *                    GetAudio            ( const model::IPlugin * plugin );
    audio::AudioEntity *                    GetAudio            ( const SceneNode * sceneNode );

};


// *************************************
//
template< typename KEYTYPE >
void         AssetTracker::RegisterAsset        ( std::map< KEYTYPE, UInt32 > & keysMap, KEYTYPE key, std::vector< KEYTYPE > & order )
{
    if( keysMap.count( key ) == 0 )
    {
        keysMap[ key ] = 0;
        order.push_back( key );
    }

    keysMap[ key ]++;
}

// *************************************
//
template< typename KEYTYPE >
void         AssetTracker::UnregisterAsset      ( std::map< KEYTYPE, UInt32 > & keysMap, KEYTYPE key )
{
    if( keysMap.count( key ) > 0 && keysMap[ key ] > 0 )
    {
        keysMap[ key ]--;
    }
}

} //bv
