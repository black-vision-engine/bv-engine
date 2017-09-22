#include "MockOperation.h"

#include "Engine/Editors/BVProjectEditor.h"



// ***********************
//
MockOperation::MockOperation( std::vector< OpEntry > & opRegister, const std::string & id )
    :   m_opRegister( opRegister )
    ,   m_operationID( id )
{}

// ***********************
//
bool            MockOperation::Undo     ( bv::BVProjectEditor * /*editor*/ )
{
    OpEntry entry;
    entry.OperationID = m_operationID;
    entry.Type = OpType::Undo;

    m_opRegister.push_back( entry );

    return true;
}

// ***********************
//  
bool            MockOperation::Redo     ( bv::BVProjectEditor * /*editor*/ )
{
    OpEntry entry;
    entry.OperationID = m_operationID;
    entry.Type = OpType::Redo;

    m_opRegister.push_back( entry );

    return true;
}
