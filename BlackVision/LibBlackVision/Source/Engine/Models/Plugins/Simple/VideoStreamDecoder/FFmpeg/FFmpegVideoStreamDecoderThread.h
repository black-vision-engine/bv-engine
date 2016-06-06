#pragma once

#include <atomic>
#include <mutex>

#include "Threading/Thread.h"

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/Interfaces/IVideoDecoder.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegVideoStreamDecoder.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegDemuxer.h"


namespace bv {


class FFmpegVideoDecoder;


class FFmpegVideoStreamDecoderThread : public Thread
{

private:

    FFmpegVideoDecoder *        m_decoder;
    FFmpegVideoStreamDecoder *  m_videoDecoder;
    FFmpegDemuxer *				m_demuxer;

	mutable std::mutex			m_mutex;
	std::condition_variable		m_cond;

	std::atomic< bool >			m_stopped;
	std::atomic< bool >			m_running;


public:

							    FFmpegVideoStreamDecoderThread	( FFmpegVideoDecoder * decoder, FFmpegVideoStreamDecoder * videoDecoder, FFmpegDemuxer * demuxer );
	virtual					    ~FFmpegVideoStreamDecoderThread	();
	void						Kill				            ();

    void					    Restart				            ();
	void					    Stop				            ();
        
	bool					    Stopped		            		() const;

protected:

	virtual void			    Run					            () override;

};

DEFINE_UPTR_TYPE( FFmpegVideoStreamDecoderThread )

} //bv
