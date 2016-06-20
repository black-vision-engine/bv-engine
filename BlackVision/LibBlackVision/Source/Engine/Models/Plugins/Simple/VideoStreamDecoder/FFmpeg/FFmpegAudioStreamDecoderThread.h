#pragma once

#include <atomic>
#include <mutex>

#include "Threading/Thread.h"

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/Interfaces/IAVDecoder.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegAudioStreamDecoder.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegDemuxer.h"
#include "DataTypes/QueueConcurrent.h"


namespace bv {


class FFmpegAVDecoder;


class FFmpegAudioStreamDecoderThread : public Thread
{

private:

    FFmpegAVDecoder *        m_decoder;
    FFmpegAudioStreamDecoder *  m_audioDecoder;
    FFmpegDemuxer *				m_demuxer;

	mutable std::mutex			m_mutex;
	std::condition_variable		m_cond;

	std::atomic< bool >			m_stopped;
	std::atomic< bool >			m_running;

	QueueConcurrent< AVMediaData >   m_bufferQueue;

public:

							    FFmpegAudioStreamDecoderThread	( FFmpegAVDecoder * decoder, FFmpegAudioStreamDecoder * audioDecoder, FFmpegDemuxer * demuxer );
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
