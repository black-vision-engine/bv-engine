#pragma once

#include <atomic>
#include <mutex>

#include "Threading/Thread.h"

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/Interfaces/IVideoDecoder.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegAudioStreamDecoder.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegDemuxer.h"
#include "DataTypes/QueueConcurrent.h"


namespace bv {


class FFmpegVideoDecoder;


class FFmpegAudioStreamDecoderThread : public Thread
{

private:

    FFmpegVideoDecoder *        m_decoder;
    FFmpegAudioStreamDecoder *  m_audioDecoder;
    FFmpegDemuxer *				m_demuxer;

	mutable std::mutex			m_mutex;
	std::condition_variable		m_cond;

	std::atomic< bool >			m_stopped;
	std::atomic< bool >			m_running;

	QueueConcurrent< AudioMediaData >   m_bufferQueue;

public:

							    FFmpegAudioStreamDecoderThread	( FFmpegVideoDecoder * decoder, FFmpegAudioStreamDecoder * audioDecoder, FFmpegDemuxer * demuxer );
	virtual					    ~FFmpegAudioStreamDecoderThread	();
	void						Kill				            ();

    void					    Restart				            ();
	void					    Stop				            ();
        
	bool					    Stopped		            		() const;

protected:

	virtual void			    Run					            () override;

};

DEFINE_UPTR_TYPE( FFmpegAudioStreamDecoderThread )

} //bv
