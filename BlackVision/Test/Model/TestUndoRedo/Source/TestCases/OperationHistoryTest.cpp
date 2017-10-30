#include "Framework/FrameworkTest.h"


#include "Engine/Editors/UndoRedo/OperationHistory.h"
#include "Utils/MockOperation.h"





// ***********************
// OperationHistory should hold only specified number of task in history. Old tasks should be deleted.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor_UndoRedo_OperationHistory, OperationHistoryDeletesOldTasks )
{
    bv::OperationHistory history( 5 );
    std::vector< OpEntry > opRegister;
    

    history.AddOperation( CreateOperation( opRegister, "Op1" ) );
    history.AddOperation( CreateOperation( opRegister, "Op2" ) );
    history.AddOperation( CreateOperation( opRegister, "Op3" ) );
    history.AddOperation( CreateOperation( opRegister, "Op4" ) );
    history.AddOperation( CreateOperation( opRegister, "Op5" ) );
    history.AddOperation( CreateOperation( opRegister, "Op6" ) );

    EXPECT_TRUE( history.Undo( GetProjectEditor(), 5 ) );
    EXPECT_FALSE( history.Undo( GetProjectEditor(), 1 ) );

    // Only 5 operations should apear.
    ASSERT_EQ( opRegister.size(), 5 );

    EXPECT_EQ( opRegister[ 0 ].Type, OpType::Undo );      EXPECT_EQ( opRegister[ 0 ].OperationID, "Op6" );
    EXPECT_EQ( opRegister[ 1 ].Type, OpType::Undo );      EXPECT_EQ( opRegister[ 1 ].OperationID, "Op5" );
    EXPECT_EQ( opRegister[ 2 ].Type, OpType::Undo );      EXPECT_EQ( opRegister[ 2 ].OperationID, "Op4" );
    EXPECT_EQ( opRegister[ 3 ].Type, OpType::Undo );      EXPECT_EQ( opRegister[ 3 ].OperationID, "Op3" );
    EXPECT_EQ( opRegister[ 4 ].Type, OpType::Undo );      EXPECT_EQ( opRegister[ 4 ].OperationID, "Op2" );
}

// ***********************
// Changing history size to greater value shouldn't affect history.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor_UndoRedo_OperationHistory, ChangeHistorySizeGreater )
{
    bv::OperationHistory history( 2 );
    std::vector< OpEntry > opRegister;

    history.AddOperation( CreateOperation( opRegister, "Op1" ) );
    history.AddOperation( CreateOperation( opRegister, "Op2" ) );
    history.AddOperation( CreateOperation( opRegister, "Op3" ) );

    history.SetHistoryLength( 5 );

    history.AddOperation( CreateOperation( opRegister, "Op4" ) );
    history.AddOperation( CreateOperation( opRegister, "Op5" ) );
    history.AddOperation( CreateOperation( opRegister, "Op6" ) );
    history.AddOperation( CreateOperation( opRegister, "Op7" ) );

    EXPECT_TRUE( history.Undo( GetProjectEditor(), 5 ) );
    EXPECT_FALSE( history.Undo( GetProjectEditor(), 1 ) );

    // Only 5 operations should apear.
    ASSERT_EQ( opRegister.size(), 5 );

    EXPECT_EQ( opRegister[ 0 ].Type, OpType::Undo );      EXPECT_EQ( opRegister[ 0 ].OperationID, "Op7" );
    EXPECT_EQ( opRegister[ 1 ].Type, OpType::Undo );      EXPECT_EQ( opRegister[ 1 ].OperationID, "Op6" );
    EXPECT_EQ( opRegister[ 2 ].Type, OpType::Undo );      EXPECT_EQ( opRegister[ 2 ].OperationID, "Op5" );
    EXPECT_EQ( opRegister[ 3 ].Type, OpType::Undo );      EXPECT_EQ( opRegister[ 3 ].OperationID, "Op4" );
    EXPECT_EQ( opRegister[ 4 ].Type, OpType::Undo );      EXPECT_EQ( opRegister[ 4 ].OperationID, "Op3" );
}

