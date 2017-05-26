#include "stdafx.h"

#include "AVFileOutput.h"
#include "AVFileOutputHandler.h"

namespace bv
{

// **************************
//
AVFileOutput::AVFileOutput          ( unsigned int width, unsigned int height, AVFileOutputHandler * handler )
    : OutputInstance( width, height, handler )
    , m_handler( handler )
{

}

// **************************
//
AVFileOutput::~AVFileOutput         ()
{
}

// **************************
//
void AVFileOutput::StartToAVFileRendering  ( const std::string & outputFilePath )
{
    Enable();
    m_handler->StartToAVFileRendering( outputFilePath );
}

// **************************
//
void AVFileOutput::StopToAVFileRendering   ()
{
    Disable();
    m_handler->StopToAVFileRendering();
}

} // bv