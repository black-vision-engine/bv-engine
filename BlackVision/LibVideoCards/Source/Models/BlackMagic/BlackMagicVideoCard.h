#pragma once

#include "Interfaces/IVideoCard.h"
#include "Interfaces/IVideoCardDescriptor.h"
#include "BlackMagicVideoCardDesc.h"
#include "BlackMagicUtils.h"
#include "Memory/AVFrame.h"
#include "DeckLinkAPI_h.h"
#include "BlackMagicVCThread.h"
#include "AudioVideoOutputDelegate.h"

#include <atomic>
#include <mutex>
#include <condition_variable>

#include <ctime>

namespace bv {
namespace videocards {
namespace blackmagic
{


/**@defgroup BlackMagicVideoCard BlackMagic
@ingroup VideoCards*/



// ***************************************************************
/// @ingroup BlackMagicVideoCard
class VideoCard : public IVideoCard
{
private:

    UInt32                                      m_deviceID;
    IDeckLink *                                 m_device;
    IDeckLinkOutput *                           m_decklinkOutput;
    IDeckLinkConfiguration *                    m_configuration;
	IDeckLinkKeyer *							m_keyer;
	IDeckLinkDisplayMode *						m_displayMode;
	IDeckLinkVideoConversion *					m_convertion;

	BMDTimeValue								m_frameDuration;
	BMDTimeScale								m_frameTimescale;
	UInt32										m_uiTotalFrames;

	UInt64										m_linkedVideoOutputID;
    ChannelOutputData							m_output;
    BlackMagicVCThreadUPtr						m_blackMagicVCThread;

    UInt32                                      m_audioChannelsNum;
    UInt32                                      m_audioSampleSize;

    mutable UInt64								m_lastFrameTime;

    AudioVideoOutputDelegate	*				m_audioVideoOutputDelegate;

    typedef QueueConcurrentLimited< AVFrameConstPtr >    FrameQueue;
    FrameQueue									m_frameQueue;

    mutable std::condition_variable             m_waitDisplay;
    mutable std::mutex                          m_mutex;

	mutable UInt64								m_frameNum;

    bool                                        m_audioEnabled;

    FrameProcessingCompletedCallbackType        m_frameProcessingCompletedCallback;

    static UInt32   PREROLL_FRAMES_NUM;

	void					FrameProcessed		( const AVFrameConstPtr & frame );

	bool					InitKeyer			( const ChannelOutputData & ch );

public:

    static UInt32           AvailableVideoCards;

public:
    
                            VideoCard           ( UInt32 deviceID );
    virtual                 ~VideoCard          () override;

	bool                    InitVideoCard       ();
    virtual void            SetVideoOutput      ( bool enable ) override;
    virtual void            SetVideoInput       ( VideoInputID inputId, bool enable ) override;

    virtual VideoCardID     GetVideoCardID      () const { return m_deviceID; }

    void                    AddOutput           ( ChannelOutputData output );

    virtual void            PreStart            () override;
    virtual void            Start               () override;
    virtual void            Stop                () override;
    
    virtual void            EnableAudioChannel  ( AudioSampleType audioSampleType, UInt32 sampleRate, UInt32 channelCount ) override;

	virtual void            ProcessFrame        ( const AVFrameConstPtr & data, UInt64 avOutputID ) override;
    virtual AVFramePtr      QueryInputFrame     ( VideoInputID inputID ) override;

    virtual void            SetFrameProcessingCompletedCallback( FrameProcessingCompletedCallbackType callback ) override;

	virtual std::set< UInt64 >	GetDisplayedVideoOutputsIDs () const override;
    InputChannelsDescsVec       GetInputChannelsDescs       () const override;
    OutputChannelsDescsVec      GetOutputChannelsDescs      () const override;
	
    virtual UInt32              GetRequiredFPS  () const override;

private:

	bool                    InitDevice          ();
	bool                    InitOutput          ();

	void					FrameCompleted		( IDeckLinkVideoFrame * completedFrame );
	void					DisplayNextFrame	( IDeckLinkVideoFrame * complitedFrame );

    bool					RenderAudioSamples  ( bool preroll );

	void					UpdateFrameTime		( UInt64 t );
	UInt64					GetFrameTime		() const;

