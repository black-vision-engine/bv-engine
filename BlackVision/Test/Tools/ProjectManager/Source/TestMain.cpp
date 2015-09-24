#include "ProjectManager.h"

#include "gtest/gtest.h"

using namespace bv;

int main( int argc, char **argv )
{
	auto pm0 = ProjectManager::GetInstance( "bv_media" );

	pm0->ListProjectsNames();

	::testing::InitGoogleTest( &argc, argv );
	return RUN_ALL_TESTS();
}
