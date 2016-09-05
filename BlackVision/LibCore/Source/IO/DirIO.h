#pragma once

#include <vector>
#include <string>

#include "CoreDEF.h"

namespace bv
{

class Dir
{
private:

public:

    static std::vector< std::string >       ListFiles           ( const std::string & path, const std::string & wildcardFilter );
    static std::string                      WorkingDirectory    ();
    static bool                             Exists              ( const std::string & path );
    static bool                             CreateDir           ( const std::string & path, bool createRecusive = false );
    static bool                             RemoveDir           ( const std::string & path );
    static bool                             RenameDir           ( const std::string & path, const std::string & newName );
    static bool                             CopyDir             ( const std::string & path, const std::string & newName );
    static UInt64                           GetSize             ( const std::string & path );
};

} //bv
