#include "stdafx.h"
#include "InputSlots.h"





namespace bv
{

const SlotIndex       InputSlots::sInvalidIdx = std::numeric_limits< SlotIndex >::max();


// ***********************
//
InputSlots::InputSlots()
{}

// ***********************
//
Expected< SlotIndex >       InputSlots::RegisterSource        ( Texture2DPtr tex, const std::string & name )
{
    if( !CanAddSource( tex, name ) )
        return "Can't add source. Name already exists or texture is nullptr";

    InputSlot texSlot;
    texSlot.SlotName = name;
    texSlot.Texture = tex;

    return AddSource( texSlot );
}

// ***********************
//
bool                        InputSlots::UnregisterSource      ( SlotIndex slotIdx )
{
    if( IsValidIndex( slotIdx ) )
    {
        if( m_slots[ slotIdx ].References > 0 )
        {
            // FIXME: Error handling
        }

        m_slots[ slotIdx ].Texture = nullptr;
        m_slots[ slotIdx ].SlotName = "";

        return true;
    }

    return false;
}

// ***********************
//
bool                        InputSlots::UnregisterSource      ( const std::string & name )
{
    return UnregisterSource( FindSourceByName( name ) );
}

// ***********************
//
Texture2DPtr                InputSlots::AccessSource          ( const std::string & name )
{
    return AccessSource( FindSourceByName( name ) );
}

// ***********************
//
void                        InputSlots::ReleaseSource         ( const std::string & name )
{
    ReleaseSource( FindSourceByName( name ) );
}

// ***********************
//
Texture2DPtr                InputSlots::AccessSource          ( SlotIndex slotIdx )
{
    if( !IsValidIndex( slotIdx ) )
        return nullptr;

    m_slots[ slotIdx ].References++;
    return m_slots[ slotIdx ].Texture;
}

// ***********************
//
void                        InputSlots::ReleaseSource         ( SlotIndex slotIdx )
{
    if( !IsValidIndex( slotIdx ) )
        return ;

    m_slots[ slotIdx ].References--;
}


// ========================================================================= //
// Internal implementations
// ========================================================================= //



// ***********************
//
SlotIndex                   InputSlots::FindSourceByName      ( const std::string & name ) const
{
    for( SlotIndex idx = 0; idx < m_slots.size(); ++idx )
    {
        if( !IsEmptySlot( idx ) && m_slots[ idx ].SlotName == name )
            return idx;
    }

    return sInvalidIdx;
}

// ***********************
//
SlotIndex                   InputSlots::FindEmptySlot         () const
{
    for( SlotIndex idx = 0; idx < m_slots.size(); idx++ )
    {
        if( IsEmptySlot( idx ) )
            return idx;
    }

    return sInvalidIdx;
}

// ***********************
//
Expected< SlotIndex >       InputSlots::AddSource             ( const InputSlot & slot )
{
    SlotIndex emptySlot = FindEmptySlot();
    if( emptySlot == sInvalidIdx )
    {
        m_slots.push_back( slot );
        return m_slots.size() - 1;
    }
    else
    {
        m_slots[ emptySlot ] = slot;
        return emptySlot;
    }
}

// ***********************
//  
bool                InputSlots::IsValidIndex          ( SlotIndex idx ) const
{
    if( idx == sInvalidIdx )
        return false;

    if( idx >= m_slots.size() )
        return false;

    if( IsEmptySlot( idx ) )
        return false;

    return true;
}

// ***********************
//

bool                InputSlots::IsEmptySlot           ( SlotIndex idx ) const
{
    if( !m_slots[ idx ].Texture )
        return true;
    return false;
}

// ***********************
//

bool                InputSlots::CanAddSource          ( Texture2DPtr tex, const std::string & name ) const
{
    if( !tex )
        return false;

    auto idx = FindSourceByName( name );
    if( idx != sInvalidIdx )
        return false;

    return true;
}




}	// bv


