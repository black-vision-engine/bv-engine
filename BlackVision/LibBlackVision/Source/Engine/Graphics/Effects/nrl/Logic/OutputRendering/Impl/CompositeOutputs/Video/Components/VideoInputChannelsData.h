#pragma once

#include <vector>

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/CompositeOutputs/Video/VideoTypes.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/CompositeOutputs/Video/Components/VideoInputChannel.h"
#include "Engine/Graphics/Effects/nrl/Logic/Components/RenderedChannelsData.h"

#include "Engine/Graphics/Resources/Textures/Texture2D.h"


namespace bv { namespace nrl {

// FIXME: nrl - implement (if necessary) additional logic to cache readback textures - or move this logic somewhere else
class VideoInputChannelsData
{
private:

    // Hackish deferred initialization
    OutputStaticDataVec                 m_preUniqueOutputSetups;
    UintUintMapping                     m_preMapping;

    bool                                m_preInitialized;

    // Initialization 
    VideoInputChannelsMap               m_outputToChannelsMapping;
    VideoInputChannelsVec               m_videoInputChannels;
    VideoInpuChannelToOutputMapping     m_channelToOutputMaping;

    const RenderedChannelsData *        m_originalRenderedChannelsData;

    bool                                m_postInitialized;

private:

    //FIXME: use the 'delete' keyword
    VideoInputChannelsData                ( const VideoInputChannelsData & src );
    VideoInputChannelsData &  operator =  ( VideoInputChannelsData & src );


public:

                                VideoInputChannelsData  ();
                                ~VideoInputChannelsData ();

    bool                        IsInitialized           () const;
    
    void                        PreInitialize           ( OutputStaticDataVec & uniqueOutputSetups, const UintUintMapping & mapping );
    void                        PostInitialize          ( const RenderedChannelsData * rcd );

    const VideoInputChannel *   GetInputChannel         ( unsigned int videoOutputID ) const;

    unsigned int                GetNumVideoInputChannels() const;
    const VideoInputChannel *   GetVideoInputChannelAt  ( unsigned int idx ) const;
    const VideoCardIDVec &      GetAsignedVideoCards    ( const VideoInputChannel * vic ) const;

    void                        InvalidateCachedTextures();
    //void                        InvalidateCachedTexture ( RenderChannelType rct );
    //Texture2DPtr                ReadColorTexture        ( NRenderContext * ctx, RenderChannelType rct ) const;
  
public:

    static VideoInputChannelsData *   Create            ( const std::vector< OutputStaticData > & uniqueOutputSetups, const std::hash_map< unsigned int, unsigned int > & mapping );

};

} //nrl
} //bv
