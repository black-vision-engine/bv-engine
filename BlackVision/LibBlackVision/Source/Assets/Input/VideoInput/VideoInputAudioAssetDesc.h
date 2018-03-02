#pragma once

#include "Serialization/ConversionHelper.h"

#include "Assets/Input/AudioInputAssetDesc.h"

#include "VideoInput/VideoInputChannelDesc.h"



namespace bv
{

class VideoInputAudioAssetDesc;
DEFINE_PTR_TYPE( VideoInputAudioAssetDesc )
DEFINE_CONST_PTR_TYPE( VideoInputAudioAssetDesc )


// ***********************
/// Descriptor allows to load VideoInputAsset but it can be used as simple input slot texture as well.
/// That's why it inherits from TextureInputAssetDesc, but doesn't use it's members.
class VideoInputAudioAssetDesc : public AudioInputAssetDesc
{
private:

    static const std::string						uid;

private:

    videocards::VideoInputID    m_videoInputIdx;

    mutable std::string         m_key;

protected:

    virtual const std::string &			GetUID				() const override;

private:

    explicit        VideoInputAudioAssetDesc              ( videocards::VideoInputID inputIdx );


public:

    virtual void                        Serialize           ( ISerializer & ser ) const;
    virtual void                        Deserialize         ( const IDeserializer & ser );

    static ISerializableConstPtr        Create              ( const IDeserializer & deser );
    static VideoInputAudioAssetDescPtr  Create              ( videocards::VideoInputID inputIdx );


    virtual std::string					GetKey				() const override;
    virtual std::string                 GetProposedShortKey () const override;
    virtual bool						IsCacheable			() const override;

    virtual SizeType                    EstimateMemoryUsage () const override;

    virtual VoidConstPtr				QueryThis			() const override;

    static const std::string &			UID					();


    videocards::VideoInputID            GetVideoInputID     () const { return m_videoInputIdx; }

protected:

    std::string                         ComputeKey          () const;
};



// ***********************
/// Returns AssetDescriptor UID for Asset in template parameter.
/// @note AssetDescriptor uid and Asset uid are different strings.
template<> inline const std::string &       GetAssetDescUID< VideoInputAudioAssetDesc >()
{
    return VideoInputAudioAssetDesc::UID();
}





}	// bv

