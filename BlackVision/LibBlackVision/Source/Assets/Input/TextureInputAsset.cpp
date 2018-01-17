#include "stdafx.h"
#include "TextureInputAsset.h"

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
{}

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


