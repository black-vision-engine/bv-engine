#include "PresetAccessor.h"

namespace bv
{

// ********************************
//
PresetAccessor::PresetAccessor	( const Path & path )
    : m_path( path )
{}

// ********************************
//
void                        PresetAccessor::SavePreset ( const model::BasicNodeConstPtr node, const Path & path ) const
{

}
    
// ********************************
//
model::BasicNodeConstPtr    PresetAccessor::LoadPreset( const Path & path ) const
{
}

// ********************************
//
PathVec                     PresetAccessor::ListPresets( const Path & path ) const
{
}

// ********************************
//
PathVec                     PresetAccessor::ListPresets() const
{

}

} // bv