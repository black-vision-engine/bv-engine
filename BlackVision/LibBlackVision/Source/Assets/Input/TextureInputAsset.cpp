#include "stdafx.h"
#include "TextureInputAsset.h"

#include "Engine/Events/EventManager.h"

#include "Application/ApplicationContext.h"


namespace bv
{

const std::string       TextureInputAsset::uid = "TEXTURE_INPUT_ASSET";



// ========================================================================= //
// Creation
// ========================================================================= //

// ***********************
//
TextureInputAsset::TextureInputAsset        ( InputSlotsPtr slots, InputSlotBinding binding )
    : m_numReferences( 0 )
    , m_slots( slots )
    , m_binding( binding )
{
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &TextureInputAsset::HandleEvents ), SlotAddedEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &TextureInputAsset::HandleEvents ), SlotRemovedEvent::Type() );
}

// ***********************
//
TextureInputAsset::~TextureInputAsset()
{
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &TextureInputAsset::HandleEvents ), SlotAddedEvent::Type() );
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &TextureInputAsset::HandleEvents ), SlotRemovedEvent::Type() );
}

// ***********************
//
TextureInputAssetPtr            TextureInputAsset::Create       ( InputSlotsPtr slots, InputSlotBinding binding )
{
    return TextureInputAssetPtr( new TextureInputAsset( slots, binding ) );
}

// ***********************
//
void                            TextureInputAsset::EvaluateSlot()
{
    auto inputSlot = m_binding.EvaluateInputSlot( m_slots.get() );
    if( inputSlot.IsValid() )
    {
        m_texture = inputSlot.GetVal().Texture;
    }
    else
    {
        m_texture = m_slots->GetFallbackSlot().Texture;
    }

    m_updateID = ApplicationContext::Instance().GetUpdateCounter() + 1;
}

// ***********************
//
void                        TextureInputAsset::OnSlotAdded      ( SlotAddedEventPtr evt )
{
    if( ShouldReevaluate( evt.get() ) )
    {
        EvaluateSlot();
    }
}

// ***********************
//
void                        TextureInputAsset::OnSlotRemoved    ( SlotRemovedEventPtr evt )
{
    if( ShouldReevaluate( evt.get() ) )
    {
        EvaluateSlot();
    }
}

// ***********************
//
void                        TextureInputAsset::HandleEvents     ( IEventPtr evt )
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
bool                        TextureInputAsset::ShouldReevaluate ( SlotEventBase * evt )
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
const std::string &         TextureInputAsset::GetUID       () const
{
    return UID();
}

// ***********************
//
const std::string &         TextureInputAsset::UID          ()
{
    return uid;
}

// ***********************
//
Texture2DPtr                TextureInputAsset::GetTexture   () const
{
    return m_texture;
}

// ***********************
//
VoidConstPtr                TextureInputAsset::QueryThis    () const
{
    return shared_from_this();
}

// ***********************
//
void                        TextureInputAsset::AddReference () const
{
    m_numReferences++;
    m_binding.AddReference( m_slots.get() );
}

// ***********************
//
void                        TextureInputAsset::ReleaseReference () const
{
    m_numReferences--;
    m_binding.ReleaseReference( m_slots.get() );
}

// ***********************
//
UInt64                      TextureInputAsset::GetUpdateID          () const
{
    return m_updateID;
}

// ***********************
//
UInt32                      TextureInputAsset::GetWidth             ( UInt32 level ) const
{
    return GetTexture()->GetWidth( level );
}

// ***********************
//
UInt32                      TextureInputAsset::GetHeight            ( UInt32 level ) const
{
    return GetTexture()->GetHeight( level );
}

// ***********************
//
UInt32                      TextureInputAsset::GetDepth             ( UInt32 ) const
{
    return 1;
}

// ***********************
//
UInt32                      TextureInputAsset::GetNumMipMapLevels   () const
{
    return GetTexture()->GetNumLevels();
}

// ***********************
//
TextureFormat               TextureInputAsset::GetFormat            () const
{
    return GetTexture()->GetFormat();
}

// ***********************
//
DataBuffer::Semantic        TextureInputAsset::GetSemantic          () const
{
    return GetTexture()->GetSemantic();
}


}	// bv


