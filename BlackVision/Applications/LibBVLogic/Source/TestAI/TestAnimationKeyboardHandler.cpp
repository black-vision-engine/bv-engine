#include "stdafxBVApp.h"

#include "TestAnimationKeyboardHandler.h"

#include "Engine/Models/BVProjectEditor.h"

#include "TestingScenes/MockScenes.h"
#include "BVAppLogic.h"


namespace bv {

// *********************************
//
void    TestAnimationKeyboardHandler::HandleKey   ( unsigned char c, BVAppLogic * logic )
{
    { c; }
    { logic; }
/*
    // FIXME: the code below is must be used with an animation plugin
    unsigned char d = c - '0';

    if( d <= 10 )
    {
        auto root = m_modelScene->GetSceneRoot();

        SetParameter( root->GetPlugin( "animation" )->GetParameter( "frameNum" ), TimeType( 0.f ), float( d ) );
    }

    if( c == 'i' || c == 'I' )
    {
        m_renderLogic->PrintGLStats( c == 'I' );
    }
*/
}

} //bv
