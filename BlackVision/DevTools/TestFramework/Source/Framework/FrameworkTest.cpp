#include "FrameworkTest.h"
#include "BVTestAppLogic.h"


namespace bv
{

// ***********************
//
void            FrameworkTest::RunImpl      () const
{
    const_cast< FrameworkTest * >( this )->RunImplNotConst();
}

// ***********************
//
void            FrameworkTest::RunImplNotConst  ()
{


    m_appLogic->HandleFrame( 0.0f, nullptr, nullptr );
}

// ***********************
//
void            FrameworkTest::SetAppLogic      ( BVTestAppLogic * logic )
{
    m_appLogic = logic;
}


}	// bv

