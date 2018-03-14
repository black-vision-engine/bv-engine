#pragma once

#include "Interfaces/IVideoCard.h"
#include "Interfaces/IVideoCardDescriptor.h"
#include "Memory/AVFrame.h"

#include "Channel.h"
#include "Output/OutputChannel.h"
#include "Input/InputChannel.h"

#include "BlueFishVideoCardDesc.h"



namespace bv {
namespace videocards {
namespace bluefish
{

DEFINE_PTR_TYPE( CBlueVelvet4 )



/**@defgroup BlueFishVideoCard BlueFish
@ingroup VideoCards*/


// ***********************
/// @ingroup BlueFishVideoCard
class VideoCard : public IVideoCard
{
private:

	CBlueVelvet4Ptr         m_SDK;
	UInt32                  m_deviceID;

	UInt32                  m_updFmt;
	UInt32                  m_memFmt;
	UInt32                  m_engineMode;


	UInt32                  m_resolution;
	UInt32                  m_refresh;
    bool                    m_interlaced;
    bool                    m_flipped;


	VARIANT                 varVal;
    ULONG                   GoldenSize;
	ULONG					PixelsPerLine;
	ULONG					VideoLines;
    ULONG                   FieldCount;

	std::vector< Channel * > m_channels;

public:
	                        VideoCard                   ( UInt32 deviceID );
	virtual                 ~VideoCard                  () override; 

    bool                    AttachVideoCard             ();
    bool                    DetachVideoCard             ();
	void                    InitVideoCard               ();

    virtual void            SetVideoOutput              ( bool enable ) override;
    virtual void            SetVideoInput               ( VideoInputID inputId, bool enable ) override;

    virtual VideoCardID     GetVideoCardID              () const { return m_deviceID; }

	void				    AddChannel					( Channel * channel );
	void                    RouteChannel                ( ULONG source, ULONG destination, ULONG linkType );
	Channel *			    GetChannelByName			( ChannelName channelName ) const;   

    void                    PreStart                    () override;
    virtual void            Start                       () override;
    void                    Stop                        () override;

    virtual void            ProcessFrame                ( const AVFrameConstPtr & avFrame, UInt64 avOutputID ) override;
    virtual AVFramePtr      QueryInputFrame             ( VideoInputID inputID ) override;

    std::set< UInt64 >	    GetDisplayedVideoOutputsIDs () const override;
    InputChannelsDescsVec   GetInputChannelsDescs       () const override;
    OutputChannelsDescsVec  GetOutputChannelsDescs      () const override;

    virtual void            SetFrameProcessingCompletedCallback( FrameProcessingCompletedCallbackType ) override {}

    virtual void            EnableAudioChannel      ( AudioSampleType , UInt32 , UInt32 ) override {};

    static UInt32           EnumerateDevices            ();

    UInt32                  GetRequiredFPS              () const override;
};

DEFINE_PTR_TYPE( VideoCard )

} //bluefish
} //videocards
} //bv
