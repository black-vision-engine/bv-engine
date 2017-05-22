#include "AudioVideoOutputDelegate.h"

#include "UseLoggerVideoModule.h"

#include "BlackMagicVideoCard.h"

namespace bv { namespace videocards { namespace blackmagic {

//**************************************
//
AudioVideoOutputDelegate::AudioVideoOutputDelegate					( VideoCard * owner )
	: m_owner( owner )
{}

//**************************************
//
HRESULT AudioVideoOutputDelegate::ScheduledFrameCompleted		( IDeckLinkVideoFrame * completedFrame, BMDOutputFrameCompletionResult result )
{
	if( result != BMDOutputFrameCompletionResult::bmdOutputFrameCompleted )
	{
        if( result == BMDOutputFrameCompletionResult::bmdOutputFrameDisplayedLate )
		    LOG_MESSAGE( SeverityLevel::warning ) << "Scheduled frame is delayed";
        else if( result == BMDOutputFrameCompletionResult::bmdOutputFrameDropped )
            LOG_MESSAGE( SeverityLevel::warning ) << "Scheduled frame is dropped";
        else if( result == BMDOutputFrameCompletionResult::bmdOutputFrameFlushed )
        {
            LOG_MESSAGE( SeverityLevel::warning ) << "Scheduled frame is flushed " << m_owner->m_deviceID;
            return S_OK;
        }
	}

	m_owner->FrameCompleted( completedFrame );
	return S_OK;
}

//**************************************
//
HRESULT	AudioVideoOutputDelegate::ScheduledPlaybackHasStopped	()
{
	return S_OK;
}

//**************************************
//
HRESULT AudioVideoOutputDelegate::RenderAudioSamples          ( BOOL preroll )
{
    if( m_owner->RenderAudioSamples( preroll ? true : false ) )
        return S_OK;
    else
        return E_FAIL;
}

//**************************************
//
AudioVideoOutputDelegate::~AudioVideoOutputDelegate()
{}

//**************************************
//
HRESULT	AudioVideoOutputDelegate::QueryInterface( REFIID, LPVOID *ppv )
{
	*ppv = NULL;
	return E_NOINTERFACE;
}

//**************************************
//
ULONG	AudioVideoOutputDelegate::AddRef()
{
	return InterlockedIncrement( ( LONG* ) &m_refCount );
}

//**************************************
//
ULONG	AudioVideoOutputDelegate::Release()
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