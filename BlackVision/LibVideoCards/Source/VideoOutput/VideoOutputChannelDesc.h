#pragma once


#include "CoreDEF.h"

#include "Memory/AVFrame.h"
#include "Serialization/ConversionHelper.h"

#include <string>



namespace bv {
namespace videocards
{

typedef UInt64 VideoOutputID;



/**@brief Descriptor of video input channel on video card.
@ingroup VideoCards*/
class VideoOutputChannelDesc
{
private:

    std::string         m_cardName;
    std::string         m_channelName;

    UInt32              m_videoCardID;
    VideoOutputID       m_videoOutputID;
    AVFrameDescriptor   m_dataDesc;

public:

    explicit    VideoOutputChannelDesc      ( UInt32 cardId, VideoOutputID outputID, const std::string & cardName, const std::string & channelName, const AVFrameDescriptor & dataDesc );

    UInt32                  GetCardID       () const { return m_videoCardID; }
    VideoOutputID           GetOutputID     () const { return m_videoOutputID; }

    const std::string &     GetCardName     () const { return m_cardName; }
    const std::string &     GetChannelName  () const { return m_channelName; }

    const AVFrameDescriptor &   GetDataDesc () const { return m_dataDesc; }

};


// ========================================================================= //
// Implementation
// ========================================================================= //

// ***********************
//
inline VideoOutputChannelDesc::VideoOutputChannelDesc    ( UInt32 cardId, VideoOutputID id, const std::string & cardName, const std::string & channelName, const AVFrameDescriptor & dataDesc )
    : m_cardName( cardName )
    , m_channelName( channelName )
    , m_videoOutputID( id )
    , m_videoCardID( cardId )
    , m_dataDesc( dataDesc )
{}


}   // videocards
}   // bv


