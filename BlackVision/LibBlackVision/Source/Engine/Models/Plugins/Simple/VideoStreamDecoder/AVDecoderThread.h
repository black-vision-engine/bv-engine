#pragma once

#include <atomic>
#include <mutex>
#include <condition_variable>

#include "CoreDEF.h"

#include "Threading/Thread.h"
#include "Tools/SimpleTimer.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/Interfaces/IAVDecoder.h"


namespace bv
{

class FFmpegStreamDecoder;

class AVDecoderThread : public Thread
{

private:

	FFmpegStreamDecoder *		m_streamDecoder;

	mutable std::mutex			m_mutex;
	std::condition_variable		m_cond;

	std::atomic< bool >			m_stopped;
	std::atomic< bool >			m_running;
	std::atomic< bool >			m_paused;

    SimpleTimer					m_timer;

public:
								AVDecoderThread	    ( FFmpegStreamDecoder * streamDecoder );
	virtual						~AVDecoderThread	();
	void						Kill				();

	void						Play				();
	void						Stop				();
	bool						Pause				();

	bool						Paused				() const;
	bool						Stopped				() const;

protected:

	virtual void				Run					() override;

};

DEFINE_UPTR_TYPE( AVDecoderThread )

} //bv
