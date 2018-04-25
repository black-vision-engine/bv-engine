#pragma once

#include "Engine/Types/Enums.h"
#include "Memory/MemoryChunk.h"
#include "CoreDEF.h"

#include "Assets/Asset.h"


namespace bv {

class AVAsset;
DEFINE_CONST_PTR_TYPE( AVAsset )



/**@brief Audio video asset.
@ingroup Assets*/
class AVAsset : public Asset, public std::enable_shared_from_this< AVAsset >
{
private:

	static const std::string    uid;

	std::string                 m_streamPath;

    bool                        m_videoEnabled;
    bool                        m_audioEnabled;

    //video only data
	TextureFormat				m_textureFormat;

	//raw videos data
	UInt32						m_width;
	UInt32						m_height;
	Float64						m_frameRate;
	VideoPixelFormat			m_videoFormat;

protected:

	virtual VoidConstPtr						QueryThis	        () const override;

public:

	explicit									AVAsset    ( const std::string & streamPath, TextureFormat textureFormat, UInt32 width, UInt32 height, Float64 frameRate, VideoPixelFormat videoFormat );

	static const std::string &					UID                 ();

	const std::string &							GetUID              () const override;

	const std::string &							GetStreamPath       () const;
	UInt32										GetWidth            () const;
	UInt32										GetHeight           () const;
	Float64										GetFrameRate        () const;
	VideoPixelFormat							GetVideoFormat      () const;

	TextureFormat								GetTextureFormat    () const;

    bool							            IsVideoEnabled      () const;
	bool							            IsAudioEnabled      () const;

	static AVAssetConstPtr				        Create		        ( const std::string & streamPath, TextureFormat textureFormat, UInt32 width, UInt32 height, Float64 frameRate, VideoPixelFormat videoFormat );

};

} // bv