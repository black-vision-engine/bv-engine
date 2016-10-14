#pragma once

#include <map>
#include <set>

#include "Engine/Models/Interfaces/ITextureDescriptor.h"
#include "Engine/Graphics/Resources/Textures/Texture.h"

#include "Engine/Events/Events.h"
#include "Engine/Events/EventManager.h"


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
    
    Renderer *                              m_renderer;
    audio::AudioRenderer *                  m_audioRenderer;

    BVProjectEditor *                       m_projectEditor;

    std::map< AssetUID, UInt32 >            m_registeredUIDs;
    std::map< AssetKey, UInt32 >            m_registeredKeys;

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

    template< typename MAP, typename KEY >
    void                                    RegisterAsset       ( MAP & map, KEY key );

    template< typename MAP, typename KEY >
    void                                    UnregisterAsset     ( MAP & map, KEY key );

    audio::AudioEntity *                    GetAudio            ( const model::IPlugin * plugin );
    audio::AudioEntity *                    GetAudio            ( const SceneNode * sceneNode );

};


// *************************************
//
template< typename MAP, typename KEY >
void         AssetTracker::RegisterAsset        ( MAP & map, KEY key )
{
    if( map.count( key ) == 0 )
    {
        map[ key ] = 0;
    }

    map[ key ]++;
}

// *************************************
//
template< typename MAP, typename KEY >
void         AssetTracker::UnregisterAsset      ( MAP & map, KEY key )
{
    if( map.count( key ) > 0 && map[ key ] > 0 )
    {
        map[ key ]--;
    }
}

} //bv
