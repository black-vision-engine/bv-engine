#include "BVAppLogic.h"

namespace bv
{

// *********************************
//
BVAppLogic::BVAppLogic      ()
    : m_startTime( 0 )
{
}

// *********************************
//
BVAppLogic::~BVAppLogic     ()
{
}

// *********************************
//
void BVAppLogic::SetStartTime    ( unsigned long millis )
{
    m_startTime = millis;
}

// *********************************
//
void BVAppLogic::OnUpdate ( unsigned long millis )
{
    
}

} //bv