// ***********************
// Changing history size should delete oldest tasks.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor_UndoRedo_OperationHistory, ChangeHistorySizeLower )
{
    bv::OperationHistory history( 5 );
    std::vector< OpEntry > opRegister;

    history.AddOperation( CreateOperation( opRegister, "Op1" ) );
    history.AddOperation( CreateOperation( opRegister, "Op2" ) );
    history.AddOperation( CreateOperation( opRegister, "Op3" ) );
    history.AddOperation( CreateOperation( opRegister, "Op4" ) );
    history.AddOperation( CreateOperation( opRegister, "Op5" ) );

    // Change history size.
    history.SetHistoryLength( 2 );

    EXPECT_TRUE( history.Undo( GetProjectEditor(), 2 ) );
    EXPECT_FALSE( history.Undo( GetProjectEditor(), 1 ) );

    // Only 2 operations should apear.
    ASSERT_EQ( opRegister.size(), 2 );

    EXPECT_EQ( opRegister[ 0 ].Type, OpType::Undo );      EXPECT_EQ( opRegister[ 0 ].OperationID, "Op5" );
    EXPECT_EQ( opRegister[ 1 ].Type, OpType::Undo );      EXPECT_EQ( opRegister[ 1 ].OperationID, "Op4" );
}

// ***********************
// Changing history size while there are operations on stack to redo.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor_UndoRedo_OperationHistory, ChangeHistorySizeLowerRedoCase )
{
    bv::OperationHistory history( 5 );
    std::vector< OpEntry > opRegister;

    history.AddOperation( CreateOperation( opRegister, "Op1" ) );
    history.AddOperation( CreateOperation( opRegister, "Op2" ) );
    history.AddOperation( CreateOperation( opRegister, "Op3" ) );
    history.AddOperation( CreateOperation( opRegister, "Op4" ) );
    history.AddOperation( CreateOperation( opRegister, "Op5" ) );

    EXPECT_TRUE( history.Undo( GetProjectEditor(), 2 ) );

    // Change history size.
    history.SetHistoryLength( 2 );
    opRegister.clear();

    EXPECT_TRUE( history.Undo( GetProjectEditor(), 2 ) );
    EXPECT_FALSE( history.Undo( GetProjectEditor(), 1 ) );

    // Only 2 operations should apear.
    ASSERT_EQ( opRegister.size(), 2 );

    EXPECT_EQ( opRegister[ 0 ].Type, OpType::Undo );      EXPECT_EQ( opRegister[ 0 ].OperationID, "Op3" );
    EXPECT_EQ( opRegister[ 1 ].Type, OpType::Undo );      EXPECT_EQ( opRegister[ 1 ].OperationID, "Op2" );
}


