
#include "Channel.h"
namespace bv
{

namespace videocards{
//**************************************
//
Channel::Channel( std::string name, std::string type, unsigned short renderer, unsigned short resolution, unsigned short refresh, bool interlaced, bool flipped, bool playback, bool capture,bool playthrough, std::string inputType, long referenceMode, int refH, int refV )
{
	m_Name = name;
	m_Type = type;
	m_Renderer = renderer;
	m_Resolution = resolution;
	m_Refresh = refresh;
	m_Interlaced = interlaced;
	m_Flipped = flipped;
	m_Playback = playback;
	m_Capture = capture;
	m_playthrough = playthrough;
	m_InputType = inputType;
	m_referenceMode = referenceMode;
	m_refH = refH;
	m_refV = refV;
	m_VideoMode = ParseVideoMode(m_Resolution, m_Refresh, m_Interlaced);
	if(capture) this->m_CaptureChannel = new CFifoCapture();
	if(playback) this->m_PlaybackChannel = new CFifoPlayback();
}

//**************************************
//
Channel::~Channel()
{
    if(m_playthrough)
	{
	    m_PlaythroughThreadArgs->bDoRun = FALSE;
        DWORD dw = WaitForSingleObject(m_PlaythroughThreadHandle, (DWORD)-1);
		{dw;}
		CloseHandle(m_PlaythroughThreadHandle);
	    //delete m_PlaythroughThreadArgs->pInputFifo;
	   // delete m_PlaythroughThreadArgs->pOutputFifo;
	    //delete m_PlaythroughThreadArgs;
		//m_PlaythroughThreadArgs = NULL;
    }

	if(m_Playback)
	{
		m_PlaybackChannel->StopPlaybackThread();
		//delete m_PlaybackChannel;
		//m_PlaybackChannel = NULL;
	}

    if(m_Capture)
    {
	    m_CaptureChannel->StopCaptureThread();
	    //delete m_CaptureChannel;
	    //m_CaptureChannel = NULL;
    }


}

//**************************************
//
std::string Channel::GetName()
{
	return m_Name;
}

//**************************************
//
std::string Channel::GetType()
{
	return m_Type;
}
//**************************************
//
std::string Channel::GetInputType()
{
	return m_InputType;
}
//**************************************
//
CFifoBuffer* Channel::GetCaptureBuffer()
{
	return &m_CaptureFifoBuffer;
}

//**************************************
//
CFifoBuffer* Channel::GetPlaybackBuffer()
{
	return &m_PlaybackFifoBuffer;
}

//**************************************
//
void Channel::InitThreads()
{
	 if(m_Capture)this->m_CaptureChannel->InitThread();
	if(m_Playback) this->m_PlaybackChannel->InitThread();
}

//**************************************
//
void Channel::StartThreads()
{
	 if(m_Capture)this->m_CaptureChannel->StartCaptureThread();
	if(m_Playback)this->m_PlaybackChannel->StartPlaybackThread();
}
//**************************************
//
void Channel::StopThreads()
{
	if(m_Playback)this->m_PlaybackChannel->StopPlaybackThread();
	 if(m_Capture)this->m_CaptureChannel->StopCaptureThread();
}
//**************************************
//
void Channel::SuspendThreads()
{
	if(m_Playback)this->m_PlaybackChannel->SuspendPlaybackThread();
	 if(m_Capture)this->m_CaptureChannel->SuspendCaptureThread();
}
//**************************************
//
void Channel::ResumeThreads()
{
	 if(m_Capture)this->m_CaptureChannel->ResumeCaptureThread();
	if(m_Playback)this->m_PlaybackChannel->ResumePlaybackThread();
}
//**************************************
//
CFifoCapture* Channel::GetCaptureChannel()
{
	return m_CaptureChannel;
}
//**************************************
//
CFifoPlayback* Channel::GetPlaybackChannel()
{
	return m_PlaybackChannel;
}

//**************************************
//
void Channel::GenerateBlack()
{    
    if(GetPlaybackChannel()->m_pSDK)
    {
        VARIANT varVal;       
        varVal.ulVal = ENUM_BLACKGENERATOR_ON;
        GetPlaybackChannel()->m_pSDK->SetCardProperty(VIDEO_BLACKGENERATOR, varVal);

    }
    
    else
        cout << "BlueFish Playback SDK not INITIALISED" << endl;
}

//**************************************
//
unsigned int __stdcall Channel::PlaythroughThread(void * pArg)
{
	MainThreadArgs* pParams = (MainThreadArgs*)pArg;
	//std::shared_ptr<CFrame> pFrameIn = NULL;
	//std::shared_ptr<CFrame> pFrameOut = NULL;
	//BLUE_UINT32 FrameNumber = 0;
	//BLUE_UINT32 Thickness = 100;	//in lines;
	cout << "Thread " << pParams->strChannel.c_str() << " running." << endl;

	while(pParams->bDoRun)
	{
		//if(!pFrameIn)
			//threadsafe
			//pFrameIn = pParams->pInputFifo->GetLiveBuffer();
			//pFrameIn = pParams->pInputFifo->m_threadsafebuffer.pop();

		//if(true)
		//{
		//	//pFrameOut = std::make_shared<CFrame>(1,pParams->pOutputFifo->m_GoldenSize, pParams->pOutputFifo->m_BytesPerLine);//GoldenSize,pThis->BytesPerLine);//pParams->pOutputFifo->m_threadsafebuffer.pop();//pParams->pOutputFifo->GetFreeBuffer();
		//	if(true)
		//	{
		//		//copy In to Out
		//		if(true)//pFrameIn->m_nSize == pFrameOut->m_nSize)
		//		{
					//if(pParams->bModifyCapturedFrame)
					//{
						//the black line will only shift by 2 lines per update
					//	BlueMemZero((pFrameIn->m_pBuffer+pFrameIn->m_nSize/2+pFrameIn->m_nBytesPerLine*2*FrameNumber), pFrameIn->m_nBytesPerLine*Thickness);
					//	FrameNumber = (++FrameNumber%100);	//this is just arbitrary to get our "Animation" to use 100 frames before repeating
					//}
					//BlueMemCpy(pFrameOut->m_pBuffer, pFrameIn->m_pBuffer, pFrameIn->m_nSize);
					//pFrameOut->m_lFieldCount = pFrameIn->m_lFieldCount;		

					pParams->pOutputFifo->m_threadsafebuffer.push(pParams->pInputFifo->m_threadsafebuffer.pop());

					//cout << pParams->strChannel.c_str();
				}
	
				//else
				//{
				//	cout << "Input/Output buffers don't match!" << endl;
				//	pParams->bDoRun = FALSE;
				//	break;
				//}

	
				/*pParams->pInputFifo->PutFreeBuffer(pFrameIn);
				pFrameIn = NULL;
				pParams->pOutputFifo->PutLiveBuffer(pFrameOut);
				pFrameOut = NULL;*/
	//		}
	//	}
	//}

	cout << "Thread " << pParams->strChannel.c_str() << " exiting." << endl;
	_endthreadex(0);
	return 0;
}

//**************************************
//
void Channel::StartDuplexThread()
{

	if(m_playthrough)
	{
		m_PlaythroughThreadArgs = new MainThreadArgs;
		m_PlaythroughThreadArgs->bDoRun = TRUE;
        m_PlaythroughThreadArgs->pInputFifo = GetCaptureBuffer();//>m_CaptureFifoBuffer;
        m_PlaythroughThreadArgs->pOutputFifo = GetPlaybackBuffer();//&this->m_PlaybackFifoBuffer;
		m_PlaythroughThreadArgs->strChannel = this->m_Name;
		m_PlaythroughThreadID = 0;
		m_PlaythroughThreadHandle = (HANDLE)_beginthreadex(NULL, 0, &PlaythroughThread, m_PlaythroughThreadArgs, CREATE_SUSPENDED, &m_PlaythroughThreadID);
		if(!m_PlaythroughThreadHandle)
		{
			cout << "Error starting Main Thread StartDuplexThread" << endl;
			delete this->m_CaptureChannel;
			delete this->m_PlaybackChannel;
		}
		SetThreadPriority(m_PlaythroughThreadHandle, THREAD_PRIORITY_TIME_CRITICAL);
	}

    if(m_Capture) this->m_CaptureChannel->StartCaptureThread();	//this actually just resumes the threads; it would take too long to start them from scracth
    if(m_Playback) this->m_PlaybackChannel->StartPlaybackThread();	//that's why we created them before as suspended

	if(m_playthrough) ResumeThread(m_PlaythroughThreadHandle);
}

//**************************************
//
ULONG Channel::ParseVideoMode(unsigned short resolution, unsigned short refresh, bool interlaced)
{
	ULONG VideoMode=100;
	if(interlaced)
	{
		switch(resolution)
		{
		 case 1080:
			 {
				switch(refresh)
				{
				 case 5000:
					 {
						 VideoMode = VID_FMT_1080I_5000;
						 break;
					 }
				 case 5994:
					 {
						 VideoMode = VID_FMT_1080I_5994;
						 break;
					 }
				 case 6000:
					 {
						 VideoMode = VID_FMT_1080I_6000;
						 break;
					 }
				 default:
					 {
						 cout << "Unknown refresh rate for Interlaced 1080 resolution" << endl;
					 }
				}
				break;
			 }
		 case 576:
			 {
				switch(refresh)
				{
				 case 5000:
					 {
						 VideoMode = VID_FMT_576I_5000;
						 break;
					 }
				 default:
					 {
						 cout << "Unknown refresh rate for Interlaced 576 resolution" << endl;
					 }
				}
				break;
			 }
		 case 486:
			 {
				switch(refresh)
				{
				 case 5994:
					 {
						 VideoMode = VID_FMT_486I_5994;
						 break;
					 }
				 default:
					 {
						 cout << "Unknown refresh rate for Interlaced 486 resolution" << endl;
					 }
				}
				break;
			 }
		 default:
			 {
				 cout << "Unknown Resolution for Interlaced Setting" << endl;
				 break;
			 }
		}
	}
	else
	{
		switch(resolution)
		{
		 case 1080:
			 {
				switch(refresh)
				{
				 case 2397:
					 {
						 VideoMode = VID_FMT_1080P_2397;
						 break;
					 }
				 case 2400:
					 {
						 VideoMode = VID_FMT_1080P_2400;
						 break;
					 }
				 case 2500:
					 {
						 VideoMode = VID_FMT_1080P_2500;
						 break;
					 }
				 case 2997:
					 {
						 VideoMode = VID_FMT_1080P_2997;
						 break;
					 }
				 case 3000:
					 {
						 VideoMode = VID_FMT_1080P_3000;
						 break;
					 }
				 case 4800:
					 {
						 VideoMode = VID_FMT_1080P_4800;
						 break;
					 }
				 case 5000:
					 {
						 VideoMode = VID_FMT_1080P_5000;
						 break;
					 }
				 case 5994:
					 {
						 VideoMode = VID_FMT_1080P_5994;
						 break;
					 }
				 case 6000:
					 {
						 VideoMode = VID_FMT_1080P_6000;
						 break;
					 }
				 default:
					 {
						 cout << "Unknown refresh rate for Progressive 1080 resolution" << endl;
					 }
				}
				break;
			 }
		 case 720:
			 {
				switch(refresh)
				{
				 case 2398:
					 {
						 VideoMode = VID_FMT_720P_2398;
						 break;
					 }
				 case 2400:
					 {
						 VideoMode = VID_FMT_720P_2400;
						 break;
					 }
				 case 2500:
					 {
						 VideoMode = VID_FMT_720P_2500;
						 break;
					 }
				 case 2997:
					 {
						 VideoMode = VID_FMT_720P_2997;
						 break;
					 }
				 case 3000:
					 {
						 VideoMode = VID_FMT_720P_3000;
						 break;
					 }
				 case 5000:
					 {
						 VideoMode = VID_FMT_720P_5000;
						 break;
					 }
				 case 5994:
					 {
						 VideoMode = VID_FMT_720P_5994;
						 break;
					 }
				 case 6000:
					 {
						 VideoMode = VID_FMT_720P_6000;
						 break;
					 }
				 default:
					 {
						 cout << "Unknown refresh rate for Progressive 720 resolution" << endl;
					 }
				}
				break;
			 }
		 default:
			 {
				cout << "Unknown resolution" << endl;
				break;
			 }
		}
	}

	if(VideoMode==100)  
        cout << "UNKNOWN RESOLUTION/REFRESH RATE CONFIGURATION" << endl;

	return VideoMode;
}

}
}