#include "AnimationResourceDescr.h"

#include "System/DirIO.h"


namespace bv { namespace model {

// *******************************
//
AnimationResourceDescr::AnimationResourceDescr                          ( const std::vector< std::wstring > & frames, const std::string & name )
    : m_name( name )
    , m_frames( frames )
{
}

// *******************************
//
AnimationResourceDescr::~AnimationResourceDescr                         ()
{
}

// *******************************
//
const std::string &         AnimationResourceDescr::GetName             () const
{
    return m_name;
}

// *******************************
//
PluginResourceType          AnimationResourceDescr::GetResourceType     () const
{
    return PluginResourceType::PRT_ANIMATION;
}

// *******************************
//
const std::vector< std::wstring > & AnimationResourceDescr::GetFrames   () const
{
    return m_frames;
}

// *******************************
//
AnimationResourceDescr *     AnimationResourceDescr::CreateFromDirFrames( const std::wstring & path, const std::wstring & filter, const std::string & name )
{
    auto files = Dir::ListFiles( path, filter );

    if ( files.size() > 0 )
    {
        return new AnimationResourceDescr( files, name );
    }

    return nullptr;
}

} //model
} //bv
