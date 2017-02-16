#include "VideoOutputDelegate.h"

#include "UseLoggerVideoModule.h"

#include "BlackMagicVideoCard.h"

namespace bv { namespace videocards { namespace blackmagic {

//**************************************
//
VideoOutputDelegate::VideoOutputDelegate					( VideoCard * owner )
	: m_owner( owner )
{}

//**************************************
//
HRESULT VideoOutputDelegate::ScheduledFrameCompleted		( IDeckLinkVideoFrame * completedFrame, BMDOutputFrameCompletionResult result )
{
	if( result == BMDOutputFrameCompletionResult::bmdOutputFrameDisplayedLate )
	{
		LOG_MESSAGE( SeverityLevel::warning ) << "Scheduled frame is delayed";
	}

	m_owner->FrameCompleted( completedFrame );
	return S_OK;
}

//**************************************
//
HRESULT	VideoOutputDelegate::ScheduledPlaybackHasStopped	()
{
	return S_OK;
}

//**************************************
//
VideoOutputDelegate::~VideoOutputDelegate()
{}

//**************************************
//
HRESULT	VideoOutputDelegate::QueryInterface( REFIID, LPVOID *ppv )
{
	*ppv = NULL;
	return E_NOINTERFACE;
}

//**************************************
//
ULONG	VideoOutputDelegate::AddRef()
{
	return InterlockedIncrement( ( LONG* ) &m_refCount );
}

//**************************************
//
ULONG	VideoOutputDelegate::Release()
{
	ULONG			newRefValue;

	newRefValue = InterlockedDecrement( ( LONG* ) &m_refCount );
	if( newRefValue == 0 )
	{
		delete this;
		return 0;
	}

	return newRefValue;
}

} //blackmagic
} //videocards
} //bv