#include "FrameworkTest.h"
#include "BVTestAppLogic.h"

#include <excpt.h>

int filter( unsigned int code, struct _EXCEPTION_POINTERS * ) {

    CHECK( false );
//    EndTestAfterThisFrame( true );

    puts( "in filter." );

    if( code == EXCEPTION_ACCESS_VIOLATION )
    {

        puts( "caught AV as expected." );

        return EXCEPTION_EXECUTE_HANDLER;

    }

    else
    {

        puts( "didn't catch AV, unexpected." );

        return EXCEPTION_CONTINUE_SEARCH;

    };

}

namespace bv
{

extern HighResolutionTimer GTimer;



// ***********************
//
void            FrameworkTest::RunImpl      () const
{
    const_cast< FrameworkTest * >( this )->PreRunImpl();
    const_cast< FrameworkTest * >( this )->RunImplNotConst();
    const_cast< FrameworkTest * >( this )->PostRunImpl();
}

// ***********************
//
void            FrameworkTest::RunImplNotConst  ()
{
    __try
    {
        m_frameTime = ComputeFrameTimeImpl();

        // Events
        PreEvents();
        m_appLogic->EventsPhase();

        // Model
        PreModelUpdate();
        m_appLogic->ModelUpdatePhase( m_frameTime );

        // Engine
        PreRender();
        m_appLogic->RenderPhase( m_frameTime, m_appLogic->m_renderer, m_appLogic->m_audioRenderer );
        PostRender();

        GTimer.StartTimer();
    }
    __except( filter( GetExceptionCode(), GetExceptionInformation() ) )
    {
    }
    //catch( const std::exception & ex )
    //{
    //    CHECK( !ex.what() );
    //    EndTestAfterThisFrame( true );
    //}
    //catch( ... )
    //{
    //    CHECK( !"Unexpected exception" );
    //    EndTestAfterThisFrame( true );
    //}
}

// ***********************
//
BVProjectEditor *   FrameworkTest::GetProjectEditor     () const
{
    return m_appLogic->GetBVProject()->GetProjectEditor();
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
{    m_isLastFrame = value;     }

// ***********************
//
void FrameworkTest::UseOverridenTime( bool value )
{    m_overrideTime = value;    }

// ========================================================================= //
// Internal
// ========================================================================= //

// ***********************
//
TimeType        FrameworkTest::ComputeFrameTimeImpl()
{
    if( m_overrideTime )
        return ComputeFrameTime();
    else
        return m_appLogic->ComputeFrameTime();
}

// ***********************
//
void            FrameworkTest::PreRunImpl()
{
    // Empty for now...
}

// ***********************
//
void            FrameworkTest::PostRunImpl()
{
    m_frameNum++;
}


}	// bv

