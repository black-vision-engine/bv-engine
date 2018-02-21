#pragma once

#include "Assets/AssetDescriptor.h"
#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"

#include "InputSlotBinding.h"


namespace bv
{

class TextureInputAssetDesc;
DEFINE_PTR_TYPE( TextureInputAssetDesc )


// ***********************
/*@brief Descriptor for loading texture from input slots.

Use this as base class for input slot specialized classes. Remember to add
uid in IsDerivedFromTextureInputAssetDesc function.

@ingroup Assets
@ingroup InputSlots*/
class TextureInputAssetDesc : public AssetDesc, public std::enable_shared_from_this< AssetDesc >
{
private:

    static const std::string						uid;

private:

    InputSlotBinding            m_binding;


    mutable std::string         m_key;

protected:

    virtual const std::string &			GetUID				() const override;

protected:

    explicit        TextureInputAssetDesc   ( InputSlotBinding binding );


public:

    virtual void                        Serialize           ( ISerializer & ser ) const;
    virtual void                        Deserialize         ( const IDeserializer & ser );

    static ISerializableConstPtr        Create              ( const IDeserializer & deser );
    static TextureInputAssetDescPtr	    Create              ( InputSlotBinding binding );


    virtual std::string					GetKey				() const override;
    virtual std::string                 GetProposedShortKey () const override;
    virtual bool						IsCacheable			() const override;

    virtual SizeType                    EstimateMemoryUsage () const override;

    virtual VoidConstPtr				QueryThis			() const override;

    static const std::string &			UID					();


    const InputSlotBinding &            BindingInfo         () const { return m_binding; }

protected:

    std::string                         ComputeKey          () const;
};



// ***********************
/// Returns AssetDescriptor UID for Asset in template parameter.
/// @note AssetDescriptor uid and Asset uid are different strings.
template<> inline const std::string &       GetAssetDescUID< TextureInputAssetDesc >()
{
    return TextureInputAssetDesc::UID();
}


// ***********************
/*@brief Checks if descriptor can be treated as TextureInputAssetDesc.
If you implement new input slot asset, remember to add uid to this function.*/
bool                IsTextureSlotAssetDesc          ( const std::string & uid );


}	// bv

