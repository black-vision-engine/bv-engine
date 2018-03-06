#pragma once


#include "Assets/AssetDescriptor.h"

#include "Assets/Input/VideoInput/VideoInputTextureAssetDesc.h"
#include "Assets/Input/VideoInput/VideoInputAudioAssetDesc.h"
#include "Assets/Input/AudioInputAssetDesc.h"
#include "VideoInput/VideoInputChannelDesc.h"



namespace bv
{

class VideoInputAssetDesc;
DEFINE_PTR_TYPE( VideoInputAssetDesc )
DEFINE_CONST_PTR_TYPE( VideoInputAssetDesc )


// ***********************
// 
class VideoInputAssetDesc : public AssetDesc, public std::enable_shared_from_this< AssetDesc >
{
private:

    static const std::string						uid;

private:

    videocards::VideoInputID    m_videoFillIdx;
    videocards::VideoInputID    m_videoKeyIdx;

    mutable std::string         m_key;

protected:

    virtual const std::string &			GetUID				() const override;

private:

    explicit        VideoInputAssetDesc   ( videocards::VideoInputID fillIdx, videocards::VideoInputID keyIdx );


public:

    virtual void                        Serialize           ( ISerializer & ser ) const;
    virtual void                        Deserialize         ( const IDeserializer & ser );

    static ISerializableConstPtr        Create              ( const IDeserializer & deser );
    static VideoInputAssetDescPtr	    Create              ( videocards::VideoInputID fillIdx, videocards::VideoInputID keyIdx );

    VideoInputTextureAssetDescPtr       CreateTextureDesc   ( videocards::VideoType type ) const;
    VideoInputAudioAssetDescPtr         CreateAudioDesc     () const;

    virtual std::string					GetKey				() const override;
    virtual std::string                 GetProposedShortKey () const override;
    virtual bool						IsCacheable			() const override;

    virtual SizeType                    EstimateMemoryUsage () const override;

    virtual VoidConstPtr				QueryThis			() const override;

    static const std::string &			UID					();


    videocards::VideoInputID            GetVideoInputFillID () const { return m_videoFillIdx; }
    videocards::VideoInputID            GetVideoInputKeyID  () const { return m_videoKeyIdx; }

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

