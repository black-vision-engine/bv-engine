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

    static const SlotIndex       sInvalidIdx;

private:

    std::vector< TextureSlot >      m_slots;

public:

    explicit    TextureSlots    ();


    Expected< SlotIndex >   RegisterSource      ( Texture2DPtr tex, const std::string & name );
    bool                    UnregisterSource    ( SlotIndex slotIdx );
    bool                    UnregisterSource    ( const std::string & name );

    Texture2DPtr            AccessSource        ( const std::string & name );
    void                    ReleaseSource       ( const std::string & name );

    Texture2DPtr            AccessSource        ( SlotIndex slotIdx );
    void                    ReleaseSource       ( SlotIndex slotIdx );

private:

    SlotIndex               FindSourceByName    ( const std::string & name ) const;
    SlotIndex               FindEmptySlot       () const;
    Expected< SlotIndex >   AddSource           ( const TextureSlot & slot );

    bool                IsValidIndex        ( SlotIndex idx ) const;
    bool                CanAddSource        ( Texture2DPtr tex, const std::string & name ) const;
};


}	// bv


