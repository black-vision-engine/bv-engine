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
    // Deffered initialization of inputs.
    if( m_inputsToRegister.size() > 0 )
    {
        for( auto & input : m_inputsToRegister )
        {
            input->RegisterInputs( ctx, m_inputSlots );
            m_inputsHandlers.push_back( input );
        }

        m_inputsToRegister.clear();
    }

    for( auto inputHandler : m_inputsHandlers )
    {
        inputHandler->ProcessInputs( ctx );
    }
}

// ***********************
//
void            InputLogic::AppendInputHandler  ( IInputHandlerPtr newHandler )
{
    if( std::find( m_inputsHandlers.begin(), m_inputsHandlers.end(), newHandler ) == m_inputsHandlers.end() &&
        std::find( m_inputsToRegister.begin(), m_inputsToRegister.end(), newHandler ) == m_inputsToRegister.end() )
    {
        m_inputsToRegister.push_back( newHandler );
    }
    else
    {
        LOG_MESSAGE( SeverityLevel::warning ) << "Can't add input handler to InputLogic. Handler already existed.";
    }
}


}	// bv

