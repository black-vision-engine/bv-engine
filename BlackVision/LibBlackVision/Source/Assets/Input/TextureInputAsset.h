#pragma once


#include "ITextureInputAsset.h"
#include "TextureInputAssetDesc.h"

#include "Assets/AssetManager.h"		// Only for LoadTypedAsset template specialization

#include "Engine/Graphics/InputSlots/InputSlots.h"
#include "Engine/Graphics/Resources/Textures/Texture2D.h"

#include "InputSlotBinding.h"


namespace bv
{

class TextureInputAsset;
DEFINE_CONST_PTR_TYPE( TextureInputAsset )
DEFINE_PTR_TYPE( TextureInputAsset )



/**@brief Gets data from input slot.
@ingroup Assets
@ingroup InputSlots*/
class TextureInputAsset : public ITextureInputAsset, public std::enable_shared_from_this< TextureInputAsset >
{
    friend TextureInputAssetConstPtr InputSlots::CreateAsset( InputSlotsPtr, TextureInputAssetDescConstPtr);
private:

    static const std::string		uid;

protected:

    InputSlotsPtr           m_slots;
    InputSlotBinding        m_binding;

    Texture2DPtr            m_texture;

    mutable UInt64                  m_updateID;         ///< This indicates that bound texture changed not it's content.
    mutable std::atomic< UInt32 >   m_numReferences;    ///< Remembers number of references to update it in new asset when texture under slot changes.

protected:

    explicit            TextureInputAsset   ( InputSlotsPtr slots, InputSlotBinding binding );

    virtual void        EvaluateSlot        ();

public:

    static TextureInputAssetPtr     Create   ( InputSlotsPtr slots, InputSlotBinding binding );

    virtual VoidConstPtr            QueryThis       () const override;
    virtual const std::string &	    GetUID			() const override;

    static const std::string &	    UID				();


public:
    
    ///@name Inherited from ITextureInputAsset
    ///@{

    virtual void                    AddReference        () const override;
    virtual void                    ReleaseReference    () const override;

    virtual UInt64                  GetUpdateID         () const override;
    
    virtual UInt32                  GetWidth            ( UInt32 level = 0 ) const override;
    virtual UInt32                  GetHeight           ( UInt32 level = 0 ) const override;
    virtual UInt32                  GetDepth            ( UInt32 level = 0 ) const override;
    virtual UInt32                  GetNumMipMapLevels  () const override;

    virtual TextureFormat           GetFormat           () const override;
    virtual DataBuffer::Semantic    GetSemantic         () const override;

    ///@}

public:

    Texture2DPtr                    GetTexture      () const;
};


// ***********************
/// Returns AssetDescriptor UID for Asset in template parameter.
/// @note AssetDescriptor uid and Asset uid are different strings.
template<> inline const std::string &   GetAssetDescUID< TextureInputAsset >()
{
    return TextureInputAssetDesc::UID();
}


}	// bv
