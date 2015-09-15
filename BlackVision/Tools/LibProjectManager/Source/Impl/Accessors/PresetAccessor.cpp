#include "PresetAccessor.h"
#include "SceneDescriptor.h"

namespace bv
{

std::string PresetAccessor::m_fileExt = ".*\\.bvpreset";

// ********************************
//
PresetAccessorConstPtr PresetAccessor::Create( const Path & path, model::TimelineManager * tm )
{
    return PresetAccessorConstPtr( new PresetAccessor( path, tm ) );
}

// ********************************
//
PresetAccessor::PresetAccessor	( const Path & path, model::TimelineManager * tm )
    : m_path( path )
    , m_tm( tm )
{}

// ********************************
//
void                        PresetAccessor::SavePreset ( const model::BasicNodeConstPtr node, const Path & path ) const
{
    File::Touch( ( m_path / path ).Str() );
    SceneDescriptor::SaveScene( node, m_tm, ( m_path / path ).Str() );
}
    
// ********************************
//
model::BasicNodeConstPtr    PresetAccessor::LoadPreset( const Path & path ) const
{
    return SceneDescriptor::LoadScene( ( m_path / path ).Str(), m_tm );
}

// ********************************
//
PathVec                     PresetAccessor::ListPresets( const Path & path ) const
{
    return Path::List( ( m_path / path ).Str(), true, m_fileExt );
}

// ********************************
//
PathVec                     PresetAccessor::ListPresets() const
{
    return Path::List( m_path, true, m_fileExt );
}

} // bv