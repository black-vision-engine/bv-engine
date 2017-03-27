#pragma once

#include "BlackMagic/DeckLinkAPI_h.h"

namespace bv { namespace videocards { namespace blackmagic {

class VideoCard;

class AudioVideoOutputDelegate : public IDeckLinkVideoOutputCallback, public IDeckLinkAudioOutputCallback
{
	VideoCard *				m_owner;

	ULONG					m_refCount;

	virtual					~AudioVideoOutputDelegate();

	// IDeckLinkVideoOutputCallback
	virtual HRESULT STDMETHODCALLTYPE	ScheduledFrameCompleted		( IDeckLinkVideoFrame * completedFrame, BMDOutputFrameCompletionResult result ) override;
	virtual HRESULT	STDMETHODCALLTYPE	ScheduledPlaybackHasStopped () override;
    virtual HRESULT STDMETHODCALLTYPE	RenderAudioSamples          ( BOOL preroll ) override;

public:
	explicit AudioVideoOutputDelegate ( VideoCard * owner );

	// IUnknown
	virtual HRESULT	STDMETHODCALLTYPE	QueryInterface ( REFIID iid, LPVOID * ppv );
	virtual ULONG	STDMETHODCALLTYPE	AddRef ();
	virtual ULONG	STDMETHODCALLTYPE	Release ();

};

} //bluefish
} //videocards
} //bv