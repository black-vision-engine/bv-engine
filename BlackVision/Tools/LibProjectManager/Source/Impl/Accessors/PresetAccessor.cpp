#include "PresetAccessor.h"

namespace bv
{

std::string PresetAccessor::m_fileExt = ".*\\.bvpreset";

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

    auto sob = SerializeObject();

    node->Serialize( sob );

    sob.Save( ( m_path / path ).Str() );
}
    
// ********************************
//
model::BasicNodePtr         PresetAccessor::LoadPreset( const Path & path ) const
{
    auto dos = DeserializeObject( ( m_path / path ).Str(), m_tm );

    return std::static_pointer_cast< model::BasicNode >( model::BasicNode::Create( dos ) );
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