#pragma once


#include "ITextureInputAsset.h"

#include "Assets/AssetManager.h"		// Only for LoadTypedAsset template specialization

#include "Engine/Graphics/InputSlots/InputSlots.h"
#include "Engine/Graphics/Resources/Textures/Texture2D.h"

#include "InputSlotBinding.h"


namespace bv
{


/**@brief Gets data from input slot.
@ingroup Assets
@ingroup InputSlots*/
class TextureInputAsset : public ITextureInputAsset, public std::enable_shared_from_this< TextureInputAsset >
{
private:

    static const std::string		uid;

protected:

    InputSlotsPtr           m_slots;
    InputSlotBinding        m_binding;

public:




    virtual const std::string &	    GetUID			() const override;

    static const std::string &	    UID				();


public:
    
    // Inherited via ITextureInputAsset
    virtual VoidConstPtr            QueryThis           () const override;

    virtual void                    AddReference        () const override;
    virtual void                    ReleaseReference    () const override;

    virtual SlotIndex               GetTextureSlotIndex () const override;
    virtual UInt64                  GetUpdateID         () const override;
    
    virtual UInt32                  GetWidth            ( UInt32 level = 0 ) const override;
    virtual UInt32                  GetHeight           ( UInt32 level = 0 ) const override;
    virtual UInt32                  GetDepth            ( UInt32 level = 0 ) const override;
    virtual UInt32                  GetNumMipMapLevels  () const override;

    virtual TextureFormat           GetFormat           () const override;
    virtual DataBuffer::Semantic    GetSemantic         () const override;

protected:

    Texture2DPtr                    GetTexture      () const;
};



}	// bv
