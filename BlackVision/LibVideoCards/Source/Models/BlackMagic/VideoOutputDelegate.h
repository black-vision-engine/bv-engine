#pragma once

#include "BlackMagic/DeckLinkAPI_h.h"

namespace bv { namespace videocards { namespace blackmagic {

class VideoCard;

class VideoOutputDelegate : public IDeckLinkVideoOutputCallback
{
	VideoCard *				m_owner;

	ULONG					m_refCount;

	virtual					~VideoOutputDelegate();

	// IDeckLinkVideoOutputCallback
	virtual HRESULT STDMETHODCALLTYPE	ScheduledFrameCompleted		( IDeckLinkVideoFrame * completedFrame, BMDOutputFrameCompletionResult result ) override;
	virtual HRESULT	STDMETHODCALLTYPE	ScheduledPlaybackHasStopped () override;

public:
	explicit VideoOutputDelegate	( VideoCard * owner );

	// IUnknown
	virtual HRESULT	STDMETHODCALLTYPE	QueryInterface ( REFIID iid, LPVOID * ppv );
	virtual ULONG	STDMETHODCALLTYPE	AddRef ();
	virtual ULONG	STDMETHODCALLTYPE	Release ();

};

} //bluefish
} //videocards
} //bv