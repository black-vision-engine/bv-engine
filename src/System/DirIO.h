#pragma once

#include <vector>
#include <string>


namespace bv
{

class Dir
{
private:

public:

    static std::vector< std::string >  ListFiles( const std::string & path, const std::string & wildcardFilter );

};

} //bv
