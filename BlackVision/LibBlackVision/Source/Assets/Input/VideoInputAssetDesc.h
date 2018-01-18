#pragma once

#include "TextureInputAssetDesc.h"

#include "VideoInput/VideoInputChannelDesc.h"



namespace bv
{

class VideoInputAssetDesc;
DEFINE_PTR_TYPE( VideoInputAssetDesc )


// ***********************
//
class VideoInputAssetDesc : public TextureInputAssetDesc
{
private:

    static const std::string						uid;

private:

    videocards::VideoInputID    m_videoInputIdx;

    mutable std::string         m_key;

protected:

    virtual const std::string &			GetUID				() const override;

private:

    explicit        VideoInputAssetDesc   ( videocards::VideoInputID inputIdx );


public:

    virtual void                        Serialize           ( ISerializer & ser ) const;
    virtual void                        Deserialize         ( const IDeserializer & ser );

    static ISerializableConstPtr        Create              ( const IDeserializer & deser );
    static VideoInputAssetDescPtr	    Create              ( videocards::VideoInputID inputIdx );


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
template<> inline const std::string &       GetAssetDescUID< VideoInputAssetDesc >()
{
    return VideoInputAssetDesc::UID();
}




}	// bv

