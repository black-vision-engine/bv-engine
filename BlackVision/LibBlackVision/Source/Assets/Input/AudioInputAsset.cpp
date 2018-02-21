#include "stdafx.h"
#include "AudioInputAsset.h"

#include "Engine/Events/EventManager.h"

#include "Application/ApplicationContext.h"


namespace bv
{

const std::string       AudioInputAsset::uid = "AUDIO_INPUT_ASSET";



// ========================================================================= //
// Creation
// ========================================================================= //

// ***********************
//
AudioInputAsset::AudioInputAsset        ( InputSlotsPtr slots, InputSlotBinding binding )
    : m_numReferences( 0 )
    , m_slots( slots )
    , m_binding( binding )
{
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &AudioInputAsset::HandleEvents ), SlotAddedEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &AudioInputAsset::HandleEvents ), SlotRemovedEvent::Type() );
}

// ***********************
//
AudioInputAsset::~AudioInputAsset()
{
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &AudioInputAsset::HandleEvents ), SlotAddedEvent::Type() );
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &AudioInputAsset::HandleEvents ), SlotRemovedEvent::Type() );
}

// ***********************
//
AudioInputAssetPtr            AudioInputAsset::Create       ( InputSlotsPtr slots, InputSlotBinding binding )
{
    return AudioInputAssetPtr( new AudioInputAsset( slots, binding ) );
}

// ***********************
//
void                            AudioInputAsset::EvaluateSlot()
{
    auto inputSlot = m_binding.EvaluateInputSlot( m_slots.get() );
    if( inputSlot.IsValid() )
    {
        m_audio = inputSlot.GetVal().Audio;
    }
    else
    {
        m_audio = m_slots->GetFallbackSlot().Audio;
    }
}

// ***********************
//
void                        AudioInputAsset::OnSlotAdded      ( SlotAddedEventPtr evt )
{
    if( ShouldReevaluate( evt.get() ) )
    {
        EvaluateSlot();
    }
}

// ***********************
//
void                        AudioInputAsset::OnSlotRemoved    ( SlotRemovedEventPtr evt )
{
    if( ShouldReevaluate( evt.get() ) )
    {
        EvaluateSlot();
    }
}

// ***********************
//
void                        AudioInputAsset::HandleEvents     ( IEventPtr evt )
{
    if( evt->GetEventType() == SlotAddedEvent::Type() )
    {
        OnSlotAdded( std::static_pointer_cast< SlotAddedEvent >( evt ) );
    }
    else if( evt->GetEventType() == SlotRemovedEvent::Type() )
    {
        OnSlotRemoved( std::static_pointer_cast< SlotRemovedEvent >( evt ) );
    }
}

// ***********************
//
bool                        AudioInputAsset::ShouldReevaluate ( SlotEventBase * evt )
{
    if( m_binding.GetType() == InputSlotBinding::Type::ByIndex && evt->Index == m_binding.GetIndex() )
        return true;

    if( m_binding.GetType() == InputSlotBinding::Type::ByName && evt->Name == m_binding.GetName() )
        return true;

    return false;
}


// ========================================================================= //
// Implemnetation
// ========================================================================= //


// ***********************
//
const std::string &         AudioInputAsset::GetUID       () const
{
    return UID();
}

// ***********************
//
const std::string &         AudioInputAsset::UID          ()
{
    return uid;
}

// ***********************
//
UInt32                      AudioInputAsset::GetFrequency   () const
{
    return m_audio->GetFrequency();
}

// ***********************
//
AudioFormat                 AudioInputAsset::GetFormat      () const
{
    return m_audio->GetFormat();
}

// ***********************
//
MemoryChunkConstPtr         AudioInputAsset::GetFrame() const
{
    if( m_audio )
        return m_audio->GetData();

    return MemoryChunkConstPtr();
}

// ***********************
//
VoidConstPtr                AudioInputAsset::QueryThis    () const
{
    return shared_from_this();
}

// ***********************
//
void                        AudioInputAsset::AddReference () const
{
    m_numReferences++;
    m_binding.AddReference( m_slots.get() );
}

// ***********************
//
void                        AudioInputAsset::ReleaseReference () const
{
    m_numReferences--;
    m_binding.ReleaseReference( m_slots.get() );
}



}	// bv


