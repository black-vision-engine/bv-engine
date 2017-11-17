#pragma once


#include "CoreDEF.h"

#include "Memory/AVFrame.h"

#include <string>



namespace bv {
namespace videocards
{

typedef UInt32 VideoInputID;
typedef UInt32 VideoCardID;


/**@brief Descriptor of video input channel on video card.
@ingroup VideoCards*/
class VideoInputChannelDesc
{
private:

    std::string         m_cardName;
    std::string         m_channelName;

    VideoCardID         m_videoCardID;
    VideoInputID        m_videoInputID;
    AVFrameDescriptor   m_dataDesc;

public:

    explicit    VideoInputChannelDesc       ( VideoCardID cardId, VideoInputID inputID, const std::string & cardName, const std::string & channelName, const AVFrameDescriptor & dataDesc );

    VideoCardID             GetCardID       () const { return m_videoCardID; }
    VideoInputID            GetInputID      () const { return m_videoInputID; }

    const std::string &     GetCardName     () const { return m_cardName; }
    const std::string &     GetChannelName  () const { return m_channelName; }
    
    const AVFrameDescriptor &   GetDataDesc () const { return m_dataDesc; }

public:


    /**@brief Checks if objects describe the same video card channel.*/
    bool                    IsEquivalent    ( const VideoInputChannelDesc & other ) const;

};


// ========================================================================= //
// Implementation
// ========================================================================= //

// ***********************
//
inline VideoInputChannelDesc::VideoInputChannelDesc    ( VideoCardID cardId, VideoInputID id, const std::string & cardName, const std::string & channelName, const AVFrameDescriptor & dataDesc )
    : m_cardName( cardName )
    , m_channelName( channelName )
    , m_videoInputID( id )
    , m_videoCardID( cardId )
    , m_dataDesc( dataDesc )
{}

// ***********************
//
inline bool             VideoInputChannelDesc::IsEquivalent     ( const VideoInputChannelDesc & other ) const
{
    if( m_videoInputID != other.m_videoInputID )
        return false;

    if( m_cardName != other.m_cardName )
        return false;

    if( m_channelName != other.m_channelName )
        return false;

    return true;
}


}   // videocards
}   // bv


