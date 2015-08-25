#include "ProjectManager.h"


#include "gtest/gtest.h"


#include "boost/filesystem.hpp"
#include "boost/regex.hpp"
#include <iostream>
 
using namespace boost::filesystem;
 
int main()
{
  path current_dir("./../../"); //
  boost::regex pattern("a.*"); // list all files starting with a
  for (recursive_directory_iterator iter(current_dir), end;
       iter != end;
       ++iter)
  {
	  auto p = iter->path();
	  std::cout << absolute( p.normalize() ).string() << std::endl;
    std::string name = iter->path().filename().string();
    if (regex_match(name, pattern))
      std::cout << iter->path() << "\n";
  }
}

//int main( int argc, char **argv )
//{
//	::testing::InitGoogleTest( &argc, argv );
//	return RUN_ALL_TESTS();
//}