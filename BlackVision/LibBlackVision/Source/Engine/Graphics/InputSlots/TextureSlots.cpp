#include "stdafx.h"
#include "TextureSlots.h"





namespace bv
{

const SlotIndex       TextureSlots::sInvalidIdx = std::numeric_limits< SlotIndex >::max();


// ***********************
//
TextureSlots::TextureSlots()
{}

// ***********************
//
Expected< SlotIndex >       TextureSlots::RegisterSource        ( Texture2DPtr tex, const std::string & name )
{
    if( !CanAddSource( tex, name ) )
        return "Can't add source. Name already exists or texture is nullptr";

    TextureSlot texSlot;
    texSlot.SlotName = name;
    texSlot.Texture = tex;

    return AddSource( texSlot );
}

// ***********************
//
bool                        TextureSlots::UnregisterSource      ( SlotIndex slotIdx )
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
bool                        TextureSlots::UnregisterSource      ( const std::string & name )
{
    return UnregisterSource( FindSourceByName( name ) );
}

// ***********************
//
Texture2DPtr                TextureSlots::AccessSource          ( const std::string & name )
{
    return AccessSource( FindSourceByName( name ) );
}

// ***********************
//
void                        TextureSlots::ReleaseSource         ( const std::string & name )
{
    ReleaseSource( FindSourceByName( name ) );
}

// ***********************
//
Texture2DPtr                TextureSlots::AccessSource          ( SlotIndex slotIdx )
{
    if( !IsValidIndex( slotIdx ) )
        return nullptr;

    m_slots[ slotIdx ].References++;
    return m_slots[ slotIdx ].Texture;
}

// ***********************
//
void                        TextureSlots::ReleaseSource         ( SlotIndex slotIdx )
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
SlotIndex                   TextureSlots::FindSourceByName      ( const std::string & name ) const
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
SlotIndex                   TextureSlots::FindEmptySlot         () const
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
Expected< SlotIndex >       TextureSlots::AddSource             ( const TextureSlot & slot )
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
bool                TextureSlots::IsValidIndex          ( SlotIndex idx ) const
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

bool                TextureSlots::IsEmptySlot           ( SlotIndex idx ) const
{
    if( !m_slots[ idx ].Texture )
        return true;
    return false;
}

// ***********************
//

bool                TextureSlots::CanAddSource          ( Texture2DPtr tex, const std::string & name ) const
{
    if( !tex )
        return false;

    auto idx = FindSourceByName( name );
    if( idx != sInvalidIdx )
        return false;

    return true;
}




}	// bv


