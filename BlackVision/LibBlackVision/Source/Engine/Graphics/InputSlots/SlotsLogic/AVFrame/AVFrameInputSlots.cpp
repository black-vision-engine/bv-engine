#include "stdafx.h"
#include "AVFrameInputSlots.h"






namespace bv
{

// ***********************
//
AVFrameInputSlots::AVFrameInputSlots    ( InputSlotsPtr slots )
    :   m_inputSlots( slots )
{}

// ***********************
//
void        AVFrameInputSlots::UpdateSlot       ( SlotIndex slotIdx, AVFramePtr frame )
{
    slotIdx;
    frame;
    assert( !"Implment me" );
}


}	// bv

