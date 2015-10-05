#pragma once

#include "System/Path.h"

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Timeline/TimelineManager.h"

namespace bv
{

class PresetAccessor;
DEFINE_CONST_PTR_TYPE( PresetAccessor )

class PresetAccessor
{
public:
    void                        SavePreset          ( const model::BasicNodeConstPtr node, const Path & path ) const;
    model::BasicNodeConstPtr    LoadPreset          ( const Path & path ) const;

    PathVec                     ListPresets         ( const Path & path ) const;
    PathVec                     ListPresets         () const;

    static PresetAccessorConstPtr Create            ( const Path & path, model::TimelineManager * tm );

private:

    explicit                    PresetAccessor		( const Path & path, model::TimelineManager * tm );


    static std::string          m_fileExt;

    model::TimelineManager *    m_tm;
    Path                        m_path;
};

} // bv