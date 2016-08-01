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
    class AudioRenderer;
}

class AssetTracker;
DEFINE_UPTR_TYPE(AssetTracker)

//FIXME: AssetTracker tracks only textures (engine). There is no need to track anything more for now, but it should be extended to all kind of assets.
class AssetTracker
{

private:
    
    Renderer *                              m_renderer;
    audio::AudioRenderer *                  m_audioRenderer;

    BVProjectEditor *                       m_projectEditor;

    std::map< TextureConstPtr, UInt32 >     m_registeredAssetsMap;
    std::set< TextureConstPtr >             m_unregisteredAssets;

public:

                                            AssetTracker        ( Renderer * renderer, audio::AudioRenderer * audioRenderer, BVProjectEditor * projectEditor );
    
    void                                    RegisterAsset       ( ITextureDescriptorConstPtr asset );
    void                                    UnregisterAsset     ( ITextureDescriptorConstPtr asset );

    std::vector< TextureConstPtr >          GetUnusedAssets     ();

    void                                    ProcessEvent        ( IEventPtr evt );

private:
    
    void                                    RegisterAsset       ( TextureConstPtr asset );
    void                                    UnregisterAsset     ( TextureConstPtr asset );

};


} //bv
