#include "PresetAccessor.h"
#include "SceneDescriptor.h"

namespace bv
{

std::string PresetAccessor::m_fileExt = ".*\\.bvpreset";

// ********************************
//
PresetAccessorConstPtr PresetAccessor::Create( const Path & path )
{
    return PresetAccessorConstPtr( new PresetAccessor( path ) );
}

// ********************************
//
PresetAccessor::PresetAccessor	( const Path & path )
    : m_path( path )
{}

// ********************************
//
void                        PresetAccessor::SavePreset ( const model::SceneModelPtr node, const Path & path ) const
{
    File::Touch( ( m_path / path ).Str() );
    SceneDescriptor::SaveScene( node, ( m_path / path ).Str() );
}
    
// ********************************
//
model::SceneModelPtr    PresetAccessor::LoadPreset( const Path & path ) const
{
    return SceneDescriptor::LoadScene( ( m_path / path ).Str() );
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