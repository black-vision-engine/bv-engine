#include "stdafx.h"
#include "InputSlots.h"

#include "Assets/Input/TextureInputAsset.h"
#include "Assets/Input/TextureInputAssetDesc.h"

#include "Engine/Graphics/Resources/Textures/Texture2D.h"

#include "Engine/Events/EventManager.h"

#include "Engine/Events/InnerEvents/InputSlots/SlotRemovedEvent.h"
#include "Engine/Events/InnerEvents/InputSlots/FirstSlotRefEvent.h"
#include "Engine/Events/InnerEvents/InputSlots/SlotAddedEvent.h"
#include "Engine/Events/InnerEvents/InputSlots/AllSlotRefsRemovedEvent.h"


namespace bv
{

const SlotIndex       InputSlots::sInvalidIdx = std::numeric_limits< SlotIndex >::max();


// ***********************
//
InputSlots::InputSlots()
    :   m_fallbackSlot( CreateFallbackTexture(), CreateFallbackAudio() )
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

    // Send event on first reference to this slot.
    if( m_slots[ slotIdx ].References == 1 )
    {
        GetDefaultEventManager().ConcurrentQueueEvent( std::make_shared< FirstSlotRefEvent >( slotIdx, m_slots[ slotIdx ].Descriptor.SlotName ) );
    }
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

    // Send event that slot is unused.
    if( m_slots[ slotIdx ].References == 0 )
    {
        GetDefaultEventManager().ConcurrentQueueEvent( std::make_shared< AllSlotRefsRemovedEvent >( slotIdx, m_slots[ slotIdx ].Descriptor.SlotName ) );
    }

    if( m_slots[ slotIdx ].References < 0 )
    {
        LOG_MESSAGE( SeverityLevel::error ) << "Slot [" << m_slots[ slotIdx ].Descriptor.SlotName << ", index [" << Convert::T2String( slotIdx ) << "] - negative number of references.";
    }
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
//
Texture2DPtr                    InputSlots::CreateFallbackTexture   ()
{
    auto width = 8;
    auto height = 8;
    TextureFormat format = TextureFormat::F_A8R8G8B8;
    DataBuffer::Semantic semantic = DataBuffer::Semantic::S_TEXTURE_STATIC;
    UInt32 levels = 1;

    auto tex = std::make_shared< Texture2D >( format, width, height, semantic, levels );
    tex->SetData( GenerateFallbackTexture( width, height, 4 ) );

    return tex;
}

// ***********************
//
audio::AudioEntity *            InputSlots::CreateFallbackAudio     ()
{
    return nullptr;
}

// ***********************
//
MemoryChunkPtr                  InputSlots::GenerateFallbackTexture ( UInt32 width, UInt32 height, UInt32 bpp )
{
    auto chunk = MemoryChunk::Create( width * height * bpp );
    
    // Fully transparent and empty texture.
    chunk->Clear();

    return chunk;
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


