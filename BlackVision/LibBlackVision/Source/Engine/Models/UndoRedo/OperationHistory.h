#pragma once

#include "IRevertable.h"

#include <memory>
#include <vector>

namespace bv
{

class BVProjectEditor;


class OperationHistory
{
private:

    std::vector< IRevertableUPtr >      m_historyStack;
    UInt16                              m_maxSize;
    UInt16                              m_nextOp;

public:
    explicit            OperationHistory    ();
    explicit            OperationHistory    ( UInt16 historySize );


    void                AddOperation        ( IRevertableUPtr && operation );


    bool                Undo                ( BVProjectEditor * editor, Int16 numSteps );
    bool                Redo                ( BVProjectEditor * editor, Int16 numSteps );

    void                SetHistoryLength    ( UInt16 size );
};



}	// bv