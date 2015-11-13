#include "TestKeyboardHandler.h"

#include "BVAppLogic.h"
#include "Serialization/XML/XMLSerializer.h"

namespace bv {

// *********************************
//
TestKeyboardHandler::~TestKeyboardHandler()
{
}

// *********************************
//
void TestKeyboardHandler::HandleKey( unsigned char c, BVAppLogic * logic )
{
    if( c == 's' )
    {
        auto sob = new XMLSerializer();
        logic->GetBVScene()->Serialize( *sob );
        sob->Save( "test.xml" );
        delete sob;
    }

    { c; }
    { logic; }
	// To be implemented in subclasses

    //auto root = m_modelScene->GetSceneRoot();
    //auto timerPlugin = root->GetPlugin("timer");
    //if(c == 'q')
    //{
    //    model::StartTimerPlugin( timerPlugin );
    //}

    //if(c == 'w')
    //{
    //    model::StopTimerPlugin( timerPlugin );
    //}
    //   

    //if(c == 'a')
    //{
    //    model::SetTimeTimerPlugin( timerPlugin, 3600.f * 5 + 60.f * 4 + 23.f + 0.12f );
    //}

    //if(c == 's')
    //{
    //    model::SetTimeTimerPlugin( timerPlugin, 43.f + 0.88f );
    //}
        
    //FIXME: keypressed event was used here to set text in all currently loaded Text plugins
    //KeyPressedSendEvent( c );
    //TODO: implement whatever you want here

}

} //bv
