#pragma once

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegStreamDecoder.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/Interfaces/IAVDefs.h"

#include "Assets/VideoStream/AVAsset.h"


namespace bv {

class FFmpegDemuxer;

class FFmpegAudioStreamDecoder : public FFmpegStreamDecoder
{
private:

    static const AVSampleFormat         SUPPORTED_FORMATS[];

private:
	
    SwrContext *			            m_swrCtx;

    Int32                               m_sampleRate;
    AVSampleFormat                      m_format;
    Int32                               m_nbChannels;

	QueueConcurrent< AVMediaData >      m_bufferQueue;

    UInt32                              m_maxQueueSize;

    bool                                m_needConversion;

public:

							FFmpegAudioStreamDecoder	( AVAssetConstPtr asset, AVFormatContext * formatCtx, Int32 streamIdx, UInt32 maxQueueSize = 5 );
							~FFmpegAudioStreamDecoder	();

    Int32                   GetSampleRate               () const;
    AudioFormat             GetFormat                   () const;

    bool                    GetData                     ( AVMediaData & data );

    bool                    ProcessPacket               ( FFmpegDemuxer * demuxer );
	
	bool			        DecodePacket				( AVPacket * packet );
	AVMediaData			    ConvertFrame				();

	void					Reset						();

	virtual Int32			GetStreamIdx				() const override;

private:

    AVSampleFormat          GetSupportedFormat          ( AVSampleFormat format );

    static bool             IsSupportedFormat           ( AVSampleFormat format );
    static AudioFormat      ConvertFormat               ( AVSampleFormat format, Int32 nbChannels );

};

DEFINE_UPTR_TYPE( FFmpegAudioStreamDecoder )

} //bv
