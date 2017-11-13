#pragma once

#include "Interfaces/IVideoInputSlots.h"
#include "InputSlots.h"



namespace bv
{


// ***********************
//
class VideoInputSlots : public IVideoInputSlots
{
private:

    InputSlotsPtr     m_textureSlots;

public:

    explicit        VideoInputSlots ();
    virtual         ~VideoInputSlots();
};



}	// bv


