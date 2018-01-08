#pragma once


#include "Assets/Asset.h"




namespace bv
{



/**@brief Interface for assets using input slots.

This is interface to be used in model. Model shouldn't see any engine objects.

Objects using input slots should always add and release references when they use input.
Note that this isn't shared pointer like behavior. Underlying slot can always be released
even if someone uses it. Reference counting can only help in optimizing input usage
(for example video input can stop loading coming frames from video card).

@ingroup Assets
@ingroup InputSlots*/
class ITextureInputAsset : public Asset
{
private:
public:

    virtual void        AddReference        () const = 0;
    virtual void        ReleaseReference    () const = 0;

    virtual SlotIndex   GetTextureSlotIndex () const = 0;
    virtual UInt64      GetUpdateID         () const = 0;

public:

    ///@name Texture information
    ///@{
    virtual UInt32                      GetWidth        ( UInt32 level = 0 ) const = 0;
    virtual UInt32                      GetHeight       ( UInt32 level = 0 ) const = 0;
    virtual UInt32                      GetDepth        ( UInt32 level = 0 ) const = 0;

    virtual TextureFormat               GetFormat       () const = 0;
    virtual DataBuffer::Semantic        GetSemantic     () const = 0;

    virtual UInt32                      GetNumMipMapLevels  () const = 0;
    ///@}
};

DEFINE_PTR_TYPE( ITextureInputAsset );
DEFINE_CONST_PTR_TYPE( ITextureInputAsset );


}	// bv


