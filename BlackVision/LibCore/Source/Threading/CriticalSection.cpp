#include "CriticalSection.h"


namespace bv 
{

// *************************************
//
CriticalSection::CriticalSection()
    : m_mtx()
    , m_criticalSection( m_mtx, std::defer_lock )
{}
 
// *************************************
//
CriticalSection::~CriticalSection()
{}

// *************************************
//
void CriticalSection::Lock()
{ 
    m_criticalSection.lock();
}

// *************************************
//
void CriticalSection::Unlock()
{ 
    m_criticalSection.unlock();
}

} //bv
