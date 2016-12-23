#pragma once

#include <atomic>
#include <mutex>

#include "Threading/Thread.h"

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/Interfaces/IAVDecoder.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegStreamDecoder.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegDemuxer.h"
#include "DataTypes/QueueConcurrent.h"


namespace bv {


class FFmpegStreamsDecoderThread : public Thread
{

private:

    FFmpegStreamDecoder *       m_videoStreamDecoder;
    FFmpegStreamDecoder *       m_audioStreamDecoder;
    FFmpegDemuxer *				m_demuxer;

	mutable std::mutex			m_mutex;
	std::condition_variable		m_cond;

	std::atomic< bool >			m_stopped;
	std::atomic< bool >			m_running;

public:

                                FFmpegStreamsDecoderThread      ( FFmpegStreamDecoder * videoStreamDecoder, FFmpegStreamDecoder * audioStreamDecoder, FFmpegDemuxer * demuxer );
	virtual					    ~FFmpegStreamsDecoderThread     ();
	void						Kill				            ();

    void					    Restart				            ();
	void					    Stop				            ();
        
	bool					    Stopped		            		() const;

protected:

	virtual void			    Run					            () override;

};

DEFINE_UPTR_TYPE( FFmpegStreamsDecoderThread )

} //bv
