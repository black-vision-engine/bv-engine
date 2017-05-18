#pragma once

#include "Assets/AssetDescriptor.h"
#include "Engine/Types/Enums.h"
#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"

#include "CoreDEF.h"

#include <string>
#include <vector>

namespace bv
{

class AVAssetDesc;
DEFINE_CONST_PTR_TYPE( AVAssetDesc )

class AVAssetDesc : public AssetDesc, public std::enable_shared_from_this< AVAssetDesc >
{
private:

    static const std::string	uid;

    std::string					m_streamPath;

    //raw videos data
    UInt32						m_width;
    UInt32						m_height;
    Float64						m_frameRate;
    VideoPixelFormat			m_videoFormat;

    TextureFormat				m_textureFormat;

    mutable std::string                 m_key;

protected:


    virtual VoidConstPtr						QueryThis					() const override;

public:

    explicit									AVAssetDesc		( const std::string & streamPath, TextureFormat textureFormat );
    explicit									AVAssetDesc		( const std::string & streamPath, TextureFormat textureFormat, UInt32 width, UInt32 height, Float64 frameRate, VideoPixelFormat videoFormat );


    virtual void								Serialize					( ISerializer& sob ) const;
    static ISerializableConstPtr			    Create						( const IDeserializer& dob );

    virtual const std::string &					GetUID						() const override;
    
    virtual bool								IsCacheable					() const override;
    
    virtual std::string							GetKey						() const override;
    virtual SizeType                            EstimateMemoryUsage         () const override;

    static const std::string &					UID							();

    const std::string &							GetStreamPath				() const;
    UInt32										GetWidth					() const;
    UInt32										GetHeight					() const;
    Float64										GetFrameRate				() const;
    VideoPixelFormat							GetVideoFormat				() const;

    TextureFormat								GetTextureFormat			() const;
    
    static AVAssetDescConstPtr			        Create						( const std::string & streamPath, TextureFormat textureFormat );
    static AVAssetDescConstPtr			        Create						( const std::string & streamPath, TextureFormat textureFormat, UInt32 width, UInt32 height, Float64 frameRate, VideoPixelFormat videoFormat  );

private:

    std::string                                 ComputeKey                  () const;
};

// ***********************
/// Returns AssetDescriptor UID for Asset in template parameter.
/// @note AssetDescriptor uid and Asset uid are different strings.
template<> inline const std::string& GetAssetDescUID<AVAsset>()
{
    return AVAssetDesc::UID();
}


} // bv