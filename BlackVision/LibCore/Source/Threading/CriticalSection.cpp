#include "stdafxCore.h"

#include "CriticalSection.h"


namespace bv 
{

// *************************************
//
CriticalSection::CriticalSection()
    : m_mtx()
{}
 
// *************************************
//
CriticalSection::~CriticalSection()
{}

// *************************************
//
void CriticalSection::Lock()
{ 
    m_mtx.lock();
}

// *************************************
//
void CriticalSection::Unlock()
{ 
    m_mtx.unlock();
}

} //bv
