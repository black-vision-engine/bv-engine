#pragma once

// ***********************
//
// Include this file in your application entry point main.cpp file.
// Remember to include it only once in whole project.

#include "gtest/gtest.h"

#include "TestEnvironment.h"






// ***********************
//
int main( int argc, char **argv )
{
    auto env = new bv::TestEnvironment( &argc, argv );
    bv::TestEnvironment::SetEnvironment( env );

    env->SetUp();

    ::testing::InitGoogleTest( &argc, argv );
    auto result = RUN_ALL_TESTS();

    env->TearDown();
    return result;
}

