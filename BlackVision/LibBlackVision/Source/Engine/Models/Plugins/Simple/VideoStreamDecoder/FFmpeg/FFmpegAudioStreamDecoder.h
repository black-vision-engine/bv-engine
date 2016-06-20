#pragma once

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegStreamDecoder.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/Interfaces/IAVDefs.h"

#include "Assets/VideoStream/AVAsset.h"


namespace bv {

class FFmpegDemuxer;

class FFmpegAudioStreamDecoder : public FFmpegStreamDecoder
{
private:

    static const AVSampleFormat         DEFAULT_FORMAT;

private:
	
    UInt32                              m_frameSize;

    SwrContext *			            m_swrCtx;

    Int32                               m_sampleRate;
    AudioFormat                         m_format;

	QueueConcurrent< AVMediaData >      m_bufferQueue;

    UInt32                              m_maxQueueSize;

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

};

DEFINE_UPTR_TYPE( FFmpegAudioStreamDecoder )

} //bv
