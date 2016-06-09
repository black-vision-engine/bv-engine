#pragma once

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/Interfaces/IAVDefs.h"


namespace bv {

class IVideoDecoder
{
public:

	virtual 						~IVideoDecoder			() {};

	virtual void					Play					() = 0;
	virtual void					Pause					() = 0;
	virtual void					Stop					() = 0;
    
	virtual VideoMediaData			GetVideoMediaData		() = 0;
	virtual AudioMediaData		    GetAudioMediaData		() = 0;
    virtual VideoMediaData			GetSingleFrame  		( TimeType frameTime) = 0;

	virtual SizeType				GetFrameSize			() const = 0;

	virtual UInt32					GetWidth				() const = 0;
	virtual UInt32					GetHeight				() const = 0;
	virtual Float64					GetFrameRate			() const = 0;

    virtual bool                    HasVideo                () const = 0;
    virtual bool                    HasAudio                () const = 0;

	/** Accurate seeking.
	@param[time] in seconds 
    */
	virtual void					Seek					( Float64 time ) = 0;
	virtual void				    FlushBuffers			() = 0;

	virtual void					Reset					() = 0;

	virtual bool					IsEOF					() const = 0;
	virtual bool					IsFinished				() const = 0;
    
//protected:

	virtual bool					NextVideoDataReady		() = 0;
	virtual bool					NextAudioDataReady		() = 0;
    
	friend class VideoDecoderThread;

};

DEFINE_PTR_TYPE( IVideoDecoder )
DEFINE_CONST_PTR_TYPE( IVideoDecoder )

} //bv
