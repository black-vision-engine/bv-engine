#pragma once

#include "Engine/Editors/UndoRedo/IRevertable.h"

#include <string>
#include <vector>




namespace bv
{

class BVProjectEditor;

}	// bv


// ***********************
//
enum OpType
{
    Undo,
    Redo
};


struct OpEntry
{
    std::string     OperationID;
    OpType          Type;
};


// ***********************
//
class MockOperation : public bv::IRevertable
{
private:

    std::vector< OpEntry > &            m_opRegister;
    std::string                         m_operationID;

public:

    explicit        MockOperation       ( std::vector< OpEntry > & opRegister, const std::string & id );


    // Inherited via IRevertable
    virtual bool        Undo        ( bv::BVProjectEditor * editor ) override;
    virtual bool        Redo        ( bv::BVProjectEditor * editor ) override;

};

DEFINE_UPTR_TYPE( MockOperation );


// ***********************
//
inline MockOperationUPtr        CreateOperation     ( std::vector< OpEntry > & opRegister, const std::string & id )
{
    auto op = new MockOperation( opRegister, id );
    return MockOperationUPtr( op );
}
