#pragma once

#include "System/Path.h"

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/SceneModel.h"
#include "Engine/Models/Timeline/TimelineManager.h"

namespace bv
{

class PresetAccessor;
DEFINE_CONST_PTR_TYPE( PresetAccessor )

class PresetAccessor
{
public:
    void                        SavePreset          ( const model::BasicNodePtr node, const Path & path ) const;
    model::BasicNodePtr         LoadPreset          ( const Path & path, const model::OffsetTimeEvaluatorPtr & timeline ) const;

    PathVec                     ListPresets         ( const Path & path ) const;
    PathVec                     ListPresets         () const;

    static PresetAccessorConstPtr Create            ( const Path & path );

private:

    explicit                    PresetAccessor		( const Path & path );


    static std::string          m_fileExt;

    Path                        m_path;
};

} // bv