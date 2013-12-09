#include "MockFrameReader.h"

#include "Engine/Events/Interfaces/IEventManager.h"
#include "Engine/Events/Events.h"


namespace bv
{

// *********************************
//
MockFrameReader::MockFrameReader    ()
{
    m_buf = new char[ 2048 * 2048 * 4 ];

    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &MockFrameReader::ProcessFrame ), FrameRenderedEvent::Type() );

    printf( "Mock Frame Reader created and registered to global Event Manager\n" );
}


// *********************************
//
MockFrameReader::~MockFrameReader   ()
{
    delete[]  m_buf;
}

// *********************************
//
void MockFrameReader::ProcessFrame  ( IEventPtr evt )
{
    static int w = 0;
    static int h = 0;

    FrameRenderedEventPtr evtTyped = std::static_pointer_cast<FrameRenderedEvent>( evt );

    if( w != evtTyped->Width() || h != evtTyped->Height() )
    {
        w = evtTyped->Width();
        h = evtTyped->Height();

        printf( "Framebuffer resulotion changed to %dx%d\n", w, h );
    }

    //TODO: instead of this memcpy some functions pushing data to DeckLink should be called here
    //memcpy( m_buf, evtTyped->FrameDataPtr(), w * h * 4 );
}

} //bv
