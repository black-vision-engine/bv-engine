#include "AssetAccessor.h"

#include "ProjectManager.h"


namespace bv
{

// ***********************
//
Path         AssetAccessor::GetThumbnailPath            ( const Path & absPath )
{
    return Path( absPath.Str() + ".bvthumb" );
}

} //bv