#pragma once

#include "System/Path.h"

#include "Engine/Models/BasicNode.h"

namespace bv
{

class PresetAccessor
{
public:
    explicit                    PresetAccessor		( const Path & path, model::TimelineManager * tm );

    void                        SavePreset          ( const model::BasicNodeConstPtr node, const Path & path ) const;
    model::BasicNodePtr         LoadPreset          ( const Path & path ) const;

    PathVec                     ListPresets         ( const Path & path ) const;
    PathVec                     ListPresets         () const;

private:

    static std::string          m_fileExt;

    model::TimelineManager *    m_tm;
    Path                        m_path;
};

} // bv