#include "stdafx.h"
#include "InputLogic.h"

#include <algorithm>

#include "UseLoggerLibBlackVision.h"


namespace bv
{

// ***********************
//
InputLogic::InputLogic()
    : m_inputSlots( std::make_shared< InputSlots >() )
{}

// ***********************
//
InputLogic::~InputLogic()
{
    // This shouldn't be necassary, but better to unregister all handlers in case
    // they make something compilcated.
    for( auto inputHandler : m_inputsHandlers )
    {
        inputHandler->UnregisterInputs();
    }
}

// ***********************
//
void            InputLogic::ProcessInputs       ( RenderContext * ctx )
{
    for( auto inputHandler : m_inputsHandlers )
    {
        inputHandler->ProcessInputs( ctx );
    }
}

// ***********************
//
void            InputLogic::AppendInputHandler  ( RenderContext * ctx, IInputHandlerPtr newHandler )
{
    if( std::find( m_inputsHandlers.begin(), m_inputsHandlers.end(), newHandler ) != m_inputsHandlers.end() )
    {
        newHandler->RegisterInputs( ctx, m_inputSlots );
        m_inputsHandlers.push_back( newHandler );
    }
    else
    {
        LOG_MESSAGE( SeverityLevel::warning ) << "Can't add input handler to InputLogic. Handler already existed.";
    }
}


}	// bv

