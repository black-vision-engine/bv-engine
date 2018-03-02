#pragma once


#include "Assets/Input/AudioInputAsset.h"
#include "VideoInputAssetDesc.h"

#include "Assets/AssetManager.h"		// Only for LoadTypedAsset template specialization

#include "Engine/Graphics/InputSlots/InputSlots.h"
#include "Engine/Graphics/InputSlots/SlotsLogic/VideoInput/VideoInputSlots.h"



namespace bv
{

class VideoInputAudioAsset;
DEFINE_CONST_PTR_TYPE( VideoInputAudioAsset )
DEFINE_PTR_TYPE( VideoInputAudioAsset )



/**@brief Gets data from input slot.
@ingroup Assets
@ingroup InputSlots*/
class VideoInputAudioAsset : public AudioInputAsset
{
    friend VideoInputAudioAssetConstPtr VideoInputSlots::CreateAsset( VideoInputSlotsPtr, VideoInputAudioAssetDescConstPtr );
private:

    static const std::string		uid;

protected:

    VideoInputSlotsPtr          m_videoSlots;

    videocards::VideoInputID    m_videoInputIdx;
    videocards::VideoType       m_videoType;

protected:

    explicit            VideoInputAudioAsset  ( VideoInputSlotsPtr slots, videocards::VideoInputID videoInputIdx );

    virtual void        EvaluateSlot        ();

public:

    static VideoInputAudioAssetPtr      Create          ( VideoInputSlotsPtr slots, videocards::VideoInputID videoInputIdx );

    virtual const std::string &	        GetUID			() const override;
    static const std::string &	        UID				();

};


// ***********************
/// Returns AssetDescriptor UID for Asset in template parameter.
/// @note AssetDescriptor uid and Asset uid are different strings.
template<> inline const std::string &   GetAssetDescUID< VideoInputAudioAsset >()
{
    return VideoInputAudioAsset::UID();
}


}	// bv
