#pragma once

#include <atomic>
#include <mutex>
#include <condition_variable>

#include "Threading/Thread.h"
#include "Tools/SimpleTimer.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/Demuxer/FFmpegDemuxer.h"
#include "DataTypes/QueueConcurrent.h"


namespace bv {


class FFmpegDemuxerThread : public Thread
{

private:

    FFmpegDemuxer *				m_demuxer;

	mutable std::mutex			m_mutex;
	std::condition_variable		m_cond;

	std::atomic< bool >			m_stopThread;
	std::atomic< bool >			m_stopped;
	std::atomic< bool >			m_running;

public:

								FFmpegDemuxerThread	    ( FFmpegDemuxer * demuxer );
	virtual						~FFmpegDemuxerThread    ();
	void						Kill				    ();

	void						Restart				    ();
	void						Stop				    ();

	bool						Stopped			    	() const;

protected:

	virtual void				Run				    	() override;

};

DEFINE_UPTR_TYPE( FFmpegDemuxerThread )

} //bv
