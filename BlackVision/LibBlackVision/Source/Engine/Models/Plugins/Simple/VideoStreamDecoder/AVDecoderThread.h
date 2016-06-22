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

class AVDecoderThread : public Thread
{

private:

	IAVDecoder *				m_decoder;

	mutable std::mutex			m_mutex;
	std::condition_variable		m_cond;

	std::atomic< bool >			m_stopped;
	std::atomic< bool >			m_running;

    SimpleTimer					m_videoTimer;
    SimpleTimer					m_audioTimer;

public:
								AVDecoderThread	    ( IAVDecoder * decoder );
	virtual						~AVDecoderThread	();
	void						Kill				();

	void						Play				();
	void						Stop				();

	bool						Stopped				() const;

protected:

	virtual void				Run					() override;

};

DEFINE_UPTR_TYPE( AVDecoderThread )

} //bv
