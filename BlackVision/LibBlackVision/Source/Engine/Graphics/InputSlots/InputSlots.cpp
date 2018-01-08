#include "stdafx.h"
#include "InputSlots.h"

#include "Assets/Input/TextureInputAsset.h"
#include "Assets/Input/TextureInputAssetDesc.h"



namespace bv
{

const SlotIndex       InputSlots::sInvalidIdx = std::numeric_limits< SlotIndex >::max();


// ***********************
//
InputSlots::InputSlots()
{}

// ***********************
//
Expected< SlotIndex >       InputSlots::RegisterSource        ( InputSlot inputSlot, const std::string & name )
{
    std::lock_guard< std::recursive_mutex > guard( m_lock );

    if( !CanAddSource( inputSlot, name ) )
        return "Can't add source. Name already exists or texture is nullptr";

    InputEntry entry( inputSlot );
    entry.Descriptor.SlotName = name;

    return AddSource( entry );
}

// ***********************
//
bool                        InputSlots::UnregisterSource      ( SlotIndex slotIdx )
{
    std::lock_guard< std::recursive_mutex > guard( m_lock );

    if( IsValidIndex( slotIdx ) )
    {
        if( m_slots[ slotIdx ].References > 0 )
        {
            // FIXME: Error handling
        }

        m_slots[ slotIdx ].Slot.Texture = nullptr;
        m_slots[ slotIdx ].Slot.Audio = nullptr;
        m_slots[ slotIdx ].Descriptor.SlotName = "";

        return true;
    }

    return false;
}

// ***********************
//
bool                        InputSlots::UnregisterSource      ( const std::string & name )
{
    std::lock_guard< std::recursive_mutex > guard( m_lock );

    return UnregisterSource( FindSourceByName( name ) );
}

// ***********************
//
Expected< InputSlot >       InputSlots::AccessSource          ( const std::string & name )
{
    std::lock_guard< std::recursive_mutex > guard( m_lock );

    return AccessSource( FindSourceByName( name ) );
}

// ***********************
//
void                        InputSlots::ReferenceSource       ( const std::string & name )
{
    std::lock_guard< std::recursive_mutex > guard( m_lock );

    ReferenceSource( FindSourceByName( name ) );
}

// ***********************
//
void                        InputSlots::ReferenceSource       ( SlotIndex slotIdx )
{
    std::lock_guard< std::recursive_mutex > guard( m_lock );

    if( !IsValidIndex( slotIdx ) )
        return;

    m_slots[ slotIdx ].References++;
}


// ***********************
//
void                        InputSlots::ReleaseSource         ( const std::string & name )
{
    std::lock_guard< std::recursive_mutex > guard( m_lock );

    ReleaseSource( FindSourceByName( name ) );
}

// ***********************
//
Expected< InputSlot >       InputSlots::AccessSource          ( SlotIndex slotIdx )
{
    std::lock_guard< std::recursive_mutex > guard( m_lock );

    if( !IsValidIndex( slotIdx ) )
        return Expected< InputSlot >();

    return m_slots[ slotIdx ].Slot;
}

// ***********************
//
void                        InputSlots::ReleaseSource         ( SlotIndex slotIdx )
{
    std::lock_guard< std::recursive_mutex > guard( m_lock );

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
        if( !IsEmptySlot( idx ) && m_slots[ idx ].Descriptor.SlotName == name )
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
Expected< SlotIndex >       InputSlots::AddSource             ( const InputEntry & entry )
{
    SlotIndex emptySlot = FindEmptySlot();
    if( emptySlot == sInvalidIdx )
    {
        m_slots.push_back( entry );
        return m_slots.size() - 1;
    }
    else
    {
        m_slots[ emptySlot ] = entry;
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
    if( !m_slots[ idx ].Slot.Texture && !m_slots[ idx ].Slot.Audio )
        return true;
    return false;
}

// ***********************
//

bool                InputSlots::CanAddSource          ( InputSlot inputSlot, const std::string & name ) const
{
    if( !inputSlot.Texture && !inputSlot.Audio )
        return false;

    auto idx = FindSourceByName( name );
    if( idx != sInvalidIdx )
        return false;

    return true;
}

// ***********************
/// This function creates TextureInputAsset. We need to do this under lock, thats why this code is here.
/// @Note: We must provide shared_ptr to InputSlots object and I don't won't to use enable_shared_from_this.
TextureInputAssetConstPtr       InputSlots::CreateAsset     ( InputSlotsPtr thisPtr, TextureInputAssetDescConstPtr desc )
{
    std::lock_guard< std::recursive_mutex > guard( m_lock );

    auto asset = TextureInputAsset::Create( thisPtr, desc->BindingInfo() );
    asset->EvaluateSlot();

    return asset;
}




}	// bv


