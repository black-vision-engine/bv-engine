
#include "FifoBuffer.h"
namespace bv
{

namespace videocards{
using namespace std;

/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
CFifoBuffer::CFifoBuffer() 
	//m_hFreeMutexEmpty(NULL),
	//m_hFreeMutexLive(NULL),
	//m_hLiveMutexEmpty(NULL),
	//m_hLiveMutexLive(NULL)
{
}

CFifoBuffer::~CFifoBuffer()
{
    //m_threadsafebuffer.q_.destroy();
    //m_threadsafebuffer.clear();
    /*
	while(!m_qFreeBuffers.empty())
	{
		m_FreeLock.lock();
		m_qFreeBuffers.front();
		m_qFreeBuffers.pop();
		m_FreeLock.unlock();
	}

	while(!m_qLiveBuffers.empty())
	{
		m_LiveLock.lock();
		m_qLiveBuffers.front();
		m_qLiveBuffers.pop();
		m_LiveLock.unlock();
	}

	CFrame* pFrame = NULL;
	while(!m_qBuffers.empty())
	{
		pFrame = m_qBuffers.front();
		m_qBuffers.pop();
		delete pFrame;
		pFrame = NULL;
	}

	if(m_hFreeMutexEmpty)	CloseHandle(m_hFreeMutexEmpty);	m_hFreeMutexEmpty = NULL;
	if(m_hFreeMutexLive)	CloseHandle(m_hFreeMutexLive);	m_hFreeMutexLive = NULL;
	if(m_hLiveMutexEmpty)	CloseHandle(m_hLiveMutexEmpty);	m_hLiveMutexEmpty = NULL;
	if(m_hLiveMutexLive)	CloseHandle(m_hLiveMutexLive);	m_hLiveMutexLive = NULL;
    */
}

void CFifoBuffer::Init(BLUE_UINT32 Count, BLUE_UINT32 Size, BLUE_UINT32 BytesPerLine)
{
	m_GoldenSize = Size;
	m_BytesPerLine = BytesPerLine;
	{Count;}
	/*CFrame* pFrame = NULL;
	for(BLUE_UINT32 n=0; n<Count; n++)
	{
		pFrame = new CFrame(n, Size, BytesPerLine);
		m_qBuffers.push(pFrame);
		m_qFreeBuffers.push(pFrame);
	}



	if(m_hFreeMutexEmpty)	CloseHandle(m_hFreeMutexEmpty);	m_hFreeMutexEmpty = NULL;
	if(m_hFreeMutexLive)	CloseHandle(m_hFreeMutexLive);	m_hFreeMutexLive = NULL;
	if(m_hLiveMutexEmpty)	CloseHandle(m_hLiveMutexEmpty);	m_hLiveMutexEmpty = NULL;
	if(m_hLiveMutexLive)	CloseHandle(m_hLiveMutexLive);	m_hLiveMutexLive = NULL;

	m_hFreeMutexEmpty = CreateSemaphore(NULL, 0, (LONG)Count, NULL);	//signal that we can't add any buffers to the free queue
    m_hFreeMutexLive = CreateSemaphore(NULL, (LONG)Count, (LONG)Count, NULL);	//signal that we can add a buffer to the live queue as
																				//there are no buffers in the live queue
	m_hLiveMutexEmpty = CreateSemaphore(NULL, (LONG)Count, (LONG)Count, NULL);
    m_hLiveMutexLive = CreateSemaphore(NULL, 0, (LONG)Count, NULL);*/
}
/*
void CFifoBuffer::PutFreeBuffer(CFrame* pFrame)
{
	if(!pFrame)
		return;

	DWORD dwWaitResult = WaitForSingleObject(m_hFreeMutexEmpty, 100);	//decreases the semaphore
	if(dwWaitResult == WAIT_OBJECT_0)
	{
		m_FreeLock.lock();
		m_qFreeBuffers.push(pFrame);
		m_FreeLock.unlock();
		ReleaseSemaphore(m_hFreeMutexLive, 1, NULL);
	}
}

void CFifoBuffer::PutLiveBuffer(CFrame* pFrame)
{
	if(!pFrame)
		return;

	DWORD dwWaitResult = WaitForSingleObject(m_hLiveMutexEmpty, 100);
	if(dwWaitResult == WAIT_OBJECT_0)
	{
		m_LiveLock.lock();
		m_qLiveBuffers.push(pFrame);
		m_LiveLock.unlock();
		ReleaseSemaphore(m_hLiveMutexLive, 1, NULL);
	}
}

CFrame* CFifoBuffer::GetFreeBuffer()
{
	CFrame* pFrame = NULL;
	//cout << "GetFreeBuffer" << endl;
	//DWORD dwWaitResult = WaitForSingleObject(m_hFreeMutexLive, 100);	//decreases the semaphore
	DWORD dwWaitResult = WaitForSingleObject(m_hFreeMutexLive, 0);	//decreases the semaphore
	if(dwWaitResult == WAIT_OBJECT_0)
	{
		m_FreeLock.lock();
		if(!m_qFreeBuffers.empty())
		{
			pFrame = m_qFreeBuffers.front();
			m_qFreeBuffers.pop();
		}
		m_FreeLock.unlock();
		ReleaseSemaphore(m_hFreeMutexEmpty, 1, NULL);
	}

	return pFrame;
}

CFrame* CFifoBuffer::GetLiveBuffer()
{
	CFrame* pFrame = NULL;
	//cout << "GetLiveBuffer" << endl;
	//DWORD dwWaitResult = WaitForSingleObject(m_hLiveMutexLive, 100);	//decreases the semaphore
	DWORD dwWaitResult = WaitForSingleObject(m_hLiveMutexLive, 0);	//decreases the semaphore
	if(dwWaitResult == WAIT_OBJECT_0)
	{
		m_LiveLock.lock();
		if(!m_qLiveBuffers.empty())
		{
			pFrame = m_qLiveBuffers.front();
			m_qLiveBuffers.pop();
		}
		m_LiveLock.unlock();
		ReleaseSemaphore(m_hLiveMutexEmpty, 1, NULL);
	}

	return pFrame;
}
*/
}
}