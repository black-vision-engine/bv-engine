#pragma once


#include "TextureSlot.h"

#include <vector>


namespace bv
{

typedef SizeType SlotIndex;


// ***********************
//
class TextureSlots
{
private:

    static SlotIndex       sInvalidIdx;

private:

    std::vector< TextureSlot >      m_slots;

public:

    explicit    TextureSlots    ();


    SlotIndex           RegisterSource      ( Texture2DPtr tex, const std::string & name );
    void                UnregisterSource    ( SlotIndex slotIdx );
    void                UnregisterSource    ( const std::string & name );

    Texture2DPtr        AccessSource        ( const std::string & name );
    void                ReleaseSource       ( const std::string & name );

    Texture2DPtr        AccessSource        ( SlotIndex slotIdx );
    void                ReleaseSource       ( SlotIndex slotIdx );

private:

    SlotIndex           FindSourceByName    ( const std::string & name ) const;
    SlotIndex           FindEmptySlot       () const;
    SlotIndex           AddSource           ( const TextureSlot & slot );

    bool                IsValidIndex        ( SlotIndex idx ) const;
};


}	// bv


