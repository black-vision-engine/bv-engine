#include "FileIO.h"

#include <sys/stat.h>

#include <fstream>


namespace bv
{

// ********************************
//
bool FileExists( const std::string & fileName )
{
    struct stat info;
    int ret = -1;

    ret = stat( fileName.c_str(), &info );
    return 0 == ret;
}

// ********************************
//
bool ReadFile( std::ostream & out, const std::string & fileName )
{    
    if( !FileExists( fileName ) )
    {
        return false;
    }

    std::ifstream inFile( fileName, std::ios::in );
    if( !inFile )
    {
        return false;
    }

    while( inFile.good() )
    {
        int c = inFile.get();
        if( ! inFile.eof() )
            out << (char) c;
    }

    inFile.close();

    return true;
}

} //bv
