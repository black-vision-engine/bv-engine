#pragma once

#include <vector>
#include <string>


namespace bv
{

class Dir
{
private:

public:

    static std::vector< std::wstring >  ListFiles( const std::wstring & path, const std::wstring & wildcardFilter );

};

} //bv