// ***********************
// OperationHistory should hold only specified number of task in history. Old tsks should be deleted.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor_UndoRedo_OperationHistory, RepeatedUndoRedo )
{
    bv::OperationHistory history( 2 );
    std::vector< OpEntry > opRegister;

    history.AddOperation( CreateOperation( opRegister, "Op1" ) );
    history.AddOperation( CreateOperation( opRegister, "Op2" ) );
    history.AddOperation( CreateOperation( opRegister, "Op3" ) );

    // First repeat
    EXPECT_TRUE( history.Undo( GetProjectEditor(), 2 ) );
    EXPECT_FALSE( history.Undo( GetProjectEditor(), 1 ) );

    EXPECT_TRUE( history.Redo( GetProjectEditor(), 1 ) );
    EXPECT_TRUE( history.Redo( GetProjectEditor(), 1 ) );
    EXPECT_FALSE( history.Redo( GetProjectEditor(), 1 ) );

    // Only 4 operations should apear.
    ASSERT_EQ( opRegister.size(), 4 );

    EXPECT_EQ( opRegister[ 0 ].Type, OpType::Undo );      EXPECT_EQ( opRegister[ 0 ].OperationID, "Op3" );
    EXPECT_EQ( opRegister[ 1 ].Type, OpType::Undo );      EXPECT_EQ( opRegister[ 1 ].OperationID, "Op2" );
    EXPECT_EQ( opRegister[ 2 ].Type, OpType::Redo );      EXPECT_EQ( opRegister[ 2 ].OperationID, "Op2" );
    EXPECT_EQ( opRegister[ 3 ].Type, OpType::Redo );      EXPECT_EQ( opRegister[ 3 ].OperationID, "Op3" );

    // Second repeat
    opRegister.clear();

    EXPECT_TRUE( history.Undo( GetProjectEditor(), 2 ) );
    EXPECT_FALSE( history.Undo( GetProjectEditor(), 1 ) );

    EXPECT_TRUE( history.Redo( GetProjectEditor(), 1 ) );
    EXPECT_TRUE( history.Redo( GetProjectEditor(), 1 ) );
    EXPECT_FALSE( history.Redo( GetProjectEditor(), 1 ) );

    // Only 4 operations should apear.
    ASSERT_EQ( opRegister.size(), 4 );

    EXPECT_EQ( opRegister[ 0 ].Type, OpType::Undo );      EXPECT_EQ( opRegister[ 0 ].OperationID, "Op3" );
    EXPECT_EQ( opRegister[ 1 ].Type, OpType::Undo );      EXPECT_EQ( opRegister[ 1 ].OperationID, "Op2" );
    EXPECT_EQ( opRegister[ 2 ].Type, OpType::Redo );      EXPECT_EQ( opRegister[ 2 ].OperationID, "Op2" );
    EXPECT_EQ( opRegister[ 3 ].Type, OpType::Redo );      EXPECT_EQ( opRegister[ 3 ].OperationID, "Op3" );

}

// ***********************
// Adding new operations after doing undo, removes posibility to redo operations. New operatio creates new 
// redo branch and old branch is deleted.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor_UndoRedo_OperationHistory, DeleteRedoOldBranch )
{
    bv::OperationHistory history( 5 );
    std::vector< OpEntry > opRegister;

    history.AddOperation( CreateOperation( opRegister, "Op1" ) );
    history.AddOperation( CreateOperation( opRegister, "Op2" ) );
    history.AddOperation( CreateOperation( opRegister, "Op3" ) );
    history.AddOperation( CreateOperation( opRegister, "Op4" ) );
    history.AddOperation( CreateOperation( opRegister, "Op5" ) );

    EXPECT_TRUE( history.Undo( GetProjectEditor(), 2 ) );

    history.AddOperation( CreateOperation( opRegister, "Op6" ) );

    opRegister.clear();

    // Check if there are no operations on top of stack.
    EXPECT_FALSE( history.Redo( GetProjectEditor(), 1 ) );

    // Revert all operations.
    EXPECT_TRUE( history.Undo( GetProjectEditor(), 4 ) );
    EXPECT_FALSE( history.Undo( GetProjectEditor(), 1 ) );

    // Only 5 operations should apear.
    ASSERT_EQ( opRegister.size(), 4 );

    EXPECT_EQ( opRegister[ 0 ].Type, OpType::Undo );      EXPECT_EQ( opRegister[ 0 ].OperationID, "Op6" );
    EXPECT_EQ( opRegister[ 1 ].Type, OpType::Undo );      EXPECT_EQ( opRegister[ 1 ].OperationID, "Op3" );
    EXPECT_EQ( opRegister[ 2 ].Type, OpType::Undo );      EXPECT_EQ( opRegister[ 2 ].OperationID, "Op2" );
    EXPECT_EQ( opRegister[ 3 ].Type, OpType::Undo );      EXPECT_EQ( opRegister[ 3 ].OperationID, "Op1" );
}


