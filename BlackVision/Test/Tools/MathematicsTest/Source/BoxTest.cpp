#include "gtest/gtest.h"


#include "Mathematics/Box.h"




// ***********************
// Default constructor creates empty box in point (0.0, 0.0, 0.0).
TEST( Box, Creation )
{
    bv::mathematics::Box box;

    EXPECT_TRUE( box.IsEmpty() );
    
    EXPECT_EQ( box.Height(), 0.0 );
    EXPECT_EQ( box.Width(), 0.0 );
    EXPECT_EQ( box.Depth(), 0.0 );

    EXPECT_EQ( box.Center(), glm::vec3( 0.0, 0.0, 0.0 ) );
}

// ***********************
// Constructor creates non empty cube.
TEST( Box, NonEmptyCreation )
{
    bv::mathematics::Box box( -1.0, 1.0, 1.0, -1.0, -1.0, 1.0 );

    EXPECT_TRUE( !box.IsEmpty() );

    EXPECT_EQ( box.Height(), 2.0 );
    EXPECT_EQ( box.Width(), 2.0 );
    EXPECT_EQ( box.Depth(), 2.0 );

    EXPECT_EQ( box.Center(), glm::vec3( 0.0, 0.0, 0.0 ) );
}


// ***********************
// Including empty box leaves it in empty state.
TEST( Box, EmptyIncludesEmpty )
{
    bv::mathematics::Box box1;
    bv::mathematics::Box box2;

    box1.Include( box2 );

    EXPECT_TRUE( box1.IsEmpty() );
}

// ***********************
// Including empty box leaves box in previous state
TEST( Box, IncludeEmpty )
{
    bv::mathematics::Box box1( -1.0, 1.0, 1.0, -1.0, 0.0, 0.5 );
    bv::mathematics::Box box2;

    box1.Include( box2 );

    EXPECT_TRUE( !box1.IsEmpty() );

    EXPECT_EQ( box1.Height(), 2.0 );
    EXPECT_EQ( box1.Width(), 2.0 );
    EXPECT_EQ( box1.Depth(), 0.5 );

    EXPECT_EQ( box1.Center(), glm::vec3( 0.0, 0.0, 0.25 ) );
}

// ***********************
// Empty box including point beacomes equal to this point.
TEST( Box, EmptyIncludesPoint )
{
    bv::mathematics::Box box;

    box.Include( glm::vec3( 1.0, 1.0, 1.0 ) );

    EXPECT_TRUE( !box.IsEmpty() );

    EXPECT_EQ( box.Height(), 0.0 );
    EXPECT_EQ( box.Width(), 0.0 );
    EXPECT_EQ( box.Depth(), 0.0 );

    EXPECT_EQ( box.Center(), glm::vec3( 1.0, 1.0, 1.0 ) );
}

// ***********************
// When cube includes point, box is extended to this point.
TEST( Box, CubeIncludePoint )
{
    bv::mathematics::Box box( -1.0, 1.0, 1.0, -1.0, -1.0, 1.0 );

    box.Include( glm::vec3( 2.0, 2.0, 2.0 ) );

    EXPECT_TRUE( !box.IsEmpty() );

    EXPECT_EQ( box.Height(), 3.0 );
    EXPECT_EQ( box.Width(), 3.0 );
    EXPECT_EQ( box.Depth(), 3.0 );

    EXPECT_EQ( box.Center(), glm::vec3( 0.5, 0.5, 0.5 ) );
}