    UInt32                  AudioFrameSizeInBytes() const;

    static UInt32           EnumerateDevices    ();

	friend class BlackMagicVCThread;
	friend class AudioVideoOutputDelegate;
	friend class VideoCardDesc;
};

} //bluefish
} //videocards
} //bv

//#pragma once
//#include "../../VideoCardBase.h"
//
//#include "DeckLinkAPI_h.h"
//namespace bv
//{
//
//namespace videocards{
//
//class RenderDelegate;
//
//class BlackMagicVideoCard : VideoCardBase
//{
//private:
//    IDeckLink*                      pDL;
//	IDeckLinkOutput*                pDLOutput;
//	IDeckLinkMutableVideoFrame*	    pDLVideoFrame;
//	unsigned __int32                *uiFrameWidth;
//	unsigned __int32                *uiFrameHeight;
//	BMDTimeValue                    *frameDuration;
//	BMDTimeScale                    *frameTimescale;
//    RenderDelegate                  **RenderDelegates;
//	unsigned __int32                uiFPS;
//	unsigned __int32                *uiTotalFrames;
//	CRITICAL_SECTION            	pMutex;	
//    CRITICAL_SECTION            	*Mutex;	
//
//	int                             device_id; 
//    void**	                        pFrame;
//
//    int                             ChannelsCount;
//    IDeckLinkKeyer**    			deckLinkKeyers;
//    // videocard outputs
//    IDeckLinkOutput**			    HardwareOutputs;
//    IDeckLinkMutableVideoFrame**	VideoFrames;
//    HRESULT                         ScheduledFrameCompleted (IDeckLinkVideoFrame* completedFrame, BMDOutputFrameCompletionResult result);
//public:
//	                                BlackMagicVideoCard             (void);
//	                                BlackMagicVideoCard             (unsigned int id);
//	                                ~BlackMagicVideoCard            (void);
//	virtual bool                    InitVideoCard                   ( const std::vector<int> & hackBuffersUids ) override;
//	void                            BailOut                         ();
//	void                            RouteChannel                    ();
//	void                            InitBuffer                      ();
//	void                            InitOutputChannel               ();
//	int                             InitSDK                         ();
//	bool                            Init                            ();
//	bool                            DetectVideoCard                 ();
//    void                            DeliverFrameFromRAM             (unsigned char * );
//	void                            DeliverFrameFromRAM             (std::shared_ptr<CFrame> Frame );
//    void                            Black                           ();
//    int                             getChannelsCount                ();
//    bool                            InitOutputChannels              ();
//    bool                            InitHardwareOutputs             ();
//    bool                            InitVideoOuput                  (IDeckLinkOutput *output,VideoOutputDescriptor videoMode);    
//	bool                            RenderToDevice                  (int outputId);    
//    void                            SetReferenceModeValue           (string ref);
//
//    ///GPUDirect
//    
//	bool                            InitSDKGPUDirect                ();
//    bool                            DirectGPUPreRender              ();
//    bool                            DirectGPUPostRender             ();
//    void                            DeliverFrameFromGPU             (unsigned int bufferPointer);
//};
//
//
//
//////////////////////////////////////////////
//// Render Delegate Class
//////////////////////////////////////////////
//
//class RenderDelegate : public IDeckLinkVideoOutputCallback
//{
//	BlackMagicVideoCard*	m_pOwner;
//    int OutputId;
//	
//public:
//	RenderDelegate (BlackMagicVideoCard* pOwner, int outputId);
//	~RenderDelegate ();
//	
//	// IUnknown needs only a dummy implementation
//	virtual HRESULT	STDMETHODCALLTYPE	QueryInterface (REFIID iid, LPVOID *ppv)	{{iid;ppv;} return E_NOINTERFACE;}
//	virtual ULONG	STDMETHODCALLTYPE	AddRef ()									{return 1;}
//	virtual ULONG	STDMETHODCALLTYPE	Release ()									{return 1;}
//	
//	virtual HRESULT	STDMETHODCALLTYPE	ScheduledFrameCompleted (IDeckLinkVideoFrame* completedFrame, BMDOutputFrameCompletionResult result);
//	virtual HRESULT	STDMETHODCALLTYPE	ScheduledPlaybackHasStopped ();
//};
//
//}
//}
