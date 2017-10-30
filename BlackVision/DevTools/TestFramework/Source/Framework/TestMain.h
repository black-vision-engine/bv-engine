#pragma once

// ***********************
//
// Include this file in your application entry point main.cpp file.
// Remember to include it only once in whole project.

#include "gtest/gtest.h"

#include "TestEnvironment.h"


static bv::TestEnvironment * gEnvironment = nullptr;



// ***********************
//
int main( int argc, char **argv )
{
    gEnvironment = new bv::TestEnvironment( argc, argv );
    
    ::testing::AddGlobalTestEnvironment( gEnvironment );

    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}

