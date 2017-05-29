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
    PreEvents();

    PostEvents();

    PreModelUpdate();

    PreRender();

    PostRender();

    //m_appLogic->HandleFrame( 0.0f, nullptr, nullptr );
}

// ***********************
//
void            FrameworkTest::SetAppLogic      ( BVTestAppLogic * logic )
{
    m_appLogic = logic;
}


// ========================================================================= //
// User interaction functions
// ========================================================================= //

// ***********************
//
void            FrameworkTest::EndTestAfterThisFrame        ( bool value )
{
    m_isLastFrame = value;
}


}	// bv

