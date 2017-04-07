#pragma once

#include "Memory/MemoryChunk.h"
#include "CoreDEF.h"
#include "AVFrame.h"

#include <functional>
#include <set>

namespace bv { namespace videocards {

class IVideoCard
{
public:
    typedef std::function< void ( UInt64, bool ) > FrameProcessingCompletedCallbackType;


    virtual                     ~IVideoCard             () {}

    virtual void                PreStart                () = 0;
	virtual void                Start                   () = 0;
    virtual void                Stop                    () = 0;

	virtual void                SetVideoOutput          ( bool enable ) = 0;

    //TODO: handle frames from GPU
    virtual void                ProcessFrame            ( const AVFrameConstPtr & data, UInt64 avOutputID ) = 0;

    virtual void                EnableAudioChannel      ( AudioSampleType audioSampleType, UInt32 sampleRate, UInt32 channelCount ) = 0;

    virtual void                SetFrameProcessingCompletedCallback( FrameProcessingCompletedCallbackType callback  ) = 0;

	virtual std::set< UInt64 >	GetDisplayedVideoOutputsIDs() const = 0;

    virtual UInt32              GetRequiredFPS          () const = 0;

	//virtual IPlaybackControl*   GetPlaybackControl      () const = 0;
	//virtual void                EnableVideoOutput       () = 0;
	//virtual void                DisableVideoOutput      () = 0;
	//virtual bool                SetupDevice             (unsigned int deviceIndex) = 0;
	//virtual bool                ReleaseDevice           () = 0;
	//virtual const TCHAR*        GetFormatDescription    () const = 0;
	//virtual bool                SetVideoFormat          (const tstring& strDesiredFrameFormat)
	//{
	//	LOG << TEXT("SetVideoFormat is no supported");
	//	return false; 
 //   }
};

DEFINE_PTR_TYPE( IVideoCard )
DEFINE_CONST_PTR_TYPE( IVideoCard )

} //videocards
} //bv
