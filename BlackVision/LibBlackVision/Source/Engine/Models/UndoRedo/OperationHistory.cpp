#include "stdafx.h"
#include "OperationHistory.h"

#include "Mathematics/glm_inc.h"



#include "Memory/MemoryLeaks.h"



namespace bv
{

// ***********************
//
OperationHistory::OperationHistory    ()
    :   m_maxSize( 15 )
    ,   m_nextOp( 0 )
{
    m_historyStack.reserve( 15 );
}

// ***********************
//
OperationHistory::OperationHistory    ( UInt16 historySize )
    : m_maxSize( historySize )
    , m_nextOp( 0 )
{
    m_historyStack.reserve( historySize );
}

// ***********************
//
void        OperationHistory::AddOperation        ( IRevertableUPtr && operation )
{
    if( m_maxSize != 0 )
    {
        // If there're elements after our m_nextOp pointer, they must be forgotten;
        if( m_nextOp < m_historyStack.size() )
        {
            m_historyStack.erase( m_historyStack.begin() + m_nextOp, m_historyStack.end() );
        }

        // If history stack reached it's max size, we forget first element in vector.
        if( m_historyStack.size() == m_maxSize )
        {
            m_historyStack.erase( m_historyStack.begin() );
            m_nextOp--;
        }


        m_historyStack.push_back( std::move( operation ) );
        m_nextOp++;
    }
}

// ***********************
//
void        OperationHistory::SetHistoryLength    ( UInt16 size )
{
    m_maxSize = size;
    
    if( m_nextOp > size )
    {
        // Delete operations from bottom of stack.
        SizeType diff = size - m_nextOp;
        m_historyStack.erase( m_historyStack.begin(), m_historyStack.begin() + diff );
    }

    m_historyStack.reserve( size );
}

// ***********************
//
bool        OperationHistory::Undo                ( BVProjectEditor * editor, Int16 numSteps )
{
    if( m_maxSize != 0 )
    {
        bool result = true;

        if( m_nextOp < numSteps )
        {
            // We can't revert so many steps.
            result = false;
            numSteps = m_nextOp;
        }

        while( numSteps > 0 )
        {
            m_nextOp--;
            m_historyStack[ m_nextOp ]->Undo( editor );

            numSteps--;
        }

        return result;
    }
    return false;
}

// ***********************
//
bool        OperationHistory::Redo                ( BVProjectEditor * editor, Int16 numSteps )
{
    if( m_maxSize != 0 )
    {
        bool result = true;

        Int16 maxRedoSteps = static_cast< Int16 >( m_historyStack.size() - m_nextOp );
        if( maxRedoSteps < numSteps )
        {
            // We can't revert so many steps.
            result = false;
            numSteps = maxRedoSteps;
        }

        while( numSteps > 0 )
        {
            m_historyStack[ m_nextOp ]->Redo( editor );

            numSteps--;
            m_nextOp++;
        }

        return result;
    }
    return false;
}


}	// bv

