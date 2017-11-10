#pragma once


#include "InputSlot.h"

#include <vector>


namespace bv
{

typedef SizeType SlotIndex;


// ***********************
//
class InputSlots
{
private:

    static const SlotIndex       sInvalidIdx;

private:

    std::vector< InputEntry >       m_slots;

public:

    explicit    InputSlots    ();


    Expected< SlotIndex >   RegisterSource      ( InputSlot inputSlot, const std::string & name );
    bool                    UnregisterSource    ( SlotIndex slotIdx );
    bool                    UnregisterSource    ( const std::string & name );

    InputSlot               AccessSource        ( const std::string & name );
    void                    ReleaseSource       ( const std::string & name );

    InputSlot               AccessSource        ( SlotIndex slotIdx );
    void                    ReleaseSource       ( SlotIndex slotIdx );

private:

    SlotIndex               FindSourceByName    ( const std::string & name ) const;
    SlotIndex               FindEmptySlot       () const;
    Expected< SlotIndex >   AddSource           ( const InputEntry & entry );

    bool                IsValidIndex        ( SlotIndex idx ) const;
    bool                IsEmptySlot         ( SlotIndex idx ) const;
    bool                CanAddSource        ( InputSlot inputSlot, const std::string & name ) const;
};

DEFINE_PTR_TYPE( InputSlots )


}	// bv


