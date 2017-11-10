#pragma once


#include "IInputHandler.h"
#include "Engine/Graphics/InputSlots/InputSlots.h"

#include <vector>


namespace bv
{

// ***********************
//
class InputLogic
{
private:

    InputSlotsPtr                          m_inputSlots;
    std::vector< IInputHandlerPtr >        m_inputsHandlers;

public:

    explicit            InputLogic      ();
    virtual             ~InputLogic     ();

    void                ProcessInputs   ();


public:

    void                AddInputHandler ( IInputHandlerPtr newHandler );

public:

    InputSlotsPtr       GetInputSlots   () const { return m_inputSlots; }
};




}	// bv
