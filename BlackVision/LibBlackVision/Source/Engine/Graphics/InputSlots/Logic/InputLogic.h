#pragma once


#include "IInputHandler.h"
#include "Engine/Graphics/InputSlots/InputSlots.h"

#include <vector>


namespace bv
{

class RenderContext;


// ***********************
//
class InputLogic
{
private:

    InputSlotsPtr                          m_inputSlots;
    std::vector< IInputHandlerPtr >        m_inputsHandlers;

    std::vector< IInputHandlerPtr >        m_inputsToRegister;      ///< Deffered initialization.

public:

    explicit            InputLogic      ();
    virtual             ~InputLogic     ();

    void                ProcessInputs   ( RenderContext * ctx );


public:

    void                AppendInputHandler  ( IInputHandlerPtr newHandler );

public:

    InputSlotsPtr       GetInputSlots       () const { return m_inputSlots; }
};




}	// bv
