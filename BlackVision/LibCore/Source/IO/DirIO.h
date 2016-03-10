#pragma once

#include <vector>
#include <string>


namespace bv
{

class Dir
{
private:

public:

    static std::vector< std::string >   ListFiles           ( const std::string & path, const std::string & wildcardFilter );
    static std::string                  WorkingDirectory    ();
	static bool							Exists				( const std::string & path );
	static bool							CreateDir			( const std::string & path, bool createRecusive = false );
    static bool							RemoveDir			( const std::string & path );
	static bool							RenameDir			( const std::string & path,const std::string & newName );
};

} //bv
