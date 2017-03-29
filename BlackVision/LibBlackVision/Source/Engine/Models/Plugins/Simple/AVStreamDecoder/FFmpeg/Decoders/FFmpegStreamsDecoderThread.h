#pragma once

#include <atomic>
#include <mutex>

#include "Threading/Thread.h"

#include "Engine/Models/Plugins/Simple/AVStreamDecoder/Interfaces/IAVDecoder.h"
#include "Engine/Models/Plugins/Simple/AVStreamDecoder/FFmpeg/Decoders/FFmpegStreamDecoder.h"
#include "Engine/Models/Plugins/Simple/AVStreamDecoder/FFmpeg/Demuxer/FFmpegDemuxer.h"

#include "DataTypes/QueueConcurrent.h"


namespace bv {


class FFmpegStreamsDecoderThread : public Thread
{

private:

    FFmpegStreamDecoder *       m_streamDecoder;

	mutable std::mutex			m_mutex1;

	mutable std::mutex			m_mutex;
	std::condition_variable		m_cond;

	std::atomic< bool >			m_stopThread;
	std::atomic< bool >			m_stopped;
	std::atomic< bool >			m_running;

public:

                                FFmpegStreamsDecoderThread      ( FFmpegStreamDecoder * streamDecoder );
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
