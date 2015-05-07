#pragma once
#include "../../VideoCardBase.h"

#include "Log.h"
#include "DeckLinkAPI_h.h"
namespace bv
{

namespace videocards{

class RenderDelegate;

class BlackMagicVideoCard : VideoCardBase
{
private:
    IDeckLink*                      pDL;
	IDeckLinkOutput*                pDLOutput;
	IDeckLinkMutableVideoFrame*	    pDLVideoFrame;
	unsigned __int32                *uiFrameWidth;
	unsigned __int32                *uiFrameHeight;
	BMDTimeValue                    *frameDuration;
	BMDTimeScale                    *frameTimescale;
    RenderDelegate                  **RenderDelegates;
	unsigned __int32                uiFPS;
	unsigned __int32                *uiTotalFrames;
	CRITICAL_SECTION            	pMutex;	
    CRITICAL_SECTION            	*Mutex;	

	int                             device_id; 
    void**	                        pFrame;

    int                             ChannelsCount;
    IDeckLinkKeyer**    			deckLinkKeyers;
    // videocard outputs
    IDeckLinkOutput**			    HardwareOutputs;
    IDeckLinkMutableVideoFrame**	VideoFrames;
    HRESULT                         ScheduledFrameCompleted (IDeckLinkVideoFrame* completedFrame, BMDOutputFrameCompletionResult result);
public:
	                                BlackMagicVideoCard             (void);
	                                BlackMagicVideoCard             (unsigned int id);
	                                ~BlackMagicVideoCard            (void);
	virtual bool                    InitVideoCard                   ( const std::vector<int> & hackBuffersUids ) override;
	void                            BailOut                         ();
	void                            RouteChannel                    ();
	void                            InitBuffer                      ();
	void                            InitOutputChannel               ();
	int                             InitSDK                         ();
	bool                            Init                            ();
	bool                            DetectVideoCard                 ();
    void                            DeliverFrameFromRAM             (unsigned char * );
    bool                            ActivateVideoCard               ();
    bool                            DeactivateVideoCard             ();
    void                            Black                           ();
    int                             getChannelsCount                ();
    bool                            InitOutputChannels              ();
    bool                            InitHardwareOutputs             ();
    bool                            InitVideoOuput                  (IDeckLinkOutput *output,VideoOutputDescriptor videoMode);    
	bool                            RenderToDevice                  (int outputId);    
    void                            SetReferenceModeValue           (string ref);

    ///GPUDirect
    
	bool                            InitSDKGPUDirect                ();
    bool                            DirectGPUPreRender              ();
    bool                            DirectGPUPostRender             ();
    void                            DeliverFrameFromGPU             (unsigned int bufferPointer);
};



////////////////////////////////////////////
// Render Delegate Class
////////////////////////////////////////////

class RenderDelegate : public IDeckLinkVideoOutputCallback
{
	BlackMagicVideoCard*	m_pOwner;
    int OutputId;
	
public:
	RenderDelegate (BlackMagicVideoCard* pOwner, int outputId);
	~RenderDelegate ();
	
	// IUnknown needs only a dummy implementation
	virtual HRESULT	STDMETHODCALLTYPE	QueryInterface (REFIID iid, LPVOID *ppv)	{{iid;ppv;} return E_NOINTERFACE;}
	virtual ULONG	STDMETHODCALLTYPE	AddRef ()									{return 1;}
	virtual ULONG	STDMETHODCALLTYPE	Release ()									{return 1;}
	
	virtual HRESULT	STDMETHODCALLTYPE	ScheduledFrameCompleted (IDeckLinkVideoFrame* completedFrame, BMDOutputFrameCompletionResult result);
	virtual HRESULT	STDMETHODCALLTYPE	ScheduledPlaybackHasStopped ();
};

}
}