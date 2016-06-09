#pragma once

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/IFFmpegStreamDecoder.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/Interfaces/IAVDefs.h"

#include "Assets/VideoStream/VideoStreamAsset.h"


namespace bv {

class FFmpegDemuxer;

class FFmpegAudioStreamDecoder : public IFFmpegStreamDecoder
{

private:
	
    SwrContext *			            m_swrCtx;

    AudioFormat                         m_format;

	QueueConcurrent< AudioMediaData >   m_bufferQueue;

    UInt32                              m_maxQueueSize;

public:

							FFmpegAudioStreamDecoder	( VideoStreamAssetConstPtr asset, AVFormatContext * formatCtx, Int32 streamIdx, UInt32 maxQueueSize = 5 );
							~FFmpegAudioStreamDecoder	();

    AudioFormat             GetAudioFormat              () const;

    bool                    GetData                     ( AudioMediaData & data );

    bool                    ProcessPacket               ( FFmpegDemuxer * demuxer );
	
    /** Converts time from seconds to the stream specific time base timestamp */
	Int64					ConvertTime					( Float64 time );

	virtual bool			DecodePacket				( AVPacket * packet ) override;
	AudioMediaData			ConvertFrame				();

	void					Reset						();

	virtual Int32			GetStreamIdx				() const override;

};

DEFINE_UPTR_TYPE( FFmpegAudioStreamDecoder )

} //bv
