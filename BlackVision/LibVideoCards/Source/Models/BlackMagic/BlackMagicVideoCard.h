#pragma once

#include "Interfaces/IVideoCard.h"
#include "Interfaces/IVideoCardDescriptor.h"
#include "BlackMagicUtils.h"
#include "AVFrame.h"
#include "BlackMagic/DeckLinkAPI_h.h"
#include "BlackMagicVCThread.h"
#include "VideoOutputDelegate.h"

#include <atomic>
#include <mutex>
#include <condition_variable>

#include <ctime>

namespace bv { namespace videocards { namespace blackmagic {

// ***************************** DESCRIPTOR **********************************
//
class VideoCardDesc : public IVideoCardDesc
{
private:

    std::string             m_uid;

public:

                                            VideoCardDesc       ();

    virtual IVideoCardPtr                   CreateVideoCard     ( const IDeserializer & deser ) const override;

    virtual const std::string &             GetVideoCardUID     () const override;

};

// ***************************************************************
//
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

    ChannelOutputData							m_output;
    BlackMagicVCThreadUPtr						m_blackMagicVCThread;

    UInt64										m_lastFrameTime;

    VideoOutputDelegate	*						m_videoOutputDelegate;

    typedef QueueConcurrentLimited< AVFramePtr >    FrameQueue;
    FrameQueue									m_frameQueue;

    mutable std::condition_variable             m_waitDisplay;
    mutable std::mutex                          m_mutex;

	MemoryChunkPtr								m_prevFrame;

    FrameProcessingCompletedCallbackType        m_frameProcessingCompletedCallback;

	bool					InitKeyer			( const ChannelOutputData & ch );

public:

    static UInt32           AvailableVideoCards;

public:
    
                            VideoCard           ( UInt32 deviceID );
    virtual                 ~VideoCard          () override;

    virtual void            SetVideoOutput      ( bool enable ) override;

    void                    AddOutput           ( ChannelOutputData output );

    virtual void            Start               () override;
    virtual void            ProcessFrame        ( AVFramePtr data ) override;
    virtual void            SetFrameProcessingCompletedCallback( FrameProcessingCompletedCallbackType callback ) override;


    virtual void            DisplayFrame        () const override;

	bool                    InitVideoCard       ();
private:

	bool                    InitDevice          ();
	bool                    InitOutput          ();


	void					FrameCompleted		( IDeckLinkVideoFrame * completedFrame );
	void					DisplayNextFrame	( IDeckLinkVideoFrame * complitedFrame );

	void					UpdateFrameTime		( UInt64 t );
	UInt64					GetFrameTime		() const;

	void					InterlaceFrame		( AVFramePtr frame );

    static UInt32           EnumerateDevices    ();

	friend class VideoOutputDelegate;
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