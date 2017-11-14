#pragma once


#include "CoreDEF.h"

#include <string>



namespace bv {
namespace videocards
{

typedef UInt32 VideoInputID;


// ***********************
//
class VideoInputSlotDesc
{
private:

    std::string         m_cardName;
    std::string         m_channelName;

    VideoInputID        m_videoInputID;

public:

    VideoInputID        GetInputID      () const { return m_videoInputID; }
};




}   // videocards
}   // bv

