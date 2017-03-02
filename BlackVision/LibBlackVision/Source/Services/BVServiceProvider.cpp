#include "stdafx.h"
#include "BVServiceProvider.h"

namespace bv 
{

// ******************************
// 
BVServiceProvider::BVServiceProvider()
    : m_vcm( nullptr )
{}

// ******************************
// 
void                                BVServiceProvider::RegisterVideoCardManager( videocards::VideoCardManager * vcm )
{
    m_vcm = vcm;
}

#ifdef BV_TESTS
// ******************************
// 
void                                BVServiceProvider::RegisterBVProject       ( BVProject * bvProj )
{
    m_bvProj = bvProj;
}

// ******************************
// 
BVProject *                         BVServiceProvider::GetBVProject             ()
{
    return m_bvProj;
}

#endif // BV_TESTS

// ******************************
// 
videocards::VideoCardManager *      BVServiceProvider::GetVideoCardManager()
{
    return m_vcm;
}

// ******************************
//
BVServiceProvider &                 BVServiceProvider::GetInstance()
{
    static BVServiceProvider instance;
    return instance;
}

} // bv