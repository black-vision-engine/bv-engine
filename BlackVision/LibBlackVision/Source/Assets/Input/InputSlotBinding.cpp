#include "stdafx.h"
#include "InputSlotBinding.h"

#include "Serialization/DeserializeContext.h"
#include "Exceptions/Serialization/SerializationException.h"


namespace bv
{


// ***********************
//
InputSlotBinding::InputSlotBinding      ( const std::string & name )
    : m_bindType( Type::ByName )
    , m_name( name )
    , m_slotIdx( 0 )
{}

// ***********************
//
InputSlotBinding::InputSlotBinding      ( SlotIndex idx )
    : m_bindType( Type::ByIndex )
    , m_slotIdx( idx )
{}

// ***********************
//
void                             InputSlotBinding::Serialize     ( ISerializer & ser ) const
{
    ser.EnterChild( "slotBinding" );

    if( m_bindType == Type::ByIndex )
    {
        ser.SetAttribute( "type", "ByIndex" );
        ser.SetAttribute( "index", Convert::T2String( m_slotIdx ) );
    }
    else
    {
        ser.SetAttribute( "type", "ByName" );
        ser.SetAttribute( "name", m_name );
    }

    ser.ExitChild();    // SlotBinding
}

// ***********************
//
Expected< InputSlotBinding >    InputSlotBinding::Create        ( const IDeserializer & deser )
{
    if( deser.EnterChild( "slotBinding" ) )
    {
        auto type = deser.GetAttribute( "type" );
        if( type == "ByIndex" )
        {
            auto expectedSlotIdx = Convert::String2T< SlotIndex >( deser.GetAttribute( "index" ) );
            
            if( expectedSlotIdx.IsValid() )
                return InputSlotBinding( expectedSlotIdx.GetVal() );
            else
                Warn< SerializationException >( deser, "Expected valid slot index." );
        }
        else if( type == "ByName" )
        {
            if( deser.HasAttribute( "name" ) )
                return InputSlotBinding( deser.GetAttribute( "name" ) );
            else
                Warn< SerializationException >( deser, "Expected valid slot name" );
        }
        else
            Warn< SerializationException >( deser, "Invalid InputSlotBinding type." );
    }
    else
    {
        Warn< SerializationException >( deser, "There's no SlotBinding entry." );
    }

    return Expected< InputSlotBinding >();
}

// ***********************
//
Expected< InputSlot >           InputSlotBinding::EvaluateInputSlot         ( InputSlots * slots ) const
{
    if( m_bindType == Type::ByIndex )
        return slots->AccessSource( m_slotIdx );
    else
        return slots->AccessSource( m_name );
}

// ***********************
//
void                            InputSlotBinding::AddReference              ( InputSlots * slots ) const
{
    if( m_bindType == Type::ByIndex )
        return slots->ReferenceSource( m_slotIdx );
    else
        return slots->ReferenceSource( m_name );
}

// ***********************
//
void                            InputSlotBinding::ReleaseReference          ( InputSlots * slots ) const
{
    if( m_bindType == Type::ByIndex )
        return slots->ReleaseSource( m_slotIdx );
    else
        return slots->ReleaseSource( m_name );
}




}	// bv



