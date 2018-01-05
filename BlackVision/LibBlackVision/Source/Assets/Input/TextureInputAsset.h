#pragma once


#include "ITextureInputAsset.h"

#include "Assets/AssetManager.h"		// Only for LoadTypedAsset template specialization

#include "Engine/Graphics/InputSlots/InputSlots.h"
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

};



}	// bv
