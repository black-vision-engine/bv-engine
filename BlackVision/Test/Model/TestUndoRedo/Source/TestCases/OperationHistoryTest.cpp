#include "Framework/FrameworkTest.h"


#include "Engine/Editors/UndoRedo/OperationHistory.h"
#include "Utils/MockOperation.h"





// ***********************
// OperationHistory should hold only specified number of task in history. Old tsks should be deleted.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor.UndoRedo.OperationHistory, OperationHistoryDeletesOldTasks )
{
    bv::OperationHistory history( 5 );
    std::vector< OpEntry > opRegister;
    

    history.AddOperation( CreateOperation( opRegister, "Op1" ) );
    history.AddOperation( CreateOperation( opRegister, "Op2" ) );
    history.AddOperation( CreateOperation( opRegister, "Op3" ) );
    history.AddOperation( CreateOperation( opRegister, "Op4" ) );
    history.AddOperation( CreateOperation( opRegister, "Op5" ) );
    history.AddOperation( CreateOperation( opRegister, "Op6" ) );

    CHECK( history.Undo( GetProjectEditor(), 5 ) );
    CHECK( !history.Undo( GetProjectEditor(), 1 ) );

    // Only 5 operations should apear.
    REQUIRE CHECK( opRegister.size() == 5 );

    CHECK( opRegister[ 0 ].Type == OpType::Undo );      CHECK( opRegister[ 0 ].OperationID == "Op6" );
    CHECK( opRegister[ 1 ].Type == OpType::Undo );      CHECK( opRegister[ 1 ].OperationID == "Op5" );
    CHECK( opRegister[ 2 ].Type == OpType::Undo );      CHECK( opRegister[ 2 ].OperationID == "Op4" );
    CHECK( opRegister[ 3 ].Type == OpType::Undo );      CHECK( opRegister[ 3 ].OperationID == "Op3" );
    CHECK( opRegister[ 4 ].Type == OpType::Undo );      CHECK( opRegister[ 4 ].OperationID == "Op2" );
}

