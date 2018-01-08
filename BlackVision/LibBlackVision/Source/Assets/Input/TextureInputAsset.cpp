#include "stdafx.h"
#include "TextureInputAsset.h"




namespace bv
{

const std::string       TextureInputAsset::uid = "TEXTURE_INPUT_ASSET";



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
    assert( !"Implemnet me" );
    return Texture2DPtr();
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
    assert( !"Implemnet me" );
}

// ***********************
//
void                        TextureInputAsset::ReleaseReference () const
{
    assert( !"Implemnet me" );
}

// ***********************
//
SlotIndex                   TextureInputAsset::GetTextureSlotIndex  () const
{
    assert( !"Implemnet me" );
    return SlotIndex();
}

// ***********************
//
UInt64                      TextureInputAsset::GetUpdateID          () const
{
    assert( !"Implemnet me" );
    return UInt64();
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


