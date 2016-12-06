#pragma once

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegStreamDecoder.h"

#include "Assets/VideoStream/AVAsset.h"


namespace bv {

class FFmpegAudioStreamDecoder : public FFmpegStreamDecoder
{
private:

    static const AVSampleFormat         SUPPORTED_FORMATS[];

private:
	
    SwrContext *			            m_swrCtx;

    Int32                               m_sampleRate;
    AVSampleFormat                      m_format;
    Int32                               m_nbChannels;

    bool                                m_needConversion;

public:

							FFmpegAudioStreamDecoder	( AVAssetConstPtr asset, AVFormatContext * formatCtx, Int32 streamIdx, UInt32 maxQueueSize = 10 );
							~FFmpegAudioStreamDecoder	();

    Int32                   GetSampleRate               () const;
    AudioFormat             GetFormat                   () const;

	virtual AVMediaData		ConvertFrame				() override;

private:

    AVSampleFormat          GetSupportedFormat          ( AVSampleFormat format );

    static bool             IsSupportedFormat           ( AVSampleFormat format );
    static AudioFormat      ConvertFormat               ( AVSampleFormat format, Int32 nbChannels );

};

} //bv
