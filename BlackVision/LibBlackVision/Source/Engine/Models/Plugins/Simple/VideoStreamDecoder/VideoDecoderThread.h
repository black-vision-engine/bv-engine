#pragma once

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/Interfaces/IVideoDecoder.h"

#include "Threading/Thread.h"
#include "CoreDEF.h"

namespace bv
{

class VideoDecoderThread : public Thread
{

private:
	IVideoDecoder *				m_decoder;

public:
								VideoDecoderThread	( IVideoDecoder * decoder );
    virtual						~VideoDecoderThread	();

	virtual void				Run					() override;
};

DEFINE_UPTR_TYPE( VideoDecoderThread )

} //bv
