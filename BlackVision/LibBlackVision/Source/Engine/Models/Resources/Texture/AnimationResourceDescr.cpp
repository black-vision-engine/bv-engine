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
AnimationResourceDescrPtr     AnimationResourceDescr::CreateFromDirFrames( const std::string & path, const std::string & filter )
{
    auto files = Dir::ListFiles( path, filter );

    if ( files.size() > 0 )
    {
        return std::make_shared< AnimationResourceDescr >( files );
    }

    return nullptr;
}

// *******************************
//
ITextureResourceDescrConstPtr   QueryTextureResourceDescr   ( IPluginResourceDescrConstPtr resDescr )
{
    if ( resDescr->GetResourceType() == PluginResourceType::PRT_TEXTURE )
    {
        return std::static_pointer_cast< const ITextureResourceDescr >( resDescr );
    }

    return nullptr;
}

} //model
} //bv
