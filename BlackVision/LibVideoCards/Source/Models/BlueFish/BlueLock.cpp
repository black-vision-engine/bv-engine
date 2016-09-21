#include "BlueLock.h"


namespace bv { namespace videocards { namespace bluefish {

BlueLock::BlueLock() :
	m_hMutex(NULL)
{
	m_hMutex = CreateMutex(NULL, FALSE, NULL);
}

BlueLock::~BlueLock()
{
	CloseHandle(m_hMutex);
}

void BlueLock::lock()
{
	WaitForSingleObject(m_hMutex, INFINITE);
}

void BlueLock::unlock()
{
	ReleaseMutex(m_hMutex);
}

} //bluefish
} //videocards
} //bv