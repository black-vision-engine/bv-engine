#include "gtest/gtest.h"


#include "Mathematics/Interpolators/CompositeInterpolator.h"
#include "Mathematics/Transform/MatTransform.h"

#include "Utils/Comparators/ParamComparator.h"


#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"
#include "Engine/Models/Timeline/TimelineHelper.h"


using namespace bv;





// ***********************
// Simple AddKey function test.
TEST( Model_ParamValModel, CompositeInterpolator_AddKeys )
{
    CompositeInterpolator< TimeType, float > interpolator;

    interpolator.AddKey( 1.0f, 2.0f );
    interpolator.AddKey( 2.0f, 22.0f );
    interpolator.AddKey( 5.0f, 222.0f );
    interpolator.AddKey( 8.0f, 2222.0f );

    ASSERT_EQ( interpolator.GetNumKeys(), 4 );

    EXPECT_EQ( interpolator.GetKeys()[ 0 ].t, 1.0f );
    EXPECT_EQ( interpolator.GetKeys()[ 0 ].val, 2.0f );

    EXPECT_EQ( interpolator.GetKeys()[ 1 ].t, 2.0f );
    EXPECT_EQ( interpolator.GetKeys()[ 1 ].val, 22.0f );

    EXPECT_EQ( interpolator.GetKeys()[ 2 ].t, 5.0f );
    EXPECT_EQ( interpolator.GetKeys()[ 2 ].val, 222.0f );

    EXPECT_EQ( interpolator.GetKeys()[ 3 ].t, 8.0f );
    EXPECT_EQ( interpolator.GetKeys()[ 3 ].val, 2222.0f );
}



// ***********************
// Adding key under the same time overrides previous value in this key.
TEST( Model_ParamValModel, CompositeInterpolator_AddNewKeyOverridesPrevious )
{
    CompositeInterpolator< TimeType, float > interpolator;

    interpolator.AddKey( 1.0f, 2.0f );
    
    ASSERT_EQ( interpolator.GetNumKeys(), 1 );
    EXPECT_EQ( interpolator.GetKeys()[ 0 ].t, 1.0f );
    EXPECT_EQ( interpolator.GetKeys()[ 0 ].val, 2.0f );

    interpolator.AddKey( 1.0f, 10.0f );

    ASSERT_EQ( interpolator.GetNumKeys(), 1 );
    EXPECT_EQ( interpolator.GetKeys()[ 0 ].t, 1.0f );
    EXPECT_EQ( interpolator.GetKeys()[ 0 ].val, 10.0f );
}

// ***********************
// Simple RemoveKey test.
TEST( Model_ParamValModel, CompositeInterpolator_RemoveKey )
{
    CompositeInterpolator< TimeType, float > interpolator;

    interpolator.AddKey( 1.0f, 2.0f );
    interpolator.AddKey( 2.0f, 22.0f );
    interpolator.AddKey( 5.0f, 222.0f );
    interpolator.AddKey( 8.0f, 2222.0f );

    ASSERT_EQ( interpolator.GetNumKeys(), 4 );

    interpolator.RemoveKey( 5.0f );

    ASSERT_EQ( interpolator.GetNumKeys(), 3 );

    EXPECT_EQ( interpolator.GetKeys()[ 0 ].t, 1.0f );
    EXPECT_EQ( interpolator.GetKeys()[ 0 ].val, 2.0f );

    EXPECT_EQ( interpolator.GetKeys()[ 1 ].t, 2.0f );
    EXPECT_EQ( interpolator.GetKeys()[ 1 ].val, 22.0f );

    EXPECT_EQ( interpolator.GetKeys()[ 2 ].t, 8.0f );
    EXPECT_EQ( interpolator.GetKeys()[ 2 ].val, 2222.0f );
}

// ***********************
// Last key can't be removed.
TEST( Model_ParamValModel, CompositeInterpolator_RemoveLastKey )
{
    CompositeInterpolator< TimeType, float > interpolator;

    interpolator.AddKey( 1.0f, 2.0f );
    interpolator.AddKey( 2.0f, 22.0f );
    interpolator.AddKey( 5.0f, 222.0f );
    interpolator.AddKey( 8.0f, 2222.0f );

    ASSERT_EQ( interpolator.GetNumKeys(), 4 );

    interpolator.RemoveKey( 5.0f );
    interpolator.RemoveKey( 1.0f );
    interpolator.RemoveKey( 8.0f );

    ASSERT_EQ( interpolator.GetNumKeys(), 1 );

    interpolator.RemoveKey( 2.0f );

    // Still one key remains.
    ASSERT_EQ( interpolator.GetNumKeys(), 1 );
}

// ***********************
// Simple move key test.
TEST( Model_ParamValModel, CompositeInterpolator_MoveKey )
{
    CompositeInterpolator< TimeType, float > interpolator;

    interpolator.AddKey( 1.0f, 2.0f );
    interpolator.AddKey( 2.0f, 22.0f );
    interpolator.AddKey( 5.0f, 222.0f );
    interpolator.AddKey( 8.0f, 2222.0f );

    ASSERT_EQ( interpolator.GetNumKeys(), 4 );

    ASSERT_TRUE( interpolator.MoveKey( 1.0f, 10.0f ) );

    ASSERT_EQ( interpolator.GetNumKeys(), 4 );

    EXPECT_EQ( interpolator.GetKeys()[ 0 ].t, 2.0f );
    EXPECT_EQ( interpolator.GetKeys()[ 0 ].val, 22.0f );

    EXPECT_EQ( interpolator.GetKeys()[ 1 ].t, 5.0f );
    EXPECT_EQ( interpolator.GetKeys()[ 1 ].val, 222.0f );

    EXPECT_EQ( interpolator.GetKeys()[ 2 ].t, 8.0f );
    EXPECT_EQ( interpolator.GetKeys()[ 2 ].val, 2222.0f );

    EXPECT_EQ( interpolator.GetKeys()[ 3 ].t, 10.0f );
    EXPECT_EQ( interpolator.GetKeys()[ 3 ].val, 2.0f );
}

// ***********************
// Try to move key to existing one. Move operation should fail.
// All keys should remain in their places.
TEST( Model_ParamValModel, CompositeInterpolator_MoveKeyToExistingOne )
{
    CompositeInterpolator< TimeType, float > interpolator;

    interpolator.AddKey( 1.0f, 2.0f );
    interpolator.AddKey( 2.0f, 22.0f );
    interpolator.AddKey( 5.0f, 222.0f );
    interpolator.AddKey( 8.0f, 2222.0f );

    ASSERT_EQ( interpolator.GetNumKeys(), 4 );

    ASSERT_FALSE( interpolator.MoveKey( 1.0f, 2.0f ) );

    ASSERT_EQ( interpolator.GetNumKeys(), 4 );

    // Check if notinh changed.
    EXPECT_EQ( interpolator.GetKeys()[ 0 ].t, 1.0f );
    EXPECT_EQ( interpolator.GetKeys()[ 0 ].val, 2.0f );

    EXPECT_EQ( interpolator.GetKeys()[ 1 ].t, 2.0f );
    EXPECT_EQ( interpolator.GetKeys()[ 1 ].val, 22.0f );

    EXPECT_EQ( interpolator.GetKeys()[ 2 ].t, 5.0f );
    EXPECT_EQ( interpolator.GetKeys()[ 2 ].val, 222.0f );

    EXPECT_EQ( interpolator.GetKeys()[ 3 ].t, 8.0f );
    EXPECT_EQ( interpolator.GetKeys()[ 3 ].val, 2222.0f );
}

