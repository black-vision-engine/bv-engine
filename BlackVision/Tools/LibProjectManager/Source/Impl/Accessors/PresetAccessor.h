#pragma once

#include "System/Path.h"

#include "Engine/Models/BasicNode.h"

namespace bv
{

class PresetAccessor
{
public:
    explicit                    PresetAccessor		( const Path & path );

    void                        SavePreset          ( const model::BasicNodeConstPtr node, const Path & path ) const;
    model::BasicNodeConstPtr    LoadPreset          ( const Path & path ) const;

    PathVec                     ListPresets         ( const Path & path ) const;
    PathVec                     ListPresets         () const;

private:
    Path            m_path;
};

} // bv