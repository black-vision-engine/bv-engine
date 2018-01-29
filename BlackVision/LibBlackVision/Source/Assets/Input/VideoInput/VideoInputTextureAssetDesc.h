#pragma once

#include "Serialization/ConversionHelper.h"

#include "Assets/Input/TextureInputAssetDesc.h"

#include "VideoInput/VideoInputChannelDesc.h"



namespace bv
{

class VideoInputTextureAssetDesc;
DEFINE_PTR_TYPE( VideoInputTextureAssetDesc )
DEFINE_CONST_PTR_TYPE( VideoInputTextureAssetDesc )


// ***********************
/// Descriptor allows to load VideoInputAsset but it can be used as simple input slot texture as well.
/// That's why it inherits from TextureInputAssetDesc, but doesn't use it's members.
class VideoInputTextureAssetDesc : public TextureInputAssetDesc
{
public:

    enum class VideoType
    {
        Fill,
        Key,

        Total
    };

private:

    static const std::string						uid;

private:

    videocards::VideoInputID    m_videoInputIdx;
    VideoType                   m_inputType;

    mutable std::string         m_key;

protected:

    virtual const std::string &			GetUID				() const override;

private:

    explicit        VideoInputTextureAssetDesc   ( videocards::VideoInputID inputIdx, VideoType type );


public:

    virtual void                        Serialize           ( ISerializer & ser ) const;
    virtual void                        Deserialize         ( const IDeserializer & ser );

    static ISerializableConstPtr            Create              ( const IDeserializer & deser );
    static VideoInputTextureAssetDescPtr    Create              ( videocards::VideoInputID inputIdx, VideoType type );


    virtual std::string					GetKey				() const override;
    virtual std::string                 GetProposedShortKey () const override;
    virtual bool						IsCacheable			() const override;

    virtual SizeType                    EstimateMemoryUsage () const override;

    virtual VoidConstPtr				QueryThis			() const override;

    static const std::string &			UID					();


    videocards::VideoInputID            GetVideoInputID     () const { return m_videoInputIdx; }
    VideoType                           GetVideoType        () const { return m_inputType; }

protected:

    std::string                         ComputeKey          () const;
};



// ***********************
/// Returns AssetDescriptor UID for Asset in template parameter.
/// @note AssetDescriptor uid and Asset uid are different strings.
template<> inline const std::string &       GetAssetDescUID< VideoInputTextureAssetDesc >()
{
    return VideoInputTextureAssetDesc::UID();
}

DECLARE_ENUM_SERIALIZATION( VideoInputTextureAssetDesc::VideoType )



}	// bv

