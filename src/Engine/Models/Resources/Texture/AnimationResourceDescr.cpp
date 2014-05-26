#include "AnimationResourceDescr.h"

#include "System/DirIO.h"


namespace bv { namespace model {

// *******************************
//
AnimationResourceDescr::AnimationResourceDescr                          ( const std::vector< std::string > & frames )
    : m_frames( frames )
{
}

// *******************************
//
AnimationResourceDescr::~AnimationResourceDescr                         ()
{
}

// *******************************
//
PluginResourceType          AnimationResourceDescr::GetResourceType     () const
{
    return PluginResourceType::PRT_ANIMATION;
}

// *******************************
//
const std::vector< std::string > & AnimationResourceDescr::GetFrames   () const
{
    return m_frames;
}

// *******************************
//
AnimationResourceDescr *     AnimationResourceDescr::CreateFromDirFrames( const std::string & path, const std::string & filter )
{
    auto files = Dir::ListFiles( path, filter );

    if ( files.size() > 0 )
    {
        return new AnimationResourceDescr( files );
    }

    return nullptr;
}

// *******************************
//
const ITextureResourceDescr *   QueryTextureResourceDescr   ( const IPluginResourceDescr * resDescr )
{
    if ( resDescr->GetResourceType() == PluginResourceType::PRT_TEXTURE )
    {
        return static_cast< const ITextureResourceDescr * >( resDescr );
    }

    return nullptr;
}

} //model
} //bv
