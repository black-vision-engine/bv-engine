#pragma once

#include <atomic>
#include <mutex>
#include <condition_variable>

#include "CoreDEF.h"

#include "Threading/Thread.h"
#include "Tools/SimpleTimer.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/Interfaces/IVideoDecoder.h"

namespace bv
{

class VideoDecoderThread : public Thread
{

private:
	IVideoDecoderPtr			m_decoder;

	std::mutex					m_mutex;
	std::condition_variable		m_cond;

	std::atomic< bool >			m_paused;
	std::atomic< bool >			m_stopped;

    SimpleTimer					m_timer;

public:
								VideoDecoderThread	( IVideoDecoderPtr decoder );
    virtual						~VideoDecoderThread	();

	void						Pause				();
	void						Resume				();
	void						Stop				();

protected:
	virtual void				Run					() override;
};

DEFINE_UPTR_TYPE( VideoDecoderThread )

} //bv
