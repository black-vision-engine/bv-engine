#include "AnimationResourceDescr.h"


namespace bv { namespace model {

// *******************************
//
AnimationResourceDescr::AnimationResourceDescr                          ( const std::vector< std::string > & frames, const std::string & name )
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
const std::vector< std::string > &  AnimationResourceDescr::GetFrames   () const
{
    return m_frames;
}

// *******************************
//
AnimationResourceDescr *     AnimationResourceDescr::CreateFromDirFrames( const std::string & path, const std::string & filter )
{
}


} //model
} //bv
