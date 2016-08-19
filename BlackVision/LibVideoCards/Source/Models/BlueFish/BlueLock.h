#pragma once

#include "win_sock.h"


namespace bv { namespace videocards { namespace bluefish {

class BlueLock
{
public:

	BlueLock();
	~BlueLock();

	void lock();
	void unlock();

private:

	HANDLE	m_hMutex;

};

} //bluefish
} //videocards
} //bv