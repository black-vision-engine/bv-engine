#pragma once

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/Interfaces/IAVDefs.h"


namespace bv {

class IAVDecoder
{
public:

	virtual 						~IAVDecoder			() {};

	virtual void					Play					() = 0;
	virtual void					Pause					() = 0;
	virtual void					Stop					() = 0;
    
	virtual bool			        GetVideoMediaData		( AVMediaData & data ) = 0;
	virtual bool		            GetAudioMediaData		( AVMediaData & data ) = 0;
    virtual AVMediaData			    GetSingleFrame  		( TimeType frameTime) = 0;

	virtual SizeType				GetVideoFrameSize	    () const = 0;

	virtual UInt32					GetWidth				() const = 0;
	virtual UInt32					GetHeight				() const = 0;

	virtual Int32				    GetSampleRate			() const = 0;
    virtual AudioFormat			    GetAudioFormat			() const = 0;

	virtual UInt64				    GetDuration			    () const = 0;

    virtual bool                    HasVideo                () const = 0;
    virtual bool                    HasAudio                () const = 0;

	/** Accurate seeking.
	@param[time] in seconds 
    */
	virtual void					Seek					( Float64 time, bool flushBuffers = true ) = 0;
	virtual void				    FlushBuffers			() = 0;

	virtual bool					IsEOF					() const = 0;
	virtual bool					IsFinished				() const = 0;
    
    virtual void                    Mute                    ( bool mute ) = 0;

//protected:

    virtual bool					NextDataReady           ( UInt64 t, bool block ) = 0;
    
	friend class AVDecoderThread;

};

DEFINE_PTR_TYPE( IAVDecoder )
DEFINE_CONST_PTR_TYPE( IAVDecoder )

} //bv
