#pragma once


#include "Assets/Input/TextureInputAsset.h"
#include "VideoInputAssetDesc.h"

#include "Assets/AssetManager.h"		// Only for LoadTypedAsset template specialization

#include "Engine/Graphics/InputSlots/InputSlots.h"
#include "Engine/Graphics/InputSlots/SlotsLogic/VideoInput/VideoInputSlots.h"



namespace bv
{

class VideoInputTextureAsset;
DEFINE_CONST_PTR_TYPE( VideoInputTextureAsset )
DEFINE_PTR_TYPE( VideoInputTextureAsset )



/**@brief Gets data from input slot.
@ingroup Assets
@ingroup InputSlots*/
class VideoInputTextureAsset : public TextureInputAsset
{
    friend VideoInputTextureAssetConstPtr VideoInputSlots::CreateAsset( VideoInputSlotsPtr, VideoInputTextureAssetDescConstPtr );
private:

    static const std::string		uid;

protected:

    VideoInputSlotsPtr          m_slots;

    videocards::VideoInputID    m_videoInputIdx;
    videocards::VideoType       m_videoType;

protected:

    explicit            VideoInputTextureAsset  ( VideoInputSlotsPtr slots, videocards::VideoInputID videoInputIdx, videocards::VideoType type );

    virtual void        EvaluateSlot        ();

public:

    static VideoInputTextureAssetPtr    Create   ( VideoInputSlotsPtr slots, videocards::VideoInputID videoInputIdx, videocards::VideoType type );

    virtual const std::string &	        GetUID			() const override;
    static const std::string &	        UID				();

};


// ***********************
/// Returns AssetDescriptor UID for Asset in template parameter.
/// @note AssetDescriptor uid and Asset uid are different strings.
template<> inline const std::string &   GetAssetDescUID< VideoInputTextureAsset >()
{
    return VideoInputTextureAsset::UID();
}


}	// bv
