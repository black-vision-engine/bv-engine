#pragma once

#include "Memory/MemoryChunk.h"
#include "CoreDEF.h"
#include "Memory/AVFrame.h"

#include "VideoInput/VideoInputChannelDesc.h"
#include "VideoOutput/VideoOutputChannelDesc.h"

#include "VideoCardManagerUtils.h"

#include <functional>
#include <set>



namespace bv {
namespace videocards
{

typedef std::vector< VideoInputChannelDesc > InputChannelsDescsVec;
typedef std::vector< VideoOutputChannelDesc > OutputChannelsDescsVec;


/**@defgroup VideoCards Video Cards*/


/**@brief VideoCard interface.
@ingroup VideoCards*/
class IVideoCard
{
public:
    typedef std::function< void ( UInt64, bool ) > FrameProcessingCompletedCallbackType;

public:

    virtual                     ~IVideoCard             () {}

    virtual void                PreStart                () = 0;
	virtual void                Start                   () = 0;
    virtual void                Stop                    () = 0;

	virtual void                SetVideoOutput          ( bool enable ) = 0;
    virtual void                SetVideoInput           ( VideoInputID inputId, bool enable ) = 0;

    virtual VideoCardID         GetVideoCardID          () const = 0;

public:

    ///@name API for editor
    ///@{

    virtual ReturnResult        SetReferenceMode        ( ReferenceMode mode ) = 0;

    virtual ReturnResult        SetReferenceH           ( UInt32 offsetH ) = 0;
    virtual ReturnResult        SetReferenceV           ( UInt32 offsetV ) = 0;

    virtual ReferenceInfo       GetReferenceInfo        () const = 0;
    virtual bool                IsLocked                () const = 0;

    ///@}


public:
    //TODO: handle frames from GPU
    virtual void                ProcessFrame            ( const AVFrameConstPtr & data, UInt64 avOutputID )     = 0;
    virtual AVFramePtr          QueryInputFrame         ( VideoInputID inputID )                                = 0;

    virtual void                EnableAudioChannel      ( AudioSampleType audioSampleType, UInt32 sampleRate, UInt32 channelCount ) = 0;

    virtual void                SetFrameProcessingCompletedCallback( FrameProcessingCompletedCallbackType callback  ) = 0;

    virtual UInt32                      GetRequiredFPS                  () const = 0;

    virtual std::set< UInt64 >	        GetDisplayedVideoOutputsIDs     () const = 0;
    virtual InputChannelsDescsVec       GetInputChannelsDescs           () const = 0;
    virtual OutputChannelsDescsVec      GetOutputChannelsDescs          () const = 0;

};

DEFINE_PTR_TYPE( IVideoCard )
DEFINE_CONST_PTR_TYPE( IVideoCard )

} //videocards
} //bv
