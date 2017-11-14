#include "stdafx.h"
#include "VideoInputSlots.h"




namespace bv
{


// ***********************
//
VideoInputSlots::VideoInputSlots    ( InputSlotsPtr slots )
    :   m_avInputSlots( slots )
{}

// ***********************
//
VideoInputSlots::~VideoInputSlots()
{}

// ***********************
//
bool            VideoInputSlots::RegisterVideoInputChannel      ( const videocards::VideoInputChannelDesc & vidInputDesc )
{
    vidInputDesc;
    return false;
}

bool            VideoInputSlots::Exists                         ( videocards::VideoInputID id ) const
{
    id;
    return false;
}


}	// bv


