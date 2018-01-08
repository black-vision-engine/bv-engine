#pragma once

#include "CoreDEF.h"

#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"

#include "Engine/Graphics/InputSlots/InputSlots.h"

#include <string>


namespace bv
{

class InputSlots;


// ***********************
/// Class evaluates slot entry based on it's internal state.
/// @ingroup InputSlots
class InputSlotBinding
{
public:

    enum class Type
    {
        ByName,
        ByIndex
    };

private:

    Type            m_bindType;

    SlotIndex       m_slotIdx;
    std::string     m_name;

public:

                InputSlotBinding    ( const std::string & name );
    explicit    InputSlotBinding    ( SlotIndex idx );


    Type                    GetType     () const { return m_bindType; }
    const std::string &     GetName     () const { return m_name; }
    SlotIndex               GetIndex    () const { return m_slotIdx; }

    void                                    Serialize   ( ISerializer & ser ) const;
    static Expected< InputSlotBinding >     Create      ( const IDeserializer & deser );

public:

    Expected< InputSlot >   EvaluateInputSlot           ( InputSlots * slots ) const;
    void                    AddReference                ( InputSlots * slots ) const;
    void                    ReleaseReference            ( InputSlots * slots ) const;
};


} // bv


