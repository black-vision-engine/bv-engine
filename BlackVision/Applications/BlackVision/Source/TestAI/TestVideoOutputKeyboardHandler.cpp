#include "stdafxBVApp.h"

#include "TestVideoOutputKeyboardHandler.h"

#include "BVAppLogic.h"


namespace bv {

// *********************************
//
TestVideoOutputKeyboardHandler::TestVideoOutputKeyboardHandler  ()
    : m_overwriteAlpha( false )
    , m_alpha( 1.f )
{
}

// *********************************
//
void            TestVideoOutputKeyboardHandler::HandleKey       ( unsigned char c, BVAppLogic * logic )
{
    switch( c )
    {
        case '1':
            SetChannelMapping( logic, 0, 1, 2, 3 );
            break;
        case '2': //Alpha
            SetChannelMapping( logic, 3, 3, 3, 3 );
            break;
        case '3': //Red
            SetChannelMapping( logic, 0, 0, 0, 3 );
            break;
        case '4': //Green
            SetChannelMapping( logic, 1, 1, 1, 3 );
            break;
        case '5': //Blue
            SetChannelMapping( logic, 2, 2, 2, 3 );
            break;
        case '6':
            SetHeight( logic, 1080 );
            break;
        case '7':
            SetHeight( logic, 576 );
            break;
        case '8':
            SetHeight( logic, 240 );
            break;
        case '9':
            SetHeight( logic, 120 );
            break;
        case ' ':
            ToggleOverwriteAlpha( logic );
            break;
        case ']':
            AddToAlpha( logic, .1f );
            break;
        case '[':
            AddToAlpha( logic, -.1f );
            break;

        default:
            ;
    }
}

// *********************************
//
void                    TestVideoOutputKeyboardHandler::SetChannelMapping   ( BVAppLogic * logic, unsigned char rIdx, unsigned char gIdx, unsigned char bIdx, unsigned char aIdx )
{
    { logic; rIdx, gIdx, bIdx, aIdx; }
    // FIXME: nrl - implement
    //auto vrl = logic->GetRenderLogic()->GedVideoOutputRenderLogic();

    //if( vrl )
    //{
    //    vrl->SetChannelMapping( rIdx, gIdx, bIdx, aIdx );
    //}
}

// *********************************
//
void                    TestVideoOutputKeyboardHandler::ToggleOverwriteAlpha            ( BVAppLogic * logic )
{
    // FIXME: nrl - implement
    { logic; } 
    //m_overwriteAlpha = !m_overwriteAlpha;

    //auto vrl = logic->GetRenderLogic()->GedVideoOutputRenderLogic();

    //if( vrl )
    //{
    //    vrl->SetOverwriteAlpha( m_overwriteAlpha );
    //}
}

// *********************************
//
void                    TestVideoOutputKeyboardHandler::AddToAlpha                      ( BVAppLogic * logic, float val )
{
    // FIXME: nrl - implement
    { logic; val; }
    //m_alpha += val;

    //if ( m_alpha > 1.f )
    //    m_alpha = 1.f;

    //if ( m_alpha < 0.f )
    //    m_alpha = 0.f;


    //auto vrl = logic->GetRenderLogic()->GedVideoOutputRenderLogic();

    //if( vrl )
    //{
    //    vrl->SetAlpha( m_alpha );
    //}
}

// *********************************
//
void                    TestVideoOutputKeyboardHandler::SetHeight                       ( BVAppLogic * logic, int height )
{
    // FIXME: nrl - implement
    { logic; height; } 
    //auto vrl = logic->GetRenderLogic()->GedVideoOutputRenderLogic();

    //if( vrl )
    //{
    //    vrl->SetHeight( height );
    //}
}

} //bv
