#pragma once


#include "CoreDEF.h"

#include <string>



namespace bv {
namespace videocards
{

typedef UInt32 VideoInputID;



/**@brief Descriptor of video input channel on video card.
@ingroup VideoCards*/
class VideoInputChannelDesc
{
private:

    std::string         m_cardName;
    std::string         m_channelName;

    VideoInputID        m_videoInputID;

public:

    explicit    VideoInputChannelDesc       ( VideoInputID id, const std::string & cardName, const std::string & channelName );

    VideoInputID            GetInputID      () const { return m_videoInputID; }

    const std::string &     GetCardName     () const { return m_cardName; }
    const std::string &     GetChannelName  () const { return m_channelName; }

};


// ========================================================================= //
// Implementation
// ========================================================================= //

// ***********************
//
inline VideoInputChannelDesc::VideoInputChannelDesc    ( VideoInputID id, const std::string & cardName, const std::string & channelName )
    : m_cardName( cardName )
    , m_channelName( channelName )
    , m_videoInputID( id )
{}



}   // videocards
}   // bv


