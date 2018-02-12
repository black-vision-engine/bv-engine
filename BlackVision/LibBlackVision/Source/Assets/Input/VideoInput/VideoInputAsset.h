#pragma once


#include "Assets/Asset.h"
#include "VideoInputAssetDesc.h"

#include "Assets/Input/VideoInput/VideoInputTextureAsset.h"

#include "Assets/AssetManager.h"		// Only for LoadTypedAsset template specialization

#include "Engine/Graphics/InputSlots/InputSlots.h"
#include "Engine/Graphics/InputSlots/SlotsLogic/VideoInput/VideoInputSlots.h"



namespace bv
{

class VideoInputAsset;
DEFINE_CONST_PTR_TYPE( VideoInputAsset )
DEFINE_PTR_TYPE( VideoInputAsset )



/**@brief Collection of all assets associated with single video input.
@ingroup Assets
@ingroup InputSlots*/
class VideoInputAsset : public Asset, public std::enable_shared_from_this< VideoInputAsset >
{
private:

    static const std::string		uid;

protected:

    VideoInputTextureAssetConstPtr      m_fill;
    VideoInputTextureAssetConstPtr      m_key;

    // FIXME: Add audio later.

protected:

    explicit            VideoInputAsset             ( VideoInputTextureAssetConstPtr fill, VideoInputTextureAssetConstPtr key );

public:

    static VideoInputAssetPtr       Create          ( VideoInputTextureAssetConstPtr fill, VideoInputTextureAssetConstPtr key );


    VideoInputTextureAssetConstPtr  GetFillAsset    () const { return m_fill; }
    VideoInputTextureAssetConstPtr  GetKeyAsset     () const { return m_key; }


    virtual VoidConstPtr            QueryThis       () const override;
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
