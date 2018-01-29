#pragma once


#include "Assets/Input/TextureInputAsset.h"
#include "VideoInputAssetDesc.h"

#include "Assets/AssetManager.h"		// Only for LoadTypedAsset template specialization

#include "Engine/Graphics/InputSlots/InputSlots.h"
#include "Engine/Graphics/InputSlots/SlotsLogic/VideoInput/VideoInputSlots.h"



namespace bv
{

class VideoInputAsset;
DEFINE_CONST_PTR_TYPE( VideoInputAsset )
DEFINE_PTR_TYPE( VideoInputAsset )



/**@brief Gets data from input slot.
@ingroup Assets
@ingroup InputSlots*/
class VideoInputAsset : public TextureInputAsset
{
    friend VideoInputAssetConstPtr VideoInputSlots::CreateAsset( VideoInputSlotsPtr, VideoInputAssetDescConstPtr );
private:

    static const std::string		uid;

protected:

    VideoInputSlotsPtr          m_slots;

    videocards::VideoInputID    m_videoInputIdx;

protected:

    explicit            VideoInputAsset     ( VideoInputSlotsPtr slots, videocards::VideoInputID videoInputIdx );

    virtual void        EvaluateSlot        ();

public:

    static VideoInputAssetPtr       Create   ( VideoInputSlotsPtr slots, videocards::VideoInputID videoInputIdx );

    virtual const std::string &	    GetUID			() const override;

    static const std::string &	    UID				();

};


// ***********************
/// Returns AssetDescriptor UID for Asset in template parameter.
/// @note AssetDescriptor uid and Asset uid are different strings.
template<> inline const std::string &   GetAssetDescUID< VideoInputAsset >()
{
    return VideoInputAssetDesc::UID();
}


}	// bv
