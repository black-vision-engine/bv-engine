#pragma once

#include <vector>

#include "Engine/Graphics/Effects/Logic/OutputRendering/Impl/CompositeOutputs/Video/VideoTypes.h"

#include "Engine/Graphics/Effects/Logic/OutputRendering/Impl/CompositeOutputs/Video/Components/VideoInputChannel.h"
#include "Engine/Graphics/Effects/Logic/Components/RenderedChannelsData.h"

#include "Engine/Graphics/Resources/Textures/Texture2D.h"


namespace bv { 

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
    VideoInputChannel *         AccessInputChannel      ( unsigned int videoOutputID );

    unsigned int                GetNumVideoInputChannels() const;
    const VideoInputChannel *   GetVideoInputChannelAt  ( unsigned int idx ) const;
    const VideoCardIDVec &      GetAsignedVideoCardIds  ( const VideoInputChannel * vic ) const;

    bool                        LastFrameHadAudio       ( unsigned int videoOutputID ) const;
    void                        ToggleLastFrameHadAudio ( unsigned int videoOutputID ) const;

    void                        InvalidateCachedTextures();
    void                        InvalidateCachedTexture ( unsigned int videoOutputID );

    Texture2DPtr                ReadColorTexture        ( RenderContext * ctx, unsigned int videoOutputID ) const;
  
    bool                        IsActive                ( unsigned int videoOutputID ) const;
    bool                        ContainsValidData       ( unsigned int videoOutputID ) const;

};


} //bv
